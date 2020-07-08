#ifndef STRINGEVALUATION_BUFFEREDDATAREADER_H
#define STRINGEVALUATION_BUFFEREDDATAREADER_H

#include <iostream>
#include <string>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sstream>

#include "../include/FileDescriptor.h"

class BufferedDataReader {
public:
    BufferedDataReader(std::unique_ptr<FileDescriptor> fd) {
        upFd = std::move(fd);
        bufferSize = BUFFER_SIZE;
        buffer = new char[bufferSize];
    }

    BufferedDataReader(std::unique_ptr<FileDescriptor> fd, int _bufferSize) {
        upFd = std::move(fd);
        if (_bufferSize < 0 || _bufferSize > MAX_BUFFER_SIZE) {
            _bufferSize = 1024;
        }
        this->bufferSize = _bufferSize;
        buffer = new char[bufferSize];
    }

    virtual ~BufferedDataReader() {
        close();
    }

    // read a byte
    int read(char *c) {
        int re;
        if (dataLen == 0 && (re = refill()) <= 0) {
            return re;
        }
        *c = buffer[tail++];
        dataLen--;
        return 1;
    }

    // read a chunk of data from buffer
    int read(char *buf, int off, const int len) {
        if (!checkArgs(buf, off, len)) {
            return -1;
        }

        if (len == 0) {
            return 0;
        }

        int readnLeft = len;
        while (readnLeft > 0) {
            if (dataLen <= 0) {
                // Directly copy data to user buffer.
                if (readnLeft >= bufferSize) {
                    int re = upFd->readn(buf + off, readnLeft);
                    std::cout << buf;
                    readnLeft -= re;
                    break;
                } else {
                    // Re-fill the internal buffer.
                    int nread;
                    if ((nread = refill()) <= 0) {
                        if (len == readnLeft) {
                            return nread;
                        }
                        return len - readnLeft;
                    }
                }
            }
            int copyLen = dataLen < readnLeft ? dataLen : readnLeft;
            memcpy(buf + off, buffer + tail, copyLen);
            off += copyLen;
            tail += copyLen;
            dataLen -= copyLen;
            readnLeft -= copyLen;
            // readn length reaches expected length
            if (readnLeft == 0) {
                break;
            }
            // Nothing is available in pipe.
            // if (fd != null && fd.available() <= 0) {
            //   break;
            // }
        }
        return len - readnLeft;
    }

    // readn one line from buffer
    std::string *readLine(std::string *str) {
        // Reach file end.
        int re = 0;

        if (dataLen == 0 && (re = refill()) <= 0) {
            // fprintf(stderr, "LOG: Refill in readline() returns %d\n", re);
            return NULL;
        }

        bool eof = false, line_empty = true;
        std::stringstream sstr;
        while (true) {
            if (dataLen == 0 && refill() <= 0) {
                //fprintf(stderr, "LOG: Refill in readline() ended.\n");
                eof = true;
                break;
            }

            if (buffer[tail] == '\n') {
                tail++;
                dataLen -= 1;
                break;
            } else {
                dataLen--;
                line_empty = false;
                sstr << buffer[tail++];
            }
        }

        // If end with \n and the previous char is an '\r', discard this '\r'.
        *str = sstr.str();
        if (!line_empty) {
            if (!eof && str->at(str->length() - 1) == '\r') {
                *str = str->substr(0, str->length() - 1);
            }
        }
        return str;
    }

    // closeFd the pipe
    int close() {
        delete[] buffer;
        head = tail = 0;
        dataLen = 0;
        return 0;
    }

    // Compare contents of two buffers.
    static bool dataCompare(const char *buf1, const int off1, const char *buf2, const int off2, const int length) {
        for (int i = 0; i < length; i++) {
            if (buf1[off1 + i] != buf2[off2 + i]) {
                return false;
            }
        }
        return true;
    }

    // Re-fill the internal buffer
    int refill() {
        head = 0;
        tail = 0;
        dataLen = 0;
        int n = upFd->readn(buffer, bufferSize);
        if (n <= 0) {
            dataLen = 0;
        } else {
            head += n;
            dataLen = n;
        }
        return n;
    }

    // Check user arguments.
    bool checkArgs(char *buf, const int off, const int len) {
        if (buf == NULL) {
            return false;
        }
        return !(len < 0 || off < 0);
    }

private:
    static const int BUFFER_SIZE = 1024;
    static const int MAX_BUFFER_SIZE = 4194302;
    int bufferSize;
    char *buffer;
    std::unique_ptr<FileDescriptor> upFd;
    int head = 0;
    int tail = 0;
    int dataLen = 0; // effective data lengt
};

#endif /* STRINGEVALUATION_BUFFEREDDATAREADER_H */