#include "pch.h"
#include <iostream>
using namespace std;

/*
����������أ���Ա������ȫ�ַ���
�ڴ��  ���̳�  �̳߳�  ���ӳ�  �����
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
	void push(const T &val) // ��Ӳ���
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
	// ����һ��QueueItem�Ķ���أ�10000��QueueItem�ڵ㣩
	struct QueueItem 
	{
		QueueItem(T data = T()) :_data(data), _next(nullptr) {}
		//��QueueItem�ṩ�Զ����ڴ����
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

	QueueItem *_front; // ָ��ͷ�ڵ�  
	QueueItem *_rear;  // ָ���β
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
new��delete
1.malloc��new������
a.malloc���ֽڿ����ڴ�ģ�new�����ڴ�ʱ��Ҫָ������ new int[10]
  ����malloc�����ڴ淵�صĶ���void*  operator new -> int*
b.mallocֻ���𿪱ٿռ䣬new��������malloc�Ĺ��ܣ����Խ������ݵĳ�ʼ��
   new int(20);   new int[20]();  int()
c.malloc�����ڴ�ʧ�ܷ���nullptrָ�룻new�׳�����bad_alloc���͵��쳣
2.free��delete������
delete (int*)p: ����������������free(p)

new -> operator new
delete -> operator delete

new��delete�ܻ�����C++Ϊʲô���ֵ���Ԫ�غ�������ڴ������ͷ��أ�
new delete
new[] delete[]
������ͨ�ı������������� new/delete[]   new[]/delete

�Զ���������ͣ�������������Ϊ�˵�����ȷ��������������ô���ٶ��������ʱ��
��࿪��4���ֽڣ���¼����ĸ���
*/
// �ȵ���operator new�����ڴ�ռ䡢Ȼ����ö���Ĺ��캯������ʼ����
void* operator new(size_t size)
{
	void *p = malloc(size);
	if (p == nullptr)
		throw bad_alloc();
	cout << "operator new addr:" << p << endl;
	return p;
}
// delete p;  ����pָ�����������������ٵ���operator delete�ͷ��ڴ�ռ�
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
	delete[] p2; // Test[0]���������� ֱ��free(p2)
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