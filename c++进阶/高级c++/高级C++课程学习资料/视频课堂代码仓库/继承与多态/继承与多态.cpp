// 继承与多态.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <string>
#include <typeinfo>
using namespace std;
/*
1.继承的本质和原理
2.派生类的构造过程
3.重载、覆盖、隐藏
4.静态绑定和动态绑定
5.多态 vfptr和vftable
6.抽象类的设计原理
7.多重继承以及问题
8.虚基类 vbptr和vbtable
9.RTTI
10.C++四种类型强转
11.继承多态常见笔试面试题分享
*/

#if 0
/*
C++语言级别提供的四种类型转换方式
int a = (int)b;
const_cast : 去掉（指针或者引用）常量属性的一个类型转换
static_cast :  提供编译器认为安全的类型转换（没有任何联系的类型之间的转换就被否定了）
reinterpret_cast : 类似于C风格的强制类型转换
dynamic_cast : 主要用在继承结构中，可以支持RTTI类型识别的上下转换
*/

class Base
{
public:
	virtual void func() = 0;
};
class Derive1 : public Base
{
public:
	void func() { cout << "call Derive1::func" << endl; }
};
class Derive2 : public Base
{
public:
	void func() { cout << "call Derive2::func" << endl; }
	// Derive2实现新功能的API接口函数
	void derive02func() 
	{ 
		cout << "call Derive2::derive02func" << endl; 
	}
};
/*
typeid(*p).name() == "Derive"
*/
void showFunc(Base *p)
{
	// dynamic_cast会检查p指针是否指向的是一个Derive2类型的对象？
	// p->vfptr->vftable RTTI信息 如果是，dynamic_cast转换类型成功，
	// 返回Derive2对象的地址，给pd2；否则返回nullptr
	// static_cast编译时期的类型转换  dynamic_cast运行时期的类型转换 支持RTTI信息识别
	Derive2 *pd2 = dynamic_cast<Derive2*>(p);
	if (pd2 != nullptr)
	{
		pd2->derive02func();
	}
	else
	{
		p->func(); // 动态绑定  *p的类型 Derive2  derive02func
	}
}
int main()
{
	Derive1 d1;
	Derive2 d2;
	showFunc(&d1);
	showFunc(&d2);

	//static_cast 基类类型 《=》 派生类类型  能不能用static_cast?当然可以！
	//int *p = nullptr;
	//double* b = reinterpret_cast<double*>(p);

	//const int a = 10;
	//int *p1 = (int*)&a;
	//int *p2 = const_cast<int*>(&a);
	// const_cast<这里面必须是指针或者引用类型 int* int&>
	//int b = const_cast<int>(a);

	return 0;
}


/*
C++的多重继承 - 菱形继承的问题  派生类有多份间接基类的数据 设计的问题
好处，可以做更多代码的复用   D -> B,C    B *p = new D()   C *p = new D()
*/
class A
{
public:
	A(int data) :ma(data) { cout << "A()" << endl; }
	~A() { cout << "~A()" << endl; }
protected:
	int ma;
};
//=======================================
class B : virtual public A
{
public:
	B(int data) :A(data), mb(data) { cout << "B()" << endl; }
	~B() { cout << "~B()" << endl; }
protected:
	int mb;
};
class C : virtual public A
{
public:
	C(int data) :A(data), mc(data) { cout << "C()" << endl; }
	~C() { cout << "~C()" << endl; }
protected:
	int mc;
};
//=========================================
class D : public B, public C
{
public:
	//“A::A”: 没有合适的默认构造函数可用
	D(int data) :A(data), B(data), C(data), md(data) { cout << "D()" << endl; }
	~D() { cout << "~D()" << endl; }
protected:
	int md;
};
int main()
{
	D d(10);

	return 0;
}


/*
多重继承 ：代码的复用   一个派生类有多个基类
class C: public A, public B
{
};

抽象类(有纯虚函数的类) / 虚基类 被虚继承的类称作虚基类 vbptr和vbtable
virtual
1.修饰成员方法是虚函数
2.可以修饰继承方式，是虚继承。被虚继承的类，称作虚基类
*/
class A
{
public:
	virtual void func() { cout << "call A::func" << endl; }
	void operator delete(void *ptr)
	{
		cout << "operator delete p:" << ptr << endl;
		free(ptr);
	}
private:
	int ma;
};
class B : virtual public A
{
public:
	void func() { cout << "call B::func" << endl; }

	void* operator new(size_t size)
	{
		void *p = malloc(size);
		cout << "operator new p:" << p << endl;
		return p;
	}
private:
	int mb;
};
/*
A a; 4个字节
B b; ma,mb  8个字节+4=12个字节   vbptr
*/
int main()
{
	// 基类指针指向派生类对象，永远指向的是派生类基类部分数据的起始地址
	B b;
	A *p = &b;//new B(); // B::vftable
	cout << "main p:" << p << endl;
	p->func();
	//delete p;

	return 0;
}
#endif



#if 0
class Base
{
public:
	Base()
	{
		/*
		push ebp
		mov ebp, esp
		sub esp, 4Ch
		rep stos esp<->ebp 0xCCCCCCCC(windows VS GCC/G++)
		vfptr <- &Base::vftable
		*/
		cout << "call Base()" << endl;
		clear();
	}
	void clear() { memset(this, 0, sizeof(*this)); }
	virtual void show()
	{
		cout << "call Base::show()" << endl;
	}
};
class Derive : public Base
{
public:
	Derive()
	{
		/*
		push ebp
		mov ebp, esp
		sub esp, 4Ch
		rep stos esp<->ebp 0xCCCCCCCC(windows VS GCC/G++)
		vfptr <- &Derive::vftable
		*/
		cout << "call Derive()" << endl;
	}
	void show()
	{
		cout << "call Derive::show()" << endl;
	}
};
int main()
{
	//Base *pb1 = new Base();
	/*
	mov eax, dword ptr[pb1]
	mov ecx, dword ptr[eax] eax:0x00000000 不是Base::vftable
	call ecx
	*/
	//pb1->show(); // 动态绑定
	//delete pb1;

	Base *pb2 = new Derive();
	// 动态绑定 call Derive::show()
	/*
	vfptr里面存储的是vftable的地址
	vfptr <- vftable
	*/
	pb2->show(); 
	delete pb2;

	return 0;
}



class Base
{
private:
	virtual void show()
	{
		cout << "call Base::show" << endl;
	}
};
class Derive : public Base
{
public:
	void show()
	{
		cout << "call Derive::show" << endl;
	}
};
int main()
{
	Derive *p = new Derive();
	/*
	成员方法能不能调用，就是说方法的访问权限是不是public的，是在编译阶段就需要确定
	好的

	编译阶段 Base::show
	call Base::show   call ecx
	*/
	//“Base::show”: 无法访问 private 成员(在“Base”类中声明)
	p->show(); // 最终能调用到Derive::show，是在运行时期才确定的
	delete p;
	return 0;
}



class Base
{
public:
	virtual void show(int i = 10)
	{
		cout << "call Base::show i:" << i << endl;
	}
};
class Derive : public Base
{
public:
	void show(int i = 20)
	{
		cout << "call Derive::show i:" << i << endl;
	}
};
int main()
{
	Base *p = new Derive(); // 虚析构函数
	/*
	push 0Ah => 函数调用，参数压栈是在编译时期就确定好的
	mov eax, dword ptr[p]
	mov ecx, dword ptr[eax]
	call ecx
	*/
	p->show(); // 动态绑定 p-> Derive vfptr -> Derive vftable
	delete p;

	return 0;
}




class Animal
{
public:
	Animal(string name) :_name(name) {}
	// 纯虚函数
	virtual void bark() = 0;
protected:
	string _name;
};
// 以下是动物实体类
class Cat : public Animal
{
public:
	Cat(string name) :Animal(name) {}
	void bark() { cout << _name << " bark: miao miao!" << endl; }
};
class Dog : public Animal
{
public:
	Dog(string name) :Animal(name) {}
	void bark() { cout << _name << " bark: wang wang!" << endl; }
};
int main()
{
	Animal *p1 = new Cat("加菲猫"); // vfptr -> Dog vftable
	Animal *p2 = new Dog("二哈"); // vfptr -> Cat vftable

	int *p11 = (int*)p1;
	int *p22 = (int*)p2;
	int tmp = p11[0]; // p11[0]访问的就是Cat的前4个字节
	p11[0] = p22[0]; // p22[0]访问的就是Dog的前4个字节
	p22[0] = tmp;

	p1->bark(); // p1 -> Cat vfptr -> Dog vftable bark
	p2->bark(); // p2 -> Dog vfptr -> Cat vftable bark

	delete p1;
	delete p2;
	
	return 0;
}


/*
抽象类和普通类有什么区别？
一般把什么类设计成抽象类？ 基类
*/
// 动物的基类  泛指   类->抽象一个实体的类型
/*
定义Animal的初衷，并不是让Animal抽象某个实体的类型
1.string _name; 让所有的动物实体类通过继承Animal直接复用该属性
2.给所有的派生类保留统一的覆盖/重写接口

拥有纯虚函数的类，叫做抽象类！（Animal）
Animal a; No!!! 
抽象类不能再实例化对象了，但是可以定义指针和引用变量
*/
class Animal 
{
public:
	Animal(string name) :_name(name) {}
	// 纯虚函数
	virtual void bark() = 0;
protected:
	string _name;
};
// 以下是动物实体类
class Cat : public Animal
{
public:
	Cat(string name) :Animal(name) {}
	void bark() { cout << _name << " bark: miao miao!" << endl; }
};
class Dog : public Animal
{
public:
	Dog(string name) :Animal(name) {}
	void bark() { cout << _name << " bark: wang wang!" << endl; }
};
class Pig : public Animal
{
public:
	Pig(string name) :Animal(name) {}
	void bark() { cout << _name << " bark: heng heng!" << endl; }
};
void bark(Animal *p)
{
	p->bark(); // Animal::bark虚函数，动态绑定了 
}

// 汽车的基类
class Car // 抽象类
{
public:
	Car(string name, double oil) :_name(name), _oil(oil) {}
	// 获取汽车剩余油量还能跑的公里数
	double getLeftMiles()
	{
		// 1L 10  *  oil
		return _oil * this->getMilesPerGallon(); // 发生动态绑定了
	}
	string getName()const { return _name; }
protected:
	string _name;
	double _oil;
	virtual double getMilesPerGallon() = 0; // 纯虚函数
};
class Bnze : public Car
{
public:
	Bnze(string name, double oil) :Car(name, oil) {}
	double getMilesPerGallon() { return 20.0; }
};
class Audi : public Car
{
public:
	Audi(string name, double oil) :Car(name, oil) {}
	double getMilesPerGallon() { return 18.0; }
};
class BMW : public Car
{
public:
	BMW(string name, double oil) :Car(name, oil) {}
	double getMilesPerGallon() { return 19.0; }
};
//给外部提供一个同一的获取汽车剩余路程数的API
void showCarLeftMiles(Car &car)
{
	cout<<car.getName() << " left miles:" 
		<< car.getLeftMiles() << "公里" <<endl;
	 // 静态绑定 call Car::getLeftMiles()
}
int main()
{
	Bnze b1("奔驰", 20.0);
	Audi a("奥迪", 20.0);
	BMW b2("宝马", 20.0);
	showCarLeftMiles(b1);
	showCarLeftMiles(a);
	showCarLeftMiles(b2);
	return 0;
}


/*
如何解释多态？ 
静态（编译时期）的多态： 函数重载、模板（函数模板和类模板）
bool compare(int, int){}
bool compare(double, double){} 

compare(10, 20); call compare_int_int 在编译阶段就确定好调用的函数版本
compare(10.5, 20.5); call compare_douoble_double 在编译阶段就确定好调用的函数版本

template<typename T>
bool compare(T a, T b){}

compare<int>(10, 20);  => int 实例化一个 compare<int>
compare(10.5, 20.5); => double 实例化一个 compare<double>

动态（运行时期）的多态：   Base Derive
在继承结构中，基类指针（引用）指向派生类对象，通过该指针（引用）调用同名覆盖方法（虚函数）
，基类指针指向哪个派生类对象，就会调用哪个派生类对象的同名覆盖方法，称为多态
 pbase->bark();
 多态底层是通过动态绑定来实现的，pbase-》访问谁的vfptr=》继续访问谁的vftable
                                =》当然调用的是对应的派生类对象的方法了


继承的好处是什么？
1.可以做代码的复用
2.在基类中给所有派生类提供统一的虚函数接口，让派生类进行重写，然后就可以使用多态了
*/

// 动物的基类
class Animal
{
public:
	Animal(string name) :_name(name) {}
	virtual void bark() {}
protected:
	string _name;
};
// 以下是动物实体类
class Cat : public Animal
{
public:
	Cat(string name) :Animal(name) {}
	void bark() { cout << _name << " bark: miao miao!" << endl; }
};
class Dog : public Animal
{
public:
	Dog(string name) :Animal(name) {}
	void bark() { cout << _name << " bark: wang wang!" << endl; }
};
class Pig : public Animal
{
public:
	Pig(string name) :Animal(name) {}
	void bark() { cout << _name << " bark: heng heng!" << endl; }
};
/*
下面的一组bark API接口无法做到我们软件涉及要求的“开-闭“原则
软件设计由六大原则   “开-闭“原则  对修改关闭，对扩展开放
*/
void bark(Animal *p)
{
	p->bark(); // Animal::bark虚函数，动态绑定了 
	/*
	p->cat Cat vftable &Cat::bark
	p->dog Dog vftable &Dog::bark
	p->pig Pig vftable &Pig::bark
	*/
}
int main()
{
	Cat cat("猫咪");
	Dog dog("二哈");
	Pig pig("佩奇");

	bark(&cat);
	bark(&dog);
	bark(&pig);

	return 0;
}




/*
虚函数和动态绑定 问题：是不是虚函数的调用一定就是动态绑定？ 肯定不是的！
在类的构造函数当中，调用虚函数，也是静态绑定（构造函数中调用其它函数（虚），不会发生动态绑定的）
如果不是通过指针或者引用变量来调用虚函数，那就是静态绑定
*/
class Base
{
public:
	Base(int data = 0) :ma(data) {}
	virtual void show() { cout << "Base::show()" << endl; }
protected:
	int ma;
};
class Derive : public Base
{
public:
	Derive(int data = 0) :Base(data), mb(data) {}
	void show() { cout << "Derive::show()" << endl; }
private:
	int mb;
};
int main()
{
	Base b;
	Derive d;

	// 静态绑定 用对象本身调用虚函数，是静态绑定
	b.show(); // 虚函数 call Base::show 
	d.show(); // 虚函数 call Derive::show

	// 动态绑定（必须由指针调用虚函数）
	Base *pb1 = &b;
	pb1->show();
	Base *pb2 = &d;
	pb2->show();

	// 动态绑定（必须由引用变量调用虚函数）
	Base &rb1 = b;
	rb1.show();
	Base &rb2 = d;
	rb2.show();

	// 动态绑定（虚函数通过指针或者引用变量调用，才发生动态绑定）
	Derive *pd1 = &d;
	pd1->show();
	Derive &rd1 = d;
	rd1.show();

	Derive *pd2 = (Derive*)&b;
	pd2->show(); // 动态绑定 pd2 -> b vfptr -> Base vftable Base::show

	return 0;
}


/*
问题一：哪些函数不能实现成虚函数？

虚函数依赖：
1.虚函数能产生地址，存储在vftable当中
2.对象必须存在(vfptr -> vftable -> 虚函数地址)

构造函数
1.virtual+构造函数 NO！ 
2.构造函数中（调用的任何函数，都是静态绑定的）调用虚函数，也不会发生静态绑定
派生类对象构造过程  1.先调用的是基类的构造函数 2.才调用派生类的构造函数

static静态成员方法 NO！ virtual + static

问题二：虚析构函数 析构函数调用的时候，对象是存在的！
什么时候把基类的析构函数必须实现成虚函数？
基类的指针（引用）指向堆上new出来的派生类对象的时候， delete pb(基类的指针)
，它调用析构函数的时候，必须发生动态绑定，否则会导致派生类的析构函数无法调用
*/
class Base
{
public:
	Base(int data) :ma(data) { cout << "Base()" << endl; }
	// 虚析构函数
	virtual ~Base() { cout << "~Base()" << endl; }
	virtual void show() { cout << "call Base::show()" << endl; }
protected:
	int ma;
}; // &Base::~Base   &Base::show
class Derive : public Base // &Derive::~Derive   &Base::show 
{
public:
	Derive(int data)
		:Base(data), mb(data), ptr(new int(data))
	{
		cout << "Derive()" << endl;
	}
	// 基类的析构函数是virtual虚函数，那么派生类的析构函数自动成为虚函数
	~Derive()
	{
		delete ptr;
		cout << "~Derive()" << endl;
	}
private:
	int mb;
	int *ptr;
};
int main()
{
	Base *pb = new Derive(10);
	pb->show(); //  动态绑定 pb Base*  *pb Derive
	delete pb; // 派生类的析构函数没有被调用到！！！
	/*
	pb->Base  Base::~Base 对于析构函数的调用，就是静态绑定了
	call Base::~Base

	pb->Base Base::~Base 对于析构函数的调用，就是动态绑定了
	pb -> Derive Derive vftable &Derive::~Derive
	*/

	/*Derive d(10);
	Base *pb = &d;
	pb->show();*/

	return 0;
}

/*
虚函数，静态绑定和动态绑定

覆盖：
基类和派生类的方法，返回值、函数名以及参数列表都相同，而且基类的方法是虚函数，
那么派生类的方法就自动处理成虚函数，它们之间成为覆盖关系

一个类添加了虚函数，对这个类有什么影响？
总结一：
一个类里面定义了虚函数，那么编译阶段，编译器给这个类类型产生一个唯一的vftable虚函数
表，虚函数表中主要存储的内容就是RTTI指针和虚函数的地址。当程序运行时，每一张虚函数表
都会加载到内存的.rodata区。

总结二：
一个类里面定义了函数数，那么这个类定义的对象，其运行时，内存中开始部分，多存储一个
vfptr虚函数指针，指向相应类型的虚函数表vftable。一个类型定义的n个对象，它们的额vfptr
指向的都是同一张虚函数表

总结三：
一个类里面虚函数的个数，不影响对象内存大小（vfptr），影响的是虚函数表的大小
*/
class Base
{
public:
	Base(int data = 10) :ma(data) {}
	// 虚函数
	virtual void show() { cout << "Base::show()" << endl; }
	// 虚函数
	virtual void show(int) { cout << "Base::show(int)" << endl; }
protected:
	int ma;
};
class Derive : public Base
{
public:
	Derive(int data = 20) :Base(data), mb(data) {}
	/*
	总结四：
	如果派生类中的方法，和基类继承来的某个方法，返回值、函数名、参数列表都相同，
	而且基类的方法是virtual虚函数，那么派生类的这个方法，自动处理成虚函数
	重写《=》覆盖   
	*/
	void show() { cout << "Derive::show()" << endl; }
private:
	int mb;
};
int main()
{
	Derive d(50);
	Base *pb = &d;

	/*
	pb->Base Base::show 如果发现show是普通函数，就进行静态绑定 
											call Base::show
	pb->Base Base::show 如果发现show是虚函数，就进行动态绑定了
	mov eax, dword ptr[pb]
	mov ecx, dword ptr[eax]
	call ecx（虚函数的地址） 动态（运行时期）的绑定（函数的调用）
	*/
	pb->show(); //静态（编译时期）的绑定（函数的调用）call  Base::show (01612DAh)
	
	/*
	Base::show(int) 是一个虚函数，此时就得动态绑定了
	mov eax, dword ptr[pb]
	mov ecx, dword ptr[eax]
	call ecx（虚函数的地址） 动态（运行时期）的绑定（函数的调用）
	*/
	pb->show(10); //静态绑定 call  Base::show (01612B2h)

	cout << sizeof(Base) << endl; // 4 8
	cout << sizeof(Derive) << endl; // 8 12

	cout << typeid(pb).name() << endl; // class Base*
	/*
	pb的类型：Base -> 有没有虚函数
	如果Base没有虚函数，*pb识别的就是编译时期的类型  *pb <=> Base类型
	如果Base有虚函数，*pb识别的就是运行时期的类型 RTTI类型
	pb->d(vfptr)->Derive vftable  class Derive 
	*/ 
	cout << typeid(*pb).name() << endl; // class Base  class Derive 
	/*
	Base的两个show函数都是普通的函数
	Base::show()
	Base::show(int)
	4
	8
	class Base *
	class Base
	*/

	/*
	给Base的两个show方法定义成virtual虚函数了
	Derive::show()
	Base::show(int)
	8
	12
	class Base *
	class Derive
	*/

	return 0;
}



/*
重载、隐藏、覆盖
1.重载关系
一组函数要重载，必须处在同一个作用域当中；而且函数名字相同，参数列表不同

2.隐藏(作用域的隐藏)的关系
在继承结构当中，派生类的同名成员，把基类的同名成员给隐藏调用了


============================================================
1.把继承结构，也说成从上（基类）到下（派生类）的结构
2.
基类对象 -> 派生类对象
派生类对象 -> 基类对象

基类指针（引用）-> 派生类对象
派生类指针（引用）-> 基类对象

总结：在继承结构中进行上下的类型转换，默认只支持从下到上的类型的转换 OK
*/
class Base
{
public:
	Base(int data = 10) :ma(data) {}
	void show() { cout << "Base::show()" << endl; } //#1
	void show(int) { cout << "Base::show(int)" << endl; }//#2
protected:
	int ma;
};
class Derive : public Base
{
public:
	Derive(int data = 20) :Base(data), mb(data) {}
	void show() { cout << "Derive::show()" << endl; } //#3
private:
	int mb;
};
int main()
{
	Base b(10);
	Derive d(20);

	// 基类对象b <- 派生类对象d   类型从下到上的转换  Y
	b = d;

	// 派生类对象d <- 基类对象b   类型从上到下的转换  N
	// d = b;

	// 基类指针（引用）<- 派生类对象  类型从下到上的转换 Y
	Base *pb = &d;
	pb->show();
	//((Derive*)pb)->show();
	pb->show(10);

	// 派生类指针（引用）<- 基类对象  类型从上到下的转换  N
	Derive *pd = (Derive*)&b; // 不安全，涉及了内存的非法访问！
	pd->show();

#if 0
	Derive d;
	d.show();
	d.Base::show();
	//“Derive::show”: 函数不接受 1 个参数
	d.Base::show(10);
	d.show(20);//优先找的是派生类自己作用域的show名字成员；没有的话，才去基类里面找
#endif
	return 0;
}
#endif


#if 0
/*
1.派生类从继承可以继承来所有的成员(变量和方法)，除过构造函数和析构函数

派生类怎么初始化从基类继承来的成员变量呢？
解答：通过调用基类相应的构造函数来初始化

派生类的构造函数和析构函数，负责初始化和清理派生类部分
派生类从基类继承来的成员，的初始化和清理由谁负责呢？是由基类的构造和析构函数来负责

》》》派生类对象构造和析构的过程是：
1.派生类调用基类的构造函数，初始化从基类继承来的成员
2.调用派生类自己的构造函数，初始化派生类自己特有的成员
.....派生类对象的作用域到期了
3.调用派生类的析构函数，释放派生类成员可能占用的外部资源（堆内存，文件）
4.调用基类的析构函数，释放派生类内存中，从基类继承来的成员可能占用的外部资源（堆内存，文件）
*/
class Base
{
public:
	Base(int data) :ma(data) { cout << "Base()" << endl; }
	~Base() { cout << "~Base()" << endl; }
protected:
	int ma;
};
class Derive : public Base
{
public:
	//“Base”: 没有合适的默认构造函数可用
	Derive(int data) 
		:Base(data), mb(data)  // ma(data)
	{
		cout << "Derive()" << endl;
	}
	~Derive()
	{
		cout << "~Derive()" << endl;
	}
private:
	int mb;
};
int main()
{
	Derive d(20);

	return 0;
}



/*
1.继承的本质和原理
继承的本质：a.代码的复用  b.
类和类之间的关系：
组合：a part of... ...一部分的关系
继承：a kind of... ...一种的关系

继承方式     基类的访问限定     派生类的访问限定      (main)外部的访问限定
public 
			  public			public                 Y
			  protected			protected			   N
			  private			不可见的				   N         
protected(基类的成员的访问限定，在派生类里面是不可能超过继承方式的)
			  public			protected			   N
			  protected			protected			   N
			  private			不可见的				   N
private
			  public			private				   N
			  protected			private				   N
			  private			不可见的				   N

总结：
1.外部只能访问对象public的成员，protected和private的成员无法直接访问
2.在继承结构中，派生类从基类可以继承过来private的成员，但是派生类却无法直接访问
3.protected和private的区别？在基类中定义的成员，想被派生类访问，但是不想被外部访问，
那么在基类中，把相关成员定义成protected保护的；如果派生类和外部都不打算访问，那么
在基类中，就把相关成员定义成private私有的。

默认的继承方式是什么？
要看派生类是用class定义的，还是struct定义的？
class定义派生类，默认继承方式就是private私有的
struct定义派生类，默认继承方式就是public私有的
*/
class A
{
public:
	int ma;
protected:
	int mb;
private:
	int mc; // 自己或者友元能访问私有的成员
};
//继承 A 基类/父类   B 派生类/子类
struct B : A
{
public:
	void func() 
	{
		cout << ma << endl;
	}
	int md;
protected:
	int me;
private:
	int mf;
	// int ma;
};
class C : public B
{
	// 在C里面，请问ma的访问限定是什么？ 不可见的，但是能继承来
};
int main()
{
	B b;
	//cout << b.mb << endl;

    return 0; 
}
#endif