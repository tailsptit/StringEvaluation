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
//    memset(&client_addr, 0, sizeof(struct sockaddr_in));
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
    std::cout << "handleReadRequest() ..." << std::endl;
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

    // Try to parse header line "getSize = value\n" to get data length;
    int nread = 0;
    "";
    std::cout << "message->getState() " << message->getState()  << std::endl;

//    if (message->getState() == Message::INIT) {
//        char c;
//        while ((nread = br.read(&c)) > 0) {
//            if (c == '\n') {
//                double res = processExpression(s);
//                std::cout << "OUTPUT = " << res << std::endl;
//                s = "";
//                message->setState(Message::FINISH_READING);
////                continue;
//            } else if (c != ' ') {
//                s += c;
//            }
//        }
////        if (nread == 0 || (errno != EAGAIN && errno != 0)) {
////            removeSession(fd);
////            return;
////        }
//    }


    std::cout << "Execute expression" << std::endl;
//    message->setState(Message::FINISH_READING);
    // Begin reading message data.
    if (message->getState() == Message::READING || message->getState() == Message::INIT) {
        char buf[BUFSIZE];
        char c;
        std::string s = "";
        while ((nread = br.read(&c)) > 0) {
            std::cout << "read " << c << std::endl;
            if (c == '\n') {
                std::cout << "s =====" << s << std::endl;

//            if (message->isFull() || (c == '\n')) {
                std::cout << "message->isFull() = " << message->isFull() << std::endl;
                std::cout << "c == '\\n' = " << (c == '\n') << std::endl;

                if (s.empty()) {
                    std::cout << "EMPTY" << std::endl;
                    message->setState(Message::READING);
                    break;
                    std::cout << "AF""  EMPTY" << std::endl;
                }
                double result = processExpression(s);
                std::cout << "result TMP = " << result << std::endl;
                std::string abc = std::to_string(result);
                int len = abc.size();
                char* writeData = new char[len+1];
                std::copy(abc.begin(), abc.end(), writeData);
                writeData[len] = '\0';

                message->resetBuffer(len+1);
                message->writeToBuffer(writeData, len+1);
                message->setState(Message::FINISH_READING);
                s = "";
                break;
            } else if (c != ' '){
                s+=c;
            }
        }
        // Unexpected EOF or errno other than EAGAIN.
        if ((nread == 0 && !message->isFull()) || (errno != EAGAIN && errno != 0)) {
//            removeSession(fd);
//            return;
        }
    }
    std::cout << "Execute expression" << std::endl;

    // Check message status and submit new handlers to event manager.
    if (message->getState() == Message::INIT || message->getState() == Message::READING) {
        // std::cout << "re-adding " << fd << std::endl;
        eventManger.modifyTaskWaitingStatus(fd, EPOLLIN | EPOLLONESHOT,
                                            new CallBack(&TcpServer::handleReadRequest, this, fd));
    } else if (message->getState() == Message::FINISH_READING) {
        //std::cout << "Change to writing awating for " << fd << std::endl;
        //eventManger.RemoveTaskWaitingReadable(fd);
        eventManger.modifyTaskWaitingStatus(fd, EPOLLOUT | EPOLLONESHOT,
                                            new CallBack(&TcpServer::handleWriteRequest, this, fd));
    }
}

void TcpServer::handleWriteRequest(int fd) {
    std::cout << "handleWriteRequest" << std::endl;
    Message *message = nullptr;
    {
        std::unique_lock<std::mutex> lock(mtx);
        if (mapMessages.find(fd) == mapMessages.end()) {
            mapMessages[fd] = new Message(32);
        }
        message = mapMessages[fd];
    }

//    char *buf = new char[4];
//    char c = (char) 100;
//    memcpy(buf, &c, 4);
//    int nwrite = write(fd, buf, 4);
//    std::cout << "Send back client: FD " << fd << std::endl;
//    std::cout << "Data " << int(*buf) << std::endl;
//    char *buff = message->charBuffer();
//    std::cout << "size of write = " << std::endl;
//
//    int nwrite = write(fd, buff, 8);
//    std::cout << "nwrite = " << nwrite << std::endl;
//    std::cout << "*buff = " << *buff << std::endl;
//
    int nwrite = write(fd, message->charBuffer() + message->getWrittenSize(),
                       message->received_size() - message->getWrittenSize());
    if (nwrite < 0) {
        removeSession(fd);
    }
    message->addWrittenSize(nwrite);

    if (message->getWrittenSize() == message->received_size()) {
        std::cout <<  "finish writing, closeFd this session" << std::endl;
//        removeSession(fd);
        message->setState(Message::READING);
        eventManger.modifyTaskWaitingStatus(fd, EPOLLIN | EPOLLONESHOT,
                                            new CallBack(&TcpServer::handleReadRequest, this, fd));
    } else {
        // message->addWrittenSize(nwrite);
        if (message->getState() == Message::FINISH_READING && nwrite > 0) {
//            message->setState(Message::WRITING);
            message->setState(Message::READING);
        }
//        eventManger.modifyTaskWaitingStatus(fd, EPOLLOUT | EPOLLONESHOT,
//                                            new CallBack(&TcpServer::handleWriteRequest, this, fd));
        eventManger.modifyTaskWaitingStatus(fd, EPOLLIN | EPOLLONESHOT,
                                            new CallBack(&TcpServer::handleReadRequest, this, fd));
    }
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