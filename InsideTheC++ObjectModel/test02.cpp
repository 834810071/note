//
// Created by jxq on 19-9-16.
//
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <typeinfo>
#include <sqltypes.h>

using namespace std;
class Base
{
public:

    Base(int i) :baseI(i){};

    virtual void print(void){ cout << "调用了虚函数Base::print()"; }
    virtual ~Base(){cout<<"调用了虚函数Base::~Base()";}
    virtual void setI(){cout<<"调用了虚函数Base::setI()";}



private:

    int baseI;

};

int main ()
{
    Base b(1000);
    int * vptrAdree = (int *)(&b);
    cout << "虚函数指针（vprt）的地址是：\t"<<vptrAdree << endl;

    typedef void(*Fun)(void);
    Fun vfunc = (Fun)*( (int *)*(int*)(&b));
    cout << "第一个虚函数的地址是：" << (int *)*(int*)(&b) << endl;
    //cout << "通过地址，调用虚函数Base::print()：";
   // vfunc();



   // typedef void(*Fun)(void)
     vfunc = (Fun)*( (int * )(*(int*)(&b)+2));
    //cout << "第er个虚函数的地址是：" << (int * )(*(int*)(&b)+4) << endl;
    //cout << "通过地址，调用虚函数Base::setI()：";
    vfunc();


//    vfunc = (Fun)*( (int * )(*(int*)(&b)+8));
//    cout << "第san个虚函数的地址是：" << (int * )(*(int*)(&b)+8) << endl;
//   // cout << "通过地址，调用虚函数Base::~Base()：";
//    vfunc();
    return 0;
}