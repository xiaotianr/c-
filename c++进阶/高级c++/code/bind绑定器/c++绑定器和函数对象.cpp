#include <iostream>
#include <vector>
#include <functional>   //使用function函数对象类型
#include <algorithm>
#include <ctime>
#include <string>
#include <map>
using namespace std;
/*
绑定器和函数对象operator()

1.c++ STL中的绑定器
    bind1st:把函数对象operator()的第一个行参变量绑定成一个确定的值
    bind2nd：把函数对象operator()的第二个行参变量绑定成一个确定的值
2.c++11从Boost库中引入了bind绑定器和function函数对象机制

3.lambda表达式，底层依赖函数对象的机制实现的
*/

/*2.c++11提供的绑定器和函数对象 ：bind、function


function：绑定器，函数对象，lambda本质上都是函数对象，它们只能使用在一条语句中
    --如何在多条语句中应用？保留绑定器，函数对象，lambda的函数类型
        利用function

*/
void doShowAllBooks() { cout << "查看所有书籍信息" << endl; }
void doBorrow() { cout << "借书" << endl; }
void doBack() { cout << "还书" << endl; }
void doQueryBooks() { cout << "查询书籍" << endl; }
void doLoginOut() { cout << "注销" << endl; }
int main()
{
int choice = 0;
	//       c的函数指针在这里也可以做到，但是函数指针不能接受bind绑定器，lambda表达式等函数对象
	map<int, function<void()>> actionMap;
	actionMap.insert({ 1, doShowAllBooks }); // insert(make_pair(xx,xx));
	actionMap.insert({ 2, doBorrow });
	actionMap.insert({ 3, doBack });
	actionMap.insert({ 4, doQueryBooks });
	actionMap.insert({ 5, doLoginOut });

	for (;;)
	{
		cout << "-----------------" << endl;
		cout << "1.查看所有书籍信息" << endl;
		cout << "2.借书" << endl;
		cout << "3.还书" << endl;
		cout << "4.查询书籍" << endl;
		cout << "5.注销" << endl;
		cout << "-----------------" << endl;
		cout << "请选择:";
		cin >> choice;

		auto it = actionMap.find(choice); // map  pair  first second
		if (it == actionMap.end())
		{
			cout << "输入数字无效，重新选择!" << endl;
		}
		else
		{
			it->second();
		}

		// 不好，因为这块代码无法闭合，无法做到“开-闭原则”,会随着需求的更改进行变动
		/*switch (choice)
		{
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		default:
			break;
		}*/
	}

	return 0;
}



#if 0
void hello1()// 使用函数指针：void(*pfunc)(string)
{
    cout << "hello word!" << endl;
}
void hello2(string str)
{
    cout << str << endl;
}
int sum(int a,int b)
{
    cout << "a+b=" << a+b << endl;
    return a+b;
}
class Test
{
public:  //访问成员方法，必须依赖一个对象  void(Test::*pfunc)(string) 而且不是一个参数，还有当前的this指针
    void hello(string str) { cout << str << endl;}
};
int main()
{
    /*
    1.用函数类型实例化function
    2.通过function调用operator()函数的时候，需要根据函数类型传入相应的参数
    */
    //从function的类模板定义处，看到希望用一个函数类型实例化function
    function<void()> func1(hello1);//function<void()> func1=hello1;等价
    func1();//func1.operator();
    function<void(string)> func2=hello2;
    func2("hell0 hello2");//func2.operator(string str) => hello2(str);
    function<int(int,int)> func3=sum;
    func3(2,5);
    //函数对象本质上就是operator()
    function<int(int,int)> func4=[](const int a,const int b)->int {return a+b;};
    cout << func4(40,27) << endl;
    function<void(Test*,string)> func5=&Test::hello;
    Test t;
    func5(&t,"call Test::hello!");
    //func5(&Test(),"call Test::hello!")//可以使用临时对象
    return 0;
}


//1.c++ STL中的绑定器
//遍历打印容器
template<typename Container>
void showContainer(Container &v)
{
    // for(auto it=v.begin();it!=v.end();++it)
    for(typename Container::iterator it=v.begin();it!=v.end();++it)//typename 告知后面作用域下是个类型
    {
        cout << *it << " ";
    }
    cout << endl;
}

//绑定器实现
template<typename Compare,typename T>
class _mybind1st //绑定器是函数对象的一个应用
{
public:
    _mybind1st(Compare comp,T val):_comp(comp),_val(val){}
    bool operator()(const T &second)
    {
        return _comp(_val,second);//底层调用的还是二元函数对象
    }
private:
    Compare _comp;
    T _val;
};
//mybind1st(greater<int>(),70)
template<typename Compare,typename T>
_mybind1st<Compare,T> mybind1st(Compare comp,const T &val)
{
    //直接使用函数模板，好处是，可以进行类型的推演
    return _mybind1st<Compare,T>(comp,val);
}

template<typename Compare,typename T>
class _mybind2nd
{
public: 
    _mybind2nd(Compare comp,T val):_comp(comp),_val(val){}
    bool operator()(const T &first)
    {
        return _comp(first,_val);
    }
private:
    Compare _comp;
    T _val;
};
template<typename Compare,typename T>
_mybind2nd<Compare,T> mybind2nd(Compare comp,const T &val)
{
    return _mybind2nd<Compare,T>(comp,val);
}



//find_if实现
template<typename Iterator,typename Compare>
Iterator my_find_if(Iterator beg,Iterator end,Compare comp)
{
    for(;beg!=end;++beg)
    {
        if(comp(*beg)) return beg; //comp.operator()(*beg)
    }
    return end;
}
int main()
{
    vector<int> vec;
    srand(time(NULL));
    for(int i=0;i<20;++i)
    {
        vec.push_back(rand()%100+1);
    }
    showContainer(vec);
    sort(vec.begin(),vec.end());
    showContainer(vec);
    //geeater<int>是一个二元函数对象
    sort(vec.begin(),vec.end(),greater<int>());
    showContainer(vec);

    /*
    把70按顺序插入到vector容器中  找到第一个小于70的数字
    需要一个一元函数对象 operator()(const T &val)
    greater 比较a>b
    less 比较a<b
    绑定器+二元函数对象 =》一元函数对象
        bind1st: +greater  bool operator()(70,const _Ty& _Right)    将greater的第一个参数绑定为70，
        bind2nd：+less   bool operator()(const _Ty& _Left,70)  
    */
    //auto it1=find_if(vec.begin(),vec.end(),mybind1st(greater<int>(),70));
    auto it1=my_find_if(vec.begin(),vec.end(),mybind2nd(less<int>(),70));
    if(it1 !=vec.end())
    {
        vec.insert(it1,70);
    }
    showContainer(vec);
    return 0;
}
#endif