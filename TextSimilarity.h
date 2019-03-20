#pragma once
#include <unordered_map>
#include <string>
#include <cppjieba/Jieba.hpp>
class TextSimilarity
{
public:
	typedef std::unordered_map<std::string, int> wordFreq;
	typedef std::unordered_set<std::string> wordSet;
	TextSimilarity(std::string);

	wordFreq getWordFreq(const char* file);			//获取词频
	std::string GBKToUTF8(std::string str);			//编码转换
	std::string Utf8ToGBk(std::string str);
	std::vector<std::pair<std::string, int>> sortByValueReverse(TextSimilarity::wordFreq& wf);					//分词排序
	void selectAimWords(std::vector<std::pair<std::string, int>>& wfvec, wordSet& wset);		//将关键词存放到set中，因为set是自动取宠
	std::vector<double> getOneHot(wordSet& wset, wordFreq& wf);//需要set中的词汇，需要map中的词频
	double cosine(std::vector<double> oneHot1, std::vector<double> oneHot2);
	
private:

	void getstopWrodTable(const char* stopWordsSet);						//停用词表
	//void selectAimWords(std::vector<std::pair<std::string, int>>& wfvec, wordSet wset);		//将关键词存放到set中，因为set是自动取宠
	//std::vector<int> getOneHot(wordSet& wset, wordFreq wf);//需要set中的词汇，需要map中的词频
	
	//初始化jiaba为字符串类型
	std::string DICT;
	std::string DICT_PATH;
	std::string HMM_PATH;
	std::string USER_DICT_PATH;
	std::string IDF_PATH;
	std::string STOP_WORD_PATH;
	cppjieba::Jieba _jieba;

	wordSet _stopWordsSet;							//定义停用词表
	int MaxWordsNumber;								//定义最大候选词

};