#include <iostream>
#include <vector>
#include <algorithm> // ������C++ STL����ķ����㷨
#include <functional> // �����˺�������Ͱ���
using namespace std;


#if 0
/*
�塢�����㷨 = template + ������ + ��������
�ص�һ�������㷨�Ĳ������յĶ��ǵ�����
�ص���������㷨�Ĳ��������Խ��պ�������C����ָ�룩
sort,find,find_if,binary_search,for_each

���� + ��Ԫ�������� =�� һԪ��������
bind1st���Ѷ�Ԫ���������operator()(a, b)�ĵ�һ���βΰ�����
bind2nd���Ѷ�Ԫ���������operator()(a, b)�ĵڶ����βΰ�����
*/
int main()
{
	int arr[] = { 12,4,78,9,21,43,56,52,42,31};
	vector<int> vec(arr, arr+sizeof(arr)/sizeof(arr[0]));

	for (int v : vec)
	{
		cout << v << " ";
	}
	cout << endl;

	// Ĭ��С���������
	sort(vec.begin(), vec.end());

	for (int v : vec)
	{
		cout << v << " ";
	}
	cout << endl;

	// ���������н��ж��ֲ���
	if (binary_search(vec.begin(), vec.end(), 21)) 
	{
		cout << "binary_search ����" << endl;
	}

	auto it1 = find(vec.begin(), vec.end(), 21);
	if (it1 != vec.end())
	{
		cout << "find 21����" << endl;
	}

	// ���뺯������greater���ı�����Ԫ������ʱ�ıȽϷ�ʽ
	sort(vec.begin(), vec.end(), greater<int>());
	for (int v : vec)
	{
		cout << v << " ";
	}
	cout << endl;

	// 78 56 52 43 42 31 21 12 9 4
	// ��48������뵽vector��������  �ҵ�һ��С��48������
	// find_if��Ҫ����һ��һԪ��������
	// greater a > b    less a < b(48)
	auto it2 = find_if(vec.begin(), vec.end(),
		//bind1st(greater<int>(), 48));
		//bind2nd(less<int>(), 48));
		[](int val)->bool {return val < 48; });
	vec.insert(it2, 48);
	for (int v : vec)
	{
		cout << v << " ";
	}
	cout << endl;

	// for_each���Ա�������������Ԫ�أ�����������Ӻ��ʵĺ������������
	// ��Ԫ�ؽ��й���
	for_each(vec.begin(), vec.end(), 
		[](int val)->void
	{
		if (val % 2 == 0)
		{
			cout << val << " ";
		}
	});
	cout << endl;

	return 0;
}
#endif