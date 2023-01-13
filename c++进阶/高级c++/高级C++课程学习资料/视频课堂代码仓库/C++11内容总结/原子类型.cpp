#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic> // �����˺ܶ�ԭ������
#include <list>
using namespace std;

/*
C++11 
������Ʊ   int count=100;
lock_guard<std::mutex> guard(mtx);
count++;

lock_guard<std::mutex> guard(mtx);
count--;

�������ǱȽ��صģ��ٽ������������������Ը��ӣ���
ϵͳ���ۣ�CAS����֤����++ --������ԭ�����Ծ��㹻�ˣ� ��������
        exchange/swap    �������� => CAS��ʵ�ֵ�
*/
volatile std::atomic_bool isReady = false;
volatile std::atomic_int mycount = 0;

void task()
{
	while (!isReady)
	{
		std::this_thread::yield(); // �̳߳��õ�ǰ��CPUʱ��Ƭ���ȴ���һ�ε���
	}

	for (int i = 0; i < 100; ++i)
	{
		mycount++;
	}
}
int main()
{
	list<std::thread> tlist;
	for (int i = 0; i < 10; ++i)
	{
		tlist.push_back(std::thread(task));
	}

	std::this_thread::sleep_for(std::chrono::seconds(3));
	isReady = true;

	for (std::thread &t : tlist)
	{
		t.join();
	}
	cout << "mycount:" << mycount << endl;

	return 0;
}