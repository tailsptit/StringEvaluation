#include <iostream>

#include "../../include/ThreadPool.h"

ThreadPool::ThreadPool() : state(IDLE), poolSize(4) {}

// Constructor does nothing. Threads are created when new task submitted.
ThreadPool::ThreadPool(size_t _poolSize) : state(IDLE), poolSize(_poolSize) {}

// Destructor joins all threads
ThreadPool::~ThreadPool() {
    for (std::thread &worker: workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void ThreadPool::setPoolSize(size_t size) {
    poolSize = size;
}

size_t ThreadPool::getSize() const {
    return poolSize;
}

void ThreadPool::start() {
    std::cout << "pQueueTasks.size() = " << pQueueTasks.size() << std::endl;
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        state = RUNNING;
        unsigned int numWorkingThreads = poolSize < pQueueTasks.size() ? poolSize : pQueueTasks.size();
        for (unsigned int i = workers.size(); i < numWorkingThreads; i++) {
            workers.emplace_back(std::thread(&ThreadPool::startWorker, this));
        }
    }
    condition.notify_all();
}

// Thread worker
void ThreadPool::startWorker() {
    CallBack *task;
    while (true) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return state == STOP || !pQueueTasks.empty(); });
            if (state == STOP && pQueueTasks.empty()) {
                return;
            }
            task = (pQueueTasks.front()).release();
            pQueueTasks.pop();
            std::cout << "pQueueTasks.pop()" << std::endl;
        }
        task->run();
    }
}

// Add new work item to the pool
void ThreadPool::addTask(CallBack *task) {
    {
        std::unique_lock<std::mutex> lock(workerMutex);
        if (state == RUNNING && workers.size() < poolSize) {
            std::cout << "ThreadPool::addTask => work item to the pool" << std::endl;
            workers.emplace_back(std::thread(&ThreadPool::startWorker, this));
        }
    }

    {
        std::unique_lock<std::mutex> lock(queueMutex);
        if (state == STOP) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }
        pQueueTasks.emplace(std::unique_ptr<CallBack>(task));
        std::cout << "ThreadPool::addTask pQueueTasks.size() = " << pQueueTasks.size() << std::endl;
    }
    condition.notify_one();
}

// Blocks and wait for all previously submitted pQueueTasks to be completed.
void ThreadPool::awaitTermination() {
    condition.notify_all();
    for (std::thread &worker: workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

// Shut down the ThreadPool. This method does not wait for previously submitted
// pQueueTasks to be completed.
void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        state = STOP;
    }
    condition.notify_all();
}