#include<iostream>
using namespace std;
class A
{
    public:
    virtual void fun1(){cout<<"A1"<<endl;}
    virtual void fun2(){cout<<"A2"<<endl;}
    void fun3(){cout<<"A3"<<endl;}
};

class B:public A
{
    public:
    virtual void fun1(){cout<<"B1"<<endl;}
    virtual void fun2(){cout<<"B2"<<endl;}
    void fun3(){cout<<"B3"<<endl;}
};

int main()
{
    A *p;
    B b;
    p=&b;
    p->fun1();
    p->fun2();
    p->fun3();
}