//
// Created by tails on 08/07/2020.
//

#ifndef STRINGEVALUATION_EVENTMANAGER_H
#define STRINGEVALUATION_EVENTMANAGER_H

#include <map>
#include <mutex>
#include <thread>

#include "../include/Event.h"
#include "../include/Epoll.h"
#include "../include/CallBack.h"
#include "../include/ThreadPool.h"

class EventManager {
public:
    EventManager(int _poolSize);
    EventManager(const EventManager &) = delete;
    ~EventManager();
    EventManager &operator=(const EventManager &) = delete;

    void setThreadPoolSize(size_t size);
    size_t size();
    void addTask(CallBack *task);
    int addTaskWaitingReadable(int fd, CallBack *task);
    int addTaskWaitingWritable(int fd, CallBack *task);
    int removeAwaitingTask(int fd);
    int modifyTaskWaitingStatus(int fd, int status, CallBack *task);
    void start();
    void awaitTermination();
    void stop();
    void epollAwakeHandler(const Event *event);

private:
    Epoll epoll;
    ThreadPool threadPool;
    std::mutex _mutex;
    std::map<int, CallBack *> inactiveTasksMap;
};

#endif  /* STRINGEVALUATION_EVENTMANAGER_H */