// SystemInfo.cpp
// Get CPU hardware info

#include "../include/SystemInfo.h"
#include <thread>

namespace {
	// Fallback if we can't detect hardware threads
	constexpr unsigned int DEFAULT_THREAD_COUNT = 4;
}

unsigned int SystemInfo::getHardwareThreads() {
    unsigned int threads = std::thread::hardware_concurrency();
    // Return default if detection fails
    return threads > 0 ? threads : DEFAULT_THREAD_COUNT;
}
