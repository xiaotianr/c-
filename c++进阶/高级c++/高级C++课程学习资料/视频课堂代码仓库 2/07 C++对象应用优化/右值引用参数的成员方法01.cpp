#include "pch.h"
#include <iostream>
//#include <vector>
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
	CMyString(CMyString &&str) // str���õľ���һ����ʱ����
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

	friend CMyString operator+(const CMyString &lhs,
		const CMyString &rhs);
	friend ostream& operator<<(ostream &out, const CMyString &str);
};
CMyString operator+(const CMyString &lhs,
	const CMyString &rhs)
{
	//char *ptmp = new char[strlen(lhs.mptr) + strlen(rhs.mptr) + 1];
	CMyString tmpStr;
	tmpStr.mptr = new char[strlen(lhs.mptr) + strlen(rhs.mptr) + 1];
	strcpy(tmpStr.mptr, lhs.mptr);
	strcat(tmpStr.mptr, rhs.mptr);
	//delete []ptmp;
	return tmpStr; // 
	//return CMyString(ptmp);
}
ostream& operator<<(ostream &out, const CMyString &str)
{
	out << str.mptr;
	return out;
}

template<typename T>
struct Allocator
{
	T* allocate(size_t size) // �����ڴ濪��
	{
		return (T*)malloc(sizeof(T) * size);
	}
	void deallocate(void *p) // �����ڴ��ͷ�
	{
		free(p);
	}
	/*void construct(T *p, const T &val) // ���������
	{
		new (p) T(val); // ��λnew
	}
	void construct(T *p, T &&val) // ���������
	{
		new (p) T(std::move(val)); // ��λnew
	}*/
	template<typename Ty>
	void construct(T *p, Ty &&val)
	{
		new (p) T(std::forward<Ty>(val));
	}
	void destroy(T *p) // �����������
	{
		p->~T(); // ~T()������T���͵���������
	}
};

/*
�����ײ��ڴ濪�٣��ڴ��ͷţ����������������ͨ��allocator�ռ���������ʵ��
*/
template<typename T, typename Alloc = Allocator<T>>
class vector
{
public:
	vector(int size = 10)
	{
		// ��Ҫ���ڴ濪�ٺͶ�����ֿ�����
		//_first = new T[size];
		_first = _allocator.allocate(size);
		_last = _first;
		_end = _first + size;
	}
	~vector()
	{
		// ����������Ч��Ԫ�أ�Ȼ���ͷ�_firstָ��ָ��Ķ��ڴ�
		// delete[]_first;
		for (T *p = _first; p != _last; ++p)
		{
			_allocator.destroy(p); // ��_firstָ��ָ����������ЧԪ�ؽ�����������
		}
		_allocator.deallocate(_first); // �ͷŶ��ϵ������ڴ�
		_first = _last = _end = nullptr;
	}
	vector(const vector<T> &rhs)
	{
		int size = rhs._end - rhs._first;
		//_first = new T[size];
		_first = _allocator.allocate(size);
		int len = rhs._last - rhs._first;
		for (int i = 0; i < len; ++i)
		{
			//_first[i] = rhs._first[i];
			_allocator.construct(_first + i, rhs._first[i]);
		}
		_last = _first + len;
		_end = _first + size;
	}
	vector<T>& operator=(const vector<T> &rhs)
	{
		if (this == &rhs)
			return *this;

		//delete[]_first;
		for (T *p = _first; p != _last; ++p)
		{
			_allocator.destroy(p); // ��_firstָ��ָ����������ЧԪ�ؽ�����������
		}
		_allocator.deallocate(_first);

		int size = rhs._end - rhs._first;
		//_first = new T[size];
		_first = _allocator.allocate(size);
		int len = rhs._last - rhs._first;
		for (int i = 0; i < len; ++i)
		{
			//_first[i] = rhs._first[i];
			_allocator.construct(_first + i, rhs._first[i]);
		}
		_last = _first + len;
		_end = _first + size;
		return *this;
	}
	void pop_back() // ������ĩβɾ��Ԫ��
	{
		if (empty())
			return;
		//--_last; // ����Ҫ��_lastָ��--������Ҫ����ɾ����Ԫ��
		--_last;
		_allocator.destroy(_last);
	}
	T back()const // ��������ĩβ��Ԫ�ص�ֵ
	{
		return *(_last - 1);
	}
	bool full()const { return _last == _end; }
	bool empty()const { return _first == _last; }
	int size()const { return _last - _first; }

	//////////////////////////////////////////
	/*void push_back(const T &val) // ������ֵ
	{
		if (full())
			expand();

		_allocator.construct(_last, val);
		_last++;
	}

	void push_back(T &&val) // ������ֵ һ����ֵ���ñ���������һ����ֵ
	{
		if (full())
			expand();

		_allocator.construct(_last, std::move(val));
		_last++;
	}*/
	// void push_back(CMyString &val)
	// CMyString&& + && = void push_back(CMyString&&val)
	template<typename Ty> // ����ģ����������� + �����۵�
	void push_back(Ty &&val) //Ty CMyString& + && = CMyString&
	{
		if (full())
			expand();

		// move(��ֵ)���ƶ����壬�õ���ֵ����   (int&&)a
		// forward:��������ת�����ܹ�ʶ����ֵ����ֵ����
		_allocator.construct(_last, std::forward<Ty>(val));
		_last++;
	}
private:
	T *_first; // ָ��������ʼ��λ��
	T *_last;  // ָ����������ЧԪ�صĺ��λ��
	T *_end;   // ָ������ռ�ĺ��λ��
	Alloc _allocator; // ���������Ŀռ�����������

	void expand() // �����Ķ�������
	{
		int size = _end - _first;
		//T *ptmp = new T[2 * size];
		T *ptmp = _allocator.allocate(2 * size);
		for (int i = 0; i < size; ++i)
		{
			//ptmp[i] = _first[i];
			_allocator.construct(ptmp + i, _first[i]);
		}
		//delete[]_first;
		for (T *p = _first; p != _last; ++p)
		{
			_allocator.destroy(p);
		}
		_allocator.deallocate(_first);
		_first = ptmp;
		_last = _first + size;
		_end = _first + 2 * size;
	}
};

int main()
{
	CMyString str1 = "aaa";
	vector<CMyString> vec;

	cout << "-----------------------" << endl;
	vec.push_back(std::move(str1)); // CMyString&
	vec.push_back(CMyString("bbb"));   // CMyString&& move  forword
	cout << "-----------------------" << endl;

	return 0;
}
#endif

#if 0
int main()
{
	CMyString str1 = "aaa";

	vector<CMyString> vec;
	vec.reserve(10);

	cout << "-----------------------" << endl;
	vec.push_back(str1);
	vec.push_back(CMyString("bbb"));   // move  forword
	cout << "-----------------------" << endl;

	/*CMyString str1 = "hello ";
	CMyString str2 = "world!";
	cout << "-----------------------" << endl;
	CMyString str3 = str1 + str2;
	cout << "-----------------------" << endl;
	cout << str3 << endl;*/

	return 0;
}



CMyString GetString(CMyString &str)
{
	const char* pstr = str.c_str();
	CMyString tmpStr(pstr);
	return tmpStr;
}
int main()
{
	CMyString str1("aaaaaaaaaaaaaaaaaaaa");
	CMyString str2;
	str2 = GetString(str1);
	cout << str2.c_str() << endl;

	return 0;
}
/*
CMyString(const char*)
CMyString(const char*)
CMyString(const char*)
CMyString(const CMyString&) => tmpStr��������main����ջ֡�ϵ���ʱ����
~CMyString
operator=(const CMyString&) => main����ջ֡�ϵ���ʱ�����t2��ֵ
~CMyString
aaaaaaaaaaaaaaaaaaaa
~CMyString
~CMyString
*/

int main()
{
	// ��ֵ����
	int a = 10;
	int &b = a; // ��ֵ�����ڴ桢������   ��ֵ��û���֣���ʱ������û�ڴ�
	//int &&c = a; // �޷�����ֵ�󶨵���ֵ����

	/*
	int tmp = 20;
	const int &c = tmp;
	*/
	const int &c = 20; // ��������ֵ���ð�һ����ֵ
	/*
	int tmp = 20;
	int &&d = tmp;
	*/
	int &&d = 20; // ���԰�һ����ֵ�󶨵�һ����ֵ������
	CMyString &&e = CMyString("aaa");

	int &f = d; // һ����ֵ���ñ�����������һ����ֵ

	return 0;
}
#endif