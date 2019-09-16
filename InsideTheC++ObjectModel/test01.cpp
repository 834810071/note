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

    int getI(){ return baseI; }

    static void countI(){};

    virtual ~Base(){cout << "Base::~Base()";}

    virtual void print(void){ cout << "Base::print()"; }



private:

    int baseI;

    static int baseS;
};

struct TypeDescriptor
{
   DWORD ptrToVTable;
   DWORD spare;
   char name[ ];
};

struct RTTICompleteObjectLocator
{

   DWORD signature; //always zero ?

   DWORD offset;    //offset of this vtable in the complete class

   DWORD cdOffset;  //constructor displacement offset

   struct TypeDescriptor* pTypeDescriptor; //TypeDescriptor of the complete class

   int * ptr;
   //struct RTTIClassHierarchyDescriptor* pClassDescriptor; //describes inheritance hierarchy

};

void testBase(Base &p)
{
   // cout << "对象的内存起始地址：" << &p << endl;
    //cout << "type_info信息:" << endl;
   // RTTICompleteObjectLocator str = *((RTTICompleteObjectLocator*)*((int*)*(int*)(&p) - 8));


    //string classname(str.pTypeDescriptor->name);
    //classname = classname.substr(4, classname.find("@@") - 4);
   // cout <<  "根据type_info信息输出类名:"<< classname << endl;

    cout << "虚函数表地址:" << (int *)(&p) << endl;

    //验证虚表
    cout << "虚函数表第一个函数的地址：" << (int *)*((int*)(&p)) << endl;
    typedef void(*Fun)(void);

    cout << "虚函数表中，第二个虚函数即print（）的地址：" << ((int*)*(int*)(&p) + 4) << endl;

    //通过地址调用虚函数print（）
    //typedef void(*Fun)(void);
    //cout << sizeof((int *)*((int*)(&p))) << endl;
    Fun IsPrint=(Fun)* ((int*)*(int*)(&p) +4);
    cout << endl;
    cout<<"调用了虚函数";
    IsPrint(); //若地址正确，则调用了Base类的虚函数print（）
    cout << endl;



    //输入static函数的地址
    p.countI();//先调用函数以产生一个实例
    cout << "static函数countI()的地址：" << p.countI << endl;

    //验证nonstatic数据成员
    cout << "推测nonstatic数据成员baseI的地址：" << (int *)(&p) + 8 << endl;
    cout << "根据推测出的地址，输出该地址的值：" << *((int *)(&p) + 8) << endl;
    cout << "Base::getI():" << p.getI() << endl;

//    Fun IsPrint1=(Fun)* ((int*)*(int*)(&p));
//    cout << endl;
//    cout<<"调用了西沟函数";
//    IsPrint1();
//    cout << endl;
//    cout << "析构函数的地址:" << (int* )*(int *)*((int*)(&p)) << endl;

}


int main(int argc,char *argv[])
{
    Base b(1000);
    testBase(b);
    return 0;
}