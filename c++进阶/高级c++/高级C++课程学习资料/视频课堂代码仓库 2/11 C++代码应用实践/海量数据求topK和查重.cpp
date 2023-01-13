#include "pch.h"
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>
using namespace std;

#if 0
/*
海量数据求查重和top K问题的综合应用
查重：数据是否有重复，以及数据重复的次数
topK：元素的值前K大/小，第K大/小
题目：数据的重复次数最大/最小的前K个/第K个

哈希统计(map) + 堆/快排分割


有一个大文件，内存限制200M，求文件中重复次数最多的前10个
大文件 =》 小文件
大文件里面的数据 =》 哈希映射 =》 把数据离散的放入小文件当中
*/

// 大文件划分小文件（哈希映射）+ 哈希统计 + 小根堆(快排分割)
int main()
{
	FILE *pf1 = fopen("data.dat", "wb");
	for (int i = 0; i < 20000; ++i)
	{
		int data = rand();
		fwrite(&data, 4, 1, pf1);
	}
	fclose(pf1);

	// 打开存储数据的原始文件data.dat
	FILE *pf = fopen("data.dat", "rb");
	if (pf == nullptr)
		return 0;

	// 这里由于原始数据量缩小，所以这里文件划分的个数也变小了，11个小文件
	const int FILE_NO = 11;
	FILE *pfile[FILE_NO] = { nullptr };
	for (int i = 0; i < FILE_NO; ++i)
	{
		char filename[20];
		sprintf(filename, "data%d.dat", i + 1);
		pfile[i] = fopen(filename, "wb+");
	}

	// 哈希映射，把大文件中的数据，映射到各个小文件当中
	int data;
	while (fread(&data, 4, 1, pf) > 0)
	{
		int findex = data % FILE_NO;
		fwrite(&data, 4, 1, pfile[findex]);
	}

	// 定义一个链式哈希表
	unordered_map<int, int> numMap;
	// 先定义一个小根堆
	using P = pair<int, int>;
	using FUNC = function<bool(P&, P&)>;
	using MinHeap = priority_queue<P, vector<P>, FUNC>;
	MinHeap minheap([](auto &a, auto &b)->bool {
		return a.second > b.second; // 自定义小根堆元素大小比较方式
	});

	// 分段求解小文件的top 10大的数字，并求出最终结果
	for (int i = 0; i < FILE_NO; ++i)
	{
		// 恢复小文件的文件指针到起始位置
		fseek(pfile[i], 0, SEEK_SET);

		// 这里直接统计了数字重复的次数
		while (fread(&data, 4, 1, pfile[i]) > 0)
		{
			numMap[data]++;
		}

		int k = 0;
		auto it = numMap.begin();

		// 如果堆是空的，先往堆方10个数据
		if (minheap.empty())
		{
			// 先从map表中读10个数据到小根堆中，建立top 10的小根堆，最小的元素在堆顶
			for (; it != numMap.end() && k < 10; ++it, ++k)
			{
				minheap.push(*it);
			}
		}

		// 把K+1到末尾的元素进行遍历，和堆顶元素比较
		for (; it != numMap.end(); ++it)
		{
			// 如果map表中当前元素重复次数大于，堆顶元素的重复次数，则替换
			if (it->second > minheap.top().second)
			{
				minheap.pop();
				minheap.push(*it);
			}
		}

		// 清空哈希表，进行下一个小文件的数据统计
		numMap.clear();
	}

	// 堆中剩下的就是重复次数最大的前k个
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
// 在一组数字中 ，找出重复次数最多的前10个
int main()
{
	// 用vec存储要处理的数字
	vector<int> vec;
	for (int i = 0; i < 200000; ++i)
	{
		vec.push_back(rand());
	}

	// 统计所有数字的重复次数,key:数字的值,value:数字重复的次数
	unordered_map<int, int> numMap;
	for (int val : vec)
	{
		/* 拿val数字在map中查找，如果val不存在，numMap[val]会插入一个[val, 0]
		这么一个返回值，然后++，得到一个[val, 1]这么一组新数据
		如果val存在，numMap[val]刚好返回的是val数字对应的second重复的次数，直接++*/
		numMap[val]++;
	}

	// 先定义一个小根堆   数字=》重复的次数
	using P = pair<int, int>;
	using FUNC = function<bool(P&, P&)>;
	using MinHeap = priority_queue<P, vector<P>, FUNC>;
	MinHeap minheap([](auto &a, auto &b)->bool {
		return a.second > b.second; // 自定义小根堆元素的大小比较方式
	});

	// 先往堆放10个数据
	int k = 0;
	auto it = numMap.begin();

	// 先从map表中读10个数据到小根堆中，建立top 10的小根堆，最小的元素在堆顶
	for (; it != numMap.end() && k < 10; ++it, ++k)
	{
		minheap.push(*it);
	}

	// 把K+1到末尾的元素进行遍历，和堆顶元素比较
	for (; it != numMap.end(); ++it)
	{
		// 如果map表中当前元素重复次数大于，堆顶元素的重复次数，则替换
		if (it->second > minheap.top().second)
		{
			minheap.pop();
			minheap.push(*it);
		}
	}
	// 堆中剩下的就是重复次数最大的前k个
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
海量数据求top k的问题
1.求最大的/最小的前K个元素
2.求最大的/最小的第K个元素

10000个整数，找值前10大的元素


解法1.大根堆/小根堆 =》 优先级队列priority_queue
先用前10个整数创建一个小根堆（最小值就在堆顶），然后遍历剩下的整数，如果整数比堆顶元素
大，那么删除堆顶元素（出堆），然后再把整数入堆，遍历完所有整数，小根堆里面放的就是值最大
的前10个元素了；如果找的是第k小（大根堆堆顶）或者第k大（小根堆堆顶），只需要访问堆顶一个元素就可以了

大根堆 =》 找top K小的
小根堆 =》 找top K大的

解法2.快排分割函数
经过快排分割函数，能够在O(lgn)时间内，把小于基准数的整数调整到左边，把大于基准数的整数
调整到右边，基准数（index）就可以认为是第（index+1）小的整数了 [0,(index)]就是前index+1
小的整数了
*/

/*
快排分割函数，选择arr[i]号元素作为基数，把小于arr[i]的元素
调整到左边，把大于arr[i]的元素调整到右边并返回基数位置的下标
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
1.vector<int> &arr: 存储元素的容器
2.int i:数据范围的起始下标
3.int j:数据范围的末尾下标
4.int k:第k个元素
功能描述：通过快排分割函数递归求解第k小的数字，并返回它的值
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
	求vector容器中元素第10小的元素值  前10小的
	*/
	vector<int> vec;
	for (int i = 0; i < 100000; ++i)
	{
		vec.push_back(rand() + i);
	}

	// selectNoK返回的就是第10小的元素的值
	int pos = selectNoK(vec, 0, vec.size() - 1, 10);
	cout << vec[pos] << endl; // 第10小的
	// 如果要找前10小的，[0,pos]

	// 有一个大文件，里面放的是整数，内存限制200M，求最大的前10个
	/*
	分治的思想了
	计算一下整数文件的大小 / 200M = 要分的小文件的数量

	哈希映射  整数 % 小文件的个数 = file_index

	现在每一个小文件就可以加载到内存当中了，对每一个小文件的整数求top k元素了

	https://blog.csdn.net/QIANGWEIYUAN/article/details/88815772
	*/
	return 0;
}
#endif


#if 0
int main()
{
	/*
	求vector容器中元素值最大的前10个数字
	*/
	vector<int> vec;
	for (int i = 0; i < 100000; ++i)
	{
		vec.push_back(rand() + i);
	}

	// 算法的时间复杂度：O(n)
	// 定义小根堆  priority_queue<int> maxHeap;
	priority_queue<int, vector<int>, greater<int>> minHeap;
	// 先往小根堆放入10个元素
	int k = 0;
	for (; k < 10; ++k)
	{
		minHeap.push(vec[k]);
	}

	/*
	遍历剩下的元素依次和堆顶元素进行比较，如果比堆顶元素大，
	那么删除堆顶元素，把当前元素添加到小根堆中，元素遍历完成，
	堆中剩下的10个元素，就是值最大的10个元素
	*/
	for (; k < vec.size(); ++k)
	{
		if (vec[k] > minHeap.top()) // O(log_2_10)
		{
			minHeap.pop();
			minHeap.push(vec[k]);
		}
	}

	// 打印结果   这个是找前K个，如果是找第K个，那么只打印堆顶元素就可以了
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
海量数据的求TOP K和查重/去重的问题
海量数据的查重问题:
1.哈希表  50亿（5G）个整数（20G+20G=40G）  10亿（1G）  
2.分治思想 
3.Bloom Filter：布隆过滤器
4.字符串类型  TrieTree字典树(前缀树)
*/
int main()
{
	/*
	   #3 a,b两个文件，里面都有10亿个整数，内存限制400M，让你求出a，b两个文件当中
	   重复的元素有哪些？

	   10亿 -> 1G*4 = 4G*2=8G/400M = 27个小文件
	   a =》 全部读到内存当中，存储到哈希表  
	   b =》 从b文件度数据，在内存哈希表中查询

	   把a和b两个大文件，划分成个数相等的一系列(27)小文件（分治的思想）
	   a1.txt   b1.txt 
	   a2.txt   b2.txt
	   a3.txt   b3.txt
	   ...		...
	   a26.txt  b26.txt
	   
	   从a文件中读取数据，通过 数据%27 = file_index
	   从b文件中读取数据，通过 数据%27 = file_index

	   a和b两个文件中，数据相同的元素，进行哈希映射以后，肯定在相同序号的小文件当中
	   a1和b1，a2和b2。。。a26和b26
	*/


	/* #2 有一个文件，有大量的整数 50亿个整数，内存限制400M，
	      让你找出文件中重复的元素，重复的次数
	      50亿 5G*4=20G*2 = 40G
		  分治法的思想： 大文件划分成小文件，使得每一个小文件能够加载到内存当中，
		  求出对应的重复的元素，把结果写入到一个存储重复元素的文件当中

		  大文件 =》 小文件的个数（40G/400M = 120个小文件）
		  data0.txt
		  data1.txt
		  data2.txt
		  ...
		  data126.txt

		  遍历大文件的元素，把每一个元素根据哈希映射函数，放到对应序号的小文件当中
		  data % 127 = file_index

		  值相同的，通过一样的哈希映射函数，肯定是放在同一个小文件当中的
	*/

	
	// #1 考察基本的数据查重的思想
#if 0 
	const int SIZE = 10000;
	int ar[SIZE] = { 0 };
	for (int i = 0; i < SIZE; ++i)
	{
		ar[i] = rand();
	}

	// 在上面SIZE的数据量当中，找出谁重复了，并且统计重复的次数 int
	unordered_map<int, int> map;
	for (int val : ar)
	{
		map[val]++;
	}

	for (auto pair : map)
	{
		if (pair.second > 1)
		{
			cout << "数字：" << pair.first << " 重复次数:" << pair.second << endl;
		}
	}
#endif
	return 0;
}
#endif