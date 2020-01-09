#include <iostream>
#include <thread>
#include <zconf.h>

using namespace std;

int threadproc() {
    while (true)
    {
        cout << "I am New Thread 1!" << endl;
    }
}

int func() {
    thread t(threadproc);
    t.detach();
}

int main()
{
    func();
    while (true)
    {
        sleep(1000);
    }

    return 0;
}