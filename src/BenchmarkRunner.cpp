// BenchmarkRunner.cpp
// Benchmark execution and timing

#include "../include/BenchmarkRunner.h"
#include "../include/Timer.h"
#include "../include/ParallelMergeStrategy.h"
#include <iostream>
#include <iomanip>

namespace {
	constexpr int PRECISION_TIME = 3;
}

BenchmarkRunner::BenchmarkRunner(DataGenerator& generator, size_t size, int runs)
    : dataGenerator_(generator), dataSize_(size), numRuns_(runs) {}

BenchmarkResult BenchmarkRunner::runBenchmark(IMergeStrategy& strategy, double baselineTime) {
    size_t halfSize = dataSize_ / 2;
    auto vec1 = dataGenerator_.generateSortedData(halfSize);
    auto vec2 = dataGenerator_.generateSortedData(halfSize);
    
    double totalTime = 0.0;
    
    // Run multiple times and average the results
    for (int run = 0; run < numRuns_; ++run) {
        double time = Timer::measure([&]() {
            auto result = strategy.merge(vec1, vec2);
        });
        totalTime += time;
    }
    
    double avgTime = totalTime / numRuns_;
    double speedup = (baselineTime > 0) ? baselineTime / avgTime : 1.0;
    
    // Get thread count if this is a parallel strategy
    int threads = 1;
    if (auto* parallel = dynamic_cast<ParallelMergeStrategy*>(&strategy)) {
        threads = parallel->getThreadCount();
    }
    
    return BenchmarkResult(strategy.getName(), avgTime, speedup, threads);
}

BenchmarkResult BenchmarkRunner::runDetailedBenchmark(IMergeStrategy& strategy) {
    std::cout << "\nTest Size: " << dataSize_ << " elements\n";
    std::cout << "  Generating data... ";
    std::cout.flush();
    
    size_t halfSize = dataSize_ / 2;
    auto vec1 = dataGenerator_.generateSortedData(halfSize);
    auto vec2 = dataGenerator_.generateSortedData(halfSize);
    
    std::cout << "Done\n";
    std::cout << "  Running " << numRuns_ << " test iterations...\n";
    
    double totalTime = 0.0;
    
    for (int run = 0; run < numRuns_; ++run) {
        double time = Timer::measure([&]() {
            auto result = strategy.merge(vec1, vec2);
        });
        totalTime += time;
        std::cout << "    Run " << (run + 1) << ": " << std::fixed 
                  << std::setprecision(PRECISION_TIME) << time << " ms\n";
    }
    
    double avgTime = totalTime / numRuns_;
    std::cout << "  Average: " << std::fixed << std::setprecision(PRECISION_TIME) 
              << avgTime << " ms\n";
    
    return BenchmarkResult(strategy.getName(), avgTime, 1.0);
}
