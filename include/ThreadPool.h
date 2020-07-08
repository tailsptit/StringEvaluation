//
// Created by tails on 08/07/2020.
//

#ifndef COCCOC_THREADPOOL_H
#define COCCOC_THREADPOOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

#include "CallBack.h"

class ThreadPool {
public:
    enum State {IDLE, RUNNING, STOP};
    ThreadPool();
    ThreadPool(size_t _poolSize);
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;
    ~ThreadPool();

    void setPoolSize(size_t size);
    size_t getSize() const;
    void addTask(CallBack *task);
    void awaitTermination();
    void start();
    void stop();

private:
    void startWorker();

    // need to keep track of threads so we can join them
    std::vector<std::thread> workers;

    // the task queue
    std::queue<std::unique_ptr<CallBack>> pQueueTasks;

    // synchronization
    std::mutex workerMutex;
    std::mutex queueMutex;
    std::condition_variable condition;
    State state;
    unsigned int poolSize;
};

#endif /* COCCOC_THREADPOOL_H */
