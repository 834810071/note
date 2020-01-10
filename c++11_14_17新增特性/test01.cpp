#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>

using namespace std;

class Task
{
public:

    Task(int taskID)
    {
        this->taskID = taskID;
    }

    void doTask()
    {
        cout << "handle a task, taskID: " << taskID << ", threadID: " << this_thread::get_id() << endl;
    }

private:
    int taskID;
};

mutex mymutex;
list<Task*> tasks;
condition_variable mycv;

void* consumer_thread()
{
    Task* pTask = NULL;
    while (true)
    {
        unique_lock<mutex> guard(mymutex);
        while (tasks.empty())
        {
            mycv.wait(guard);
        }

        pTask = tasks.front();
        tasks.pop_front();

        if (pTask == NULL)
        {
            continue;
        }

        pTask->doTask();
        delete pTask;
        pTask = NULL;
    }

    return NULL;
}

void* producer_thread()
{
    int taskID = 0;
    Task* pTask = NULL;

    while (true)
    {
        pTask = new Task(taskID);

        {
            lock_guard<mutex> guard(mymutex);
            tasks.push_back(pTask);
            cout << "produce a task, taskID = " << taskID << ", threadID: " << this_thread::get_id() << endl;
        }

        mycv.notify_one();
        ++taskID;

        this_thread::sleep_for(chrono::seconds(1));
    }
    return NULL;
}

int main()
{
    thread consumer1(consumer_thread);
    thread consumer2(consumer_thread);
    thread consumer3(consumer_thread);
    thread consumer4(consumer_thread);
    thread consumer5(consumer_thread);

    thread producer1(producer_thread);

    producer1.join();
    consumer1.join();
    consumer2.join();
    consumer3.join();
    consumer4.join();
    consumer5.join();

    return 0;
}