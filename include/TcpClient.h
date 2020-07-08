//
// Created by tails on 08/07/2020.
//

#ifndef STRINGEVALUATION_TCPCLIENT_H
#define STRINGEVALUATION_TCPCLIENT_H

#include <map>
#include <mutex>
#include <thread>
#include <iostream>
#include <functional>

#include "Socket.h"
#include "TcpServer.h"

class TcpClient {
public:
    TcpClient(const std::string _hostname, const int port);
    int runOneSession();
    void runConcurrentSessions(int num);

private:
    Message *generateTestMessage();

    const std::string hostname_;
    const int port_;
};

#endif  /* STRINGEVALUATION_TCPCLIENT_H */
