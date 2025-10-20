// BenchmarkRunner.h
// Runs benchmarks and measures performance

#ifndef BENCHMARK_RUNNER_H
#define BENCHMARK_RUNNER_H

#include "DataGenerator.h"
#include "IMergeStrategy.h"
#include "BenchmarkResult.h"

// Handles running benchmarks and measuring execution time
class BenchmarkRunner {
private:
    DataGenerator& dataGenerator_;
    size_t dataSize_;
    int numRuns_;
    
public:
    BenchmarkRunner(DataGenerator& generator, size_t size, int runs = 5);
    
    // Run a benchmark and get average time
    BenchmarkResult runBenchmark(IMergeStrategy& strategy, double baselineTime = 0.0);
    
    // Run benchmark with detailed output
    BenchmarkResult runDetailedBenchmark(IMergeStrategy& strategy);
};

#endif // BENCHMARK_RUNNER_H
