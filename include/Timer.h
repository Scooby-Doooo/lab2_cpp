// Timer.h
// Simple timer for measuring execution time

#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <functional>

// Helper class for timing code execution
class Timer {
private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    
public:
    // Measure how long a function takes to run (in milliseconds)
    template<typename Func>
    static double measure(Func&& func) {
        auto start = Clock::now();
        func();
        auto end = Clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        return elapsed.count();
    }
};

#endif // TIMER_H
