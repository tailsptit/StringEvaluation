//
// Created by tails on 08/07/2020.
//

#ifndef COCCOC_MESSAGE_H
#define COCCOC_MESSAGE_H

#include <string.h>


class Message {
public:
    enum State {INIT, READING, FINISH_READING, WRITING, ERROR,};
    Message() {}
    Message(int _bufferSize) : bufferSize(_bufferSize) {
        buffer = new char[bufferSize];
    }
    ~Message() {
        if (buffer) {
            delete[] buffer;
        }
    };

    void setState(enum Message::State _state) {
        state = _state;
    }

    enum State getState() const {
        return state;
    };
    void resetBuffer(int size) {
        if (buffer) {
            delete[] buffer;
        }
        buffer = new char[size];
        bufferSize = size;
        receivedSize = 0;
    };
    int getBufferSize() const { return bufferSize; }
    int received_size() const { return receivedSize; }
    int getWrittenSize() const { return writtenSize; }
    void addWrittenSize(int size) { writtenSize += size; }
    void writeToBuffer(const char *data, int size) {
        if (size < 0 || !buffer) {
            return;
        }
        int write_size = size < bufferSize - receivedSize ? size : bufferSize - receivedSize;
        memcpy(buffer + receivedSize, data, write_size);
        receivedSize += write_size;
    }
    bool isFull() const {
        return receivedSize == bufferSize;
    }
    char *charBuffer() const {
        return buffer;
    }
    void setSendingVector(const std::vector<int> &v) { vec = v; }
    std::vector<int> sendingVector() const { return vec; };

private:
    char *buffer = nullptr;
    State state = INIT;
    int bufferSize = 0;
    int receivedSize = 0;
    int writtenSize = 0;
    std::vector<int> vec;
};

#endif //COCCOC_MESSAGE_H
