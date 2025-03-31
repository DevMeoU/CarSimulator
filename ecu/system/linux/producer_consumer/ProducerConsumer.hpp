#ifndef SYSTEM_LINUX_PRODUCER_CONSUMER_HPP
#define SYSTEM_LINUX_PRODUCER_CONSUMER_HPP

#include "../../queue/SynchronizedQueue.hpp"

namespace system {
namespace linux {
namespace producer_consumer {

template<typename T>
class IProducer {
public:
    virtual ~IProducer() = default;
    virtual bool isRunning() const = 0;
    virtual void stop() = 0;
    virtual void produce(base::queue::SynchronizedQueue<T>& queue) = 0;
};

template<typename T>
class IConsumer {
public:
    virtual ~IConsumer() = default;
    virtual bool isRunning() const = 0;
    virtual void stop() = 0;
    virtual void consume(base::queue::SynchronizedQueue<T>& queue) = 0;
};

} // namespace producer_consumer
} // namespace linux
} // namespace system

#endif // SYSTEM_LINUX_PRODUCER_CONSUMER_HPP