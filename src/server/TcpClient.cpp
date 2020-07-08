#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cmath>
#include <vector>
#include <set>
#include <thread>
#include <algorithm>
#include <cstring>

#include "../../include/TcpClient.h"

TcpClient::TcpClient(const std::string _hostname, const int port) : hostname_(_hostname), port_(port) {
    srand(time(NULL));
}

int TcpClient::runOneSession() {
    Socket *socket = Socket::createClientSocket(hostname_, port_);
    if (!socket) {
        return -1;
    }

    std::unique_ptr<Message> message(generateTestMessage());
    // Fill sending buffer with random data.
    int bufsize = message->getBufferSize();
    for (int i = 0; i < bufsize; i++) {
        char c = (char) (rand() % 256);
        message->writeToBuffer(&c, 1);
    }

    // send header line.
    std::string header = "getSize = " + std::to_string(bufsize) + "\n";
    socket->writen(header.c_str(), header.length());

    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // start sending chunks.
    int last_index = 0;
    for (const int index: message->sendingVector()) {
        int size = index - last_index;
        last_index = index;
        int nwrite =
                socket->writen(message->charBuffer() + message->getWrittenSize(), size);
        if (nwrite != size) {
            std::cerr << "Client write error, expect to write " << size
                      << " bytes, actual " << nwrite << std::endl;
            delete socket;
            return -1;
        }
        message->addWrittenSize(nwrite);
        // Pause for few milliseconds to create sending interval.
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 3));
    }

    // start receiving chunks
    char recv_buf[bufsize];
    int nread = 0, offset = 0;
    while ((nread = socket->readn(recv_buf + offset, bufsize - offset)) > 0) {
        offset += nread;
    }
    if (offset != bufsize) {
        std::cerr << "Client receive error, expect to getNumEvents " << bufsize
                  << " bytes, actual " << offset << std::endl;
        delete socket;
        return -1;
    }
    if (memcmp(recv_buf, message->charBuffer(), bufsize)) {
        std::cerr << "ERROR: Received message mismatch origin" << std::endl;
        delete socket;
        return -1;
    }
    //std::cout << "Success :)" << std::endl;
    delete socket;
    return 0;
}

void TcpClient::runConcurrentSessions(int num) {
    ThreadPool pool(30);
    pool.start();
    for (int i = 0; i < num; i++) {
        pool.addTask(new CallBack(&TcpClient::runOneSession, this));
    }
    pool.stop();
    pool.awaitTermination();
}

Message *TcpClient::generateTestMessage() {
    int size = rand() % 1000 + 1;
    Message *message = new Message(size);

    int num = rand() % (int) sqrt(size) + 1;
    std::vector<int> vec;
    std::set<int> all;
    int i = 0;
    while (i < num - 1) {
        int index = rand() % size;
        if (all.find(index) == all.end()) {
            all.insert(index);
            vec.push_back(index);
            i++;
        }
    }
    vec.push_back(size);
    std::sort(vec.begin(), vec.end());
    message->setSendingVector(vec);

    // std::cout << "total = " << getSize << std::endl;
    // for (const int i: vec) {
    //   std::cout << i << ", ";
    // }
    // std::cout << std::endl;
    return message;
}
