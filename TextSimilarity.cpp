#include "TextSimilarity.h"
#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
#include <cppjieba/Jieba.hpp>
#include <algorithm>
#include <assert.h>
using namespace std;

TextSimilarity::TextSimilarity(string dict)			//初始化目录
	:DICT(dict)
	, DICT_PATH(dict + "/jieba.dict.utf8")
	, HMM_PATH(dict + "/hmm_model.utf8")
	, USER_DICT_PATH(dict + "/user.dict.utf8")
	, IDF_PATH(dict + "/idf.utf8")
	, STOP_WORD_PATH(dict + "/stop_words.utf8")
	, _jieba(DICT_PATH,
			 HMM_PATH,
			 USER_DICT_PATH,
			 IDF_PATH,
			 STOP_WORD_PATH)
	,MaxWordsNumber(10)
{
	getstopWrodTable(STOP_WORD_PATH.c_str());		//初始化停用词表
}

//获取词频
TextSimilarity::wordFreq TextSimilarity::getWordFreq(const char* filename)
{
	ifstream fin(filename);		//建立一个文件
	
	if (!fin.is_open())		//确认文件是否被打开
	{
		cout << "open file" << endl;
		return wordFreq();		//如果文件没有被打开，返回一个空的map，相当一没有东西被打开
	}
	//开始读文件
	string line;
	wordFreq wf;
	while (!fin.eof())
	{
		getline(fin, line);			//对每行进行分词处理，将结果保存到len中
		line = GBKToUTF8(line);		//将GBK格式转化为UTF8
		vector<string> words;		//创建存放分词结果的容器
		//对当前文本进行分词
		_jieba.Cut(line, words, true);
		//统计词频
		for (const auto& a : words)
		{
			if (_stopWordsSet.count(a))		//检查是否在停用词表中，如果是结束此次循环
				continue;
			else
			{
				if (wf.count(a))				//如果这个词已经存在，在次数上进行+1，如果没有就将次数置为1    
					wf[a]++;
				else
					wf[a] = 1;				
			}
		}
	}
	return wf;
}

//创建停用词表
void TextSimilarity::getstopWrodTable(const char* stopWordFile)
{
	ifstream fin(stopWordFile);   //将停用词打开
	if (!fin.is_open())
	{
		cout << "open file" << stopWordFile << "faile" << endl;		//如果打开失败，输出提示信息
		return;
	}
	
	string line;		//创建保存停用词表的容器
	while (!fin.eof())	//将停用词存放在line中
	{
		getline(fin, line);	
		_stopWordsSet.insert(line);
	}
	fin.close();		//关闭停用词

}
bool sortRevrse(pair<string, int> lift, pair<string, int> right)
{
	return lift.second > right.second;			//用pair中的第二个元素来排序，也就是通过词频来排序
}

//关键词排序
vector<pair<string, int>> TextSimilarity::sortByValueReverse(TextSimilarity::wordFreq& wf)
{
	//由于stor只支持序列排序，所以需要将map中的资源拷贝到vector中；
	vector<pair<string, int>> wfvector(wf.begin(), wf.end());	
	sort(wfvector.begin(), wfvector.end(), sortRevrse);

	return wfvector;
}

//在两个文件中取出关键词存放在一起
void TextSimilarity::selectAimWords(vector<pair<string, int>>& wfvec, wordSet& wset)
{
	int len = wfvec.size();
	int size = len > MaxWordsNumber ? MaxWordsNumber : len;			//在文本内容和规定词量选择最少的
	for (int i = 0; i < size; i++)
	{
		wset.insert(wfvec[i].first);								//不能直接存放，因为wf中有两个量，只需要存放第一个
	}
}

//创建词频向量
vector<double> TextSimilarity::getOneHot(wordSet& wset, wordFreq& wf)
{
	//遍历整个wset中的每一个词，如果这个词在该文本词频中，将其的词频放到vector中，没有就将10作为词频放入该次对应的位置
	vector<double> oneHot;
	for (const auto& a : wset)
	{
		if (wf.count(a))
			oneHot.push_back(wf[a]);
		else 
			oneHot.push_back(0);
	}
	return oneHot;
}

//词频向量相似度
double TextSimilarity::cosine(vector<double> oneHot1, vector<double> oneHot2)
{
	double modular1 = 0;		//模
	double modular2 = 0;
	double products;			//点机
	assert(oneHot1.size() == oneHot2.size());
	for (int i = 0; i < oneHot1.size(); i++)
	{
		products += oneHot1[i] * oneHot2[i];
	}

	for (int i = 0; i < oneHot1.size(); i++)
	{
		modular1 += pow(oneHot1[i], 2);
	}
	for (int i = 0; i < oneHot2.size(); i++)
	{
		modular2 += pow(oneHot2[i], 2);
	}
	modular1 = pow(modular1, 0.5);
	modular2 = pow(modular2, 0.5);

	return products/(modular1*modular2);
}


//将GBK格式转化为UTF8
string TextSimilarity::GBKToUTF8(string str)
{
	//将GBK转换为UTF16
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);		//获取buffer的大小	
	wchar_t* wstr = new wchar_t[len];				//wchar_t 宽字符		//将其他编码格式转化为UTF16编码格式
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wstr, len);
	//将UTF16转换为UTF8
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);	//获取buffer的大小
	char* Utf8Str = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, Utf8Str, len, NULL, NULL);	//将UTF16编码格式转化为其他编码格式
	string out(Utf8Str);

	//释放空间，否则会出现野指针的情况
	if (wstr)
	{
		delete[] wstr;
		wstr = NULL;
	}
	if (Utf8Str)
	{
		delete[] Utf8Str;
		Utf8Str = NULL;
	}

	return out;
}

string  TextSimilarity::Utf8ToGBk(string str)
{
	//将UTF8转换成UTF16
	int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);  //获取buffer的大小
	wchar_t* wstr = new wchar_t[len];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, len);			//将UTF8编码格式转化为UTF16编码格式

	//将UTF16转换为GBK
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);		//获取buffer的大小
	char* GbkStr = new char[len];
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, GbkStr, len, NULL, NULL);		//将UTF16编码格式转化为GBK编码格式
	string out(GbkStr);

	//释放空间，否则会出现野指针的情况
	if (wstr)
	{
		delete[] wstr;
		wstr = NULL;
	}
	if (GbkStr)
	{
		delete[] GbkStr;
		GbkStr = NULL;
	}

	return out;
}