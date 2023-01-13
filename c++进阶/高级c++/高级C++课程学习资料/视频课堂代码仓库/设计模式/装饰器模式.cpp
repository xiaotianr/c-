#include "pch.h"
#include <iostream>
#include <memory>
using namespace std;




#if 0
/*
װ����ģʽ Decorator
ͨ������ʵ�ֹ�����ǿ�����⣺Ϊ����ǿ������Ĺ��ܣ�ͨ��ʵ������ķ�ʽ��
��д�ӿڣ��ǿ�����ɹ�����չ�ģ����Ǵ�������̫���������ӽ�����
*/
class Car // �������
{
public:
	virtual void show() = 0;
};

// ����ʵ���������
class Bmw : public Car
{
public:
	void show() 
	{
		cout << "����һ�����������������У���������";
	}
};
class Audi : public Car
{
public:
	void show()
	{
		cout << "����һ���µ������������У���������";
	}
};
class Benz : public Car
{
public:
	void show()
	{
		cout << "����һ�����������������У���������";
	}
};

// װ����1  ����Ѳ��
class ConcreteDecorator01 : public Car
{
public:
	ConcreteDecorator01(Car *p) :pCar(p) {}
	void show()
	{
		pCar->show();
		cout << ",����Ѳ��";
	}
private:
	Car *pCar;
};
class ConcreteDecorator02 : public Car
{
public:
	ConcreteDecorator02(Car *p) :pCar(p) {}
	void show()
	{
		pCar->show();
		cout << ",�Զ�ɲ��";
	}
private:
	Car *pCar;
};
class ConcreteDecorator03 : public Car
{
public:
	ConcreteDecorator03(Car *p) :pCar(p) {}
	void show()
	{
		pCar->show();
		cout << ",����ƫ��";
	}
private:
	Car *pCar;
};
int main()
{
	Car *p1 = new ConcreteDecorator01(new Bmw());
	p1 = new ConcreteDecorator02(p1);
	p1 = new ConcreteDecorator03(p1);
	p1->show();
	cout << endl;

	Car *p2 = new ConcreteDecorator02(new Audi());
	p2->show();
	cout << endl;

	Car *p3 = new ConcreteDecorator03(new Benz());
	p3->show();
	cout << endl;

	return 0;
}
#endif