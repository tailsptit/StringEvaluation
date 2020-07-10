//
// Created by tails on 08/07/2020.
//

#ifndef STRINGEVALUATION_MESSAGE_H
#define STRINGEVALUATION_MESSAGE_H

#include <string.h>


class Message {
public:
    enum State {INIT, READING, WRITING, ERROR,};
    Message() = default;
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
    };

    int getBufferSize() const { return bufferSize; }


    void writeToBuffer(const char *data, int size) {
        if (size < 0 || !buffer) {
            return;
        }
        memcpy(buffer, data, size);
    }

    char *charBuffer() const {
        return buffer;
    }

private:
    char *buffer = nullptr;
    int bufferSize = 0;
    State state = INIT;
};

#endif //STRINGEVALUATION_MESSAGE_H
