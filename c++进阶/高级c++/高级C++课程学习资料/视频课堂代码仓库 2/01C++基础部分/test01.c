#include <stdio.h>

/*
const���ε��������Բ��ó�ʼ��
���г���������������  
*/
void main03()
{
	const int a = 20;
	//int array[a] = {};

	int *p = (int*)&a;
	*p = 30;

	// 30 30 30
	printf("%d %d %d \n", a, *p, *(&a));
}