#ifndef BASE_QUEUE_SYNCHRONIZEDQUEUE_HPP
#define BASE_QUEUE_SYNCHRONIZEDQUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

namespace base::queue {

template<typename T>
class SynchronizedQueue {
public:
    explicit SynchronizedQueue(size_t maxSize = 100) : maxSize_(maxSize) {}

    void enqueue(const T& item) {
        std::unique_lock<std::mutex> lock(mutex_);
        notFull_.wait(lock, [this]() { return queue_.size() < maxSize_; });
        queue_.push(item);
        lock.unlock();
        notEmpty_.notify_one();
    }

    T dequeue() {
        std::unique_lock<std::mutex> lock(mutex_);
        notEmpty_.wait(lock, [this]() { return !queue_.empty(); });
        T item = queue_.front();
        queue_.pop();
        lock.unlock();
        notFull_.notify_one();
        return item;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable notEmpty_;
    std::condition_variable notFull_;
    const size_t maxSize_;
};

} // namespace base::queue

#endif // BASE_QUEUE_SYNCHRONIZEDQUEUE_HPP