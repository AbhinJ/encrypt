#include "ProcessManagement.hpp"
#include <iostream>
#include <cstring>
#include "../encryptDecrypt/Cryption.hpp"

ProcessManagement::ProcessManagement(size_t numThreads) {
    for(size_t i = 0; i < numThreads; i++){
        workers.emplace_back([this]() { workerLoop(); });
    }
}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task) {
    {
        std::lock_guard<std::mutex> queueLock(queueMutex);
        taskQueue.push(std::move(task));
    }
    cv.notify_all();
    return true;
}

void ProcessManagement::workerLoop() {
    while (true) {
        std::unique_ptr<Task> task;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock, [this]() { return stop || !taskQueue.empty(); });

            if (taskQueue.empty())
                break;

            task = std::move(taskQueue.front());
            taskQueue.pop();
        }

        if (task) {
            std::cout << "Thread " << std::this_thread::get_id()
                      << " executing task: " << task->toString() << std::endl;

            executeCryption(task->toString());
        }
    }
}

ProcessManagement::~ProcessManagement() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stop = true;
    }
    cv.notify_all();

    for (std::thread& worker : workers) {
        if (worker.joinable())
            worker.join();
    }
}