#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>


class Barrier{
private:
    std::mutex _mutex;
    std::condition_variable _cv;
    std::size_t _count;
public:
    Barrier(std::size_t count);
    void Wait();
};