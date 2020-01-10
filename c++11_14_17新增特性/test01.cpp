#include <atomic>
#include <iostream>
#include <stdio.h>

using namespace std;

int main()
{
    atomic<int> a;
    a = 99;
    printf("%d\n", (int) a);

    a++;
    printf("%d\n", (int) a);
    return 0;
}