#include "pch.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue> // C++ STL���е������������̰߳�ȫ
using namespace std;
#if 0
/*
unique_lock  condition_variable
1.lock_guard��unique_lock
2.condition_variable  wait��notify_all����
*/
std::mutex mtx; // pthread_mutex_t
std::condition_variable cv; // pthread_condition_t
int main()
{
	/*
	֪ͨ��cv�ϵȴ����̣߳����������ˣ������ɻ��ˣ�
	������cv�ϵȴ����̣߳��յ�֪ͨ���ӵȴ�״̬ =�� ����״̬ =�� ��ȡ�������� =���̼߳�������ִ��
	*/
	cv.notify_all();

	// ����������ʹ���ڼ򵥵��ٽ�������εĻ�������У��������ں������ù�����
	//unique_lock<std::mutex> lck(mtx);
	//cv.wait(lck); // => #1.ʹ�߳̽���ȴ�״̬ #2.lck.unlock���԰�mtx���ͷŵ�

	//���������ں����������ݻ��߷��ع����У�ֻ�����ڼ򵥵��ٽ�������εĻ��������
	//lock_guard<std::mutex> guard(mtx); 

	//mtx.lock();
	//mtx.unlock();

	return 0;
}







/*
C++���̱߳�� - �̼߳��ͬ��ͨ�Ż���
���̱߳���������⣺
1.�̼߳�Ļ���
��̬���� =�� �ٽ�������� =�� ��֤ԭ�Ӳ��� =��������mutex  ������������ʵ��CAS
strace ./a.out mutex => pthread_mutex_t
2.�̼߳��ͬ��ͨ��
�����ߣ��������߳�ģ��
*/

std::mutex mtx; // ���廥���������̼߳�Ļ������
std::condition_variable cv; // �����������������̼߳��ͬ��ͨ�Ų���

// ����������һ����Ʒ��֪ͨ����������һ�����������ˣ���������֪ͨ�����߼���������Ʒ
class Queue
{
public:
	void put(int val) // ������Ʒ
	{
		//lock_guard<std::mutex> guard(mtx); // scoped_ptr
		unique_lock<std::mutex> lck(mtx); // unique_ptr
		while (!que.empty())
		{
			// que��Ϊ�գ�������Ӧ��֪ͨ������ȥ���ѣ��������ˣ��ټ�������
			// �������߳̽���#1�ȴ�״̬������#2��mtx�������ͷŵ�
			cv.wait(lck);  // lck.lock()  lck.unlock
		}
		que.push(val);
		/* 
		notify_one:֪ͨ�����һ���̵߳�
		notify_all:֪ͨ���������̵߳�
		֪ͨ�������е��̣߳���������һ����Ʒ�����ǸϽ����Ѱ�
		�����̵߳õ���֪ͨ���ͻ�ӵȴ�״̬ =�� ����״̬ =�� ��ȡ���������ܼ���ִ��
		*/
		cv.notify_all(); 
		cout << "������ ����:" << val << "����Ʒ" << endl;
	}
	int get() // ������Ʒ
	{
		//lock_guard<std::mutex> guard(mtx); // scoped_ptr
		unique_lock<std::mutex> lck(mtx); // unique_ptr
		while (que.empty())
		{
			// �������̷߳���que�ǿյģ�֪ͨ�������߳���������Ʒ
			// #1 ����ȴ�״̬ # �ѻ�����mutex�ͷ�
			cv.wait(lck);
		}
		int val = que.front();
		que.pop();
		cv.notify_all(); // ֪ͨ�����߳����������ˣ��Ͻ�������
		cout << "������ ����:" << val << "����Ʒ" << endl;
		return val;
	}
private:
	queue<int> que;
};

void producer(Queue *que) // �������߳�
{
	for (int i = 1; i <= 10; ++i)
	{
		que->put(i);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
void consumer(Queue *que) // �������߳�
{
	for (int i = 1; i <= 10; ++i)
	{
		que->get();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
int main()
{
	Queue que; // �����̹߳���Ķ���

	std::thread t1(producer, &que);
	std::thread t2(consumer, &que);

	t1.join();
	t2.join();

	return 0;
}
#endif