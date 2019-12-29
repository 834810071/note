//
// Created by jxq on 19-12-27.
//

#include <iostream>

[[deprecated("use funcY instead")]] void funcx()
{
    // 实现省略...
}

int main()
{
    funcx();
    return 0;
}