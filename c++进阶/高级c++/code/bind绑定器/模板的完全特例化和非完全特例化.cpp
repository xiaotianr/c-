#include <iostream>
#include <typeinfo>
using namespace std;
/*
模板的完全特例化和非完全(部分)特例化
模板的实参推演
*/

//模版的实参推演 =》基本概念很简单
//T包含了所有的大的类型 返回值，所以行参的类型都取出来
template<typename T>  
void func(T a) 
{
    cout << typeid(T).name() << endl;
}
template<typename T>
void func2(T* a)
{
    cout << typeid(T).name() << endl;
}
template<typename R,typename A1,typename A2>
void func3(R(*a)(A1,A2))
{
    cout << typeid(R).name() << endl;
    cout << typeid(A1).name() << endl;
    cout << typeid(A2).name() << endl;
}
template<typename R,typename T,typename A1,typename A2>
void func4(R(T::*a)(A1,A2))
{
    cout << typeid(R).name() << endl;
    cout << typeid(T).name() << endl;
    cout << typeid(A1).name() << endl;
    cout << typeid(A2).name() << endl;
}
class Test
{
public:
    int sum(int a,int b) {return a+b;}
};
int sum(int a,int b) {return a+b;}
int main()
{
    func(10);//根据实惨推演出类型T
    func("aaa");
    string s="aa";
    func(s);
    func(sum);//T 推演出是函数指针类型 int (*)(int,int)
    func2(sum); // T 推演出来的就是函数类型 int (int,int)
    func3(sum);
    Test t;
    func4(&Test::sum);
    return 0;
}


#if 0
//#3 没有可匹配的完全特例化和部分特例化时匹配
template<typename T>
class Vector
{
public:
    Vector(){cout << "call Vector template init" << endl;}
};
//下面这个是对char*类型提供的完全特例化版本 #1 有完全特例化优先匹配完全特例化
template<>
class Vector<char *>
{
public:
    Vector(){cout << "call Vector<char *> init" << endl;}
};
//下面这个是针对指针类型提供的部分特例化版本 #2 没有完全特例化优先匹配部分特例化
template<typename Ty>
class Vector<Ty*>
{
public:
    Vector(){cout << "call Vector<Ty*> init" << endl;}
};

//这个是针对函数指针（有返回值，有两个行参变量）提供的部分特例化
template<typename R,typename A1,typename A2>
class Vector<R(*)(A1,A2)>
{
public:
    Vector(){cout << "call Vector<R(*)(A1,A2)> init" << endl;}
};
//针对函数类型（一个返回值，两个行参变量）提供的部分特例化
template<typename R,typename A1,typename A2>
class Vector<R(A1,A2)>
{
public:
    Vector(){cout << "call Vector<R(A1,A2)> init" << endl;}
};
int sum(int a,int b) {return a+b;}
int main()
{
    Vector<int> vec1;//call Vector template init
    Vector<char *> vec2;//call Vector<char *> init
    Vector<int *> vec3;//call Vector<Ty*> init
    Vector<int (*)(int,int)> vec4;//函数指针 //call Vector<R(*)(A1,A2)> init
    Vector<int(int,int)> vec5;//函数类型  //call Vector<R(A1,A2)> init

    //注意区分一下函数类型和函数指针类型
    //函数指针类型
    typedef int(*PFUNC1)(int,int);
    PFUNC1 pfunc1=sum;
    cout << pfunc1(10,20) << endl;

    //函数类型
    typedef int PFUNC2(int,int);
    PFUNC2 *pfunc2=sum;
    cout << (*pfunc2)(10,20) << endl;


    return 0;
}


//完全特例化
template<typename T>
bool compare(T a,T b)
{
    cout << "compare T" << endl;
    return a>b;
}
template<>
bool compare(const char *a,const char *b)
{
    cout << "compare const char*" << endl;
    return strcmp(a,b)>0;
}
int main()
{
    compare(10,20);
    compare("aaa","bbb");
    return 0;
}
#endif