#include "pch.h"

#if 0
#include <iostream>
#include <vector>
using namespace std;

/*
��������ʧЧ���⣿
1.������Ϊʲô��ʧЧ��
a������������erase�����󣬵�ǰλ�õ�����ĩβԪ�ص����еĵ�����ȫ��ʧЧ��
b������������insert�����󣬵�ǰλ�õ�����ĩβԪ�ص����еĵ�����ȫ��ʧЧ��
     ��������Ȼ��Ч    ������ȫ��ʧЧ
��Ԫ��   ->  �����/ɾ����  ->  ĩβԪ��

c��insert��˵��������������ڴ�����
   ԭ�����������еĵ�������ȫ��ʧЧ��
��Ԫ��   ->  �����/ɾ����  ->  ĩβԪ��

d����ͬ�����ĵ������ǲ��ܽ��бȽ������

2.������ʧЧ���Ժ��������ô�����
�Բ���/ɾ����ĵ��������и��²���




*/
int main()
{
	vector<int> vec;
	for (int i = 0; i < 20; ++i)
	{
		vec.push_back(rand() % 100 + 1);
	}

	for (int v : vec)
	{
		cout << v << " ";
	}
	cout << endl;

#if 0
	// ��vec���������е�ż��ǰ�����һ��С��ż��ֵ1������
	auto it = vec.begin();
	for (; it != vec.end(); ++it)
	{
		if (*it % 2 == 0)
		{
			// ����ĵ������ڵ�һ��insert֮��iterator��ʧЧ��
			it = vec.insert(it, *it-1);
			++it;
			//break;
		}
	}
#endif

#if 0
	// ��vec���������е�ż��ȫ��ɾ��
	auto it = vec.begin();
	while (it != vec.end())
	{
		if (*it % 2 == 0)
		{
			// ������ʧЧ�����⣬��һ�ε���erase�Ժ󣬵�����it��ʧЧ��
			it = vec.erase(it); // insert(it, val)   erase(it)
			//break;
		}
		else
		{
			++it;
		}
	}
#endif

	for (int v : vec)
	{
		cout << v << " ";
	}
	cout << endl;

	return 0;
}
#endif