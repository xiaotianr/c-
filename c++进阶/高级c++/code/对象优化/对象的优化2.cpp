#include <iostream>
using namespace std;

class Test
{
private:
    int ma;
public:
    //带默认值的构造：既可以默认构造，也可以带参构造
    //Test()   Test(20)
    Test(int a=10):ma(a){cout << "Test(int)" << endl;}
    ~Test() {cout << "~Test()" << endl;}
    Test(const Test &t):ma(t.ma) {cout << "Test(const Test&)" << endl;}
    void operator=(const Test &t)
    {
        cout << "operator=" << endl;
        ma=t.ma;
    }
    int getData()const {return ma;}
};
/**
 * 不能返回指针或引用
 * 返回指针或引用，要保证变量在函数结束后还存在
 * 如果返回指针，调用方就能通过指针间接的访问变量内存，
 * 但是因为这个函数返回的是函数局部的变量，函数运行完，变量就没了，内存也被回收
 * 所以不能返回指针或者引用
 */
/**
 * 1.函数参数传递过程中，对象优先按引用传递，不要按值传递
 * 2.函数返回对象的时候，应该优先返回一个临时对象，而不要返回一个定义过的对象
 * 3.接收返回值是对象的函数调用的时候，优先按初始化的方式接受， 不要按赋值的方式接受
 */
Test GetObject(Test &t) //不能返回局部的或临时对象的指针或引用
{
    int val=t.getData();
    // Test tmp(val);//static Test tmp(val);这种情况就可以返回指针或引用，因为它在数据段
    // return tmp;
    //返回临时对象
    return Test(val);//用临时对象拷贝构造一个新对象，临时对象就不产生了
}
int main()
{
    Test t1;
    Test t2=GetObject(t1);//用临时对象拷贝构造一个新对象，临时对象就不产生了

    return 0;
}