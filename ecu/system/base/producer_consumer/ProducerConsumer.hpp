#ifndef BASE_PRODUCER_CONSUMER_HPP
#define BASE_PRODUCER_CONSUMER_HPP

#include "../queue/SynchronizedQueue.hpp"

namespace base::producer_consumer {

// Interface cho producer, định nghĩa các phương thức cần thiết để sản xuất dữ liệu
template<typename T>
class IProducer {
public:
    virtual ~IProducer() = default;
    
    // Kiểm tra xem producer có đang hoạt động hay không
    virtual bool isRunning() const = 0;
    
    // Dừng hoạt động của producer
    virtual void stop() = 0;
    
    // Sản xuất dữ liệu và đưa vào queue
    virtual void produce(SynchronizedQueue<T>& queue) = 0;
};

// Interface cho consumer, định nghĩa các phương thức cần thiết để tiêu thụ dữ liệu
template<typename T>
class IConsumer {
public:
    virtual ~IConsumer() = default;
    
    // Kiểm tra xem consumer có đang hoạt động hay không
    virtual bool isRunning() const = 0;
    
    // Dừng hoạt động của consumer
    virtual void stop() = 0;
    
    // Lấy và xử lý dữ liệu từ queue
    virtual void consume(SynchronizedQueue<T>& queue) = 0;
};

} // namespace base::producer_consumer

#endif // BASE_PRODUCER_CONSUMER_HPP