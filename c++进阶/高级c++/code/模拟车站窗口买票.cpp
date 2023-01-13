#include <iostream>
#include <thread>
#include <list>
#include <mutex>//互斥锁的头文件
using namespace std;

/*
c++ thread 模拟车站三个窗口卖票的程序

线程间的互斥 =》互斥锁mutex =》 lock_guard封装mutex，类似于智能指针，利用栈上对象出作用域必须析构的特点
        
*/
int cnt=100;//车站有100张票，由三个窗口一起卖票
std::mutex mtx;//全局的一把互斥锁
/*
多线程程序
竞态条件：多线程程序执行的结果是一致的，不会随着COU对线程不同的调用顺序，而产生不同的运行结果

*/

//模拟卖票的线程函数 lock_guard unique_lock
void sellTic(int idx)
{
    while(cnt>0)
    {
        //mtx.lock();
        {
            //锁处于该{}的局部对象，出了该{}，(在该作用域return了，也会出该{}作用域),就析构了，也就解锁了
            //从而保证所有线程都能释放锁，防止死锁问题的发生
            lock_guard<std::mutex> lock(mtx);//类似于scoped_ptr,lock_guard的拷贝构造和赋值重载也被delete了
            //unique_lock<std::mutex> lck(mtx);//类似于unique_ptr,保留了右值引用的拷贝构造和赋值重载
            //lck.lock();
            ////cnt==1时，线程1执行操作，当还没有执行--操作时，线程2也进来了，阻塞在互斥锁处，线程1结束后，线程2直接进入临界区，把第0张票卖出了
            //因此还需要做一次判断
            if(cnt>0)//锁+双重判断
            {
                //临界区代码段 =》保证原子操作 =》线程见互斥操作 =》mutex
                cout << "窗口：" << idx << "卖出第：" << cnt << "张票" << endl;
                //cout << cnt << endl;
                --cnt;//对全局变量的--操作不是一个线程安全操作
                //临界区代码段
            }
            //lck.unlock();
        }
        //mtx.unlock();//为了防止程序在临界区就返回了，出现死锁，需要参考智能指针
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
int main()
{
    list<std::thread> tlist;
    for(int i=1;i<=3;++i)
    {
        tlist.push_back(std::thread(sellTic,i));
    }
    for(std::thread &t:tlist)
    {
        t.join();
    }
    cout << "所有窗口卖票结束!" << endl;
    return 0;
}