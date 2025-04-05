#ifndef THREAD_PRIORITY_H
#define THREAD_PRIORITY_H

#ifdef _WIN32
#include <windows.h>
#endif

enum class ThreadPriority {
    HIGHEST = THREAD_PRIORITY_HIGHEST,      // Độ ưu tiên cao nhất
    ABOVE_NORMAL = THREAD_PRIORITY_ABOVE_NORMAL,  // Độ ưu tiên trên trung bình
    NORMAL = THREAD_PRIORITY_NORMAL,      // Độ ưu tiên trung bình
    BELOW_NORMAL = THREAD_PRIORITY_BELOW_NORMAL,  // Độ ưu tiên dưới trung bình
    LOWEST = THREAD_PRIORITY_LOWEST       // Độ ưu tiên thấp nhất
};

#endif // THREAD_PRIORITY_H