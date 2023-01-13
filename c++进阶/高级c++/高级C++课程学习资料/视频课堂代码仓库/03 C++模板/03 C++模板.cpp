// 03 C++模板.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
using namespace std;

/*
类模板 =》 实现一个C++ STL里面的一个顺序容器 vector 向量容器
SeqStack
Queue
容器:
空间配置器allocator

template<class _Ty,
	class _Alloc = allocator<_Ty>>
	class vector

容器的空间配置器allocator 做四件事情 内存开辟/内存释放   对象构造/对象析构
*/
// 定义容器的空间配置器，和C++标准库的allocator实现一样
template<typename T>
struct Allocator
{
	T* allocate(size_t size) // 负责内存开辟
	{
		return (T*)malloc(sizeof(T) * size);
	}
	void deallocate(void *p) // 负责内存释放
	{
		free(p);
	}
	void construct(T *p, const T &val) // 负责对象构造
	{
		new (p) T(val); // 定位new
	}
	void destroy(T *p) // 负责对象析构
	{
		p->~T(); // ~T()代表了T类型的析构函数
	}
};

/*
容器底层内存开辟，内存释放，对象构造和析构，都通过allocator空间配置器来实现
*/
template<typename T, typename Alloc = Allocator<T>>
class vector
{
public:
	vector(int size = 10)
	{
		// 需要把内存开辟和对象构造分开处理
		//_first = new T[size];
		_first = _allocator.allocate(size);
		_last = _first;
		_end = _first + size;
	}
	~vector()
	{
		// 析构容器有效的元素，然后释放_first指针指向的堆内存
		// delete[]_first;
		for (T *p = _first; p != _last; ++p)
		{
			_allocator.destroy(p); // 把_first指针指向的数组的有效元素进行析构操作
		}
		_allocator.deallocate(_first); // 释放堆上的数组内存
		_first = _last = _end = nullptr;
	}
	vector(const vector<T> &rhs)
	{
		int size = rhs._end - rhs._first;
		//_first = new T[size];
		_first = _allocator.allocate(size);
		int len = rhs._last - rhs._first;
		for (int i = 0; i < len; ++i)
		{
			//_first[i] = rhs._first[i];
			_allocator.construct(_first+i, rhs._first[i]);
		}
		_last = _first + len;
		_end = _first + size;
	}
	vector<T>& operator=(const vector<T> &rhs)
	{
		if (this == &rhs)
			return *this;

		//delete[]_first;
		for (T *p = _first; p != _last; ++p)
		{
			_allocator.destroy(p); // 把_first指针指向的数组的有效元素进行析构操作
		}
		_allocator.deallocate(_first);

		int size = rhs._end - rhs._first;
		//_first = new T[size];
		_first = _allocator.allocate(size);
		int len = rhs._last - rhs._first;
		for (int i = 0; i < len; ++i)
		{
			//_first[i] = rhs._first[i];
			_allocator.construct(_first + i, rhs._first[i]);
		}
		_last = _first + len;
		_end = _first + size;
		return *this;
	}
	void push_back(const T &val) // 向容器末尾添加元素
	{
		if (full())
			expand();
		//*_last++ = val;   _last指针指向的内存构造一个值为val的对象
		_allocator.construct(_last, val);
		_last++;
	}
	void pop_back() // 从容器末尾删除元素
	{
		if (empty())
			return;
		//--_last; // 不仅要把_last指针--，还需要析构删除的元素
		--_last;
		_allocator.destroy(_last);
	}
	T back()const // 返回容器末尾的元素的值
	{
		return *(_last - 1);
	}
	bool full()const { return _last == _end; }
	bool empty()const { return _first == _last; }
	int size()const { return _last - _first; }
private:
	T *_first; // 指向数组起始的位置
	T *_last;  // 指向数组中有效元素的后继位置
	T *_end;   // 指向数组空间的后继位置
	Alloc _allocator; // 定义容器的空间配置器对象

	void expand() // 容器的二倍扩容
	{
		int size = _end - _first;
		//T *ptmp = new T[2 * size];
		T *ptmp = _allocator.allocate(2*size);
		for (int i = 0; i < size; ++i)
		{
			//ptmp[i] = _first[i];
			_allocator.construct(ptmp+i, _first[i]);
		}
		//delete[]_first;
		for (T *p = _first; p != _last; ++p)
		{
			_allocator.destroy(p);
		}
		_allocator.deallocate(_first);
		_first = ptmp;
		_last = _first + size;
		_end = _first + 2 * size;
	}
};

class Test
{
public:
	Test() { cout << "Test()" << endl; }
	~Test() { cout << "~Test()" << endl; }
	Test(const Test&) { cout << "Test(const Test&)" << endl; }
};
int main()
{
	Test t1, t2, t3;
	cout << "-------------------" << endl;
	vector<Test> vec;
	vec.push_back(t1);
	vec.push_back(t2);
	vec.push_back(t3);
	cout << "-------------------" << endl;
	vec.pop_back(); // 只需要析构对象。 要把对象的析构和内存释放分离开  delete
	cout << "-------------------" << endl;

	return 0;
}



#if 0
/*
函数模板 
模板的非类型参数 必须是整数类型（整数或者地址/引用都可以）都是常量，只能使用，而不能修改
类模板 => 实例化 => 模板类
*/


// 类模板
template<typename T=int>
class SeqStack // 模板名称+类型参数列表 = 类名称
{
public:
	// 构造和析构函数名不用加<T>，其它出现模板的地方都加上类型参数列表
	SeqStack(int size = 10)
		: _pstack(new T[size])
		, _top(0)
		, _size(size)
	{}
	~SeqStack()
	{
		delete[]_pstack;
		_pstack = nullptr;
	}
	SeqStack(const SeqStack<T> &stack)
		:_top(stack._top)
		,_size(stack._size)
	{
		_pstack = new T[_size];
		// 不要用memcopy进行拷贝
		for (int i = 0; i < _top; ++i)
		{
			_pstack[i] = stack._pstack[i];
		}
	}
	SeqStack<T>& operator=(const SeqStack<T> &stack)
	{
		if (this == &stack)
			return *this;

		delete[]_pstack;

		_top = stack._top;
		_size = stack._size;
		_pstack = new T[_size];
		// 不要用memcopy进行拷贝
		for (int i = 0; i < _top; ++i)
		{
			_pstack[i] = stack._pstack[i];
		}
		return *this;
	}

	void push(const T &val); // 入栈操作

	void pop() // 出栈操作
	{
		if (empty())
			return;
		--_top;
	}
	T top()const // 返回栈顶元素
	{
		if (empty())
			throw "stack is empty!"; // 抛异常也代表函数逻辑结束
		return _pstack[_top - 1];
	}
	bool full()const { return _top == _size; } // 栈满
	bool empty()const { return _top == 0; } // 栈空
private:
	T *_pstack;
	int _top;
	int _size;

	// 顺序栈底层数组按2倍的方式扩容
	void expand()
	{
		T *ptmp = new T[_size * 2];
		for (int i = 0; i < _top; ++i)
		{
			ptmp[i] = _pstack[i];
		}
		delete []_pstack;
		_pstack = ptmp;
		_size *= 2;
	}
};
template<typename T>
void SeqStack<T>::push(const T &val) // 入栈操作
{
	if (full())
		expand();
	_pstack[_top++] = val;
}
int main()
{
	// 类模板的选择性实例化
	// 模板类 class SeqStack<int>{};
	SeqStack<int> s1;
	s1.push(20);
	s1.push(78);
	s1.push(32);
	s1.push(15);
	s1.pop();
	cout << s1.top() << endl;

	SeqStack<> s2;

	return 0;
}

template<typename T, int SIZE>
void sort(T *arr)
{
	for (int i = 0; i < SIZE - 1; ++i)
	{
		for (int j = 0; j < SIZE - 1 - i; ++j)
		{
			if (arr[j] > arr[j + 1])
			{
				int tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
		}
	}
}
int main()
{
	int arr[] = { 12,5,7,89,32,21,35 };
	const int size = sizeof(arr) / sizeof(arr[0]);
	sort<int, 7>(arr);
	for (int val : arr)
	{
		cout << val << " ";
	}
	cout << endl;
	return 0;
}



/*
C++ 函数模板
模板的意义：对类型也可以进行参数化了
int sum(int a, int b){return a+b;}

函数模板 <= 是不进行编译的，因为类型还不知道
模板的实例化 <= 函数调用点进行实例化
模板函数  <=  才是要被编译器所编译的

模板类型参数 typename/class
模板非类型参数

模板的实参推演 => 可以根据用户传入的实参的类型，来推导出模板类型参数的具体类型

模板的特例化(专用化) 特殊（不是编译器提供的，而是用户提供的）的实例化
函数模板、模板的特例化、非模板函数的重载关系

模板代码是不能在一个文件中定义，在另外一个文件中使用的
模板代码调用之前，一定要看到模板定义的地方，这样的话，模板
才能够进行正常的实例化，产生能够被编译器编译的代码

所以，模板代码都是放在头文件当中的，然后在源文件当中直接
进行#include包含
*/

// 模板的声明
template<typename T> 
bool compare(T a, T b); // compare<const char*> *UND*
bool compare(const char*a, const char*b); // compare *UND*

int main()
{
	//bool __cdecl compare<double>(double,double)
	//bool __cdecl compare<int>(int,int)
	// 函数的调用点
	compare<int>(10, 20);
	compare<double>(10.5, 20.5);
	// 函数模板实参的推演
	compare(20, 30);
	compare<int>(30, 40.5);

	// 函数模板实参的推演 T const char*
	// 对于某些类型来说，依赖编译器默认实例化的模板代码，代码处理逻辑是有错误的
	// 编译器优先把compare处理成函数名字，没有的化，才去找compare模板
	compare("aaa", "bbb");
	compare<const char*>("aaa", "bbb");

	return 0;
}
/*
在函数调用点，编译器用用户指定的类型，从原模板实例化一份函数代码出来
模板函数
bool compare<int>(int a, int b) //
{
	return a > b;
}
bool compare<double>(double a, double b)
{
	return a > b;
}
bool compare<const char*>(const char* a, const char* b)
{
	return strcmp(a, b) > 0;
}
*/
#endif