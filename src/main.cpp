// Performance analysis of std::merge algorithm
// Compiler: MSVC 19.x (Visual Studio 2022) or GCC 13+ / Clang 17+
// Standard: C++20

#include "../include/SystemInfo.h"
#include "../include/ExperimentRunner.h"
#include <iostream>
#include <vector>

int main() {
    std::cout << "=============================================================================\n";
    std::cout << "         MERGE ALGORITHM PERFORMANCE ANALYSIS\n";
    std::cout << "=============================================================================\n";
    
    // Get system info first
    unsigned int cpuThreads = SystemInfo::getHardwareThreads();
    std::cout << "\nSystem Information:\n";
    std::cout << "  CPU Hardware Threads: " << cpuThreads << "\n";
    std::cout << "  C++ Standard: C++20\n";
    
    // We'll test with three different data sizes
    std::vector<size_t> testSizes = {
        100'000,      // 100 thousand
        1'000'000,    // 1 million
        10'000'000    // 10 million
    };
    
    std::cout << "\nTest Data Sizes:\n";
    for (size_t size : testSizes) {
        std::cout << "  " << size << " elements\n";
    }
    
    // Run all experiments
    ExperimentRunner runner(testSizes);
    
    runner.runExperiment1_SequentialMerge();
    runner.runExperiment2_PolicyMerge();
    runner.runExperiment3_KInvestigation(testSizes.back());
    
    std::cout << "\n=============================================================================\n";
    std::cout << "Analysis completed successfully.\n";
    std::cout << "=============================================================================\n";
    
    return 0;
}
