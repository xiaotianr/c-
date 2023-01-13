#include <iostream>
#include <typeinfo>
#include <string>
#include <functional>
using namespace std;

/*
function函数对象类型的实现原理

*/
void hello(string str){cout << str << endl;}
int sum(int a,int b) {return a+b;}
///////////////////////////
template<typename Fty>
class myfunction{};
/*
template<typename R,typename A1>
class myfunction<R(A1)>
{
public:
    using PFUNC= R(*)(A1);//函数指针类型
    myfunction(PFUNC pfunc):_pfunc(pfunc){}
    R operator()(A1 arg)
    {
        return _pfunc(arg);
    }
private:
    PFUNC _pfunc;
};
template<typename R,typename A1,typename A2>
class myfunction<R(A1,A2)>
{
public:
    using PFUNC= R(*)(A1,A2);//函数指针类型
    myfunction(PFUNC pfunc):_pfunc(pfunc){}
    R operator()(A1 a,A2 b)
    {
        return _pfunc(a,b);
    }
private:
    PFUNC _pfunc;
};
*/
template<typename R,typename... A>//typename...表示可变参，A表示一组类似，个数是任意的
class myfunction<R(A...)>
{
public:
    using PFUNC= R(*)(A...);//函数指针类型
    myfunction(PFUNC pfunc):_pfunc(pfunc){}
    R operator()(A... arg)
    {
        return _pfunc(arg...);//一组行参变量
    }
private:
    PFUNC _pfunc;
};

int main()
{
    myfunction<void(string)> func1=hello;
    func1("hello word!");//func1.operator()("hello word!")

    myfunction<int(int,int)> func2=sum;
    cout <<func2(10,20) << endl;

    return 0;
}
