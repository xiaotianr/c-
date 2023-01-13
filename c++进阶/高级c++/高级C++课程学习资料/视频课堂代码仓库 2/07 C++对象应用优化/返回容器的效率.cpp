#include "pch.h"
#include <iostream>
#include <vector>
using namespace std;

#if 0
class CMyString
{
public:
	CMyString(const char *str = nullptr)
	{
		cout << "CMyString(const char*)" << endl;
		if (str != nullptr)
		{
			mptr = new char[strlen(str) + 1];
			strcpy(mptr, str);
		}
		else
		{
			mptr = new char[1];
			*mptr = '\0';
		}
	}
	~CMyString()
	{
		cout << "~CMyString" << endl;
		delete[]mptr;
		mptr = nullptr;
	}
	// ����ֵ���ò����Ŀ�������
	CMyString(const CMyString &str)
	{
		cout << "CMyString(const CMyString&)" << endl;
		mptr = new char[strlen(str.mptr) + 1];
		strcpy(mptr, str.mptr);
	}
	// ����ֵ���ò����Ŀ�������
	CMyString(CMyString &&str)
	{
		cout << "CMyString(CMyString&&)" << endl;
		mptr = str.mptr;
		str.mptr = nullptr;
	}
	// ����ֵ���ò����ĸ�ֵ���غ���
	CMyString& operator=(const CMyString &str)
	{
		cout << "operator=(const CMyString&)" << endl;
		if (this == &str)
			return *this;

		delete[]mptr;

		mptr = new char[strlen(str.mptr) + 1];
		strcpy(mptr, str.mptr);
		return *this;
	}
	// ����ֵ���ò����ĸ�ֵ���غ���
	CMyString& operator=(CMyString &&str) // ��ʱ����
	{
		cout << "operator=(CMyString&&)" << endl;
		if (this == &str)
			return *this;

		delete[]mptr;

		mptr = str.mptr;
		str.mptr = nullptr;
		return *this;
	}
	const char* c_str()const { return mptr; }
private:
	char *mptr;
};

vector<CMyString> GetVector()
{
	CMyString str1 = "aaa", str2 = "bbb", str3 = "ccc";
	cout << "------------------------" << endl;

	vector<CMyString> vec;
	vec.reserve(10);
	vec.push_back(std::move(str1));
	vec.push_back(std::move(str2));
	vec.push_back(std::move(str3));

	return vec; // ƥ�������������ֵ���ò����Ŀ�������
}
int main()
{
	vector<CMyString> vec;
	
	vec = GetVector(); // ƥ�������������ֵ���ò����ĸ�ֵ����

	return 0;
}
#endif