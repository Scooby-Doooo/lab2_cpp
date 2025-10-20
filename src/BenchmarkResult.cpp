// BenchmarkResult.cpp
// Simple data holder for results

#include "../include/BenchmarkResult.h"

BenchmarkResult::BenchmarkResult(const std::string& name, double time, double sp, int threads)
    : strategyName(name), averageTime(time), speedup(sp), threadCount(threads) {}
