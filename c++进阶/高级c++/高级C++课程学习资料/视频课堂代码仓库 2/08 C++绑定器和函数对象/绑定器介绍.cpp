#include "pch.h"
#include <iostream>
#include <typeinfo>
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <thread>
using namespace std;
using namespace placeholders;

/*
C++11 bind绑定器 => 返回的结果还是一个函数对象
*/

/*
void hello(string str) { cout << str << endl; }
int sum(int a, int b) { return a + b; }
class Test
{
public:
	int sum(int a, int b) { return a + b; }
};
int main()
{
	// bind是函数模板 可以自动推演模板类型参数
	bind(hello, "hello bind!")();
	cout << bind(sum, 10, 20)() << endl;
	cout << bind(&Test::sum, Test(), 20, 30)() << endl;

	// 参数占位符  绑定器出了语句，无法继续使用
	bind(hello, _1)("hello bind 2!");
	cout << bind(sum, _1, _2)(200, 300) << endl;

	// 此处把bind返回的绑定器binder就复用起来了
	function<void(string)> func1 = bind(hello, _1);
	func1("hello china!");
	func1("hello shan xi!");
	func1("hello si chuan!");

	return 0;
}
*/

/*
muduo源码文件 threadpool.cc  thread.cc  bind function
*/

#if 0
// 线程类
class Thread
{
public:
	Thread(function<void(int)> func, int no) :_func(func), _no(no) {}
	thread start()
	{
		thread t(_func, _no); // _func(_no)
		return t;
	}
private:
	function<void(int)> _func;
	int _no;
};

// 线程池类
class ThreadPool
{
public:
	ThreadPool() {}
	~ThreadPool()
	{
		// 释放Thread对象占用的堆资源
		for (int i = 0; i < _pool.size(); ++i)
		{
			delete _pool[i];
		}
	}
	// 开启线程池
	void startPool(int size)
	{
		for (int i = 0; i < size; ++i)
		{
			_pool.push_back(
				new Thread(bind(&ThreadPool::runInThread, this, _1), i));
		}

		for (int i = 0; i < size; ++i)
		{
			_handler.push_back(_pool[i]->start());
		}

		for (thread &t : _handler)
		{
			t.join();
		}
	}
private:
	vector<Thread*> _pool;
	vector<thread> _handler;

	// 把runInThread这个成员方法充当线程函数  thread   pthread_create
	void runInThread(int id)
	{
		cout << "call runInThread! id:" << id << endl;
	}
};

int main()
{
	ThreadPool pool;
	pool.startPool(10);
	return 0;
}
#endif