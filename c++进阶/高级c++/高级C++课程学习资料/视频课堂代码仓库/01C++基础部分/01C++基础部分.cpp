// 01C++基础部分.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#if 0
/*
const  指针   引用   在函数当中的引用
const怎么理解？ 
const修饰的变量不能够再作为左值！！！初始化完成后，值不能被修改！！！

C和C++中const的区别是什么？
const的编译方式不同，C中，const就是当作一个变量来编译生成指令的。
C++中，所有出现const常量名字的地方，都被常量的初始化替换了！！！

c++的const
必须初始化的，叫常量
叫常变量   因为初始值不是立即数，是一个变量
*/

int main()
{
	int b = 20;
	const int a = b;  // 已经改了！！！
	//int array[a] = {};

	int *p = (int*)&a;
	*p = 30;

	// 20 30 20
	printf("%d %d %d \n", a, *p, *(&a));

	return 0;
}















/*
函数重载
1.C++为什么支持函数重载，C语言不支持函数重载？
C++代码产生函数符号的时候，函数名+参数列表类型组成的！
C代码产生函数符号的时候，函数名来决定！

2.函数重载需要注意些什么？
3.C++和C语言代码之间如何互相调用

函数重载
1.一组函数，其中函数名相同，参数列表的个数或者类型不同，那么这一组函数
就称作-函数重载。
2.一组函数要称得上重载，一定先是处在同一个作用域当中的。
3.const或者volatile的时候，是怎么影响形参类型的。
4.一组函数，函数名相同，参数列表也相同，仅仅是返回值不同？不叫重载

请你解释一下，什么是多态？
静态（编译时期）的多态：函数重载
动态（运行时期）的多态：

什么是函数重载？

C 调用 C++:无法直接调用了！怎么办？ 把C++源码扩在extern "C"
C++ 调用 C代码:无法直接调用了！怎么办？ 把C函数的声明扩在extern "C"里面
*/
//__FILE__
//__LINE__
// 只要是C++编译器，都内置了__cplusplus这个宏名

#ifdef __cplusplus
extern "C" {
#endif
	int sum(int a, int b) // sum  .text
	{
		return a + b;
	}
#ifdef __cplusplus
}
#endif


extern "C"{
	int sum(int a, int b) // sum_int_int  .text
	{
		return a + b;
	}
}


//无法解析的外部符号 
//"int __cdecl sum(int,int)" (?sum@@YAHHH@Z)，该符号在函数 _main 中被引用
extern "C"
{
	int sum(int a, int b);  // sum  "UND"
}

int main()
{
	int ret = sum(10, 20);  
	cout << "ret:" << ret << endl;
	return 0;
}


void func(int *a) {}  // int
void func(int *const a) {}  // int
int main()
{
	int a = 10;
	const int b = 10;

	cout << typeid(a).name() << endl;
	cout << typeid(b).name() << endl;

	return 0;
}



bool compare(int a, int b) // compare_int_int
{
	cout << "compare_int_int" << endl;
	return a > b;
}
bool compare(double a, double b) // compare_double_double
{
	cout << "compare_double_double" << endl;
	return a > b;
}
bool compare(const char *a, const char *b) // compare_const char*_const char*
{
	cout << "compare_char*_char*" << endl;
	return strcmp(a, b) > 0;
}
int main()
{
	bool compare(int a, int b); // 函数的声明

	compare(10, 20); // call compare_int_int
	compare(10.0, 20.0); // double -> int
	compare("aaa", "bbb"); // const char* => int

	return 0;
}
#endif




#if 0
/*
===> inline内联函数 和 普通函数的区别？？？
inline内联函数：在编译过程中，就没有函数的调用开销了，在函数的调用点直接
把函数的代码进行展开处理了

inline函数不再生成相应的函数符号

inline只是建议编译器把这个函数处理成内联函数
但是不是所有的inline都会被编译器处理成内联函数  -  递归

debug版本上，inline是不起作用的； inline只有在release版本下才能出现 
g++ -c main.cpp -O2   objdump -t main.o
*/
inline int sum(int x, int y)  // *.o   sum_int_int  .text
{
	return x + y;
}
int main()
{
	int a = 10;
	int b = 20;

	int ret = sum(a, b); 
	// 此处有标准的函数调用过程  参数压栈，函数栈帧的开辟和回退过程
	// 有函数调用的开销   
	// x+y  mov add mov     1000000 x+y

	return 0;
}














/*
===>形参带默认值的函数
1.给默认值的时候，从右向左给
2.调用效率的问题
3.定义出可以给形参默认值，声明也可以给形参默认值
4.形参给默认值的时候，不管是定义处给，还是声明处给，形参默认值只能出现一次
*/
// sum(, 20);
int sum(int a, int b = 20);
int sum(int a = 10, int b);
int main()
{
	int a = 10;
	int b = 20;

	int ret = sum(a, b);
	/*
	mov eax, dword ptr[ebp-8]
	push eax

	push 28H
	mov ecx, dword ptr[ebp-4]
	push ecx
	call sum
	*/
	cout << "ret:" << ret << endl;

	/*
	push 14H
	mov ecx, dword ptr[ebp-4]
	push ecx
	call sum
	*/
	ret = sum(a);

	/*
	push 14H
	push 0Ah
	call sum
	*/
	ret = sum(); // sum(20, 50);

	return 0;
}

int sum(int a, int b)
{
	return a + b;
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








int main04()
{
	//int a = 10;
	//const int *p = &a;
	//int *q = p; // int* <= const int*

	//cout << typeid(p).name() << endl;

	//int *q1 = nullptr; 
	//int *const q2 = nullptr;
	//cout << typeid(q1).name() << endl;
	//cout << typeid(q2).name() << endl;
	// const如果右边没有指针*的话，const是不参与类型的

	//int a = 10;
	//int *p1 = &a;
	//const int *p2 = &a; // const int * <= int*
	//int *const p3 = &a; // int*  <= int*
	//int *p4 = p3; // int*   <=   int*

	//int b = 20;
	//int *q1 = &b;

	//const int a = 10;
	//int *p = &a;  // *p = 30;   int*  <=  const int*
	//const int *p = &a;

	return 0;
}

/*
const和二级指针的结合
*/
int main()
{
	int a = 10;
	int *p = &a;
	const int **q = &p; //  const int**  <=  int**
	// int *const* <=  const int**
	/*
	const int *   *q = &p; 
	   *q  <=>  p
	   const int b = 20;
	   *q = &b;  
	*/
	return 0;
}

/*
const和一级指针的结合
const和二级（多级）指针的结合

const修饰的量   叫常量
普通变量的区别是什么？ C++有两点区别？1.编译方式不同  2.不能作为左值了

const修饰的量常出现的错误是：
1.常量不能再作为左值  《=  直接修改常量的值
2.不能把常量的地址泄露给一个普通的指针或者普通的引用变量 《= 可以间接修改常量的值

const和一级指针的结合：有两种情况
C++的语言规范：const修饰的是离它最近的类型
const int *p = &a;  =>  *p = 20   p = &b
可以任意指向不同的int类型的内存，但是不能通过指针间接修改指向的内存的值
int const* p;

int *const p = &a; => p = &b   *p = 20;
这个指针p现在是常量，不能再指向其它内存，但是可以通过指针解引用修改指向的内存的值
const int *const p = &a; // *p = &b   p =

const如果右边没有指针*的话，const是不参与类型的

总结const和指针的类型转换公式：
int*    <=    const int*    是错误的！
const int*  <=   int*       是可以的！

int** <= const int**        是错误的！
const int** <= int**        是错误的！

int** <= int*const*			是错误的！
int*const* <= int**         是可以的！
*/
int main()
{
	int a = 10;
	const int *p = &a;
	int *const* q = &p;
	// int *const* <= const int**

	/*
	int a = 10;
	int *const p = &a; // int* <= int*
	int **q = &p; // * <= const*

	
	int a = 10;
	int *p = &a;
	int **const q = &p; // int** <= int**

	
	int a = 10;
	int *p = &a;
	int *const*q = &p; // const* <= *

	
	int a = 10;
	int *p = &a;
	const int **q = &p;
	// *q <=> p

	
	int a = 10;
	int *const p = &a; // int* <= int*
	const int *q = p; // const int* <= int*

	
	int a = 10;
	int *const p = &a;
	int *const q = p;

	
	int a = 10;
	int *const p = &a;  // int* <= int*
	int *q = p; // int*  <=  int*

	
	int a = 10;
	const int *p = &a;  // const int* <= int*
	int *q = p; // int* <= const int*  *q = 20;
	*/
	return 0;
}
#endif

#include <iostream>
#include <typeinfo>
using namespace std;

/*
C++的引用   引用和指针的区别？
1.左值引用和右值引用
2.引用的实例

引用是一种更安全的指针。
1.引用是必须初始化的，指针可以不初始化
2.引用只有一级引用，没有多级引用；指针可以有一级指针，也可以有多级指针
3.定义一个引用变量，和定义一个指针变量，其汇编指令是一模一样的；通过引用
变量修改所引用内存的值，和通过指针解引用修改指针指向的内存的值，其底层
指令也是一模一样的

右值引用
1.int &&c = 20; 专门用来引用右值类型，指令上，可以自动产生临时量
然后直接引用临时量 c = 40;
2.右值引用变量本身是一个左值，只能用左值引用来引用它
3.不能用一个右值引用变量，来引用一个左值
*/
#if 0
void swap(int *x, int *y)
{
	int temp = *x;
	*x = *y;
	*y = temp;
}
void swap(int &x, int &y)
{
	int temp = x;
	x = y;
	y = temp;
}
int main()
{
	int a = 10;
	int b = 20;
	//swap(&a, &b);
	swap(a, b);
	cout << "a:" << a << " b:" << b << endl;

	//int a = 10;
	//int *p = &a;
	//int &b = a;
	//int &c = 20;
	//*p = 20;
	//b = 30;
	// 20 20 20 a  *p  b
	//cout << a << " " << *p << " " << b << endl;
	// 30 30 30 a  *p  b
	//cout << a << " " << *p << " " << b << endl;
	//return 0;
}

int main()
{
	int array[5] = {};
	int *p = array;
	// 定义一个引用变量，来引用array数组
	int (&q)[5] = array;

	cout << sizeof(array) << endl;
	cout << sizeof(p) << endl;
	cout << sizeof(q) << endl;

	return 0;
}


int main()
{
	int a = 10; // 左值，它有内存，有名字，值可以修改的
	int &b = a;

	//int &c = 20; // 20是右值：没内存，没名字
	//C++11提供了右值引用
	int &&c = 20;
	c = 30;

	int &e = c; // 一个右值引用变量，本身是一个左值

	/*
	int temp = 20;
	temp -> b
	*/
	const int &d = 20;
	return 0;
}







/*
const，一级指针，引用的结合
*/
int main()
{
	//int a = 10;
	//int *p = &a;
	//const int *&q = p;
	//const int **q = &p; //const int** <= int**

	//int a = 10;
	//const int *p = &a;
	//int *&q = p;
	//int **q = &p;
	//int** <= const int**

	//int a = 10;
	//int *const p = &a;
	//int *&q = p; 
	//int **q = &p; // *q = 

	// 写一句代码，在内存的0x0018ff44处写一个4字节的10
	//int *const &p = (int*)0x0018ff44;

	//int a = 10;
	//int *p = &a;
	//const int *&q = p; // typeid(q).name()
	// const int*   int* 

	return 0;
}
#endif


/*
new和delete
new和malloc的区别是什么？
delete和free的区别是什么？

malloc和free，称作C的库函数
new和delete，称作运算符

new不仅可以做内存开辟，还可以做内存初始化操作
malloc开辟内存失败，是通过返回值和nullptr做比较；而new开辟
内存失败，是通过抛出bad_alloc类型的异常来判断的。
*/
int main()
{
	// new有多少种？
	int *p1 = new int(20);

	int *p2 = new (nothrow) int;

	const int *p3 = new const int(40);

	// 定位new  
	int data = 0;
	int *p4 = new (&data) int(50);
	cout << "data:" << data << endl;



#if 0
	int *p = (int*)malloc(sizeof(int));
	if (p == nullptr)
	{
		return -1;
	}
	*p = 20;
	free(p);

	int *p1 = new int(20);
	delete p1;

	int *q = (int*)malloc(sizeof(int) * 20);
	if (q == nullptr)
	{
		return -1;
	}
	free(q);

	//int *q1 = new int[20];
	int *q1 = new int[20](); // 20个int sizeof(int) * 20
	delete[]q1;
#endif

	return 0;
}