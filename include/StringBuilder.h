//
// Created by tails on 08/07/2020.
//

#ifndef STRINGEVALUATION_STRINGBUILDER_H
#define STRINGEVALUATION_STRINGBUILDER_H

#include <string>
#include "string.h"

class StringBuilder {
public:
    StringBuilder() : bufferSize(DEFAULT_SIZE) {
        buffer = new char[bufferSize];
    }

    StringBuilder(const int _size) : bufferSize(_size), extendSize(_size) {
        buffer = new char[bufferSize];
    }

    ~StringBuilder() {
        if (buffer) {
            delete[] buffer;
        }
    }
    const char *charArray() const {
        return buffer;
    }


    unsigned int getSize() const {
        return size;
    }

    int IsEmpty() const { return getSize() == 0; }

    std::string toString() const {
        return std::string(buffer, size);
    }

    void clear() {
        size = 0;
        memset(buffer, 0, bufferSize);
    }
    void truncate(const int truncateSize) {
        if (truncateSize >= size) {
            return;
        }
        size = truncateSize;
        buffer[truncateSize] = 0;
    }

    void append(const char c) {
        if (size >= bufferSize) {
            bufferSize = bufferSize + extendSize;
            char *new_buf = new char[bufferSize];
            memcpy(new_buf, buffer, size);
            memset(new_buf + size, 0, bufferSize - size);
            delete buffer;
            buffer = new_buf;
        }
        buffer[size++] = c;
    }

    void append(std::string str) {
        append(str.c_str(), str.length());
    }


    void append(const char *str, const int appendSize) {
        if (str == NULL) {
            return;
        }
        if (size + appendSize > bufferSize) {
            bufferSize = size + appendSize;
            char *new_buf = new char[bufferSize];
            memcpy(new_buf, buffer, size);
            memcpy(new_buf + size, str, appendSize);
            size = bufferSize;
            delete buffer;
            buffer = new_buf;
        } else {
            memcpy(buffer + size, str, appendSize);
            size += appendSize;
        }
    }

private:
    static const int DEFAULT_SIZE = 128;
    int bufferSize = 0;
    int extendSize = 128;
    int size = 0;
    char *buffer = nullptr;
};


#endif /* STRINGEVALUATION_STRINGBUILDER_H */