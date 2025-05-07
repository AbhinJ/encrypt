#ifndef PROCESS_MANAGEMENT_HPP
#define PROCESS_MANAGEMENT_HPP

#include "Task.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

class ProcessManagement
{
private:
    std::queue<std::unique_ptr<Task>> taskQueue;
    std::vector<std::thread> workers;
    std::mutex queueMutex;
    std::condition_variable cv;
    bool stop = false;
    void workerLoop();
public:
    ProcessManagement(size_t numThreads = std::thread::hardware_concurrency());
    bool submitToQueue(std::unique_ptr<Task> task);
    ~ProcessManagement();
};


#endif