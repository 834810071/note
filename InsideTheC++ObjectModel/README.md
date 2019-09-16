[**深入探索C++对象模型**](https://github.com/834810071/note/blob/master/InsideTheC++ObjectModel/README.md)
=================================
# 第一章 关于对象(Object Lessons)
抽象数据类型(abstract data type, ADT)
>> member functions虽然在class的声明内，却不出现在object之中。每一个non-inline member function只会诞生一个函数实体。而inline function则会在其每个使用者身上产生一个函数实体。

>> c++在布局以及存取时间上的额外负担是由virtual引起，包括：   
>> * **virtual functioin 机制**   用以支持一个有效率的“执行期绑定”(runtime binding)。
>> * **virtual base class**   用以实现“多次出现在继承体系中的base class，有一个单一而被共享的实体”。
## 1.1 [C++对象模型](https://www.cnblogs.com/QG-whz/p/4909359.html)  (The C++ Object Model)
>> 在c++中，有两种class data members: static和nonstatic，以及三种class member functions:static、nonstatic和virtual。    

>> c++对象模型中，Nonstatic data members被配置于每一个class object之内，static data members则被存放在所有的class object之外。Static和nonstatic function members也被放在所有的class object之外。Virtual functions则以两个步骤支持：  
>> 1. 每个class产生出一堆指向virtual functions的指针，放在表格中。这个表格被称为virtual table(Vtbl)。    
>> 2. 每一个class object被添加一个指针，指向相关的virtual table。通常这个指针被称为**vptr**。vptr的设定和重置都有class的constructor、destructor和copy assignment运算符自动完成。每一个class所关联的type_info object(用以支持runtime type identification, RTTI)也有virtual table指出，在表格第一个slot处。    

>> c++支持单继承、多继承和虚拟继承(virtual就是共享的意思)。  

>> 在虚拟继承的情况下，base class不管在继承链中被派生(derived)多少次，永远只会存在一个实体(称为subobject)。  

>> c++中凡处于同一个access section的数据，必定保证以其声明次序出现在内存布局当中。

>> 当一个类本身定义了虚函数，或其父类有虚函数时，为了支持多态机制，编译器将为该类添加一个虚函数指针（vptr）。虚函数指针一般都放在对象内存布局的第一个位置上，这是为了保证在多层继承或多重继承的情况下能以最高效率取到虚函数表。
## 1.3 对象的差异(An Object Distinction)
>> c++程序设计模型直接支持三种程序设计典范：
>> 1. 程序模型(procedural model)。   
>> 2. 抽象数据类型模型(abstract data type model, ADT)。  
>> 3. 面向对象模型(object-oriented model)。  

>> c++以下列方法支持多态：   
>> 1 . 经由一组隐含的转化操作。    
```cpp
shape *ps = new circle();
```
>> 2 . 经由virtual function机制：
```cpp
ps->rotate();
```
>> 3 . 经由dynamic_case和typeid运算符：
```cpp
if (circle *pc = dynamic_cast<circle*>(ps)) ...
```
指针的类型(The Type of a Pointer)
>> 指向不同类型的各指针间的差异在其所寻址出来的object类型的不同。指针类型会教导编译器如何解释某个特定地址中的内存内容及其大小。
   
>> 一个void*的指针只能够含有一个地址，而不能够通过它操作所指的object。
   
>> 转型(cast)只影响被指出内存的大小和其内容的解释方式。  

>>枚举变量的大小，实质是常数所占内存空间的大小，大小为4(int)。

[关于C++中的虚拟继承的一些总结](https://www.cnblogs.com/BeyondAnyTime/archive/2012/06/05/2537451.html)
