
#include <iostream>
#include <vector>
using namespace std;

/*
�����ĵ�����
const_iterator:���������������  ֻ�ܶ���������д��
iterator:��ͨ�����������
reverse_iterator:��ͨ�ķ��������
const_reverse_iterator:�����ķ��������
*/
#if 0
int main()
{
	vector<int> vec;
	for (int i = 0; i < 20; ++i)
	{
		vec.push_back(rand() % 100);
	}

	// vector<int>::iterator
	// auto it1 = vec.begin(); 
	// const_iterator   <=   iterator
	/*
	class const_iterator
	{
	public:
		const T& operator*(){return *_ptr;}
	}
	class iterator : public const_iterator
	{
		T& operator*(){return *_ptr;}
	}
	*/
	vector<int>::const_iterator it1 = vec.begin();
	for (; it1 != vec.end(); ++it1)
	{
		cout << *it1 << " ";
	}
	cout << endl;

	// rbegin()�����ص������һ��Ԫ�صķ����������ʾ
	// rend�����ص�����Ԫ��ǰ��λ�õĵ������ı�ʾ
	// vector<int>::reverse_iterator
	vector<int>::const_reverse_iterator rit = vec.rbegin();
	for (; rit != vec.rend(); ++rit)
	{
		cout << *rit << " ";
	}
	cout << endl;

	/*for (int v : vec)
	{
		cout << v << " ";
	}
	*/
	cout << endl;

	return 0;
}
#endif