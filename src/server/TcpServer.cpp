#include <cstdio>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

#include "../../include/Strings.h"
#include "../../include/TcpServer.h"
#include "../../include/Evaluation.h"
#include "../../include/BufferedDataReader.h"

TcpServer::TcpServer(int port, int poolSize) : eventManger(poolSize) {
    std::cout << "Create server socket a port " << port << std::endl;
    listenSocket.reset(Socket::createServerSocket(port, true));
}

void TcpServer::start() {
    if (listenSocket->getIsClosed()) {
        fprintf(stderr, "FAIL: Server not successfully initialized yet.\n");
        return;
    }
    // TODO: Submit it to event manager.
    eventManger.addTask(new CallBack(&TcpServer::handleTcpConnection, this));
    eventManger.start();
    eventManger.awaitTermination();
    std::cout << "Terminate server" << std::endl;
}

void TcpServer::handleTcpConnection() {
    struct sockaddr_in client_addr;
    unsigned int client_addr_len = sizeof(client_addr);
    /* Accept actual connection from the client */
    while (true) {
        int accept_fd = accept(listenSocket->getFd(), (struct sockaddr *) &client_addr, &client_addr_len);
        static int num = 0;
        if (accept_fd < 0) {
            std::cerr << "ERORR: Accept connection failed" << std::endl;
            continue;
        }
        std::cout << "Accept new connection from " << inet_ntoa(client_addr.sin_addr) << ":"
                  << ntohs(client_addr.sin_port) << std::endl;
        std::cout << "Total connection: " << ++num << std::endl;
        std::cout << "Change to Reading mode. Socket Id = " << accept_fd << std::endl;
        eventManger.addTaskWaitingReadable(accept_fd, new CallBack(&TcpServer::handleReadRequest, this, accept_fd));
    }
}

double processExpression(const string &exp) {
    return Evaluation::evaluate(exp);
//    string postfix = Evaluation::postfixConversion(exp);
//    return Evaluation::evaluatePostfix(postfix);
}

void TcpServer::handleReadRequest(int fd) {
    std::cout << "Reading data from client" << std::endl;
    Message *message = nullptr;
    {
        std::unique_lock<std::mutex> lock(mtx);
        if (mapMessages.find(fd) == mapMessages.end()) {
            mapMessages[fd] = new Message(64);
        }
        message = mapMessages[fd];
    }

    // Create a buffered data reader on the socket.
    std::unique_ptr<FileDescriptor> upClientSocket(new Socket(fd, false));
    BufferedDataReader br(std::move(upClientSocket));

    // Begin reading message data.
    if (message->getState() == Message::INIT || message->getState() == Message::READING) {
        char c;
        int nread;
        bool isError = false;
        std::string error;

        std::string expresion;
        while ((nread = br.read(&c)) > 0) {
            if (c == '\n') {
                if (expresion.empty() && !isError) {
                    break;
                }
                char* writeData;
                int* len = new int;
                if (!isError){
                    double expressionResult = processExpression(expresion);
                    writeData = Evaluation::doubleToCharPointer(expressionResult, *len);
                } else {
                    writeData = Evaluation::stringToCharPointer(error, *len);
                }
                message->resetBuffer(*len + 1);
                message->writeToBuffer(writeData, *len + 1);
                message->setState(Message::WRITING);
                delete len;
                delete [] writeData;
                break;
            } else if (!Evaluation::isValid(c) && !isError){
                isError = true;
                error = "ERROR: Invalid character in expresion: ";
                error += c;
            } else if (c != ' ') {
                expresion += c;
            }
        }
        if ((nread == 0) || (errno != EAGAIN && errno != 0)) {
            std::cout << "Close connection. Socket Id " << fd << std::endl;
            removeSession(fd);
            return;
        }
    }
    // Submit new handler to event manager
    if (message->getState() == Message::INIT || message->getState() == Message::READING) {
        std::cout << "Re-Adding to Reading mode. Socket Id = " << fd << std::endl;
        eventManger.modifyTaskWaitingStatus(fd, EPOLLIN | EPOLLONESHOT,
                                            new CallBack(&TcpServer::handleReadRequest, this, fd));
    } else if (message->getState() == Message::WRITING) {
        std::cout << "Change to Writing mode. Socket = " << fd << std::endl;
        //eventManger.RemoveTaskWaitingReadable(fd);
        eventManger.modifyTaskWaitingStatus(fd, EPOLLOUT | EPOLLONESHOT,
                                            new CallBack(&TcpServer::handleWriteRequest, this, fd));
    }
}

void TcpServer::handleWriteRequest(int fd) {
    std::cout << "Sending back the result to client" << std::endl;
    Message *message = nullptr;
    {
        std::unique_lock<std::mutex> lock(mtx);
        if (mapMessages.find(fd) == mapMessages.end()) {
            mapMessages[fd] = new Message(32);
        }
        message = mapMessages[fd];
    }

    int nwrite = write(fd, message->charBuffer(), message->getBufferSize());
    if (nwrite < 0) {
        removeSession(fd);
    }
    std::cout << "Change to Reading mode. Socket Id = " << fd << std::endl;
    message->setState(Message::READING);
    eventManger.modifyTaskWaitingStatus(fd, EPOLLIN | EPOLLONESHOT,
                                        new CallBack(&TcpServer::handleReadRequest, this, fd));
}

void TcpServer::removeSession(int fd) {
    // Remove fd from Epoll.
    eventManger.removeAwaitingTask(fd);
    close(fd);
    // Delete socket fd from map
    {
        std::unique_lock<std::mutex> lock(mtx);
        if (mapMessages.find(fd) == mapMessages.end()) {
            return;
        }
        delete mapMessages[fd];
        mapMessages.erase(mapMessages.find(fd));
    }
}