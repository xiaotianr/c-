#include "pch.h"
#include <iostream>
using namespace std;

/*
运算符的重载：成员方法：全局方法
内存池 进程池 线程池 连接池 对象池
*/
template<typename T>
class Queue
{
public:
	Queue()
	{
		_front = _rear = new QueueItem();
	}
	~Queue()
	{
		QueueItem *cur = _front;
		while (cur != nullptr)
		{
			_front = _front->_next;
			delete cur;
			cur = _front;
		}
	}
	void push(const T &val) // 入队
	{
		QueueItem *item = new QueueItem(val); // malloc
		_rear->_next = item;
		_rear = item;
	}
	void pop()
	{
		if (empty())
			return;
		QueueItem *first = _front->_next;
		_front->_next = first->_next;
		if (_front->_next == nullptr)
		{
			_rear = _front;
		}
		delete first; // free
	}
	T front()const
	{
		return _front->_next->_data;
	}
	bool empty()const { return _front == _rear; }
private:
	// 产生一个QueueItem 的对象池（100000个QueueItem 节点）
	struct QueueItem 
	{
		QueueItem(T data = T()) :_data(data), _next(nullptr) {}
		//给QueueItem自定义内存管理
		void* operator new(size_t size)
		{
			if (_itemPool == nullptr)
			{
				_itemPool = (QueueItem*)new char[POOL_ITEM_SIZE*sizeof(QueueItem)];
				QueueItem *p = _itemPool;
				for (; p < _itemPool + POOL_ITEM_SIZE - 1; ++p)
				{
					p->_next = p + 1;
				}
				p->_next = nullptr;
			}

			QueueItem *p = _itemPool;
			_itemPool = _itemPool->_next;
			return p;
		}
		void operator delete(void *ptr)
		{
			QueueItem *p = (QueueItem*)ptr;
			p->_next = _itemPool;
			_itemPool = p;
		}
		T _data;
		QueueItem *_next;
		static QueueItem *_itemPool;
		static const int POOL_ITEM_SIZE = 100000;
	};

	QueueItem *_front; // ָ指向头节点
	QueueItem *_rear;  // 指向队尾
};

template<typename T>
typename Queue<T>::QueueItem *Queue<T>::QueueItem::_itemPool = nullptr;

int main()
{
	Queue<int> que;
	for (int i = 0; i < 1000000; ++i)
	{
		que.push(i); // QueueItem(i)
		que.pop(); // QueueItem
	}
	cout << que.empty() << endl;

	return 0;
}

#if 0
/*
new和delete
1.malloc和new的区别
a.malloc是按字节开辟内存的，而new开辟内存时需要指定类型 new int[10]
  所以malloc开辟内存返回的都是void*，需要做类型强转  operator new -> int*，new 不需要做类型强转
b.mallocֻ只开辟空间，new不仅仅有malloc的功能，还可以进行数据的初始化
   new int(20);   new int[20]();  int()
c.malloc开辟内存失败返回nullptrָ指针，而new抛出的是bad_alloc类型的异常
2.free和delete区别
delete (int*)p: 调用析构函数，再free(p)

new -> operator new
delete -> operator delete

new和delete能混用么？c++为什么区分单个元素和数组的内存分配和释放呢？
new delete
new[] delete[]
对于普通的编译器内置类型 new/delete[]   new[]/delete 可以混用

自定义的类类型，有析构函数，未了调用正确的析构函数，那么开辟对象数组的时候，会多开辟4个字节，记录对象的个数
*/
// new：先调用operator new开辟内存空间，然后调用对象的构造函数（初始化）
void* operator new(size_t size)
{
	void *p = malloc(size);
	if (p == nullptr)
		throw bad_alloc();
	cout << "operator new addr:" << p << endl;
	return p;
}
// delete p;  调用p指向对象的析构函数，再调用operator delete释放内存空间
void operator delete(void *ptr)
{
	cout << "operator delete addr:" << ptr << endl;
	free(ptr);
}
void* operator new[](size_t size)
{
	void *p = malloc(size);
	if (p == nullptr)
		throw bad_alloc();
	cout << "operator new[] addr:" << p << endl;
	return p;
}
void operator delete[](void *ptr)
{
	cout << "operator delete[] addr:" << ptr << endl;
	free(ptr);
}

class Test
{
public:
	Test(int data = 10) { cout << "Test()" << endl; }
	~Test() { cout << "~Test()" << endl; }
private:
	int ma;
};
int main()
{
	/*
	Test *p1 = new Test();
	delete []p1;

	
	operator new[] addr:00940268
	Test()
	Test()
	Test()
	Test()
	Test()
	~Test()
	operator delete addr:0094026C
	
	Test *p2 = new Test[5];
	cout << "p2:" << p2 << endl;
	delete[] p2; // Test[0]对象析构，然后直接free(p2)
	*/
#if 0
	try
	{
		int *p = new int;
		delete []p;

		int *q = new int[10];
		delete q;
	}
	catch (const bad_alloc &err)
	{
		cerr << err.what() << endl;
	}
#endif
	return 0;
}
#endif