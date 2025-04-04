#ifndef ITHREAD_HANDLER_H
#define ITHREAD_HANDLER_H

#include <atomic>
#include <thread>
#include <memory>

/**
 * @class IThreadHandler
 * @brief Interface for thread handlers
 */
class IThreadHandler {
public:
    virtual ~IThreadHandler() = default;
    
    /**
     * @brief Start the thread
     */
    virtual void start() = 0;
    
    /**
     * @brief Stop the thread
     */
    virtual void stop() = 0;
    
    /**
     * @brief Check if thread is running
     * @return true if running, false otherwise
     */
    virtual bool isRunning() const = 0;
    
    /**
     * @brief Get thread ID
     * @return Thread ID
     */
    virtual std::thread::id getThreadId() const = 0;

protected:
    /**
     * @brief Main thread function
     */
    virtual void threadFunction() = 0;
};

#endif // ITHREAD_HANDLER_H