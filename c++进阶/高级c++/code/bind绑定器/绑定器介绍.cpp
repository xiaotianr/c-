#include <iostream>
#include <typeinfo>
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <thread>
using namespace std;
using namespace placeholders;
/*
c++11 bind绑定器 =》返回的结果还是一个函数对象

void hello(string str){cout << str << endl;}
int sum(int a,int b) {return a+b;}
class Test
{
public:
	int sum(int a, int b) { return a + b; }
};
int main()
{
    //bind是函数模板 可以自动推演模板类型参数
    bind(hello,"hello bind!")(); //返回的是一个函数对象，可以直接调用()运算符
    cout << bind(sum,10,20)() << endl;

    //1.调用类的成员方法，前面需要取地址
    //2.第二个参数用来绑定Test对象，成员方法的调用必须得依赖对象
    cout << bind(&Test::sum,Test(),30,20)() << endl; 

    //参数占位符   
    bind(hello,_1)("hello bind2");//placeholders::_1是参数占位符
    cout << bind(sum,_1,_2)(200,300) << endl;


    //绑定器出了语句，无法继续使用 怎么解决？
    //可以通过function函数对象类型将绑定器的类型留下来，实现bind放回的绑定器的复用
    function<void(string)> func1=bind(hello,_1);
    func1("hello function");
    func1("hi");
    func1("aaa");

    return 0;
}
*/
/*
muduo源码文件 threadpool.cc thread.cc  
bind function模拟线程池的实现
*/
class Thread
{
public:
    Thread(function<void(int)>func,int no):_func(func),_no(no){}
    thread start()
    {
        thread t(_func,_no); //_func.operator()
        return t;
    }
private:    
    function<void(int)>_func;
    int _no;
};
class ThreadPool
{
public:
    ThreadPool(){}
    ~ThreadPool()
    {
        //释放Thread对象占用的堆资源
        for(int i=0;i<_pool.size();++i)
        {
            delete _pool[i];
        }
    }
    //开启线程池
    void startPool(int size)
    {
        for(int i=0;i<size;++i)
        {
            _pool.push_back(
                new Thread(bind(&ThreadPool::runInThread,this,_1),i));
        }
        for(int i=0;i<size;++i)
        {
            _handler.push_back(_pool[i]->start());
        }
        for(thread &t:_handler)
        {
            t.join();
        }
    }
private:
    vector<Thread*> _pool;
    vector<thread> _handler;

    //把这个成员方法充当线程函数  
    //c的线程函数无法接收对象，但是可以通过绑定器将对象绑定到线程函数中
    void runInThread(int id)
    {
        cout << "call runInThread! id:" << id << endl;
    }
};
int main()
{
    ThreadPool pool;
    pool.startPool(10);
    return 0;
}
