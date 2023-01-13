#include "pch.h"
#include <iostream>
using namespace std;

#if 0
// ���������Ŀռ�����������C++��׼���allocatorʵ��һ��
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
	void construct(T *p, const T &val) // ���������
	{
		new (p) T(val); // ��λnew
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
	void push_back(const T &val) // ������ĩβ���Ԫ��
	{
		if (full())
			expand();
		//*_last++ = val;   _lastָ��ָ����ڴ湹��һ��ֵΪval�Ķ���
		_allocator.construct(_last, val);
		_last++;
	}
	void pop_back() // ������ĩβɾ��Ԫ��
	{
		if (empty())
			return;
		// erase(it);  verify(it._ptr, _last);
		// insert(it, val); verify(it._ptr, _last);
		verify(_last - 1, _last);
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
	T& operator[](int index) // vec[2]
	{ 
		if (index < 0 || index >= size())
		{
			throw "OutOfRangeException";
		}
		return _first[index]; 
	}

	// insert erase

	// #1������һ��ʵ�ֳ�������Ƕ������
	class iterator
	{
	public:
		friend class vector<T, Alloc>;
		iterator(vector<T, Alloc> *pvec=nullptr
			, T *ptr = nullptr)
			:_ptr(ptr), _pVec(pvec)
		{
			Iterator_Base *itb = 
				new Iterator_Base(this, _pVec->_head._next);
			_pVec->_head._next = itb;
		}
		bool operator!=(const iterator &it)const
		{
			// ������������Ч��
			if (_pVec == nullptr || _pVec != it._pVec)
			{
				throw "iterator incompatable!";
			}
			return _ptr != it._ptr;
		}
		void operator++()
		{
			// ������������Ч��
			if (_pVec == nullptr)
			{
				throw "iterator invalid!";
			}
			_ptr++;
		}
		T& operator*() 
		{ 
			// ������������Ч��
			if (_pVec == nullptr)
			{
				throw "iterator invalid!";
			}
			return *_ptr; 
		} 
		const T& operator*()const 
		{ 
			// ������������Ч��
			if (_pVec == nullptr)
			{
				throw "iterator invalid!";
			}
			return *_ptr; 
		}
	private:
		T *_ptr;
		// ��ǰ���������������ĸ���������
		vector<T, Alloc> *_pVec;
	};
	// ��Ҫ�������ṩbegin��end����
	iterator begin() { return iterator(this, _first); }
	iterator end() { return iterator(this, _last); }

	// ��������ʧЧ
	void verify(T *first, T *last)
	{
		Iterator_Base *pre = &this->_head;
		Iterator_Base *it = this->_head._next;
		while (it != nullptr)
		{
			if (it->_cur->_ptr > first && it->_cur->_ptr <= last)
			{
				// ������ʧЧ����iterator���е�����ָ����nullptr
				it->_cur->_pVec = nullptr;
				// ɾ����ǰ�������ڵ㣬�����жϺ���ĵ������ڵ��Ƿ�ʧЧ
				pre->_next = it->_next;
				delete it;
				it = pre->_next;
			}
			else
			{
				pre = it;
				it = it->_next;
			}
		}
	}

	// �Զ���vector����insert������ʵ��
	iterator insert(iterator it, const T &val)
	{
		/* 
		1.���������� verify(_first - 1, _last);
		2.������it._ptr��ָ��Ϸ���
		*/
		verify(it._ptr - 1, _last);
		T *p = _last;
		while (p > it._ptr)
		{
			_allocator.construct(p, *(p-1));
			_allocator.destroy(p - 1);
			p--;
		}
		_allocator.construct(p, val);
		_last++;
		return iterator(this, p);
	}

	// �Զ���vector����erase������ʵ��
	iterator erase(iterator it)
	{
		verify(it._ptr - 1, _last);
		T *p = it._ptr;
		while (p < _last-1)
		{
			_allocator.destroy(p);
			_allocator.construct(p, *(p + 1));
			p++;
		}
		_allocator.destroy(p);
		_last--;
		return iterator(this, it._ptr);
	}

private:
	T *_first; // ָ��������ʼ��λ��
	T *_last;  // ָ����������ЧԪ�صĺ��λ��
	T *_end;   // ָ������ռ�ĺ��λ��
	Alloc _allocator; // ���������Ŀռ�����������

	// ����������ʧЧ���Ӵ���
	struct Iterator_Base
	{
		Iterator_Base(iterator *c=nullptr, Iterator_Base *n=nullptr)
			:_cur(c), _next(n) {}
		iterator *_cur;
		Iterator_Base *_next;
	};
	Iterator_Base _head;

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
	vector<int> vec(200);
	for (int i = 0; i < 20 ; ++i)
	{
		vec.push_back(rand() % 100 + 1);
	}

	auto it = vec.begin();
	while (it != vec.end())
	{
		if (*it % 2 == 0)
		{
			// ������ʧЧ�����⣬��һ�ε���erase�Ժ󣬵�����it��ʧЧ��
			it = vec.erase(it); // insert(it, val)   erase(it)
		}
		else
		{
			++it;
		}
	}

	for (int v : vec)
	{
		cout << v << " ";
	}
	cout << endl;



#if 0
	auto it1 = vec.end();
	vec.pop_back(); // verify(_last-1, _last)
	auto it2 = vec.end();
	cout << (it1 != it2) << endl;


	int size = vec.size();
	for (int i = 0; i < size; ++i)
	{
		cout << vec[i] << " ";
	}
	cout << endl;

	auto it = vec.begin();
	for (; it != vec.end(); ++it)
	{
		cout << *it << " ";
	}
	cout << endl;

	// foreach
	for (int val : vec) // ��ײ�ԭ������ͨ�������ĵ�������ʵ������������
	{
		cout << val << " ";
	}
	cout << endl;
#endif
	return 0;
}
#endif