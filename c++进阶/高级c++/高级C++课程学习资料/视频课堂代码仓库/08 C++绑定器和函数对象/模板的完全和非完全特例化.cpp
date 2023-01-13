#include "pch.h"
#include <iostream>
#include <typeinfo>
using namespace std;

#if 0
/*
ģ�����ȫ�������ͷ���ȫ�����֣�������
ģ���ʵ������ => ��������ܼ�
*/
int sum(int a, int b) { return a + b; }
// T���������еĴ������  ����ֵ�������βε����Ͷ�ȡ����
template<typename T>  
void func(T a)
{
	cout << typeid(T).name() << endl;
}

template<typename R, typename A1, typename A2>
void func2(R (*a)(A1, A2))
{
	cout << typeid(R).name() << endl;
	cout << typeid(A1).name() << endl;
	cout << typeid(A2).name() << endl;
}

class Test
{
public:
	int sum(int a, int b) { return a + b; }
};
template<typename R, typename T, typename A1, typename A2>
void func3(R(T::*a)(A1, A2))
{
	cout << typeid(R).name() << endl;
	cout << typeid(T).name() << endl;
	cout << typeid(A1).name() << endl;
	cout << typeid(A2).name() << endl;
}
int main()
{
	//func(10);
	//func("aaa");
	func(sum); // T  int (*)(int,int)   int (int,int)
	func2(sum);
	func3(&Test::sum); //  int (__thiscall Test::*)(int,int)

	return 0;
}

// #3
template<typename T>
class Vector 
{
public:
	Vector() { cout << "call Vector template init" << endl; }
};
// ��������Ƕ�char*�����ṩ����ȫ�������汾  #1
template<>
class Vector<char*>
{
public:
	Vector() { cout << "call Vector<char*> init" << endl; }
};
// ��������Ƕ�ָ�������ṩ�Ĳ����������汾  #2
template<typename Ty>
class Vector<Ty*>
{
public:
	Vector() { cout << "call Vector<Ty*> init" << endl; }
};

// ָ�뺯��ָ�루�з���ֵ���������βα������ṩ�Ĳ���������
template<typename R, typename A1, typename A2>
class Vector<R(*)(A1, A2)>
{
public:
	Vector() { cout << "call Vector<R(*)(A1, A2)> init" << endl; }
};

// ��Ժ�������һ������ֵ���������βα����������ṩ�Ĳ���������
template<typename R, typename A1, typename A2>
class Vector<R(A1, A2)>
{
public:
	Vector() { cout << "call Vector<R(A1, A2)> init" << endl; }
};

int sum(int a, int b) { return a + b; }
int main()
{
	Vector<int> vec1;
	Vector<char*> vec2;
	Vector<int*> vec3;
	Vector<int(*)(int, int)> vec4;
	Vector<int(int, int)> vec5; // function

	// ע������һ�º������ͺͺ���ָ������
	typedef int(*PFUNC1)(int, int);
	PFUNC1 pfunc1 = sum;
	cout << pfunc1(10, 20) << endl;

	typedef int PFUNC2(int, int);
	PFUNC2 *pfunc2 = sum;
	cout << (*pfunc2)(10, 20) << endl;

	return 0;
}


template<typename T>
bool compare(T a, T b)
{
	cout << "template compare" << endl;
	return a > b;
}
template<>
bool compare<const char*>(const char*a, const char*b)
{
	cout << "compare<const char*>" << endl;
	return strcmp(a, b) > 0;
}
int main()
{
	compare(10, 20); // 
	compare("aaa", "bbb"); // T const char*
	return 0;
}
#endif