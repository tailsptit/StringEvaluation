//
// Created by tails on 07/07/2020.
//

#ifndef COCCOC_EPOLL_H
#define COCCOC_EPOLL_H

#include <sys/epoll.h>
#include <thread>
#include <mutex>
#include <functional>

using namespace std;

class Epoll {
public:
    Epoll() {
        epollFd = epoll_create(fd_size_); //epollFd = epoll_create1(EPOLL_CLOEXEC);
    }

    ~Epoll() {
        close(epollFd);
    }

    int lockAndExecuteEvent(int fd, int event, int mode) {
        // TODO: Are epoll_wait and epoll_ctl thread-safe?
        std::unique_lock<std::mutex> lock(mtx);
        return executeEvent(fd, event, mode);
    }

    int executeEvent(int fd, int event, int mode) {
        struct epoll_event ev;
        ev.events = event;
        ev.data.fd = fd;
        int ret = epoll_ctl(epollFd, mode, fd, &ev);
        return ret;
    }

    void handleEvents(int num, const struct epoll_event *epollEvent) {
        Event activeEvent(num, epollEvent);
        std::unique_lock<std::mutex> lock(awake_cb_mtx);
        if (upAwakeCB) {
            (*upAwakeCB)(&activeEvent);
        }
    }

    void startPolling() {
        const int EPOLLEVENTS = 100;
        struct epoll_event events[EPOLLEVENTS];
        while (true) {
            auto ret = epoll_wait(epollFd, events, EPOLLEVENTS, -1);
            std::cout << "awakening " << ret << std::endl;
            handleEvents(ret, events);
        }
    }

    void Stop();

    // Set awake callback. This callback runs everytime epoll_wait returns.
    // It should be set by the user and takes one argument of type
    using EpollAwakeCallBack = std::function<void(const Event *)>;

    void setAwakeCallBack(EpollAwakeCallBack *cb) {
        std::unique_lock<std::mutex> lock(awake_cb_mtx);
        upAwakeCB.reset(cb);
    }

private:
    int epollFd;
    std::unique_ptr<EpollAwakeCallBack> upAwakeCB;
    static const int fd_size_ = 100;
    std::mutex awake_cb_mtx;
    std::mutex mtx;
};

#endif //COCCOC_EPOLL_H
