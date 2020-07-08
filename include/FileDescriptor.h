//
// Created by tails on 08/07/2020.
//

#ifndef COCCOC_FILEDESCRIPTOR_H
#define COCCOC_FILEDESCRIPTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory>

class FileDescriptor {
public:
    FileDescriptor() = default;
    FileDescriptor(const FileDescriptor &fdInterface) = delete;
    void operator=(const FileDescriptor &fdInterface) = delete;
    FileDescriptor(const int fd, const bool _autoClose);
    virtual ~FileDescriptor();
    virtual void setFd(const int _fd);
    virtual int getFd() const { return fd; }
    virtual int readn(void *buffer, const int nbytes) const = 0;
    virtual int writen(const void *buf, const int nbytes) const = 0;
    virtual int closeFd();
    void setIsClosed() { isClosed = true; }
    bool getIsClosed() const { return isClosed; }

protected:
    unsigned int fd = -1;
    bool isClosed = true;
    bool autoClose = true;
};

#endif  /* COCCOC_FILEDESCRIPTOR_H */
