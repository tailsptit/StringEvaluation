#include <cstdio>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <iostream>

#include "../include/Strings.h"
#include "../include/TcpServer.h"
#include "../include/Math.h"
#include "../include/BufferedDataReader.h"

#define BUFFER_SIZE 3

TcpServer::TcpServer(int port, int poolSize) : eventManger(poolSize) {
    std::cout<< "Create server socket a port " << port << std::endl;
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
        std::cout << "Accept new connection from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;
        std::cout << "Total connection: " << ++num << std::endl;
        // TOOD: Submit it to event manager.
        eventManger.addTaskWaitingReadable(accept_fd, new CallBack(&TcpServer::handleReadRequest, this, accept_fd));
        std::cout << "addTaskWaitingReadable" << std::endl;

    }
}

double processExpression(const string& exp){
    return Math::evaluate(exp);
//    string postfix = Math::postfixConversion(exp);
//    return Math::evaluatePostfix(postfix);
}

void TcpServer::handleReadRequest(int fd) {
    std::cout << "handleReadRequest() ..." << std::endl;
    Message *message = nullptr;
    {
        std::unique_lock<std::mutex> lock(mtx);
        if (mapMessages.find(fd) == mapMessages.end()) {
            mapMessages[fd] = new Message(32);
        }
        message = mapMessages[fd];
    }

    // Create a buffered data reader on the socket.
    std::unique_ptr<FileDescriptor> upClientSocket(new Socket(fd, false));
    BufferedDataReader br(std::move(upClientSocket));
    std::cout << "Create a buffered data reader on the socket" << std::endl;

    // Try to parse header line "getSize = value\n" to getNumEvents data length;
    int nread = 0;
    std::cout << "message->getState() = " << message->getState() << std::endl;
    string s = "";
    if (message->getState() == Message::INIT) {
        char c;
        while ((nread = br.read(&c)) > 0) {
            if (c == '\n'){
                std::cout << s << " = " << s << std::endl;
                double res = processExpression(s);
                std::cout << s << " = " << res << std::endl;
                s = "";
                continue;
            } else {
                s += c;
            }

//            std::cout << "nread = " << nread << std::endl;
//            std::cout << "c = " << c << std::endl;
//
//            message->writeToBuffer(&c, 1);
//            if (c == '\n') {
//                std::cout << "c == '\\n'" << std::endl;
//
//                std::string line(message->charBuffer(), message->received_size());
//                std::cout << "line = " << line << std::endl;
//
//                std::vector<std::string> result = Strings::splitGreedy(line, '\n');
//                if (result.size() != 2) {
//                    std::cout << "Message::ERROR = " << Message::ERROR << std::endl;
//                    message->setState(Message::ERROR);
//                    return;
//                }
//                try {
////                    int data_length = std::stoi(Strings::strip(result[1]));
//                    int data_length = std::stoi(line);
//
//                    message->resetBuffer(data_length);
//                    std::cout << "data lengh = " << data_length << std::endl;
//                    message->setState(Message::READING);
//                }
//                catch (std::exception &err) {
////                    fprintf(stderr, "Can't parse \"%s\" as int32 value",
////                            result[1].c_str());
//                    message->setState(Message::ERROR);
//                    return;
//                }
//                break;
//            }
        }
        if (nread == 0 || (errno != EAGAIN && errno != 0)) {
            removeSession(fd);
            return;
        }
    }
    message->setState(Message::READING);
    // Begin reading message data.
    if (message->getState() == Message::READING) {
        char buf[BUFFER_SIZE];
        int read_th = 0;
        while ((nread = br.read(buf, 0, BUFFER_SIZE)) > 0) {
            read_th++;
            std::cout << "read_th = " << read_th << std::endl;

            std::cout << "nread " << nread << std::endl;
            std::cout << "buf =  " << buf << std::endl;
            std::cout << "sizeof(buf)  =  " << sizeof(buf) << std::endl;

            message->writeToBuffer(buf, nread);
            if (message->isFull()) {
                message->setState(Message::FINISH_READING);
                break;
            }
        }
        // Unexpected EOF or errno other than EAGAIN.
        if ((nread == 0 && !message->isFull()) || (errno != EAGAIN && errno != 0)) {
            removeSession(fd);
            return;
        }
    }

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
    std::cout << "=======================Write_Handler() ..." << std::endl;
    Message *message = nullptr;
    {
        std::unique_lock<std::mutex> lock(mtx);
        if (mapMessages.find(fd) == mapMessages.end()) {
            mapMessages[fd] = new Message(32);
        }
        message = mapMessages[fd];
    }

    int nwrite = write(fd, message->charBuffer() + message->getWrittenSize(),
                       message->received_size() - message->getWrittenSize());
    if (nwrite < 0) {
        removeSession(fd);
    }
    message->addWrittenSize(nwrite);

    if (message->getWrittenSize() == message->received_size()) {
        // finish writing, closeFd this session.
        removeSession(fd);
    } else {
        // message->addWrittenSize(nwrite);
        if (message->getState() == Message::FINISH_READING && nwrite > 0) {
            message->setState(Message::WRITING);
        }
        eventManger.modifyTaskWaitingStatus(fd, EPOLLOUT | EPOLLONESHOT,new CallBack(&TcpServer::handleWriteRequest, this, fd));
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