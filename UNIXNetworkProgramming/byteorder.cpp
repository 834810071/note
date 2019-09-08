//
// Created by jxq on 19-9-8.
//

#include <iostream>

using namespace std;

int main()
{
    union
    {
        short s;
        char c[sizeof(s)];
    } un;
    int size = sizeof(un);
    un.s = 0x0102;
    if (sizeof(un) == 2)
    {
        if (un.c[0] == 1 && un.c[1] == 2)
        {
            cout << "big-endian" << endl;
        }
        else if (un.c[0] == 2 && un.c[1] == 1)
        {
            cout << "little-endian" << endl;
        }
        else
        {
            cout << "error 1" << endl;
        }
    }
    else
    {
        cout << "error 2" << endl;
    }

    return 0;
}