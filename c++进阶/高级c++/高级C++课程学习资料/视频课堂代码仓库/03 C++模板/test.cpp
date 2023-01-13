#include "pch.h"
#include <iostream>
using namespace std;

// ����ģ��
template<typename T> // ����һ��ģ������б�
bool compare(T a, T b) // compare��һ������ģ��
{
	cout << "template compare" << endl;
	return a > b;
}

// ���߱�����������ָ�����͵�ģ��ʵ����
template bool compare<int>(int, int);
template bool compare<double>(double, double);

// ���compare����ģ�壬�ṩconst char*���͵��������汾
template<>
bool compare<const char*>(const char *a, const char *b)
{
	cout << "compare<const char*>" << endl;
	return strcmp(a, b) > 0;
}

// ��ģ�庯�� - ��ͨ����
bool compare(const char*a, const char*b)
{
	cout << "normal compare" << endl;
	return strcmp(a, b) > 0;
}