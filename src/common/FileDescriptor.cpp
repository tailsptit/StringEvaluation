#include "../../include/FileDescriptor.h"

FileDescriptor::FileDescriptor(const int fd, const bool _autoClose) : autoClose(_autoClose) {
    setFd(fd);
}

FileDescriptor::~FileDescriptor() {
    closeFd();
}

void FileDescriptor::setFd(const int _fd) {
    if (_fd > 0) {
        fd = _fd;
        isClosed = false;
    }
}

int FileDescriptor::closeFd() {
    if (autoClose && !isClosed && fd > 0) {
        close(fd);
        isClosed = true;
        return 0;
    }
    return -1;
}

