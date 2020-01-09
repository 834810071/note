#include <iostream>
#include <thread>
#include <zconf.h>
#include <chrono>

using namespace std;

thread_local int g_mydata = 1;

void thread_func1()
{
    while (true)
    {
        ++g_mydata;
    }
}

void thread_func2()
{
    while (true)
    {
        cout << "g_mydata = " << g_mydata << ", ThreadID = " << this_thread::get_id() << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main()
{
    thread t1(thread_func1);
    thread t2(thread_func2);

    t1.join();
    t2.join();
    return 0;
}

