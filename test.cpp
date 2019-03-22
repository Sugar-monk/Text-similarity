#include "TextSimilarity.h"
#include <iostream>
#include <iomanip>
using namespace std;

void textWordFreq()
{
	TextSimilarity ts("dict");
	TextSimilarity::wordFreq wf = ts.getWordFreq("text2.txt");
	for (const auto& a : wf)
	{
		cout << ts.Utf8ToGBk(a.first) << ":" << a.second << endl;
	}

	
	//分词排序
	vector<pair<string, int>> wfvec = ts.sortByValueReverse(wf);
	for (const auto& a : wf)
	{
		cout << ts.Utf8ToGBk(a.first) << ":" << a.second << endl;
	}


}

void testOneHot1()
{
	TextSimilarity ts("dict");
	TextSimilarity::wordFreq wf = ts.getWordFreq("test.txt");		//读取文件
	vector<pair<string, int>> wfvec = ts.sortByValueReverse(wf);	//分词排序
	cout << "wfvec:" << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << ts.Utf8ToGBk(wfvec[i].first) << ":" << wfvec[i].second;
	}
	TextSimilarity::wordSet wset;
	//用排好序的词频挑选前n个候选词
	ts.selectAimWords(wfvec, wset);
	for (const auto& b : wset)
	{
		cout << ts.Utf8ToGBk(b) << " " << endl;
	}
	cout << endl;
	//根据候选词构建词频向量
	vector<double> oneHot = ts.getOneHot(wset, wf);
	cout << "oneHot:" << endl;
	for (const auto& c : oneHot)
	{
		cout << c << " ";
	}
	cout << endl;

}

void testOneHot()
{
	TextSimilarity ts("dict");
	TextSimilarity::wordFreq wf = ts.getWordFreq("test.txt");		//读取文件
	TextSimilarity::wordFreq wf2 = ts.getWordFreq("test1.txt");
	vector<pair<string, int>> wfvec = ts.sortByValueReverse(wf);	//分词排序
	vector<pair<string, int>> wfvec2 = ts.sortByValueReverse(wf2);
	
	cout << "wfvec:" << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << ts.Utf8ToGBk(wfvec[i].first) << ":" << wfvec[i].second;
	}
	cout << endl;
	cout << "wfvec2:" << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << ts.Utf8ToGBk(wfvec2[i].first) << ":" << wfvec2[i].second;
	}
	cout << endl;
	TextSimilarity::wordSet wset;
	//用排好序的词频挑选前n个候选词
	ts.selectAimWords(wfvec, wset);
	ts.selectAimWords(wfvec2, wset);
	cout << "wset:" << endl;
	for (const auto& b : wset)
	{
		cout << ts.Utf8ToGBk(b) << " " << endl;
	}
	cout << endl;
	//根据候选词构建词频向量
	vector<double> oneHot = ts.getOneHot(wset, wf);
	vector<double> oneHot2 = ts.getOneHot(wset, wf2);
	cout << "oneHot:" << endl;
	for (const auto& c : oneHot)
	{
		cout << c << " ";
	}
	cout << endl;
	cout << "oneHot2:" << endl;
	for (const auto& d : oneHot2)
	{
		cout << d << " ";
	}
	cout << endl;
	cout << "cosine:" << ts.cosine(oneHot, oneHot2) << endl;
	double ret = 0;
	ret = ts.cosine(oneHot, oneHot2) * 100;
	cout.setf(ios::fixed);
	cout << setprecision(2);
	cout << "两个文件的相似度为：" << ret << "%" << endl;

}

void testOnehot2()
{
	TextSimilarity ts("dict");
	TextSimilarity::wordFreq wf = ts.getWordFreq("test1.txt");		//读取文件
	TextSimilarity::wordFreq wf2 = ts.getWordFreq("test.txt");
	vector<pair<string, int>> wfvec = ts.sortByValueReverse(wf);	//分词排序
	vector<pair<string, int>> wfvec2 = ts.sortByValueReverse(wf2);
	TextSimilarity::wordSet wset;
	ts.selectAimWords(wfvec, wset);
	ts.selectAimWords(wfvec2, wset);
	vector<double> oneHot = ts.getOneHot(wset, wf);
	vector<double> oneHot2 = ts.getOneHot(wset, wf2);
	double ret = 0;
	ret = ts.cosine(oneHot, oneHot2) * 100;
	cout.setf(ios::fixed);
	cout << setprecision(2);
	cout << "两个文件的相似度为：" << ret << "%" << endl;
}
int main()
{
	testOneHot();
	//testOnehot2();
	//textWordFreq();
	return 0;
}