#include <netdb.h>
#include <netinet/in.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/socket.h>

#include "../../include/Socket.h"

Socket *Socket::createClientSocket(const std::string hostname, const int port, bool block) {
    struct sockaddr_in server_addr;
    struct hostent *server;

    // Create socket
    int fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ < 0) {
        fprintf(stderr, "ERROR: opening socket\n");
        return NULL;
    }

    // getNumEvents server ip
    server = gethostbyname(hostname.c_str());
    if (server == NULL) {
        fprintf(stderr, "ERROR: hostname not found\n");
        return NULL;
    }

    // set server
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr = *(struct in_addr *) *server->h_addr_list;
    server_addr.sin_port = htons(port);

    /* Now connect to the server */
    if (connect(fd_, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, "ERROR connecting\n");
        return NULL;
    }

    if (!block) {
        int x = fcntl(fd_, F_GETFL, 0);
        fcntl(fd_, F_SETFL, x | O_NONBLOCK);
    }

    Socket *new_socket = new Socket(hostname);
    new_socket->setFd(fd_);
    return new_socket;
}

Socket *Socket::createServerSocket(const int port, bool block) {
    struct sockaddr_in serv_addr;

    // Create socket
    int _fd = socket(AF_INET, SOCK_STREAM, 0);

    if (_fd < 0) {
        fprintf(stderr, "ERROR: opening socket\n");
        return NULL;
    }

    if (!block) {
        int x = fcntl(_fd, F_GETFL, 0);
        fcntl(_fd, F_SETFL, x | O_NONBLOCK);
    }

    // Set server addr
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    /* Now bind the host address using bind() call.*/
    if (bind(_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        fprintf(stderr, "ERROR: hostname binding server sockfd\n");
        return NULL;
    }

    // start listening
    listen(_fd, 5); // backlog = 5
    Socket *newSocket = new Socket();
    newSocket->setFd(_fd);
    return newSocket;
}

int Socket::readn(void *buffer, const int n) const {
    if (!isClosed && fd > 0) {
        return read(fd, buffer, n);
//        return recv(fd, buffer, n, 0);
    }
    return -1;
}

int Socket::writen(const void *buffer, const int n) const {
    if (!isClosed && fd > 0) {
        return write(fd, buffer, n);
    }
    return -1;
}

int Socket::send(void *buffer, const int n) const {
    return readn(buffer, n);
}

int Socket::receive(const void *buffer, const int n) const {
    return writen(buffer, n);
}