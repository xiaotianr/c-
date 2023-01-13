#include "pch.h"
#include <iostream>
#include <string>
#include <memory>
using namespace std;

#if 0
/*
�򵥹��� Simple Factory : 
�Ѷ���Ĵ�����װ��һ���ӿں������棬ͨ�����벻ͬ�ı�ʶ�����ش����Ķ���
�ͻ������Լ�����new���󣬲����˽���󴴽�����ϸ����

�ṩ��������ʵ���Ľӿں������պϣ����ܶ��޸Ĺر�

�������� Factory Method
Factory���࣬�ṩ��һ�����麯����������Ʒ�������������ࣨ�����Ʒ�Ĺ��������𴴽���Ӧ��
��Ʒ������������ͬ�Ĳ�Ʒ���ڲ�ͬ�Ĺ������洴�����ܹ������й������Լ���Ʒ���޸Ĺر�

ʵ���ϣ��ܶ��Ʒ���й�����ϵ�ģ�����һ����Ʒ�أ���Ӧ�÷��ڲ�ͬ�Ĺ�������ȥ����������
һ�ǲ�����ʵ�ʵĲ�Ʒ���󴴽��߼������ǹ�����̫���ˣ�����ά��

���󹤳� Abstract Factory
���й�����ϵ�ģ�����һ����Ʒ�ص����в�Ʒ�����Ľӿں���������һ�����󹤳�����AbstractFactory
�������ࣨ�����Ʒ�Ĺ�����Ӧ�ø��𴴽��ò�Ʒ���������еĲ�Ʒ

����ģʽ����Ҫ�Ƿ�װ�˶���Ĵ���
*/

// ϵ�в�Ʒ1
class Car
{
public:
	Car(string name) :_name(name) {}
	virtual void show() = 0;
protected:
	string _name;
};

class Bmw : public Car
{
public:
	Bmw(string name) :Car(name) {}
	void show()
	{
		cout << "��ȡ��һ����������:" << _name<<endl;
	}
};
class Audi : public Car
{
public:
	Audi(string name) :Car(name) {}
	void show()
	{
		cout << "��ȡ��һ���µ�����:" << _name<<endl;
	}
};

// ϵ�в�Ʒ2
class Light
{
public:
	virtual void show() = 0;
};
class BmwLight : public Light
{
public:
	void show() { cout << "BMW light!" << endl; }
};
class AudiLight : public Light
{
public:
	void show() { cout << "Audi light!" << endl; }
};

// �������� => ���󹤳�(����һ�������ϵ�Ĳ�Ʒ���ṩ��Ʒ�����ͳһ����)
class AbstractFactory
{
public:
	virtual Car* createCar(string name) = 0; // �������� ��������
	virtual Light* createCarLight() = 0; // �������� �������������Ĳ�Ʒ������
};
// ������
class BMWFactory : public AbstractFactory
{
public:
	Car* createCar(string name)
	{
		return new Bmw(name);
	}
	Light* createCarLight()
	{
		return new BmwLight();
	}
};
// �µϹ���
class AudiFactory : public AbstractFactory
{
public:
	Car* createCar(string name)
	{
		return new Audi(name);
	}
	Light* createCarLight()
	{
		return new AudiLight();
	}
};
int main()
{
	// ���ڿ��ǲ�Ʒ  һ���Ʒ���й�����ϵ��ϵ�в�Ʒ��
	unique_ptr<AbstractFactory> bmwfty(new BMWFactory());
	unique_ptr<AbstractFactory> audifty(new AudiFactory());
	unique_ptr<Car> p1(bmwfty->createCar("X6"));
	unique_ptr<Car> p2(audifty->createCar("A8"));
	unique_ptr<Light> l1(bmwfty->createCarLight());
	unique_ptr<Light> l2(audifty->createCarLight());

	p1->show();
	l1->show();

	p2->show();
	l2->show();

	return 0;
}
#endif

#if 0
class Car
{
public:
	Car(string name) :_name(name) {}
	virtual void show() = 0;
protected:
	string _name;
};

class Bmw : public Car
{
public:
	Bmw(string name) :Car(name) {}
	void show()
	{
		cout << "��ȡ��һ����������:" << _name<<endl;
	}
};
class Audi : public Car
{
public:
	Audi(string name) :Car(name) {}
	void show()
	{
		cout << "��ȡ��һ���µ�����:" << _name<<endl;
	}
};
// �򵥹���
enum CarType
{
	BMW,AUDI
};
class SimpleFactory
{
public:
	Car* createCar(CarType ct) // ������ ����-�ա�
	{
		switch (ct)
		{
		case BMW:
			return new Bmw("X1");
		case AUDI:
			return new Audi("A6");
		default:
			cerr << "���빤���Ĳ�������ȷ:" << ct << endl;
			break;
		}
		return nullptr;
	}
};
int main()
{
	//Car *p1 = new BMW("X1");
	//Car *p2 = new Audi("A6");

	unique_ptr<SimpleFactory> factory(new SimpleFactory());
	unique_ptr<Car> p1(factory->createCar(BMW));
	unique_ptr<Car> p2(factory->createCar(AUDI));
	p1->show();
	p2->show();

	return 0;
}
#endif