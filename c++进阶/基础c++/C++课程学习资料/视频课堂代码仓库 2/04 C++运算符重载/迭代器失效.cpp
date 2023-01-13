#include "pch.h"

#if 0
#include <iostream>
#include <vector>
using namespace std;

/*
迭代器失效问题？
1.迭代器为什么会失效？
a当容器调用erase后，当前元素到容器末尾的所有迭代器全部失效了
b当容器调用insert方法后，当前位置到容器末尾的所有元素的所有迭代器全部失效了
     迭代器依然有效   迭代器全部失效
首元素   ->  插入点/删除点  ->  末尾元素

c对于insert来说，如果引起容器内存扩容
   原来容器的所有迭代器就全部失效了
首元素   ->  插入点  ->  末尾元素

d不同容器的迭代器是不能进行比较运算的

2.迭代器失效后，问题该怎么解决？
对插入/删除点的迭代器进行更新操作




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
	// 给vec容器中所有偶数前面添加一个小于偶数值1的数字
	auto it = vec.begin();
	for (; it != vec.end(); ++it)
	{
		if (*it % 2 == 0)
		{
			// 这里的迭代器在第一次insert之后，iterator就失效了
			it = vec.insert(it, *it-1);
			++it;
			//break;
			//因为insert会返回当前元素新的迭代器，因此需要用新的迭代器更新it，从而避免迭代器的失效问题
		}
	}
#endif

#if 0
	// 吧vec容器中的所有偶数全部删除
	auto it = vec.begin();
	while (it != vec.end())
	{
		if (*it % 2 == 0)
		{
			// 迭代器失效的问题，第一次调用erase以后，迭代器it就失效了
			it = vec.erase(it); // insert(it, val)   erase(it)
			//break;
			//因为erase会返回当前元素新的迭代器，因此需要用新的迭代器更新it，从而避免迭代器的失效问题
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