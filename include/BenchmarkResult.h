// BenchmarkResult.h
// Stores results from a benchmark run

#ifndef BENCHMARK_RESULT_H
#define BENCHMARK_RESULT_H

#include <string>

// Simple struct to hold benchmark results
struct BenchmarkResult {
    std::string strategyName;
    double averageTime;
    double speedup;
    int threadCount;
    
    BenchmarkResult(const std::string& name, double time, double sp, int threads = 1);
};

#endif // BENCHMARK_RESULT_H
