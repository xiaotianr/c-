#include "pch.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include <map>
#include <memory>
#include <queue>
using namespace std;

/*
��Ȼlambda���ʽֻ��ʹ������䵱�У����������ʹ��֮ǰ����õ�lambda���ʽ��
��ô�죿��ʲô��������ʾlambda���ʽ��
��Ȼ����function��������ʾ���������������   bind1st/bind2nd bind����

lambda���ʽ =�� ��������   
*/

class Data
{
public:
	Data(int val1 = 10, int val2 = 10) :ma(val1), mb(val2) {}
	//bool operator>(const Data &data)const { return ma > data.ma; }
	//bool operator<(const Data &data)const { return ma < data.ma; }
	int ma;
	int mb;
};
int main()
{
	// ���ȼ�����
	//priority_queue<Data> queue;
	using FUNC = function<bool(Data&, Data&)>;
	priority_queue<Data, vector<Data>, FUNC> 
		maxHeap([](Data &d1, Data &d2)->bool
		{
			return d1.mb > d2.mb;
		});
	maxHeap.push(Data(10, 20));
	maxHeap.push(Data(15, 15));
	maxHeap.push(Data(20, 10));


	// ����ָ���Զ���ɾ����   delete p;  FILE*   fclose(FILE*)
	//unique_ptr<FILE, function<void(FILE*)>> 
		//ptr1(fopen("data.txt", "w"), [](FILE *pf) {fclose(pf); });


#if 0
	map<int, function<int(int, int)>> caculateMap;
	caculateMap[1] = [](int a, int b)->int {return a + b; };
	caculateMap[2] = [](int a, int b)->int {return a - b; };
	caculateMap[3] = [](int a, int b)->int {return a * b; };
	caculateMap[4] = [](int a, int b)->int {return a / b; };

	cout << "ѡ��:";
	int choice;
	cin >> choice;
	cout << "10 + 15:" << caculateMap[choice](10, 15) << endl;
#endif
	return 0;
}


#if 0
/*
C++11 ��������������� =�� lambda���ʽ
���������ȱ�㣺
ʹ���ڷ����㷨��������   �Ƚ�����/�Զ������  ���ȼ�����  ����ָ��

lambda���ʽ���﷨��
[�����ⲿ����](�β��б�)->����ֵ{��������};

���lambda���ʽ�ķ���ֵ����Ҫ����ô"->����ֵ"����ʡ�Ե�

[�����ⲿ����]
[]����ʾ�������κ��ⲿ����
[=]���Դ�ֵ�ķ�ʽ�����ⲿ�����б���
[&]���Դ����õķ�ʽ�����ⲿ�����б���
[this]�������ⲿ��thisָ��
[=,&a]���Դ�ֵ�ķ�ʽ�����ⲿ�����б���������a�����Դ����õķ�ʽ����
[a, b]����ֵ���ݵķ�ʽ�����ⲿ����a��b
[a, &b]��a��ֵ���ݲ���b�Դ����õķ�ʽ����
*/
template<typename T=void>
class TestLambda01
{
public:
	TestLambda01() {}
	void operator()()const
	{
		cout << "hello world!" << endl;
	}
};
template<typename T = int>
class TestLambda02
{
public:
	TestLambda02() {}
	int operator()(int a, int b)const
	{
		return a+b;
	}
};
template<typename T = int>
class TestLambda03
{
public:
	TestLambda03(int &a, int &b):ma(a), mb(b) {}
	void operator()()const
	{
		int tmp = ma;
		ma = mb;
		mb = tmp;
	}
private:
	int &ma;
	int &mb;
};
int main()
{
	vector<int> vec;
	for (int i = 0; i < 20; ++i)
	{
		vec.push_back(rand() % 100 + 1);
	}

	sort(vec.begin(), vec.end(), 
		[](int a, int b)->bool
		{
			return a > b;
		});
	for (int val : vec)
	{
		cout << val << " ";
	}
	cout << endl;

	// 65�����������  Ҫ�ҵ�һ��С��65������
	auto it = find_if(vec.begin(), vec.end(), 
		[](int a)->bool {return a < 65; });
	if (it != vec.end())
	{
		vec.insert(it, 65);
	}
	for (int val : vec)
	{
		cout << val << " ";
	}
	cout << endl;

	for_each(vec.begin(), vec.end(), [](int a)
	{
		if(a % 2 == 0)
			cout << a << " ";
	});
	cout << endl;

#if 0
	auto func1 = [](){cout << "hello world!" << endl; };
	func1();

	auto func2 = [](int a, int b)->int {return a + b; };
	cout<<func2(20, 30)<<endl;

	int a = 10;
	int b = 20;
	auto func3 = [&]() // ��a��: �޷��ڷǿɱ� lambda ���޸�ͨ�����Ʋ���
	{
		int tmp = a;
		a = b;
		b = tmp;
	};
	func3();
	cout << "a:" << a << " b:" << b << endl;

	cout << "-----------------------" << endl;

	TestLambda01<> t1;
	t1();

	TestLambda02<> t2;
	cout << t2(20, 30) << endl;

	TestLambda03<> t3(a, b);
	t3();
#endif
	return 0;
}
#endif