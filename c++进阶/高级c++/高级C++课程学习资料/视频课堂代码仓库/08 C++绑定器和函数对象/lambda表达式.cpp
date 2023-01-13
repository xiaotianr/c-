#include "pch.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include <map>
#include <memory>
#include <queue>
using namespace std;

/*
既然lambda表达式只能使用在语句当中，如果想跨语句使用之前定义好的lambda表达式，
怎么办？用什么类型来表示lambda表达式？
当然是用function类型来表示函数对象的类型了   bind1st/bind2nd bind绑定器

lambda表达式 =》 函数对象   
*/

class Data
{
public:
	Data(int val1 = 10, int val2 = 10) :ma(val1), mb(val2) {}
	//bool operator>(const Data &data)const { return ma > data.ma; }
	//bool operator<(const Data &data)const { return ma < data.ma; }
	int ma;
	int mb;
};
int main()
{
	// 优先级队列
	//priority_queue<Data> queue;
	using FUNC = function<bool(Data&, Data&)>;
	priority_queue<Data, vector<Data>, FUNC> 
		maxHeap([](Data &d1, Data &d2)->bool
		{
			return d1.mb > d2.mb;
		});
	maxHeap.push(Data(10, 20));
	maxHeap.push(Data(15, 15));
	maxHeap.push(Data(20, 10));


	// 智能指针自定义删除器   delete p;  FILE*   fclose(FILE*)
	//unique_ptr<FILE, function<void(FILE*)>> 
		//ptr1(fopen("data.txt", "w"), [](FILE *pf) {fclose(pf); });


#if 0
	map<int, function<int(int, int)>> caculateMap;
	caculateMap[1] = [](int a, int b)->int {return a + b; };
	caculateMap[2] = [](int a, int b)->int {return a - b; };
	caculateMap[3] = [](int a, int b)->int {return a * b; };
	caculateMap[4] = [](int a, int b)->int {return a / b; };

	cout << "选择:";
	int choice;
	cin >> choice;
	cout << "10 + 15:" << caculateMap[choice](10, 15) << endl;
#endif
	return 0;
}


#if 0
/*
C++11 函数对象的升级版 =》 lambda表达式
函数对象的缺点：
使用在泛型算法参数传递   比较性质/自定义操作  优先级队列  智能指针

lambda表达式的语法：
[捕获外部变量](形参列表)->返回值{操作代码};

如果lambda表达式的返回值不需要，那么"->返回值"可以省略的

[捕获外部变量]
[]：表示不捕获任何外部变量
[=]：以传值的方式捕获外部的所有变量
[&]：以传引用的方式捕获外部的所有变量
[this]：捕获外部的this指针
[=,&a]：以传值的方式捕获外部的所有变量，但是a变量以传引用的方式捕获
[a, b]：以值传递的方式捕获外部变量a和b
[a, &b]：a以值传递捕获，b以传引用的方式捕获
*/
template<typename T=void>
class TestLambda01
{
public:
	TestLambda01() {}
	void operator()()const
	{
		cout << "hello world!" << endl;
	}
};
template<typename T = int>
class TestLambda02
{
public:
	TestLambda02() {}
	int operator()(int a, int b)const
	{
		return a+b;
	}
};
template<typename T = int>
class TestLambda03
{
public:
	TestLambda03(int &a, int &b):ma(a), mb(b) {}
	void operator()()const
	{
		int tmp = ma;
		ma = mb;
		mb = tmp;
	}
private:
	int &ma;
	int &mb;
};
int main()
{
	vector<int> vec;
	for (int i = 0; i < 20; ++i)
	{
		vec.push_back(rand() % 100 + 1);
	}

	sort(vec.begin(), vec.end(), 
		[](int a, int b)->bool
		{
			return a > b;
		});
	for (int val : vec)
	{
		cout << val << " ";
	}
	cout << endl;

	// 65按序插入序列  要找第一个小于65的数字
	auto it = find_if(vec.begin(), vec.end(), 
		[](int a)->bool {return a < 65; });
	if (it != vec.end())
	{
		vec.insert(it, 65);
	}
	for (int val : vec)
	{
		cout << val << " ";
	}
	cout << endl;

	for_each(vec.begin(), vec.end(), [](int a)
	{
		if(a % 2 == 0)
			cout << a << " ";
	});
	cout << endl;

#if 0
	auto func1 = [](){cout << "hello world!" << endl; };
	func1();

	auto func2 = [](int a, int b)->int {return a + b; };
	cout<<func2(20, 30)<<endl;

	int a = 10;
	int b = 20;
	auto func3 = [&]() // “a”: 无法在非可变 lambda 中修改通过复制捕获
	{
		int tmp = a;
		a = b;
		b = tmp;
	};
	func3();
	cout << "a:" << a << " b:" << b << endl;

	cout << "-----------------------" << endl;

	TestLambda01<> t1;
	t1();

	TestLambda02<> t2;
	cout << t2(20, 30) << endl;

	TestLambda03<> t3(a, b);
	t3();
#endif
	return 0;
}
#endif