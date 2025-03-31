#ifndef BASE_QUEUE_SYNCHRONIZED_QUEUE_HPP
#define BASE_QUEUE_SYNCHRONIZED_QUEUE_HPP
#include <cstddef>
#include <chrono>

namespace base {
namespace queue {

template<typename T>
class SynchronizedQueue {
public:
    explicit SynchronizedQueue(size_t capacity);
    void enqueue(const T& item);
    T dequeue();

private:
    // Implementation details
};

} // namespace queue
} // namespace base

#endif // BASE_QUEUE_SYNCHRONIZED_QUEUE_HPP