#define READER_THREAD_COUNT 8
#define LOOP_COUNT 5000000

#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>

using namespace std;

class shared_mutex_counter
{
public:
    shared_mutex_counter() = default;

    ~shared_mutex_counter() = default;

    unsigned int get() const
    {
        shared_lock<shared_mutex> lock(mutex_);
        return value_;
    }

    void inscrement()
    {
        unique_lock<shared_mutex> lock(mutex_);
        ++value_;
    }

    void reset()
    {
        unique_lock<shared_mutex> lock(mutex_);
        value_ = 0;
    }

private:
    mutable std::shared_mutex mutex_;
    unsigned int value_ = 0;
};

class mutex_counter
{
    mutex_counter() = default;

    ~mutex_counter() = default;

    unsigned int get() const
    {
        shared_lock<mutex> lock(mutex_);
        return value_;
    }

    void inscrement()
    {
        unique_lock<mutex> lock(mutex_);
        ++value_;
    }

    void reset()
    {
        unique_lock<mutex> lock(mutex_);
        value_ = 0;
    }

private:
    mutable std::mutex mutex_;
    unsigned int value_ = 0;
};

void test_shared_mutex()
{
    shared_mutex_counter counter;
    int temp;

    auto write = [&counter]() {
        for (int i = 0; i < LOOP_COUNT; ++i)
        {
            counter.inscrement();
        }
    };

    auto reader = [&counter, &temp]() {
        for (int i = 0; i < LOOP_COUNT; ++i)
        {
            temp = counter.get();
        }
    };

    thread** tarray = new thread*[READER_THREAD_COUNT];
    clock_t start = clock();

    for (int i = 0; i < READER_THREAD_COUNT; ++i)
    {
        tarray[i] = new thread(reader);
    }

    thread tw(write);

    for (int i = 0; i < READER_THREAD_COUNT; ++i)
    {
        tarray[i]->join();
    }

    tw.join();

    clock_t end = clock();

}