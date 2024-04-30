#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <mutex>
#include <condition_variable>

class Semaphore {
public:
    Semaphore(int count = 0) : count_(count) {}

    void notify() {
        std::unique_lock<std::mutex> lock(mutex_);
        count_++;
        cv_.notify_one();
    }

    void sem_wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (count_ == 0) {
            cv_.wait(lock);
        }
        count_--;
    }
    void sem_post() {
        std::lock_guard<std::mutex> lock(mutex_);
        ++count_;
        cv_.notify_one();
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    int count_;
};

#endif /* SEMAPHORE_H */
