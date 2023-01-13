#include <iostream>
using namespace std;

#if 0
class Test
{
public:
    Test(int a=10):ma(a){cout << "Test(int)" << endl;}
    ~Test() {cout << "~Test()" << endl;}
    Test(const Test &t):ma(t.ma) {cout << "Test(const Test&)" << endl;}
    Test& operator=(const Test &t)
    {
        cout << "operator=" << endl;
        ma=t.ma;
        return *this;
    }
private:
    int ma;
};
int main()
{
    Test t1;
    Test t2(t1);//拷贝构造
    Test t3=t1;//拷贝构造
    //Test(20)显示生成临时对象  生存周期：所在的语句

    /**c++编译器对对象构造的优化：用临时对象生成新对象的时候
     * 临时对象就不产生了，直接构造新对象就可以了
    */
    Test t4 = Test(20);  //与Test t4(20);没有区别!!!

    cout << "--------------------------" << endl;


    //t4.operator=(t2)
    t4 = t2;
    //t4.operator=(const Test &t)
    //显式生成临时对象
    t4 = Test(30);//这个临时对象必须生成
    t4 = (Test)30; 
    /**int -> Test 编译器看Test类有没有int的整形构造，即Test(int)
     */ 

    //隐式生成临时对象
    t4 = 30;//int -> Test(int)
    cout << "--------------------------" << endl;

    //Test *p = &Test(40);
    //这句话结束后，p指向的是一个已经析构的临时对象

    const Test &ref = Test(50);//非常量饮用的初始值必须为左值


    cout << "--------------------------" << endl;

    return 0;
}
#endif
class Test
{
public:
	// Test() Test(10) Test(10, 10)
	Test(int a = 5, int b = 5) 
		:ma(a), mb(b)
	{
		cout << "Test(int, int)" << endl;
	}
	~Test()
	{
		cout << "~Test()" << endl;
	}
	Test(const Test &src) 
		:ma(src.ma), mb(src.mb)
	{
		cout << "Test(const Test&)" << endl;
	}
	void operator=(const Test &src)
	{
		ma = src.ma; 
		mb = src.mb; 
		cout << "operator=" << endl;
	}
private:
	int ma;
	int mb;
};
Test t1(10, 10); // 1.Test(int, int)
int main()
{
	Test t2(20, 20); // 3.Test(int, int)
	Test t3 = t2; // 4.Test(const Test&)
	// static Test t4(30, 30);
	static Test t4 = Test(30, 30); // 5.Test(int, int)
	t2 = Test(40, 40); // 6.Test(int, int) operator= ~Test()
	// (50, 50) =  (Test)50; Test(int)
	t2 = (Test)(50, 50); // 7.Test(int,int) operator=  ~Test()
	t2 = 60; //Test(int) 8.Test(int,int) operator= ~Test()
	Test *p1 = new Test(70, 70); // 9. Test(int,int) 
	Test *p2 = new Test[2]; // 10. Test(int,int) Test(int,int)
	//Test *p3 = &Test(80, 80); // 11. Test(int,int)  ~Test()
	const Test &p4 = Test(90, 90); // 12. Test(int,int)
	delete p1; // 13.~Test()
	delete[]p2; // 14. ~Test() ~Test()
}
Test t5(100, 100); // 2.Test(int, int)