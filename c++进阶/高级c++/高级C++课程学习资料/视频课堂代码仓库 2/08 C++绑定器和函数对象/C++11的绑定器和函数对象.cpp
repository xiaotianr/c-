#include "pch.h"
#include <iostream>
#include <vector> 
#include <map>
#include <functional>  // ʹ��function������������
#include <algorithm>
#include <ctime>
#include <string>
using namespace std;

#if 0
/*
C++11�ṩ�İ����ͺ�������
          bind   function
		  C++ STL  bind1st��bind2nd =�� ������һ����������
function : ��������������lambda���ʽ ����ֻ��ʹ����һ�������
*/

void doShowAllBooks() { cout << "�鿴�����鼮��Ϣ" << endl; }
void doBorrow() { cout << "����" << endl; }
void doBack() { cout << "����" << endl; }
void doQueryBooks() { cout << "��ѯ�鼮" << endl; }
void doLoginOut() { cout << "ע��" << endl; }

int main()
{
	int choice = 0;
	//       C�ĺ���ָ��
	map<int, function<void()>> actionMap;
	actionMap.insert({ 1, doShowAllBooks }); // insert(make_pair(xx,xx));
	actionMap.insert({ 2, doBorrow });
	actionMap.insert({ 3, doBack });
	actionMap.insert({ 4, doQueryBooks });
	actionMap.insert({ 5, doLoginOut });

	for (;;)
	{
		cout << "-----------------" << endl;
		cout << "1.�鿴�����鼮��Ϣ" << endl;
		cout << "2.����" << endl;
		cout << "3.����" << endl;
		cout << "4.��ѯ�鼮" << endl;
		cout << "5.ע��" << endl;
		cout << "-----------------" << endl;
		cout << "��ѡ��:";
		cin >> choice;

		auto it = actionMap.find(choice); // map  pair  first second
		if (it == actionMap.end())
		{
			cout << "����������Ч������ѡ��!" << endl;
		}
		else
		{
			it->second();
		}
		// ���ã���Ϊ�������޷��պ�  �޷���������-��ԭ��
		/*switch (choice)
		{
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		default:
			break;
		}*/
	}

	return 0;
}


void hello1()
{
	cout << "hello world!" << endl;
}
void hello2(string str) // void (*pfunc)(string)
{
	cout << str << endl;
}
int sum(int a, int b)
{
	return a + b;
}

class Test
{
public: // ��������һ������void (Test::*pfunc)(string)
	void hello(string str) { cout << str << endl; }
};
int main()
{
	/*
	1.�ú�������ʵ����function
	2.ͨ��function����operator()������ʱ����Ҫ���ݺ������ʹ�����Ӧ�Ĳ���
	*/
	// ��function����ģ�嶨�崦������ϣ����һ����������ʵ����function
	function<void()> func1 = hello1;
	func1(); // func1.operator()() => hello1()

	function<void(string)> func2 = hello2;
	func2("hello hello2!"); // func2.operator()(string str) => hello2(str)

	function<int(int, int)> func3 = sum;
	cout<<func3(20, 30)<<endl;

	// operator()
	function<int(int, int)> func4 = [](int a, int b)->int {return a + b; };
	cout << func4(100, 200) << endl;

	function<void(Test*, string)> func5 = &Test::hello;
	func5(&Test(), "call Test::hello!");

	return 0;
}
#endif