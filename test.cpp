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

	
	//�ִ�����
	vector<pair<string, int>> wfvec = ts.sortByValueReverse(wf);
	for (const auto& a : wf)
	{
		cout << ts.Utf8ToGBk(a.first) << ":" << a.second << endl;
	}


}

void testOneHot1()
{
	TextSimilarity ts("dict");
	TextSimilarity::wordFreq wf = ts.getWordFreq("test.txt");		//��ȡ�ļ�
	vector<pair<string, int>> wfvec = ts.sortByValueReverse(wf);	//�ִ�����
	cout << "wfvec:" << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << ts.Utf8ToGBk(wfvec[i].first) << ":" << wfvec[i].second;
	}
	TextSimilarity::wordSet wset;
	//���ź���Ĵ�Ƶ��ѡǰn����ѡ��
	ts.selectAimWords(wfvec, wset);
	for (const auto& b : wset)
	{
		cout << ts.Utf8ToGBk(b) << " " << endl;
	}
	cout << endl;
	//���ݺ�ѡ�ʹ�����Ƶ����
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
	TextSimilarity::wordFreq wf = ts.getWordFreq("test.txt");		//��ȡ�ļ�
	TextSimilarity::wordFreq wf2 = ts.getWordFreq("test1.txt");
	vector<pair<string, int>> wfvec = ts.sortByValueReverse(wf);	//�ִ�����
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
	//���ź���Ĵ�Ƶ��ѡǰn����ѡ��
	ts.selectAimWords(wfvec, wset);
	ts.selectAimWords(wfvec2, wset);
	cout << "wset:" << endl;
	for (const auto& b : wset)
	{
		cout << ts.Utf8ToGBk(b) << " " << endl;
	}
	cout << endl;
	//���ݺ�ѡ�ʹ�����Ƶ����
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
	cout << "�����ļ������ƶ�Ϊ��" << ret << "%" << endl;

}

void testOnehot2()
{
	TextSimilarity ts("dict");
	TextSimilarity::wordFreq wf = ts.getWordFreq("test1.txt");		//��ȡ�ļ�
	TextSimilarity::wordFreq wf2 = ts.getWordFreq("test.txt");
	vector<pair<string, int>> wfvec = ts.sortByValueReverse(wf);	//�ִ�����
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
	cout << "�����ļ������ƶ�Ϊ��" << ret << "%" << endl;
}
int main()
{
	testOneHot();
	//testOnehot2();
	//textWordFreq();
	return 0;
}