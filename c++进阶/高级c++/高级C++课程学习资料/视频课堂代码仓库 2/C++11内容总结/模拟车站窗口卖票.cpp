#include "pch.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <list>
using namespace std;

#if 0
/*
C++ thread ģ�⳵վ����������Ʊ�ĳ���

�̼߳�Ļ��� =�� ������mutex =�� lock_guard��װmutex
*/
int ticketCount = 100; // ��վ��100�ų�Ʊ������������һ����Ʊ
std::mutex mtx; // ȫ�ֵ�һ�ѻ�����
/*
���̳߳���
��̬���������̳߳���ִ�еĽ����һ�µģ���������CPU���̲߳�ͬ�ĵ���˳�򣬶�������ͬ
�����н����
*/
// ģ����Ʊ���̺߳���  lock_guard unique_lock
void sellTicket(int index)
{
	while (ticketCount > 0) // ticketCount=1  ��+˫���ж�
	{
		//mtx.lock();
		{
			// ��֤�����̶߳����ͷ�������ֹ��������ķ��� scoped_ptr
			lock_guard<std::mutex> lock(mtx); 
			if (ticketCount > 0)
			{
				// �ٽ��������  =��  ԭ�Ӳ��� =�� �̼߳以������� =�� mutex
				cout << "����:" << index << "������:" << ticketCount << "��Ʊ!" << endl;
				//cout << ticketCount << endl;
				ticketCount--;
			}
		}
		//mtx.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
int main()
{
	list<std::thread> tlist;
	for (int i = 1; i <= 3; ++i)
	{
		tlist.push_back(std::thread(sellTicket, i));
	}

	for (std::thread &t : tlist)
	{
		t.join();
	}

	cout << "���д�����Ʊ����!" << endl;

	return 0;
}
#endif