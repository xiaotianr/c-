// C++11内容总结.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <array> // vector
#include <forward_list>  // list
using namespace std;

/*
C++11 标准相关的内容 总结一下
一：关键字和语法
auto：可以根据右值，推导出右值的类型，然后左边变量的类型也就已知了
nullptr：给指针专用（能够和整数进行区别） #define NULL 0
foreach：可以遍历数组，容器等
for(Type val : container) => 底层就是通过指针或者迭代器来实现的
{
	cout<<val<<" ";
}
右值引用：move移动语义函数和forward类型完美转发函数
模板的一个新特性：typename... A  表示可变参（类型参数）

二：绑定器和函数对象
function：函数对象
bind：绑定器  bind1st和bind2nd+二元函数对象 =》 一元函数对象
lambda表达式

三：智能指针
shared_ptr和weak_ptr

四：容器
set和map：红黑树 O(lgn)
unordered_set和unordered_map：哈希表 O(1)
array：数组   vector
forward_list：前向链表   list

五：C++语言级别支持的多线程编程
createThread
pthread_create
clone
*/
