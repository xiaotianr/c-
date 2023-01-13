#include <iostream>
#include <thread>
#include <atomic> // 包含了很多原子类型
#include <list>
using namespace std;

/*
c++11 
窗口卖票 int count=100;count++ --都不是线程安全的
lock_guard<std::mutex> guard(mtx);//通过互斥锁保证原子操作
count++;

lock_guard<std::mutex> guard(mtx);
count--;

互斥锁是比较重的，临界区代码做的事情稍稍复杂、多时，没有办法才去用互斥锁，此时也应该用互斥锁
对于比较简单的临界区代码，使用互斥锁大材小用了
系统理论：CAS来保证上面的 ++ --操作的原子特性就足够了，（无锁操作）
    通过硬件层面来保证    
    CAS通过exchange/swap的指令，相当于给总线加锁，
        当一个线程在做CPU和内存直接的数据交换时，一个线程如果没有做完，是不允许其他线程再去使用总线的
    无锁队列（无锁栈）=》CAS来实现的
*/

//volatile 防止多线程对共享变量进行缓存，大家访问的都是原始内存中的这些变量的值
volatile std::atomic_bool isReady=ATOMIC_VAR_INIT(false);
volatile std::atomic_int mycount=ATOMIC_VAR_INIT(0);
void task()
{
    while(!isReady)
    {
        std::this_thread::yield();//线程出让当前的cpu时间片，等待下一次调用
    }
    for(int i=0;i<100;++i)
    {
        ++mycount;
    }
}

int main()
{
    list<std::thread> tlist;
	for (int i = 0; i < 10; ++i)
	{
		tlist.push_back(std::thread(task));
	}
    std::this_thread::sleep_for(std::chrono::seconds(2));
    isReady=true;

    for (std::thread &t : tlist)
	{
		t.join();
	}
    cout << "mycount:" << mycount << endl;

    return 0;
}

