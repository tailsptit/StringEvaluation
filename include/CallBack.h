#ifndef COCCOC_CALLBACK_H
#define COCCOC_CALLBACK_H
#include <functional>

class CallBack {
public:
    template<class F, class... Args>
    CallBack(F &&f, Args &&... args) {
        task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    }

    void run() {
        task();
    }

    virtual ~CallBack() {}

private:
    std::function<void()> task;
};

#endif /* COCCOC_CALLBACK_H */
