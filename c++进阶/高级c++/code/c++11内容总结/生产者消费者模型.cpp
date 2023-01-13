#include <iostream>
#include <thread>
#include <list>
#include <mutex>
#include <condition_variable>//条件变量
#include <queue> //c++ STL所有的容器都不是线程安全的
using namespace std;

/*
unique_lock condition_variable
1.lock_guard和unique_lock
2.condition_variable  wait和notify_all方法
*/
std::mutex mtx;//互斥锁只有一个线程能拿到  //对应于linux的pthread_mutex_t
std::condition_variable cv;//pthread_condition_t
int main()
{
    /*
    通知在cv上等待的线程，条件成立了，起来干活了！
    其他再cv上等待的线程，收到通知，从等待状态=》阻塞状态=》 获取互斥锁了 =》线程继续执行
    */
   cv.notify_all();

    //不仅可以使用在简单的临界区代码段的互斥操作中，还能用在函数调用过程中
    unique_lock<std::mutex> lck(mtx);
    cv.wait(lck);//=> #1.使线程进入等待状态 #2. lck.unlock()可以mtx互斥锁释放掉

    //不可能用在函数参数传递或者返回过程中，只能用在简单的临界区代码段的互斥操作中
    //lock_guard<std::mutex> guard(mtx);//实现类似于scoped_ptr


    return 0;
}






#if 0
/*
c++多线程编程：-线程间的同步通信机制
多线程编程两个问题：
1.线程间的互斥
    竞态条件 =》临界区代码段 =》 保证原子操作 =》互斥锁mutex  轻量级的无锁实现（CAS）
2.线程间的同步通信
    生产者 消费者线程模型
*/
std::mutex mtx;//定义互斥锁 做线程间的互斥操作
std::condition_variable cv;//定义条件变量，做线程间的同步通信操作

//生产者生产一个物品，通知消费者消费一个，消费者消费一个，通知生产者生产一个
class Queue
{
public:
    void put(int val)//生产物品
    {
        //lock_guard<std::mutex> guard(mtx);//scoped_ptr
        unique_lock<std::mutex> lck(mtx);//unique_ptr
        while(!que.empty())
        {
            //que不为空，生产者应该通知消费者去消费，消费完了，再继续生产
            //生产者线程进入等待状态，并且把mutex互斥锁释放掉
            
            cv.wait(lck);//进入等待并且会释放锁，需要的锁是unique_lock
        }
        que.push(val);
        /*
        notify_one:通知另外的一个线程的
        notift_all:通知其他所有线程的
        通知其他所有线程，我生产了一个物品，那么赶紧去消费
        其他线程得到该通知，就会从等待状态 =》阻塞状态 =》 获取互斥锁才能继续执行
        */
        cv.notify_all();
        cout << "生产者 生产：" << val << "号物品" << endl;
    }
    int get()//消费物品
    {
        //lock_guard<std::mutex> guard(mtx);
        unique_lock<std::mutex> lck(mtx);
        while(que.empty())
        {
            //消费者线程发现que是空的，通知生产者线程先生产物品
            //消费者进入等待状态，把互斥锁释放
            
            cv.wait(lck);//会释放锁
        }
        int val=que.front();
        que.pop();
        cv.notify_all();//通知其他线程我消费完了，赶紧生产吧
        cout << "消费者 消费：" << val << "号物品" << endl;
        return val;
    }
private:
    queue<int> que;
};
void producer(Queue *que)//生产者线程
{
    for(int i=1;i<=10;++i)
    {
        que->put(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
void consumer(Queue *que)//消费者线程
{
    for(int i=1;i<=10;++i)
    {
        que->get();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
int main()
{
    Queue que;//两个线程共享的队列
    std::thread t1(producer,&que);
    std::thread t2(consumer,&que);

    t1.join();
    t2.join();

    return 0;
}
#endif