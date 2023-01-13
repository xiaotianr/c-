#include <iostream>
#include <thread>
//using namespace std;
//多线程下，尽量不加这个，很容易名称冲突

/*
c++语言级别的多线程编程 =》 代码跨平台 Windows/linux/mac上直接编译运行
    以前都是调用本地系统的API，无法做到跨平台
thread/mutex/condition_variable
lock_quard/unique_lock
atomic 原子类型 基于CAS操作的原子类型 线程安全的
sleep_for

c++语言层面 thread
    Windows      linux
       |           |
   createThread  pthread_create

线程内容：
一：怎么创建启动一个线程
std::thread定义一个线程对象，传入线程所需要的线程函数和参数，线程自动开启
二：子线程如何结束
子线程函数运行完成，线程就结束了
三：主线程如何处理子线程
1.t.join 等待t线程结束,当前线程继续往下运行
2.t.detach 把t线程设置为分离线程，当主线程结束时，整个进程结束，所以子线程都自动结束了！，可能就看不到t线程的运行结果
*/
void threadHandle1(int time)
{
    //让子线程睡眠time s
    std::this_thread::sleep_for(std::chrono::seconds(time));//chrono作用域是时间相关的
    std::cout << "hello thread1!" << std::endl;
}
void threadHandle2(int time)
{
    //让子线程睡眠time s
    std::this_thread::sleep_for(std::chrono::seconds(time));//chrono作用域是时间相关的
    std::cout << "hello thread2!" << std::endl;
}
int main()
{
    //创建了一个线程对象 ,传入一个线程函数，新线程就开始运行了
    std::thread t1(threadHandle1,2);
    std::thread t2(threadHandle2,3);

    //主线程等待子线程结束，主线程继续往下运行
    t1.join();
    t2.join();

    //把子线程设置为分离线程
    //t1.detach();

    std::cout << "main thread done!" << std::endl;
    //主线程运行完成，查看如果当前进程还有未运行完成的子线程，进程就会异常终止
    return 0;
}