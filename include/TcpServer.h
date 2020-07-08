//
// Created by tails on 08/07/2020.
//

#ifndef STRINGEVALUATION_TCPSERVER_H
#define STRINGEVALUATION_TCPSERVER_H

#include <map>
#include <vector>
#include <mutex>
#include <iostream>

#include "Socket.h"
#include "Message.h"
#include "EventManager.h"

class TcpServer {
public:
    TcpServer(int port, int poolSize = 5);
    void start();

private:
    void handleTcpConnection();
    void handleReadRequest(int fd);
    void handleWriteRequest(int fd);
    void removeSession(int fd);

private:
    std::shared_ptr<Socket> listenSocket;
    EventManager eventManger;
    std::map<int, Message *> mapMessages;
    std::mutex mtx;
};

#endif  /* CSTRINGEVALUATION_TCPSERVER_H */
