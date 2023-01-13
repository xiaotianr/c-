#include <iostream>
using namespace std;

int main()
{
    int a=10;
    int *p=&a;
    const int * const * q=&p;
    return 0;
}