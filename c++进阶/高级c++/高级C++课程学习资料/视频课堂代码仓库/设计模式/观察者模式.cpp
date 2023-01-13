#include "pch.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
using namespace std;

/*
��Ϊ��ģʽ����Ҫ��ע���Ƕ���֮���ͨ��
�۲���ģʽObserver Pattern������-����ģʽ�����ģʽ:��Ҫ��ע���Ƕ����һ�Զ�Ĺ�ϵ��Ҳ���Ƕ������
������һ�����󣬵��ö����״̬�����ı�ʱ�����������ܹ����յ���Ӧ��֪ͨ��

һ�����ݣ����ݶ���   =>  ͨ����һ������ =��  ����ͼ(����1)/��״ͼ(����2)/Բ��ͼ(����3)
�����ݶ���ı�ʱ������1������2������3Ӧ�ü�ʱ���յ���Ӧ��֪ͨ��

Observer1  Observer2   Observer3

		   Subject�����⣩�����и��ģ�Ӧ�ü�ʱ֪ͨ��Ӧ�Ĺ۲��ߣ�ȥ������Ӧ���¼�
*/
// �۲��߳�����
class Observer 
{
public:
	// ������Ϣ�Ľӿ�
	virtual void handle(int msgid) = 0;
};
// ��һ���۲���ʵ��
class Observer1 : public Observer
{
public:
	void handle(int msgid)
	{
		switch (msgid)
		{
		case 1:
			cout << "Observer1 recv 1 msg!" << endl;
			break;
		case 2:
			cout << "Observer1 recv 2 msg!" << endl;
			break;
		default:
			cout << "Observer1 recv unknow msg!" << endl;
			break;
		}
	}
};
// �ڶ����۲���ʵ��
class Observer2 : public Observer
{
public:
	void handle(int msgid)
	{
		switch (msgid)
		{
		case 2:
			cout << "Observer2 recv 2 msg!" << endl;
			break;
		default:
			cout << "Observer2 recv unknow msg!" << endl;
			break;
		}
	}
};
// �������۲���ʵ��
class Observer3 : public Observer
{
public:
	void handle(int msgid)
	{
		switch (msgid)
		{
		case 1:
			cout << "Observer3 recv 1 msg!" << endl;
			break;
		case 3:
			cout << "Observer3 recv 3 msg!" << endl;
			break;
		default:
			cout << "Observer3 recv unknow msg!" << endl;
			break;
		}
	}
};

// ������
class Subject
{
public:
	// ���������ӹ۲��߶���
	void addObserver(Observer* obser, int msgid)
	{
		_subMap[msgid].push_back(obser);
		/*auto it = _subMap.find(msgid);
		if (it != _subMap.end())
		{
			it->second.push_back(obser);
		}
		else
		{
			list<Observer*> lis;
			lis.push_back(obser);
			_subMap.insert({ msgid, lis });
		}*/
	}
	// �����ⷢ���ı䣬֪ͨ��Ӧ�Ĺ۲��߶������¼�
	void dispatch(int msgid)
	{
		auto it = _subMap.find(msgid);
		if (it != _subMap.end())
		{
			for (Observer *pObser : it->second)
			{
				pObser->handle(msgid);
			}
		}
	}
private:
	unordered_map<int, list<Observer*>> _subMap;
};

int main()
{
	Subject subject;
	Observer *p1 = new Observer1();
	Observer *p2 = new Observer2();
	Observer *p3 = new Observer3();

	subject.addObserver(p1, 1);
	subject.addObserver(p1, 2);
	subject.addObserver(p2, 2);
	subject.addObserver(p3, 1);
	subject.addObserver(p3, 3);

	int msgid = 0;
	for (;;)
	{
		cout << "������Ϣid:";
		cin >> msgid;
		if (msgid == -1)
			break;
		subject.dispatch(msgid);
	}

	return 0;
}