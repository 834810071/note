////
//// Created by jxq on 19-9-16.
////
//
//#include <stdio.h>
//#include <iostream>
//#include <string>
//#include <string.h>
//
//using namespace std;
//
//class ZooAnimal
//{
//public:
//    ZooAnimal(){
//
//    }
//    virtual ~ZooAnimal(){
//
//    }
//    virtual void rotate(){
//
//    }
//private:
//    int loc;
//    string name;
//};
//
//class Bear : public ZooAnimal
//{
//public:
//    Bear(){
//
//    }
//    ~Bear(){
//
//    }
//    void rotate(){
//
//    }
//    virtual void dance(){
//
//    }
//private:
//    enum Dances {a, b};
//    Dances dances_known;
//    int cell_block;
//};
//
//class Panda : public Bear
//{
//public:
//    Panda()
//    {
//
//    }
//    ~Panda()
//    {
//
//    }
//private:
//    //int d;
//};
//
//int main(int argc,char *argv[])
//{
//    ZooAnimal za;       // 32 + 4 = 36        -12
//    ZooAnimal *pza;
//
//    Bear b; // 36 + 4 + 4 = 44     -12
//    Panda *pp = new Panda(); // 44 + 4 = 48   -16
//
//    ZooAnimal *za1 = new ZooAnimal();
//    //cout << sizeof(za1) << endl;
//    //cout << sizeof(*za1) << endl;
//    pza = &b;
//
//   cout << sizeof(int) << endl;
////    cout << sizeof(string) << endl;
//
//    cout << sizeof(za) << endl;
//    cout << sizeof(ZooAnimal) << endl;
//    cout << sizeof(*pza) << endl;
//    cout << sizeof(b) << endl;
//    cout << sizeof(pp) << endl;
//    cout << sizeof(*pp) << endl;
//    cout << sizeof(Panda) << endl;  // 64   56  -8
//    return 0;
//}

#include <iostream>

using namespace std;

class Test
{
public:
    Test(int a) { data = a; }
    virtual ~Test() { cout << "Test deconstruct" << endl; }//基类中的虚析构函数
    virtual void fun11() { cout << "Test virtual fun11" << endl; }//基类中的虚函数fun11
    virtual void fun12() { cout << "Test virtual fun12" << endl; }//基类中的虚函数fun12
    int data;
};


class Test1 :public Test
{
public:
    Test1(int d1, int d2) :Test(d2) { data1 = d1; data2 = d2; }
    int data1;
    int data2;
    virtual ~Test1() { cout << "test1 deconstruct" << endl; }//派生类中的虚析构函数
    virtual void fun1() { cout << "test1 virtual fun1" << endl; }//派生类中的虚函数fun1，不是实现基类中的fun11的多态
    virtual void fun2() { cout << "test1 virtual fun2" << endl; }//派生类中的虚函数fun2，不是实现基类中的fun12的多态
};

typedef void(*Fun)(void);//指向函数的指针


int main()
{
    Test objt(10);  //父类对象
    Test1 obj(1, 2);//定义子类对象obj
    cout << "obj's Size = " << sizeof(obj) << endl;
    cout << "obj 's Address = " << &obj << endl;
    cout << "second Test1 object's address = " << &obj + 1 << endl;//为了测试(int*)(&obj+1)和((int*)&obj+1）的区别
    cout << "third Test1 object's address = " << &obj + 2 << endl;
    cout << "base data address and data:        " << (int*)&obj + 1 << "\t" << *((int*)&obj + 1) << endl;
    cout << "derivate data1 address and data1:" << (int*)&obj + 2 << "\t" << *((int*)&obj + 2) << endl;
    cout << "derivate data2 address and data2:" << (int*)&obj + 3 << "\t" << *((int*)&obj + 3) << endl;

    //获得虚表指针，显示虚表中的内容
    cout << "vtable address = " << (int*)&obj << "\t" << "value = " << *((int*)&obj + 0) << endl;
    cout << "vtable value0 的地址 = " << ((int*)*(int*)((int*)&obj + 0) + 0) << endl;
    cout << "vtable value1 的地址 = " << ((int*)*(int*)((int*)&obj + 0) + 1) << endl;
    cout << "vtable value2 的地址 = " << ((int*)*(int*)((int*)&obj + 0) + 2) << endl;
    cout << "vtable value3 的地址 = " << ((int*)*(int*)((int*)&obj + 0) + 3) << endl;
    cout << "vtable value4 的地址 = " << ((int*)*(int*)((int*)&obj + 0) + 4) << endl;
    cout << "vtable value5 的地址 = " << ((int*)*(int*)((int*)&obj + 0) + 5) << endl;


    Fun pFun = NULL;

    pFun = (Fun)*((int*)*(int*)((int*)&obj + 0) + 8);
    pFun();


    pFun = (Fun)*((int*)*(int*)((int*)&obj + 0) + 10);
    pFun();
    //cout << sizeof((int*)*(int*)((int*)&obj + 0)) << endl;


    pFun = (Fun)*((int*)*(int*)((int*)&obj + 0) + 4);
    pFun();


    pFun = (Fun)*((int*)*(int*)((int*)&obj + 0) + 6);
    pFun();
    return 0;
}

