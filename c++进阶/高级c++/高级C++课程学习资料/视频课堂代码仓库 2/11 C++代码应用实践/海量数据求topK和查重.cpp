#include "pch.h"
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>
using namespace std;

#if 0
/*
������������غ�top K������ۺ�Ӧ��
���أ������Ƿ����ظ����Լ������ظ��Ĵ���
topK��Ԫ�ص�ֵǰK��/С����K��/С
��Ŀ�����ݵ��ظ��������/��С��ǰK��/��K��

��ϣͳ��(map) + ��/���ŷָ�


��һ�����ļ����ڴ�����200M�����ļ����ظ���������ǰ10��
���ļ� =�� С�ļ�
���ļ���������� =�� ��ϣӳ�� =�� ��������ɢ�ķ���С�ļ�����
*/

// ���ļ�����С�ļ�����ϣӳ�䣩+ ��ϣͳ�� + С����(���ŷָ�)
int main()
{
	FILE *pf1 = fopen("data.dat", "wb");
	for (int i = 0; i < 20000; ++i)
	{
		int data = rand();
		fwrite(&data, 4, 1, pf1);
	}
	fclose(pf1);

	// �򿪴洢���ݵ�ԭʼ�ļ�data.dat
	FILE *pf = fopen("data.dat", "rb");
	if (pf == nullptr)
		return 0;

	// ��������ԭʼ��������С�����������ļ����ֵĸ���Ҳ��С�ˣ�11��С�ļ�
	const int FILE_NO = 11;
	FILE *pfile[FILE_NO] = { nullptr };
	for (int i = 0; i < FILE_NO; ++i)
	{
		char filename[20];
		sprintf(filename, "data%d.dat", i + 1);
		pfile[i] = fopen(filename, "wb+");
	}

	// ��ϣӳ�䣬�Ѵ��ļ��е����ݣ�ӳ�䵽����С�ļ�����
	int data;
	while (fread(&data, 4, 1, pf) > 0)
	{
		int findex = data % FILE_NO;
		fwrite(&data, 4, 1, pfile[findex]);
	}

	// ����һ����ʽ��ϣ��
	unordered_map<int, int> numMap;
	// �ȶ���һ��С����
	using P = pair<int, int>;
	using FUNC = function<bool(P&, P&)>;
	using MinHeap = priority_queue<P, vector<P>, FUNC>;
	MinHeap minheap([](auto &a, auto &b)->bool {
		return a.second > b.second; // �Զ���С����Ԫ�ش�С�ȽϷ�ʽ
	});

	// �ֶ����С�ļ���top 10������֣���������ս��
	for (int i = 0; i < FILE_NO; ++i)
	{
		// �ָ�С�ļ����ļ�ָ�뵽��ʼλ��
		fseek(pfile[i], 0, SEEK_SET);

		// ����ֱ��ͳ���������ظ��Ĵ���
		while (fread(&data, 4, 1, pfile[i]) > 0)
		{
			numMap[data]++;
		}

		int k = 0;
		auto it = numMap.begin();

		// ������ǿյģ������ѷ�10������
		if (minheap.empty())
		{
			// �ȴ�map���ж�10�����ݵ�С�����У�����top 10��С���ѣ���С��Ԫ���ڶѶ�
			for (; it != numMap.end() && k < 10; ++it, ++k)
			{
				minheap.push(*it);
			}
		}

		// ��K+1��ĩβ��Ԫ�ؽ��б������ͶѶ�Ԫ�رȽ�
		for (; it != numMap.end(); ++it)
		{
			// ���map���е�ǰԪ���ظ��������ڣ��Ѷ�Ԫ�ص��ظ����������滻
			if (it->second > minheap.top().second)
			{
				minheap.pop();
				minheap.push(*it);
			}
		}

		// ��չ�ϣ��������һ��С�ļ�������ͳ��
		numMap.clear();
	}

	// ����ʣ�µľ����ظ���������ǰk��
	while (!minheap.empty())
	{
		auto &pair = minheap.top();
		cout << pair.first << " : " << pair.second << endl;
		minheap.pop();
	}

	return 0;
}
#endif

#if 0
// ��һ�������� ���ҳ��ظ���������ǰ10��
int main()
{
	// ��vec�洢Ҫ���������
	vector<int> vec;
	for (int i = 0; i < 200000; ++i)
	{
		vec.push_back(rand());
	}

	// ͳ���������ֵ��ظ�����,key:���ֵ�ֵ,value:�����ظ��Ĵ���
	unordered_map<int, int> numMap;
	for (int val : vec)
	{
		/* ��val������map�в��ң����val�����ڣ�numMap[val]�����һ��[val, 0]
		��ôһ������ֵ��Ȼ��++���õ�һ��[val, 1]��ôһ��������
		���val���ڣ�numMap[val]�պ÷��ص���val���ֶ�Ӧ��second�ظ��Ĵ�����ֱ��++*/
		numMap[val]++;
	}

	// �ȶ���һ��С����   ����=���ظ��Ĵ���
	using P = pair<int, int>;
	using FUNC = function<bool(P&, P&)>;
	using MinHeap = priority_queue<P, vector<P>, FUNC>;
	MinHeap minheap([](auto &a, auto &b)->bool {
		return a.second > b.second; // �Զ���С����Ԫ�صĴ�С�ȽϷ�ʽ
	});

	// �����ѷ�10������
	int k = 0;
	auto it = numMap.begin();

	// �ȴ�map���ж�10�����ݵ�С�����У�����top 10��С���ѣ���С��Ԫ���ڶѶ�
	for (; it != numMap.end() && k < 10; ++it, ++k)
	{
		minheap.push(*it);
	}

	// ��K+1��ĩβ��Ԫ�ؽ��б������ͶѶ�Ԫ�رȽ�
	for (; it != numMap.end(); ++it)
	{
		// ���map���е�ǰԪ���ظ��������ڣ��Ѷ�Ԫ�ص��ظ����������滻
		if (it->second > minheap.top().second)
		{
			minheap.pop();
			minheap.push(*it);
		}
	}
	// ����ʣ�µľ����ظ���������ǰk��
	while (!minheap.empty())
	{
		auto &pair = minheap.top();
		cout << pair.first << " : " << pair.second << endl;
		minheap.pop();
	}
	return 0;
}
#endif

#if 0
/*
����������top k������
1.������/��С��ǰK��Ԫ��
2.������/��С�ĵ�K��Ԫ��

10000����������ֵǰ10���Ԫ��


�ⷨ1.�����/С���� =�� ���ȼ�����priority_queue
����ǰ10����������һ��С���ѣ���Сֵ���ڶѶ�����Ȼ�����ʣ�µ���������������ȶѶ�Ԫ��
����ôɾ���Ѷ�Ԫ�أ����ѣ���Ȼ���ٰ�������ѣ�����������������С��������ŵľ���ֵ���
��ǰ10��Ԫ���ˣ�����ҵ��ǵ�kС������ѶѶ������ߵ�k��С���ѶѶ�����ֻ��Ҫ���ʶѶ�һ��Ԫ�ؾͿ�����

����� =�� ��top KС��
С���� =�� ��top K���

�ⷨ2.���ŷָ��
�������ŷָ�����ܹ���O(lgn)ʱ���ڣ���С�ڻ�׼����������������ߣ��Ѵ��ڻ�׼��������
�������ұߣ���׼����index���Ϳ�����Ϊ�ǵڣ�index+1��С�������� [0,(index)]����ǰindex+1
С��������
*/

/*
���ŷָ����ѡ��arr[i]��Ԫ����Ϊ��������С��arr[i]��Ԫ��
��������ߣ��Ѵ���arr[i]��Ԫ�ص������ұ߲����ػ���λ�õ��±�
*/
int partation(vector<int> &arr, int i, int j)
{
	int k = arr[i];
	while (i < j)
	{
		while (i < j && arr[j] >= k)
			j--;
		if (i < j)
			arr[i++] = arr[j];

		while (i < j && arr[i] < k)
			i++;
		if (i < j)
			arr[j--] = arr[i];
	}
	arr[i] = k;
	return i;
}
/*
params:
1.vector<int> &arr: �洢Ԫ�ص�����
2.int i:���ݷ�Χ����ʼ�±�
3.int j:���ݷ�Χ��ĩβ�±�
4.int k:��k��Ԫ��
����������ͨ�����ŷָ���ݹ�����kС�����֣�����������ֵ
*/
int selectNoK(vector<int> &arr, int i, int j, int k)
{
	int pos = partation(arr, i, j);
	if (pos == k - 1)
		return pos;
	else if (pos < k - 1)
		return selectNoK(arr, pos + 1, j, k);
	else
		return selectNoK(arr, i, pos - 1, k);
}
int main()
{
	/*
	��vector������Ԫ�ص�10С��Ԫ��ֵ  ǰ10С��
	*/
	vector<int> vec;
	for (int i = 0; i < 100000; ++i)
	{
		vec.push_back(rand() + i);
	}

	// selectNoK���صľ��ǵ�10С��Ԫ�ص�ֵ
	int pos = selectNoK(vec, 0, vec.size() - 1, 10);
	cout << vec[pos] << endl; // ��10С��
	// ���Ҫ��ǰ10С�ģ�[0,pos]

	// ��һ�����ļ�������ŵ����������ڴ�����200M��������ǰ10��
	/*
	���ε�˼����
	����һ�������ļ��Ĵ�С / 200M = Ҫ�ֵ�С�ļ�������

	��ϣӳ��  ���� % С�ļ��ĸ��� = file_index

	����ÿһ��С�ļ��Ϳ��Լ��ص��ڴ浱���ˣ���ÿһ��С�ļ���������top kԪ����

	https://blog.csdn.net/QIANGWEIYUAN/article/details/88815772
	*/
	return 0;
}
#endif


#if 0
int main()
{
	/*
	��vector������Ԫ��ֵ����ǰ10������
	*/
	vector<int> vec;
	for (int i = 0; i < 100000; ++i)
	{
		vec.push_back(rand() + i);
	}

	// �㷨��ʱ�临�Ӷȣ�O(n)
	// ����С����  priority_queue<int> maxHeap;
	priority_queue<int, vector<int>, greater<int>> minHeap;
	// ����С���ѷ���10��Ԫ��
	int k = 0;
	for (; k < 10; ++k)
	{
		minHeap.push(vec[k]);
	}

	/*
	����ʣ�µ�Ԫ�����κͶѶ�Ԫ�ؽ��бȽϣ�����ȶѶ�Ԫ�ش�
	��ôɾ���Ѷ�Ԫ�أ��ѵ�ǰԪ����ӵ�С�����У�Ԫ�ر�����ɣ�
	����ʣ�µ�10��Ԫ�أ�����ֵ����10��Ԫ��
	*/
	for (; k < vec.size(); ++k)
	{
		if (vec[k] > minHeap.top()) // O(log_2_10)
		{
			minHeap.pop();
			minHeap.push(vec[k]);
		}
	}

	// ��ӡ���   �������ǰK����������ҵ�K������ôֻ��ӡ�Ѷ�Ԫ�ؾͿ�����
	while (!minHeap.empty())
	{
		cout << minHeap.top() << " ";
		minHeap.pop();
	}
	cout << endl;

	return 0;
}
#endif







#if 0
/*
�������ݵ���TOP K�Ͳ���/ȥ�ص�����
�������ݵĲ�������:
1.��ϣ��  50�ڣ�5G����������20G+20G=40G��  10�ڣ�1G��  
2.����˼�� 
3.Bloom Filter����¡������
4.�ַ�������  TrieTree�ֵ���(ǰ׺��)
*/
int main()
{
	/*
	   #3 a,b�����ļ������涼��10�ڸ��������ڴ�����400M���������a��b�����ļ�����
	   �ظ���Ԫ������Щ��

	   10�� -> 1G*4 = 4G*2=8G/400M = 27��С�ļ�
	   a =�� ȫ�������ڴ浱�У��洢����ϣ��  
	   b =�� ��b�ļ������ݣ����ڴ��ϣ���в�ѯ

	   ��a��b�������ļ������ֳɸ�����ȵ�һϵ��(27)С�ļ������ε�˼�룩
	   a1.txt   b1.txt 
	   a2.txt   b2.txt
	   a3.txt   b3.txt
	   ...		...
	   a26.txt  b26.txt
	   
	   ��a�ļ��ж�ȡ���ݣ�ͨ�� ����%27 = file_index
	   ��b�ļ��ж�ȡ���ݣ�ͨ�� ����%27 = file_index

	   a��b�����ļ��У�������ͬ��Ԫ�أ����й�ϣӳ���Ժ󣬿϶�����ͬ��ŵ�С�ļ�����
	   a1��b1��a2��b2������a26��b26
	*/


	/* #2 ��һ���ļ����д��������� 50�ڸ��������ڴ�����400M��
	      �����ҳ��ļ����ظ���Ԫ�أ��ظ��Ĵ���
	      50�� 5G*4=20G*2 = 40G
		  ���η���˼�룺 ���ļ����ֳ�С�ļ���ʹ��ÿһ��С�ļ��ܹ����ص��ڴ浱�У�
		  �����Ӧ���ظ���Ԫ�أ��ѽ��д�뵽һ���洢�ظ�Ԫ�ص��ļ�����

		  ���ļ� =�� С�ļ��ĸ�����40G/400M = 120��С�ļ���
		  data0.txt
		  data1.txt
		  data2.txt
		  ...
		  data126.txt

		  �������ļ���Ԫ�أ���ÿһ��Ԫ�ظ��ݹ�ϣӳ�亯�����ŵ���Ӧ��ŵ�С�ļ�����
		  data % 127 = file_index

		  ֵ��ͬ�ģ�ͨ��һ���Ĺ�ϣӳ�亯�����϶��Ƿ���ͬһ��С�ļ����е�
	*/

	
	// #1 ������������ݲ��ص�˼��
#if 0 
	const int SIZE = 10000;
	int ar[SIZE] = { 0 };
	for (int i = 0; i < SIZE; ++i)
	{
		ar[i] = rand();
	}

	// ������SIZE�����������У��ҳ�˭�ظ��ˣ�����ͳ���ظ��Ĵ��� int
	unordered_map<int, int> map;
	for (int val : ar)
	{
		map[val]++;
	}

	for (auto pair : map)
	{
		if (pair.second > 1)
		{
			cout << "���֣�" << pair.first << " �ظ�����:" << pair.second << endl;
		}
	}
#endif
	return 0;
}
#endif