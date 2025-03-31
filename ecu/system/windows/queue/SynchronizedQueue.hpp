#ifndef SYSTEM_WINDOWS_QUEUE_HPP
#define SYSTEM_WINDOWS_QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

namespace car_system {
namespace windows {
namespace queue {

template<typename T>
class SynchronizedQueue {
public:
    SynchronizedQueue(size_t capacity) : capacity_(capacity) {}
    
    void enqueue(const T& item) {
        std::unique_lock<std::mutex> lock(mutex_);
        not_full_.wait(lock, [this]() { return queue_.size() < capacity_; });
        queue_.push(item);
        not_empty_.notify_one();
    }
    
    T dequeue() {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [this]() { return !queue_.empty(); });
        T item = queue_.front();
        queue_.pop();
        not_full_.notify_one();
        return item;
    }
    
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }
    
private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
    size_t capacity_;
};

} // namespace queue
} // namespace windows
} // namespace car_system

#endif // SYSTEM_WINDOWS_QUEUE_HPP