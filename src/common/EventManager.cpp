#include <unistd.h>
#include <iostream>

#include "../../include/Utils.h"
#include "../../include/EventManager.h"

// ---------------------------- EventManager -------------------------------- //
EventManager::EventManager(int _poolSize) {
    // Create at least 3 working threads. One of them is epolling thread.
    std::cout << "_poolSize = " << _poolSize << std::endl;
    std::cout << "Create " <<  Utils::max(_poolSize, 3) << " working threads. One of them is epolling thread" << std::endl;
    threadPool.setPoolSize(Utils::max(_poolSize, 3));
}

EventManager::~EventManager() {
    std::cout << "deleting event manager" << std::endl;
}

void EventManager::setThreadPoolSize(size_t size) {
    std::unique_lock<std::mutex> lock(_mutex);
    threadPool.setPoolSize(size);
}

size_t EventManager::size() {
    std::unique_lock<std::mutex> lock(_mutex);
    return threadPool.getSize();
}

void EventManager::start() {
    // Set awake callback for epoll.
    epoll.setAwakeCallBack(
            new Epoll::EpollAwakeCallBack(
                    std::bind(&EventManager::epollAwakeHandler, this, std::placeholders::_1)
            )
    );

    // First add epolling thread to work.
    std::cout << "Add epolling thread to work" << std::endl;
    threadPool.addTask(new CallBack(&Epoll::startPolling, &epoll));

    // start the internal thread pool.
    std::cout << "start the internal thread pool" << std::endl;
    threadPool.start();
}

void EventManager::epollAwakeHandler(const Event *event) {
    std::unique_lock<std::mutex> lock(_mutex);
    for (int i = 0; i < event->getNumEvents(); i++) {
        int fd = event->getEvent()[i].data.fd;
        if (inactiveTasksMap.find(fd) != inactiveTasksMap.end()) {
            //std::cout << "find task on fd " << fd << std::endl;
            threadPool.addTask(inactiveTasksMap[fd]);
        }
    }
}

void EventManager::addTask(CallBack *task) {
    threadPool.addTask(task);
}

int EventManager::addTaskWaitingReadable(int fd, CallBack *task) {
    std::unique_lock<std::mutex> lock(_mutex);
//    int ret = epoll.addMonitorReadableEvent(fd);
    int ret = epoll.lockAndExecuteEvent(fd, EPOLLIN | EPOLLONESHOT, EPOLL_CTL_ADD);
    if (ret) {
        return ret;
    }
    if (inactiveTasksMap.find(fd) != inactiveTasksMap.end()) {
        delete inactiveTasksMap[fd];
    }
    inactiveTasksMap[fd] = task;
    return 0;
}

int EventManager::addTaskWaitingWritable(int fd, CallBack *task) {
    std::unique_lock<std::mutex> lock(_mutex);
//    int ret = epoll.addMonitorWritableEvent(fd);
    int ret = epoll.lockAndExecuteEvent(fd, EPOLLOUT | EPOLLONESHOT, EPOLL_CTL_ADD);
    if (ret) {
        return ret;
    }
    if (inactiveTasksMap.find(fd) != inactiveTasksMap.end()) {
        delete inactiveTasksMap[fd];
    }
    inactiveTasksMap[fd] = task;
    return 0;
}

int EventManager::removeAwaitingTask(int fd) {
    std::unique_lock<std::mutex> lock(_mutex);
//    int ret = epoll.deleteMonitoringEvent(fd);
    int ret = epoll.lockAndExecuteEvent(fd, EPOLLIN | EPOLLONESHOT, EPOLL_CTL_DEL);
//    executeEvent(fd, EPOLLIN | EPOLLONESHOT, EPOLL_CTL_DEL);
    if (ret) {
        return ret;
    }
    if (inactiveTasksMap.find(fd) != inactiveTasksMap.end()) {
        delete inactiveTasksMap[fd];
        inactiveTasksMap.erase(inactiveTasksMap.find(fd));
    }
    return 0;
}

int EventManager::modifyTaskWaitingStatus(int fd, int event, CallBack *task) {
    std::unique_lock<std::mutex> lock(_mutex);
//    int ret = epoll.modifyMonitorEvent(fd, status);
    int ret = epoll.lockAndExecuteEvent(fd, event, EPOLL_CTL_MOD);
    if (ret) {
        return ret;
    }
    if (inactiveTasksMap.find(fd) != inactiveTasksMap.end()) {
        delete inactiveTasksMap[fd];
    }
    inactiveTasksMap[fd] = task;
    return 0;
}

void EventManager::awaitTermination() {
    threadPool.awaitTermination();
}

void EventManager::stop() {
    threadPool.stop();
}