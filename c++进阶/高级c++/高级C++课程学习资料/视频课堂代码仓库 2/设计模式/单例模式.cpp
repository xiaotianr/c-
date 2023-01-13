#include "pch.h"
#include <iostream>
#include <mutex>
using namespace std;

/*
����ģʽ��һ���಻�ܴ������ٴζ�����Զֻ�ܵõ�������һ�������ʵ��
A *p1 = new A();
A *p2 = new A();
A *p3 = new A(); 

���õ��ģ�������־ģ�飬���ݿ�ģ��

����ģʽ��
����ʽ����ģʽ����û�л�ȡʵ������ʵ��������Ѿ�������
����ʽ����ģʽ��Ψһ��ʵ������ֱ����һ�λ�ȡ����ʱ�򣬲Ų���
*/
#if 0
// �̰߳�ȫ������ʽ����ģʽ https://blog.csdn.net/QIANGWEIYUAN
class Singleton
{
public:
	static Singleton* getInstance() // #3 ��ȡ���Ψһʵ������Ľӿڷ���
	{
		// g++ -o run ����ģʽ.cpp -g   gdb run
		// ������̬�ֲ������ĳ�ʼ�����ڻ��ָ�����Ѿ��Զ�����̻߳���ָ����
		static Singleton instance; // #2 ����һ��Ψһ�����ʵ������
		return &instance;
	}
private:
	Singleton() // #1 ���캯��˽�л�
	{
		// �ܶ��ʼ���Ĵ���
	}
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
};

int main()
{
	Singleton *p1 = Singleton::getInstance();
	Singleton *p2 = Singleton::getInstance();
	Singleton *p3 = Singleton::getInstance();

	return 0;
}
#endif



#if 0
std::mutex mtx;

// ����ʽ����ģʽ => �ǲ����̰߳�ȫ���أ�   => �̰߳�ȫ������ʽ����ģʽ
class Singleton
{
public:
	// �ǲ��ǿ����뺯���أ�   ��+˫���ж�
	static Singleton* getInstance() // #3 ��ȡ���Ψһʵ������Ľӿڷ���
	{
		//lock_guard<std::mutex> guard(mtx); // ��������̫����
		if (instance == nullptr)
		{
			lock_guard<std::mutex> guard(mtx);
			if (instance == nullptr)
			{
				/*
				�����ڴ�
				��instance��ֵ
				�������
				*/
				instance = new Singleton();
			}
		}
		return instance;
	}
private:
	static Singleton *volatile instance; // #2 ����һ��Ψһ�����ʵ������
	Singleton() // #1 ���캯��˽�л�
	{

	}
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
};
Singleton*volatile Singleton::instance = nullptr;

int main()
{
	Singleton *p1 = Singleton::getInstance();
	Singleton *p2 = Singleton::getInstance();
	Singleton *p3 = Singleton::getInstance();

	return 0;
}
#endif

#if 0
// ����ʽ����ģʽ һ�����̰߳�ȫ��
class Singleton
{
public:
	static Singleton* getInstance() // #3 ��ȡ���Ψһʵ������Ľӿڷ���
	{
		return &instance;
	}
private:
	static Singleton instance; // #2 ����һ��Ψһ�����ʵ������
	Singleton() // #1 ���캯��˽�л�
	{

	}
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
};
Singleton Singleton::instance;

int main()
{
	//Singleton *p1 = Singleton::getInstance();
	//Singleton *p2 = Singleton::getInstance();
	//Singleton *p3 = Singleton::getInstance();

	return 0;
}
#endif