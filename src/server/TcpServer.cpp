#include <cstdio>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <iostream>

#include "../../include/Strings.h"
#include "../../include/TcpServer.h"
#include "../../include/Evaluation.h"
#include "../../include/BufferedDataReader.h"

#define BUFSIZE 256

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
    std::cout << "Add a CallBack for handling connection" << std::endl;
    eventManger.addTask(new CallBack(&TcpServer::handleTcpConnection, this));
    std::cout << "Start EventManager" << std::endl;
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
        std::cout << "accept_fd = " << accept_fd << std::endl;

//        int x = fcntl(accept_fd, F_GETFL, 0);
//        fcntl(accept_fd, F_SETFL, x | O_NONBLOCK);

        static int num = 0;
        printf("Accept ************** Accept %d *******************\n", ++num);
        if (accept_fd < 0) {
            std::cerr << "ERORR: Accept connection failed" << std::endl;
            continue;
        }
        std::cout << "Accept new connection from " << inet_ntoa(client_addr.sin_addr) << ":"
                  << ntohs(client_addr.sin_port) << std::endl;
        std::cout << "Total connection: " << ++num << std::endl;
        // TOOD: Submit it to event manager.
        eventManger.addTaskWaitingReadable(accept_fd, new CallBack(&TcpServer::handleReadRequest, this, accept_fd));
        std::cout << "addTaskWaitingReadable" << std::endl;

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
        char* writeData;
        int* len = new int;
        std::string expresion;
        while ((nread = br.read(&c)) > 0) {
            if (c == '\n') {
                if (expresion.empty() && !isError) {
                    writeData = Evaluation::stringToCharPointer("Empty String", *len);
                    break;
                }
                if (!isError){
                    double expressionResult = processExpression(expresion);
                    writeData = Evaluation::doubleToCharPointer(expressionResult, *len);
                }
                message->resetBuffer(*len + 1);
                message->writeToBuffer(writeData, *len + 1);
                message->setState(Message::WRITING);
                break;
            } else if (!Evaluation::isValid(c) && !isError){
                isError = true;
                string error("ERROR: Invalid character in expresion: ");
                error += c;
                std::cout << error << std::endl;
                writeData = Evaluation::stringToCharPointer(error, *len);
            } else if (c != ' ') {
                expresion += c;
            }
        }
        delete len;
        delete [] writeData;
        // Unexpected EOF or errno other than EAGAIN.
        if ((errno != EAGAIN && errno != 0)) {
            removeSession(fd);
            return;
        }
    }

    // Check message status and submit new handlers to event manager.
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

    // removeSession(fd);
    std::cout << "Change to Reading mode. Socket Id = " << fd << std::endl;
    message->setState(Message::READING);
    eventManger.modifyTaskWaitingStatus(fd, EPOLLIN | EPOLLONESHOT,
                                        new CallBack(&TcpServer::handleReadRequest, this, fd));
}

void TcpServer::removeSession(int fd) {
    // Remove fd from epoll.
    eventManger.removeAwaitingTask(fd);

    // Potential race condition: don't closeFd fd before de-registering the event from event manager.
    close(fd);

    // Delete fd_message map record.
    {
        std::unique_lock<std::mutex> lock(mtx);
        if (mapMessages.find(fd) == mapMessages.end()) {
            return;
        }
        delete mapMessages[fd];
        mapMessages.erase(mapMessages.find(fd));
    }
}