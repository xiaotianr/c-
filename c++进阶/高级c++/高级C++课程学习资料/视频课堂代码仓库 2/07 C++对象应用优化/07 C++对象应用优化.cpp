﻿// 07 C++对象应用优化.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
using namespace std;

#if 0
class Test
{
public:
	// Test() Test(10) Test(10, 10)
	Test(int a = 5, int b = 5) 
		:ma(a), mb(b)
	{
		cout << "Test(int, int)" << endl;
	}
	~Test()
	{
		cout << "~Test()" << endl;
	}
	Test(const Test &src) 
		:ma(src.ma), mb(src.mb)
	{
		cout << "Test(const Test&)" << endl;
	}
	void operator=(const Test &src)
	{
		ma = src.ma; 
		mb = src.mb; 
		cout << "operator=" << endl;
	}
private:
	int ma;
	int mb;
};
Test t1(10, 10); // 1.Test(int, int)
int main()
{
	Test t2(20, 20); // 3.Test(int, int)
	Test t3 = t2; // 4.Test(const Test&)
	// static Test t4(30, 30);
	static Test t4 = Test(30, 30); // 5.Test(int, int)
	t2 = Test(40, 40); // 6.Test(int, int) operator= ~Test()
	// (50, 50) =  (Test)50; Test(int)
	t2 = (Test)(50, 50); // 7.Test(int,int) operator=  ~Test()
	t2 = 60; //Test(int) 8.Test(int,int) operator= ~Test()
	Test *p1 = new Test(70, 70); // 9. Test(int,int) 
	Test *p2 = new Test[2]; // 10. Test(int,int) Test(int,int)
	Test *p3 = &Test(80, 80); // 11. Test(int,int)  ~Test()
	const Test &p4 = Test(90, 90); // 12. Test(int,int)
	delete p1; // 13.~Test()
	delete[]p2; // 14. ~Test() ~Test()
}
Test t5(100, 100); // 2.Test(int, int)






class Test
{
public:
	Test(int a = 10) :ma(a) 
	{ cout << "Test(int)" << endl; }
	~Test() 
	{ cout << "~Test()" << endl; }
	Test(const Test &t) :ma(t.ma) 
	{ cout << "Test(const Test&)" << endl; }
	Test& operator=(const Test &t)
	{
		cout << "operator=" << endl;
		ma = t.ma;
		return *this;
	}
private:
	int ma;
};
int main()
{
	Test t1;
	Test t2(t1);
	Test t3 = t1;
	// Test(20) 显示生成临时对象  生存周期：所在的语句
	/*
	C++编译器对于对象构造的优化：用临时对象生成新对象的时候，临时对象
	就不产生了，直接构造新对象就可以了
	*/
	Test t4 = Test(20); // Test t4(20);没有区别的！
	cout << "--------------" << endl;

	// t4.operator=(t2)
	t4 = t2;
	// t4.operator=(const Test &t)
	// 显式生成临时对象
	t4 = Test(30);
	t4 = (Test)30; // int->Test(int)

	// 隐式生成临时对象
	t4 = 30; // Test(30) int->Test(int)    char*->Test(char*)

	cout << "--------------" << endl;
	Test *p = &Test(40);
	// p指向的是一个已经析构的临时对象
	const Test &ref = Test(50);
	cout << "--------------" << endl;
	return 0;
}
#endif

/*
》》》》C++高级课程内容提纲《《《《
一：对象的应用优化、右值引用的优化
二：智能指针
三：绑定器和函数对象、lambda表达式
四：C++11内容汇总、多线程应用实践
五：设计模式
六：面向对象编程实践
1.深度遍历搜索迷宫路径
2.广度遍历搜索迷宫路径找最短路径
3.大数加减法
4.海量数据查重以及求top k问题
5.数字化男女匹配问题
七：校招C++面经讲解
八：应聘C++研发岗简历该怎么写
*/