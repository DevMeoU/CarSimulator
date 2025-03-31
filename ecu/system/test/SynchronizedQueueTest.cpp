#include "SynchronizedQueue.hpp"
#include <iostream>
#include <thread>
#include <chrono>

// Ví dụ sử dụng SynchronizedQueue với Producer và Consumer
int main() {
    SynchronizedQueue<int> queue(5); // Hàng đợi có tối đa 5 phần tử

    // Luồng Producer: tạo dữ liệu và đưa vào hàng đợi
    std::thread producer([&queue]() {
        for (int i = 1; i <= 10; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::cout << "[Producer] Producing: " << i << std::endl;
            queue.enqueue(i);
        }
    });

    // Luồng Consumer: lấy dữ liệu từ hàng đợi
    std::thread consumer([&queue]() {
        for (int i = 1; i <= 10; ++i) {
            int item = queue.dequeue();
            std::cout << "    [Consumer] Consuming: " << item << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
    });

    producer.join();
    consumer.join();

    return 0;
}