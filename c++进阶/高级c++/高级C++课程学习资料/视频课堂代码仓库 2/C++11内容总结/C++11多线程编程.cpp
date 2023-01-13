#include "pch.h"
#include <iostream>
#include <thread>
using namespace std;

/*
C++���Լ���Ķ��̱߳��=��������Կ�ƽ̨ windows/linux/mac
thread/mutex/condition_variable
lock_quard/unique_lock
atomic ԭ������ ����CAS������ԭ������ �̰߳�ȫ��
sleep_for

   C++���Բ��� thread
   windows        linux  strace ./a.out
      |             |
createThread    pthread_create

�߳����ݣ�
һ. ��ô��������һ���߳�
std::thread����һ���̶߳��󣬴����߳�����Ҫ���̺߳����Ͳ�����
�߳��Զ�����

��. ���߳���ν���
���̺߳���������ɣ��߳̾ͽ�����

��. ���߳���δ������߳�
t.join() : �ȴ�t�߳̽�������ǰ�̼߳�����������
t.detach() : ��t�߳�����Ϊ�����̣߳����߳̽������������̽������������̶߳��Զ������ˣ�
*/

#if 0
void threadHandle1(int time)
{
	// �����߳�˯��time��
	std::this_thread::sleep_for(std::chrono::seconds(time));
	cout << "hello thread1!" << endl;
}
void threadHandle2(int time)
{
	// �����߳�˯��time��
	std::this_thread::sleep_for(std::chrono::seconds(time));
	cout << "hello thread2!" << endl;
}
int main()
{
	// ������һ���̶߳���,����һ���̺߳���,���߳̾Ϳ�ʼ������
	std::thread t1(threadHandle1, 2);
	std::thread t2(threadHandle2, 3);
	// ���̵߳ȴ����߳̽��������̼߳�����������
	t1.join();
	t2.join();
	// �����߳�����Ϊ�����߳�
	//t1.detach();

	cout << "main thread done!" << endl;

	// ���߳�������ɣ��鿴�����ǰ���̻���δ������ɵ����߳�
	// ���̾ͻ��쳣��ֹ
	return 0;
}
#endif