#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include <map>
#include <memory>
#include <queue>
using namespace std;

/*
既然lambda表达式只能使用在语句当中，如果想跨语句使用之前定义好的lambda表达式
怎么办？用什么类型来表示lambda表达式？ 
当然是用function类型来表示函数对象的类型   bind1st bind2nd bind绑定器 lambda表达式 的返回值都是函数对象类型

lambda表达式  =》 函数对象  =》用function表示
*/
class Data
{
public:
	Data(int val1 = 10, int val2 = 10) :ma(val1), mb(val2) {}
	// bool operator>(const Data &data)const { return ma > data.ma; }
	// bool operator<(const Data &data)const { return ma < data.ma; }
	int ma;
	int mb;
};
int main()
{
    //优先级队列
    //priority_queue<Data> q;
    using FUNC=function<bool(Data&,Data&)>;
    priority_queue<Data,vector<Data>,FUNC> 
    q([](Data &a,Data &b)->bool
    {
        return a.ma>b.ma;
    });
    q.push(Data(10,20));
    q.push(Data(15,15));
    q.push(Data(20,10));
    

    //智能指针自定义删除器
    unique_ptr<FILE,function<void(FILE*)>>(fopen("data.txt","w"),[](FILE*pf){fclose(pf);});



#if 0
    map<int,function<int(int,int)>> caculateMap;
    caculateMap[1]=[](int a,int b)->int{return a+b;};
    caculateMap[2]=[](int a,int b)->int{return a-b;};
    caculateMap[3]=[](int a,int b)->int{return a*b;};
    caculateMap[4]=[](int a,int b)->int{return a/b;};

    cout << "10+15=" << caculateMap[1](10,25) << endl;
#endif
    return 0;
}















#if 0
/*
c++11 函数对象的升级版 =》 lambda表达式
函数对象的缺点：一般不会单独使用函数对象，使用在范型算法参数传递过程中  比较性质/自定义操作  优先级队列 智能智能 
    使用函数对象需要定义一个类，而且这个类几乎就是只使用一次

lambda表达式的语法；
[捕获外部变量](形参列表)->返回值{操作代码}；
如果lambda表达式的返回值不需要，那么"->返回值"可以省略
[]:表示不捕获任何外部变量
[=]:表示以传值的方式捕获外部的所有变量
[&]:以传引用的方式捕获外部的所有变量
[this]:捕获外部的this指针
[=,&a]:以传值的方式捕获外部的所有变量,但是a变量以传引用的方式捕获
[a,b]:以值传递的方式捕获外部变量a和b
[a,&b]:a以值传递捕获，b以传引用的方式捕获
*/


//[]()->void{cout << "hello wordl!" << endl;};等价
template<typename T=void>
class TestLambda01
{
public:
    TestLambda01(){} //相当于[],这里没有接收任何外部变量
    void operator()() const
    {
        cout << "hello wordl!" << endl;
    }
};
template<typename T>
class TestLambda02
{
public:
    TestLambda02(){} //相当于[],这里没有接收任何外部变量
    int operator()(T a,T b) const
    {
       return a+b;
    }
};
template<typename T=int>
class TestLambda03
{
public:
    TestLambda03(int a,int b):_a(a),_b(b){} //相当于[],这里没有接收任何外部变量
    void operator()() const//常方法中成员变量不可以修改
    {
       int tmp=_a;
        _a=_b;
        _b=tmp;
    }
private:
    mutable int _a,_b;
};
template<typename T=int>
class TestLambda04
{
public:
    TestLambda04(int &a,int &b):_a(a),_b(b){} //相当于[],这里没有接收任何外部变量
    void operator()() const//常方法中成员变量不可以修改
    {
       int tmp=_a;
        _a=_b;
        _b=tmp;
    }
private:
     int &_a;
     int &_b;
};
int main()
{

    srand(time(NULL));
    vector<int> v;
    for(int i=0;i<20;++i)
    {
        v.push_back(rand()%101);
    }
    sort(v.begin(),v.end(),[](int a,int b)->bool{return a>b;});
    for(int val:v)
    {
        cout << val << " ";
    }
    cout << endl;
    auto it1=find_if(v.begin(),v.end(),[](int val)->bool{return val<65;});
    if(it1!=v.end())
    {
        v.insert(it1,65);
    }
    else{v.push_back(65);}
    for(int val:v)
    {
        cout << val << " ";
    }cout << endl;
    for_each(v.begin(),v.end(),[](int a){if(!(a&1)) cout << a << " ";});
    cout << endl;


#if 0    
    // auto func1=[]()->void{cout << "hello wordl!" << endl;};
    auto func1=[](){cout << "hello wordl!" << endl;};//->void 可以省略
    func1();
    TestLambda01<>t1;
    t1();

    auto func2=[](int a,int b)->int{return a+b;};
    cout << func2(10,20) << endl;   
    TestLambda02<int>t2;
    cout << t2(10,20) << endl;  

    int a=10,b=20;
    auto func3=[a,b]()mutable     //[=]报错：无法再非可变lambda中修改复制捕获“a”,相当于常方法不可以修改成员变量
                                // 同样的，可以在()后加mutable，使得常方法可以修改成员变量
    {
        int tmp=a;
        a=b;
        b=tmp;
    };
    func3();
    cout << "a:" << a << " b:" << b << endl;
    TestLambda03<>t3(a,b);
    t3();
    cout << "a:" << a << " b:" << b << endl;

    auto func4=[&a,&b]()
    {
        int temp=a;
        a=b;
        b=temp;
    };
    func4();
    cout << "a:" << a << " b:" << b << endl;
    TestLambda04<>t4(a,b);
    t4();
    cout << "a:" << a << " b:" << b << endl;

#endif
    return 0;
}

#endif