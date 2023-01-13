#include <iostream>
#include <queue>
#include <vector>
#include <set>
using namespace std;


#if 0
int main()
{
	set<int, greater<int>> set1;
	for (int i = 0; i < 10; ++i)
	{
		set1.insert(rand() % 100);
	}

	for (int v : set1)
	{
		cout << v << " ";
	}
	cout << endl;

	return 0;
}



int main()
{
	priority_queue<int> que1; // vector
	for (int i = 0; i < 10; ++i)
	{
		que1.push(rand() % 100);
	}

	while (!que1.empty())
	{
		cout << que1.top() << " ";
		que1.pop();
	}
	cout << endl;

	using MinHeap = priority_queue<int, vector<int>, greater<int>>;
	MinHeap que2; // vector
	for (int i = 0; i < 10; ++i)
	{
		que2.push(rand() % 100);
	}

	while (!que2.empty())
	{
		cout << que2.top() << " ";
		que2.pop();
	}
	cout << endl;

	return 0;
}


/*
��������  =>  C��������ĺ���ָ��
*/

// ʹ��C�ĺ���ָ��������
/*
template<typename T>
inline bool mygreater(T a, T b)
{
	return a > b;
}
template<typename T>
inline bool myless(T a, T b)
{
	return a < b;
}
*/

/*
1.ͨ�������������operator()������ʡ�Ժ����ĵ��ÿ�������ͨ������ָ��
���ú��������ܹ�inline�������ã�Ч�ʸ�

2.��Ϊ�����������������ɵģ����Կ��������صĳ�Ա������������¼��������ʹ��
ʱ�������Ϣ
*/
// C++��������İ汾ʵ��
template<typename T>
class mygreater
{
public:
	bool operator()(T a, T b) // ��Ԫ��������
	{
		return a > b;
	}
};
template<typename T>
class myless
{
public:
	bool operator()(T a, T b) // ��Ԫ��������
	{
		return a < b;
	}
};

// compare��C++�Ŀ⺯��ģ��
template<typename T, typename Compare>
bool compare(T a, T b, Compare comp)
{
	// ͨ������ָ����ú�������û�а취�����ģ�Ч�ʺܵͣ���Ϊ�к������ÿ���
	return comp(a, b);  // operator()(a, b);
}
int main()
{
	cout << compare(10, 20, mygreater<int>()) << endl;
	cout << compare(10, 20, myless<int>()) << endl;
	//cout << compare('b', 'y') << endl;

	return 0;
}
#endif