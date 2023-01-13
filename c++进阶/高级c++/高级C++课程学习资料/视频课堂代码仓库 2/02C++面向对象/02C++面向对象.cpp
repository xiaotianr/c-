// 02C++面向对象.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#if 0
/*
C++ OOP面向对象   OOP编程，this指针
C： 各种各样的函数的定义  struct
C++：类   =>  实体的抽象类型
实体（属性、行为） ->  ADT(abstract data type)
 |                            |
对象			  <-(实例化) 类（属性->成员变量  行为->成员方法）

OOP语言的四大特征是什么？
抽象    封装/隐藏    继承    多态

类 -》 商品实体
访问限定符：public公有的 private私有的 protected保护的
*/
const int NAME_LEN = 20;
class CGoods  // => 商品的抽象数据类型
{
public: // 给外部提供公有的成员方法，来访问私有的属性
	// 做商品数据初始化用的  
	void init(const char *name, double price, int amount);
	// 打印商品信息
	void show();
	// 给成员变量提供一个getXXX或setXXX的方法 类体内实现的方法，自动处理成inline内联函数
	void setName(char *name) { strcpy(_name, name); }
	void setPrice(double price) { _price = price; }
	void setAmount(int amount) { _amount = amount; }

	const char* getName() { return _name; }
	double getPrice() { return _price; }
	int getAmount() { return _amount; }
private: // 属性一般都是私有的成员变量
	char _name[NAME_LEN];
	double _price;
	int _amount;
};
void CGoods::init(const char *name, double price, int amount)
{
	strcpy(this->_name, name);
	this->_price = price;
	this->_amount = amount;
}
void CGoods::show()
{
	cout << "name:" << this->_name << endl;
	cout << "price:" << this->_price << endl;
	cout << "amount:" << this->_amount << endl;
}
int main()
{
	/*
	CGoods可以定义无数的对象，每一个对象都有自己的成员变量
	但是它们共享一套成员方法

	show() => 怎么知道处理哪个对象的信息？
	init(name, price, amount) => 怎么知道把信息初始化给哪一个对象的呢？
	
	类的成员方法一经编译，所有的方法参数，都会加一个this指针，接收
	调用该方法的对象的地址
	*/
	// cl 02C++面向对象.cpp /d1reportSingleClassLayoutCGoods
	// 对象内存大小的时候  对象的内存大小 =》 成员变量有关
	CGoods good1; // 类实例化了一个对象

	// init(&good1, "面包", 10.0, 200)
	good1.init("面包", 10.0, 200);
	// show(&good1)
	good1.show();

	good1.setPrice(20.5);
	good1.setAmount(100);
	good1.show();

	CGoods good2;// 类实例化了一个对象
	good2.init("空调", 10000.0, 50);
	good2.show();

    return 0; 
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

/*
OOP编程复习一下
OOP实现一个顺序栈 

构造函数和析构函数
函数的名字和类名一样
没有返回值
*/
class SeqStack
{
public:
	// 构造函数 SeqStack s1;  SeqStack s2(20);
	SeqStack(int size = 10) // 是可以带参数的，因此可以提供多个构造函数，叫做构造函数的重载
	{
		cout << this << " SeqStack()" << endl;
		_pstack = new int[size];
		_top = -1;
		_size = size;
	}
	// 析构函数
	~SeqStack() // 是不带参数的，所有析构函数只能有一个
	{
		cout << this << " ~SeqStack()" << endl;
		delete[]_pstack;
		_pstack = nullptr;
	}
	void push(int val)
	{
		if (full())
			resize();
		_pstack[++_top] = val;
	}
	void pop()
	{
		if (empty())
			return;
		--_top;
	}
	int top()
	{
		return _pstack[_top];
	}
	bool empty() { return _top == -1; }
	bool full() { return _top == _size - 1; }
private:
	int *_pstack; // 动态开辟数组，存储顺序栈的元素
	int _top; // 指向栈顶元素的位置
	int _size; // 数组扩容的总大小

	void resize()
	{
		int *ptmp = new int[_size * 2];
		for (int i = 0; i < _size; ++i)
		{
			ptmp[i] = _pstack[i];
		} // memcpy(ptmp, _pstack, sizeof(int)*_size); realloc
		delete[]_pstack;
		_pstack = ptmp;
		_size *= 2;
	}
};
SeqStack gs;
int main()
{
	/*
	.data 
	heap
	stack
	*/
	SeqStack *ps = new SeqStack(60); // malloc内存开辟+SeqStack(60)对象构造
	ps->push(70);
	ps->push(80);
	ps->pop();
	cout << ps->top() << endl;
	delete ps; //先调用ps->~SeqStack()+然后free(ps)   delete和free的区别

	// 1.开辟内存  2.调用构造函数
	SeqStack s;
	//s.init(5);  // 对象成员变量的初始化操作

	for (int i = 0; i < 15; ++i)
	{
		s.push(rand() % 100);
	}

	while (!s.empty())
	{
		cout << s.top() << " ";
		s.pop();
	}

	//s.release(); // 释放对象成员变量占用的外部堆内存（外部资源）

	SeqStack s1(50);

	//s1.~SeqStack(); // 析构函数调用以后，我们说对象不存在了

	s1.push(30); // 堆内存的非法访问了！！！

	return 0; // 
}




/*
this指针 =》 类-》很多对象  共享一套成员方法
成员方法，方法的参数都会添加一个this指针

构造函数：
	定义对象时，自动调用的；可以重载的；构造完成，对象产生了
析构函数：
	不带参数，不能重载，只有一个析构函数；析构完成，对象就不存在了
.data对象
heap new delete
stack

对象的浅拷贝和深拷贝
*/
class SeqStack
{
public:
	// 构造函数 
	SeqStack(int size = 10)
	{
		cout << this << " SeqStack()" << endl;
		_pstack = new int[size];
		_top = -1;
		_size = size;
	}
	// 自定以拷贝构造函数 《= 对象的浅拷贝现在有问题了
	SeqStack(const SeqStack &src)
	{
		cout << "SeqStack(const SeqStack &src)" << endl;
		_pstack = new int[src._size];
		for (int i = 0; i <= src._top; ++i)
		{
			_pstack[i] = src._pstack[i];
		}
		_top = src._top;
		_size = src._size;
	}
	// 析构函数
	~SeqStack() 
	{
		cout << this << " ~SeqStack()" << endl;
		delete[]_pstack;
		_pstack = nullptr;
	}
	// 赋值重载函数 s1 = s1;
	void operator=(const SeqStack &src)
	{
		cout << "operator=" << endl;
		// 防止自赋值
		if (this == &src)
			return;

		// 需要先释放当前对象占用的外部资源
		delete[]_pstack;

		_pstack = new int[src._size];
		for (int i = 0; i <= src._top; ++i)
		{
			_pstack[i] = src._pstack[i];
		}
		_top = src._top;
		_size = src._size;
	}
	void push(int val)
	{
		if (full())
			resize();
		_pstack[++_top] = val;
	}
	void pop()
	{
		if (empty())
			return;
		--_top;
	}
	int top()
	{
		return _pstack[_top];
	}
	bool empty() { return _top == -1; }
	bool full() { return _top == _size - 1; }
private:
	int *_pstack; // 动态开辟数组，存储顺序栈的元素
	int _top; // 指向栈顶元素的位置
	int _size; // 数组扩容的总大小

	void resize()
	{
		int *ptmp = new int[_size * 2];
		for (int i = 0; i < _size; ++i)
		{
			ptmp[i] = _pstack[i];
		} // memcpy(ptmp, _pstack, sizeof(int)*_size); realloc
		delete[]_pstack;
		_pstack = ptmp;
		_size *= 2;
	}
};
int main()
{
	SeqStack s; // 没有提供任何构造函数的时候，会为你生成默认构造和默认析构，是空函数
	SeqStack s1(10);
	SeqStack s2 = s1; // #1  默认拷贝构造函数-》做直接内存数据拷贝
	//SeqStack s3(s1); // #2

	// s2.operator=(s1) 
	// void operator=(const SeqStack &src)
	s1 = s1; // 默认的赋值函数 =》 做直接的内存拷贝 

	return 0;
}


//循环队列  memcpy  realloc
class Queue
{
public:
	Queue(int size = 5)
	{
		_pQue = new int[size];
		_front = _rear = 0;
		_size = size;
	}
	//Queue(const Queue&) = delete;
	//Queue& operator=(const Queue&) = delete;
	Queue(const Queue &src)
	{
		_size = src._size;
		_front = src._front;
		_rear = src._rear;
		_pQue = new int[_size];
		for (int i = _front; 
			i != _rear; 
			i = (i + 1) % _size)
		{
			_pQue[i] = src._pQue[i];
		}
	}
	Queue& operator=(const Queue &src)
	{
		if (this == &src)
			return *this;

		delete[]_pQue;

		_size = src._size;
		_front = src._front;
		_rear = src._rear;
		_pQue = new int[_size];
		for (int i = _front;
			i != _rear;
			i = (i + 1) % _size)
		{
			_pQue[i] = src._pQue[i];
		}
		return *this;
	}
	~Queue()
	{
		delete[]_pQue;
		_pQue = nullptr;
	}
	void push(int val) // 入队操作
	{
		if (full())
			resize();
		_pQue[_rear] = val;
		_rear = (_rear + 1) % _size;
	}
	void pop() // 出队操作
	{
		if (empty())
			return;
		_front = (_front + 1) % _size;
	}
	int front() // 获取队头元素
	{
		return _pQue[_front];
	}
	bool full() { return (_rear + 1) % _size == _front; }
	bool empty() { return _front == _rear; }
private: 
	int *_pQue; // 申请队列的数组空间
	int _front; // 指示队头的位置
	int _rear;  // 指示队尾的位置
	int _size;  // 队列扩容的总大小

	void resize()
	{
		int *ptmp = new int[2 * _size];
		int index = 0;
		for (int i = _front;
			i != _rear;
			i = (i + 1) % _size)
		{
			ptmp[index++] = _pQue[i];
		}
		delete[]_pQue;
		_pQue = ptmp;
		_front = 0;
		_rear = index;
		_size *= 2;
	}
};
int main()
{
	Queue queue;
	for (int i = 0; i < 20; ++i)
	{
		queue.push(rand() % 100);
	}

	while (!queue.empty())
	{
		cout << queue.front() << " ";
		queue.pop();
	}
	cout << endl;

	Queue queue1 = queue;
	queue1 = queue;

	return 0;
}

/*
浅拷贝：自定义拷贝构造函数和赋值重载函数
String类型
循环队列 Queue
*/
class String
{
public:
	String(const char *str = nullptr) // 普通构造函数
	{
		if (str != nullptr)
		{
			m_data = new char[strlen(str) + 1];
			strcpy(this->m_data, str);
		}
		else
		{
			m_data = new char[1]; // new char;
			*m_data = '\0'; // 0
		}
	}
	String(const String &other)  // 拷贝构造函数
	{
		m_data = new char[strlen(other.m_data) + 1];
		strcpy(m_data, other.m_data);
	}
	~String(void)  // 析构函数
	{
		delete[]m_data;
		m_data = nullptr;
	}
	// String& 是为了支持连续的operator=赋值操作
	String& operator=(const String &other) // 赋值重载函数
	{
		if (this == &other)
		{
			return *this;  // str1
		}

		delete[]m_data;

		m_data = new char[strlen(other.m_data) + 1];
		strcpy(m_data, other.m_data);
		return *this; // str1
	}
private:
	char *m_data; // 用于保存字符串
};
int main()
{
	// 调用带const char*参数的构造函数
	String str1;
	String str2("hello");
	String str3 = "world";

	// 调用拷贝构造函数
	String str4 = str3;
	String str5(str3);

	// 调用赋值重载函数
	/*
	str1 = str2
	str1.operator=(str2) => str1
	str3 = str1
	*/
	str3 = str1 = str2;

	return 0;
}
#endif


#if 0
class Test
{
public:
	Test(int data = 10) :mb(data), ma(mb) {}
	void show() { cout << "ma:" << ma << " mb:" << mb << endl; }
private:
	// 成员变量的初始化和它们定义的顺序有关，和构造函数初始化列表中出现的先后顺序无关！
	int ma;
	int mb;
};
int main()
{
	Test t; // 0xCCCCCCCC  -858993460  
	t.show();
	return 0;
}


/*
日期类
*/
class CDate
{
public:
	CDate(int y, int m, int d) // 自定义了一个构造函数，编译器就不会再产生默认构造了
	{
		_year = y;
		_month = m;
		_day = d;
	}
	void show()
	{
		cout << _year << "/" << _month << "/" << _day << endl;
	}
private:
	int _year;
	int _month;
	int _day;
};

/*
构造函数的初始化列表 ： 可以指定当前对象成员变量的初始化方式
CDate信息   CGoods商品信息的一部分  a part of...  组合的关系
*/
class CGoods
{
public:
	// “CDate”: 没有合适的默认构造函数可用
	CGoods(const char *n, int a, double p, int y, int m, int d)
		:_date(y, m, d)
		,_amount(a) // int _amount = a;
		,_price(p) // #1 构造函数的初始化列表
	{
		// #2 当前类类型构造函数体
		strcpy(_name, n);
	}
	void show()
	{
		cout << "name:" << _name << endl;
		cout << "amount:" << _amount << endl;
		cout << "price:" << _price << endl;
		_date.show();
	}
private:
	char _name[20];
	int _amount;
	double _price;
	CDate _date;  // 成员对象  1.分配内存  2.调用构造函数
};
int main()
{
	CGoods good("商品", 100, 35.0, 2019, 5, 12);
	good.show();

	return 0;
}





/*
类的各种成员 - 成员方法/变量

普通的成员方法 => 编译器会添加一个this形参变量
1.属于类的作用域
2.调用该方法时，需要依赖一个对象(常对象是无法调用的 实参:const CGoods*   CGoods *this)
3.可以任意访问对象的私有成员   protected继承  public private

static静态成员方法 => 不会生成this形参
1.属于类的作用域
2.用类名作用域来调用方法
3.可以任意访问对象的私有成员，仅限于不依赖对象的成员（只能调用其它的static静态成员）

const常成员方法 => const CGoods *this
1.属于类的作用域
2.调用依赖一个对象，普通对象或者常对象都可以
3.可以任意访问对象的私有成员，但是只能读，而不能写
*/

class CDate
{
public:
	CDate(int y, int m, int d) // 自定义了一个构造函数，编译器就不会再产生默认构造了
	{
		_year = y;
		_month = m;
		_day = d;
	}
	void show()const
	{
		cout << _year << "/" << _month << "/" << _day << endl;
	}
private:
	int _year;
	int _month;
	int _day;
};

/*
构造函数的初始化列表 ： 可以指定当前对象成员变量的初始化方式
CDate信息   CGoods商品信息的一部分  a part of...  组合的关系
*/
class CGoods
{
public:
	// “CDate”: 没有合适的默认构造函数可用
	CGoods(const char *n, int a, double p, int y, int m, int d)
		:_date(y, m, d)
		, _amount(a) // int _amount = a;
		, _price(p) // #1 构造函数的初始化列表
	{
		// #2 当前类类型构造函数体
		strcpy(_name, n);
		_count++; // 记录所有产生的新对象的数量
	}
	// 普通成员方法
	void show() // 打印商品的私有的信息 CGoods *this
	{
		cout << "name:" << _name << endl;
		cout << "amount:" << _amount << endl;
		cout << "price:" << _price << endl;
		_date.show();
	}
	// 常成员方法  只要是只读操作的成员方法，一律实现成const常成员方法
	void show()const // const CGoods *this
	{
		cout << "name:" << _name << endl;
		cout << "amount:" << _amount << endl;
		cout << "price:" << this->_price << endl;
		_date.show();
	}
	// 静态成员方法 没有this指针的
	static void showCGoodsCount() // 打印的是所有商品共享的信息
	{
		cout << "所有商品的种类数量是：" << _count << endl;
	}
private:
	char _name[20];
	int _amount;
	double _price;
	CDate _date;  // 成员对象  1.分配内存  2.调用构造函数
	static int _count; // 不属于对象，而是属于类级别的 声明 用来记录商品对象的总数量
};
// static成员变量一定要在类外进行定义并且初始化
int CGoods::_count = 0;
int main()
{
	CGoods good1("商品1", 100, 35.0, 2019, 5, 12);
	good1.show();

	CGoods good2("商品2", 100, 35.0, 2019, 5, 12);
	good2.show();

	CGoods good3("商品3", 100, 35.0, 2019, 5, 12);
	good3.show();

	CGoods good4("商品4", 100, 35.0, 2019, 5, 12);
	good4.show();

	// 统计所有商品的总数量
	CGoods::showCGoodsCount();

	const CGoods good5("非卖品商品5", 100, 35.0, 2019, 5, 12);
	good5.show(); // CGoods::show(&good5)  const CGoods*  -> const CGoods *this

	return 0;
}
#endif


#include "pch.h"
#include <iostream>
using namespace std;

/*
指向类成员(成员变量和成员方法)的指针
*/
class Test
{
public:
	void func() { cout << "call Test::func" << endl; }
	static void static_func() { cout << "Test::static_func" << endl; }

	int ma;
	static int mb;
};
int Test::mb;
int main()
{
	Test t1;
	Test *t2 = new Test();

	// 指向成员方法的指针
	// 无法从“void (__thiscall Test::* )(void)”转换为“void (__cdecl *)(void)”
	void (Test::*pfunc)() = &Test::func;
	(t1.*pfunc)();
	(t2->*pfunc)();

	// 如何定义函数指针指向类的static成员方法呢？

#if 0
	// int a=10; int *p=&a; *p=30;
	//无法从“int Test::* ”转换为“int *”
	int Test::*p = &Test::ma;
	t1.*p = 20;
	cout << t1.*p << endl;

	t2->*p = 30;
	cout << t2->*p << endl;

	int *p1 = &Test::mb;
	*p1 = 40;
	cout << *p1 << endl;
#endif

	delete t2;
	return 0;
}