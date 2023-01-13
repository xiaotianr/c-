#include <iostream>
#include <vector>
using namespace std;

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
	//带左值引用参数的拷贝构造
	CMyString(const CMyString &str)
	{
		cout << "CMyString(const CMyString&)" << endl;
		mptr = new char[strlen(str.mptr) + 1];
		strcpy(mptr, str.mptr);
	}
    //带右值引用参数的拷贝构造
    CMyString(CMyString &&str)//str引用的就是一个临时对象
	{
		cout << "CMyString(CMyString&&)" << endl;
		mptr =str.mptr;
		str.mptr=nullptr;
	}

	//带左值引用参数的赋值重载构造
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
    //带右值引用参数的赋值重载构造
	CMyString& operator=(CMyString &&str) //临时对象
	{
		cout << "operator=(const CMyString&)" << endl;
		if (this == &str)
			return *this;

		delete[]mptr;

		mptr=str.mptr;
        str.mptr=nullptr;
		return *this;
	}
	
	const char* c_str()const { return mptr; }
private:
	char *mptr;
    friend CMyString operator+(const CMyString &lhs,
	const CMyString &rhs);
    friend ostream& operator<<(ostream &out,const CMyString &str);
};

CMyString GetString(CMyString &str)
{
    const char* pstr=str.c_str();
    CMyString tmpStr(pstr);
    return tmpStr;
}
CMyString operator+(const CMyString &lhs,
	const CMyString &rhs)
{
	//char *ptmp = new char[strlen(lhs.mptr) + strlen(rhs.mptr) + 1];//效率太慢了
    CMyString tmpStr=new char[strlen(lhs.mptr) + strlen(rhs.mptr) + 1];
	strcpy(tmpStr.mptr, lhs.mptr);
	strcat(tmpStr.mptr, rhs.mptr);
    // CMyString tmpStr(ptmp);
    //delete []ptmp;
    return tmpStr;//带右值引用参数的拷贝构造
	//return CMyString(ptmp);//直接这样写，ptmp new出来了的内存无法释放，内存泄漏
}
ostream& operator<<(ostream &out,const CMyString &str)
{
    out << str.mptr;
    return out;
}

template<typename T>
struct Allocator
{
	T* allocate(size_t size) 
	{
		return (T*)malloc(sizeof(T) * size);
	}
	void deallocate(void *p) 
	{
		free(p);
	}
	/*void construct(T *p, const T &val) 
	{
		new (p) T(val); // ��λnew
	}
	void construct(T *p, T &&val) 
	{
		new (p) T(std::move(val)); 
	}*/
	template<typename Ty>
	void construct(T *p, Ty &&val) 
	{
		new (p) T(std::forward<Ty>(val));
	}
	void destroy(T *p) // 
	{
		p->~T(); // 
	}
};
#if 0
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
	/*void push_back(const T &val) //接收左值
	{
		if (full())
			expand();

		_allocator.construct(_last, val);
		_last++;
	}

	void push_back(T &&val) // 接受右值   =》一个右值引用变量本身还是一个左值
	{
		if (full())
			expand();

		_allocator.construct(_last, std::move(val));//move:将左值强转为对应的右值
		_last++;
	}*/
	// void push_back(CMyString &val)
	// CMyString&& + && = void push_back(CMyString&&val)
	template<typename Ty> // 函数模块的类型推演+引用折叠
	void push_back(Ty &&val) //Ty CMyString& + && = CMyString&
	{
		if (full())
			expand();

		// move(左值ֵ)移动语义，得倒右值类型，类型强转   (int&&)a
		// forward:类型的完美转发，能够识别左值和右值类型
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
#endif

int main()
{
    CMyString str1="aaa";
    vector<CMyString> vec;
    vec.reserve(10);

    cout << "-----------------------" << endl;
    vec.push_back(str1);
    vec.push_back(CMyString("bbb"));
    cout << "-----------------------" << endl;

    /*CMyString str1="hello ";
    CMyString str2="word!";
    cout << "-----------------------" << endl;
    CMyString str3=str1+str2;
    cout << "-----------------------" << endl;
    cout << str3 << endl;*/

    return 0;
}

/*
CMyString(const char*)
CMyString(const char*)
CMyString(const char*)
CMyString(const CMyString&)=》temStr拷贝构造mian函数栈帧上的临时对象
~CMyString
operator=(const CMyString&)=》mian函数栈帧上的临时对象给str2赋值
~CMyString
aaaaaaaaaaaa
~CMyString
~CMyString
*/

#if 0
int main2()
{
    // 右值引用
    int a=10;
    int &b=a;//左值：有内存或有名字  右值：没名字（临时量）或没内存
    //int &&c=a;//无法将左值绑定到右值引用
    //int &c=20;//不能用左值引用绑定一个右值
    /*
    int tmp=20;
    const int &c=tmp;
    */
    const int &c=20;
    int &&d=20;//可以把一个右值绑定到一个右值引用，和上面的区别是，可以通过d改变临时量的值！！！
    //CMyString &e=CMyString("eee");//同理，无法将左值绑定到右值引用
    CMyString &&e=CMyString("eee");

    //int &&f=d;//一个右值引用变量，本身是一个左值
    int &f=d;
    return 0;
}
#endif