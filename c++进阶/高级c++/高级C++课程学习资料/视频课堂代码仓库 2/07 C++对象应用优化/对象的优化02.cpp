#include "pch.h"
#include <iostream>
using namespace std;

#if 0
class Test
{
public:
	// Test()  Test(20)
	Test(int data = 10) :ma(data)
	{
		cout << "Test(int)" << endl;
	}
	~Test()
	{
		cout << "~Test()" << endl;
	}
	Test(const Test &t):ma(t.ma)
	{
		cout << "Test(const Test&)" << endl;
	}
	void operator=(const Test &t)
	{
		cout << "operator=" << endl;
		ma = t.ma;
	}
	int getData()const { return ma; }
private:
	int ma;
};
// ���ܷ��ؾֲ��Ļ�����ʱ�����ָ�������
/*
1. �����������ݹ����У��������Ȱ����ô��ݣ���Ҫ��ֵ����
2. �������ض����ʱ��Ӧ�����ȷ���һ����ʱ���󣬶���Ҫ����һ��������Ķ���
3. ���շ���ֵ�Ƕ���ĺ������õ�ʱ�����Ȱ���ʼ���ķ�ʽ���գ���Ҫ����ֵ�ķ�ʽ����
*/
Test GetObject(Test &t) 
{
	int val = t.getData();
	/*Test tmp(val);
	return tmp;*/
	// ������ʱ����
	return Test(val); 
}
int main()
{
	Test t1;
	Test t2 = GetObject(t1);
	//t2 = GetObject(t1);
	return 0;
}
#endif