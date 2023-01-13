#include "pch.h"
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

#if 0
// 编程题目：请实现以下类的方法，完成大数的加减法
class BigInt
{
public:
	BigInt(string str) :strDigit(str) {}
private:
	string strDigit;   // 使用字符串存储大整数

	friend ostream& operator<<(ostream &out, const BigInt &src);
	friend BigInt operator+(const BigInt &lhs, const BigInt &rhs);
	friend BigInt operator-(const BigInt &lhs, const BigInt &rhs);
};
// 打印函数
ostream& operator<<(ostream &out, const BigInt &src)
{
	out << src.strDigit;
	return out;
}
// 大数加法
BigInt operator+(const BigInt &lhs, const BigInt &rhs)
{
	/*
	遍历字符串l，r，从后往前遍历
	同位置的数字相加， 进位 flag  存入一个结果当中 string result
	同时完成
	某个字符串先完成   都要考虑进位
	*/
	string result;
	bool flag = false;
	int size1 = lhs.strDigit.length() - 1;
	int size2 = rhs.strDigit.length() - 1;
	int i = size1, j = size2;

	for (; i >= 0 && j >= 0; --i, --j)
	{
		int ret = lhs.strDigit[i] - '0' + rhs.strDigit[j] - '0';
		if (flag)
		{
			ret += 1;
			flag = false;
		}

		if (ret >= 10)
		{
			ret %= 10;
			flag = true;
		}
		result.push_back(ret + '0');
	}

	// i j
	if (i >= 0)
	{
		while (i >= 0)
		{
			int ret = lhs.strDigit[i] - '0';
			if (flag)
			{
				ret += 1;
				flag = false;
			}

			if (ret >= 10)
			{
				ret %= 10;
				flag = true;
			}
			result.push_back(ret + '0');
			i--;
		}
	}
	else if (j >= 0)
	{
		while (j >= 0)
		{
			int ret = rhs.strDigit[j] - '0';
			if (flag)
			{
				ret += 1;
				flag = false;
			}

			if (ret >= 10)
			{
				ret %= 10;
				flag = true;
			}
			result.push_back(ret + '0');
			j--;
		}
	}

	if (flag)
	{
		result.push_back('1');
	}

	reverse(result.begin(), result.end());
	return result; // return BigInt(result);
}
// 大数减法
BigInt operator-(const BigInt &lhs, const BigInt &rhs)
{
	/*
	找大的字符串左减数，小的左被减数
	遍历两个字符串，减法，借位（bool flag）， string result 存下来
	*/
	string result;
	bool flag = false;
	bool minor = false;

	string maxStr = lhs.strDigit;
	string minStr = rhs.strDigit;
	if (maxStr.length() < minStr.length())
	{
		maxStr = rhs.strDigit;
		minStr = lhs.strDigit;
		minor = true;
	}
	else if (maxStr.length() == minStr.length())
	{
		if (maxStr < minStr)
		{
			maxStr = rhs.strDigit;
			minStr = lhs.strDigit;
			minor = true;
		}
		else if (maxStr == minStr)
		{
			return string("0");
		}
	}
	else
	{
		;
	}

	int size1 = maxStr.length() - 1;
	int size2 = minStr.length() - 1;
	int i = size1, j = size2;

	for (; i >= 0 && j >= 0; --i, --j)
	{
		int ret = maxStr[i] - minStr[j];
		if (flag)
		{
			ret -= 1;
			flag = false;
		}

		if (ret < 0)
		{
			ret += 10;
			flag = true;
		}
		result.push_back(ret + '0');
	}

	while (i >= 0)
	{
		int ret = maxStr[i] - '0';
		if (flag)
		{
			ret -= 1;
			flag = false;
		}

		if (ret < 0)
		{
			ret += 10;
			flag = true;
		}

		result.push_back(ret + '0');
		i--;
	}

	string retStr;
	auto it = result.rbegin();
	for (; it != result.rend(); ++it)
	{
		if (*it != '0')
		{
			break;
		}
	}
	for (; it != result.rend(); ++it)
	{
		retStr.push_back(*it);
	}
	// 100000
	if (minor)
	{
		retStr.insert(retStr.begin(), '-');
	}

	//reverse(result.begin(), result.end());
	return retStr;
}
int main()
{
	BigInt int1("9785645649886874535428765");
	BigInt int2("28937697857832167849697653231243");
	BigInt int3("9785645649886874535428765");
	//28937707643477817736572188660008
	//28937707643477817736572188660008
	cout << int1 + int2 << endl;
	//28937688072186517962823117802478
	//28937688072186517962823117802478
	cout << int1 - int2 << endl;

	BigInt int4("123");
	BigInt int5("99");
	cout << int5 - int4 << endl;

	return 0;
}
#endif