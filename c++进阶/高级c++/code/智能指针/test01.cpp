#include <iostream>
#include <memory>
#include <thread>
#include <functional>
using namespace std;

/*
CSmartPtr：强智能指针 可以改变资源的引用计数（自己写的）
C++11:
shared_ptr:强智能指针 可以改变资源的引用计数
weak_ptr:弱智能指针 不会改变资源的引用计数
    之后观察资源，不会利用访问资源
    根本没有重载operator* operator->

weak_ptr =>观察   shared_ptr =》观察  资源（内存）

强智能指针循环引用（交叉引用）是什么问题？什么结果？怎么解决？
    造成new出来的资源无法释放！！！资源泄露问题
    怎么解决？
        定义对象的时候，用强智能指针；引用对象的地方，使用若智能指针
*/
/*
C++ 非常著名的开源网络库muduo库
多线程访问共享对象的线程安全问题

*/

/*
智能指针的删除器 deletor  因为智能指针可以管理很多资源，如文件、数组等，不时都用delete释放资源
智能指针：保证资源绝对的释放  delete ptr；
*/
//unique_ptr  shared_ptr
/*
~unique_ptr(){是一个函数对象的调用  deletor(ptr)}

template<typename T>
class default_delete
{
public:
    void operator()(T *ptr)
    {
        delete ptr;
    }
};

*/
// template<typename T>
// class myDeletor
// {
// public:
//     void operator()(T *ptr) const
//     {
//         cout << "call MyDeletor.operator()" << endl;
//         delete []ptr;
//     }
// };
// template<typename T>
// class myFileDeletor
// {
// public:
//     void operator()(T *ptr) const
//     {
//         cout << "call myFileDeletor.operator()" << endl;
//         fclose(ptr);
//     }
// };
int main()
{
    // unique_ptr<int,myDeletor<int> > ptr1(new int[100]); //delete []ptr;
    // unique_ptr<FILE,myFileDeletor<FILE> > ptr2(fopen("data.txt","w"));

    //lambda表达式 =》 函数对象  function    //lambda表达式本身就是个函数对象
    unique_ptr<int,function<void (int *)> > ptr1(new int[100],
        [](int *p)->void{
            cout << "call lambda release new int[100]" << endl;
            delete []p;
        }
    ); 
    unique_ptr<FILE,function<void (FILE *)> > ptr2(fopen("data.txt","w"),
        [](FILE *p){
            cout << "call lambda release data.txt" << endl;
            fclose(p);
        }
    );
    return 0;
}






#if 0
class A
{
public:
    A(){cout << "A()" << endl;}
    ~A(){cout << "~A()" << endl;}
    void testA() {cout << "非常好用的方法！" << endl;}
};
//子线程
void handle01(weak_ptr<A> pw)
{
    //q在访问A对象的时候，需要侦测A对象是否存活
    shared_ptr<A>sp=pw.lock();
    if(sp!=nullptr)
    {
        sp->testA();
    }
    else
    {
        cout << "A对象已经析构，不能再访问！" << endl;
    }
}
//main线程
int main()
{
    {
        shared_ptr<A> p(new A());
        thread t1(handle01,weak_ptr<A>(p));//启动线程t1
        t1.detach();
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    //阻塞等带子线程结束
    return 0;
}


class B;
class A
{
public:
    A(){cout << "A()" << endl;}
    ~A(){cout << "~A()" << endl;}
    void testA() {cout << "非常好用的方法！" << endl;}
    weak_ptr<B> _ptrb;
};
class B
{
    public:
    B(){cout << "B()" << endl;}
    ~B(){cout << "~B()" << endl;}
    void func()
    {
        //_ptra->testA();//弱智能指针不能去访问资源
        shared_ptr<A> ps=_ptra.lock();//提升方法
        if(ps)//提升成功，说明资源还在   为空说明提升失败，说明资源以及被释放
        {
            ps->testA();
        }
    }
    weak_ptr<A> _ptra;
};
int main()
{
    shared_ptr<A> pa(new A());
    shared_ptr<B> pb(new B());
    pa->_ptrb=pb;
    pb->_ptra=pa;
    pb->func();
    cout << pa.use_count() << endl;
    cout << pb.use_count() << endl;
    return 0;
}

//对资源进行引用计数的类
template<typename T>
class RefCnt
{
public:
    RefCnt(T *ptr=nullptr):mptr(ptr)
    {
        if(mptr) mcount=1;
    }
    void addRef() {++mcount;}//增加资源的引用计数
    int delRef() {return --mcount;}
private:
    T *mptr;
    int mcount;//shared_ptr定义的是auomic_int类型,通过CAS保证++--的线程安全
};
//智能指针 保证能做到资源的自动释放
//利用栈上对象出作用域自动析构的特征来做到资源的自动释放
//CSmartPt<int> *p=new CSmartPt<int>(new int);//智能指针放在堆区，就是很普通的指针
template<typename T>
class CSmartPtr   //相比shared_ptr，waek_ptr有些不足：在多线程下计数的++--不安全
{
public:
    CSmartPtr(T *ptr=nullptr)
        :mptr(ptr)
        {
            mpRefCnt=new RefCnt<T>(mptr);
        }

    CSmartPtr(const CSmartPtr<T> &src)
        :mptr(src.mptr),mpRefCnt(src.mpRefCnt)
    {
        if(mptr) mpRefCnt->addRef();
    }
    CSmartPtr<T>& operator=(const CSmartPtr<T>&src)
    {
        if(this==&src) return *this;
        
        if(0==mpRefCnt->delRef())
        {
            delete mptr;
        }
        mptr=src.mptr;
        mpRefCnt=src.mpRefCnt;
        mpRefCnt->addRef();
        return *this;
    }
    ~CSmartPtr()
    {
        if(0==mpRefCnt->delRef())
        {
            delete mptr;
        }
    }

    T& operator*() {return *mptr;}//因为返回的必须是一个可改变的值，因此返回引用
    T* operator->() {return mptr;};

private:
    T* mptr;//指向资源的指针
    RefCnt<T> *mpRefCnt;//指向该资源引用计数的指针
};
int main()
{
    /*
    带引用计数的智能指针shared_ptr和weak_ptr
    带引用计数的好处：多个智能指针可以管理同一个资源
    带引用计数：给每个对象资源，匹配一个引用计数
    智能指针 =》 引用资源的时候 =》引用计数+1
    智能指针 =》 不使用资源的时候 =》 引用计数-1  =》 ！=0   ==0时资源释放
    */
   CSmartPtr<int> ptr1(new int);
   CSmartPtr<int> ptr2(ptr1);
   CSmartPtr<int> ptr3;
   ptr3=ptr2;
   *ptr1=20;
   cout << *ptr2 << " " << *ptr3 << endl;
   return 0;
}

int main()
{
    /*
    不带引用计数的智能指针
    C++库里面提供的
    auto_ptr;   不推荐使用
    C++11新标准：
    socped_ptr：直接删除了拷贝构造和赋值构造,因此使用较少
        socped_ptr(const scoped_ptr<T>&)=delete
        scoped_ptr<T>& operator=(const scoped_ptr<T>&)=delete;
    推荐使用：
    unique_ptr:虽然删除了拷贝构造和赋值改造，但是提供了右值引用的拷贝构造和赋值构造
        unique_ptr(const unique_ptr<T>&)=delete
        unique_ptr<T>& operator=(const unique_ptr<T>&)=delete;

        unique_ptr(unique_ptr<T> &&src)
        unique_ptr<T>& operator=(unique_ptr<T> &&src)

        template<typename T>
        unique_ptr<T> getSmartPtr()
        {
            unique_ptr<T> ptr(new T());
            return ptr;
        }

        unique_ptr<int> ptr1 = getSmartPtr<int>();//临时对象调用右值引用
        ptr1=getSmartPtr<int>();
    怎么解决浅拷贝问题！！！
        std::move => c++11 右值引用 得倒当前变量的右值类型
    */
   unique_ptr<int> p1(new int);
   unique_ptr<int> p2(std::move(p1));//p1移动后就不再持有资源了
//    CSmartPt<int> p1(new int);
//    CSmartPt<int> p2(p1);//默认做的是浅拷贝
//     //虽然重写拷贝构造函数可以改变深浅拷贝的问题
//     //但是对于普通指针p2(p1)代表： p1 p2 => new int ；p1 p2共同管理一块内存
//     *p1=20;
//     *p2=30;//用户认为改变的依然是p1指向的那个值
     
    // auto_ptr<int> ptr1(new int);
    // auto_ptr<int> ptr2(ptr1);  //auto_ptr的拷贝构造就是永远让最后一个智能指针管理资源，前面的智能指针全部置空
    // //不推荐使用auto_ptr，  vector<auto_ptr<int>> vec1;vec2(vec1);不要使用在容器中
    // *ptr2=20;
    // cout << *ptr1 << endl;

   return 0;
}

int main()
{
    //int *p=new int(10); *p p-> 也需要实现
    CSmartPt<int> ptr1(new int);
    *ptr1=20;
    class Test
    {
        public:
            void test() {cout <<"call Test::test" << endl;}
    };
    CSmartPt<Test> ptr2(new Test());
    //(ptr2.operator->())->test();
    ptr2->test();//(*ptr2).test();

    return 0;
}
int main1()
{
    //data段  heap堆  stack栈
    int *p=new int(10);
    *p=30;
    delete p;


    return 0;
}
#endif