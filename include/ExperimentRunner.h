// ExperimentRunner.h
// Runs all benchmark experiments

#ifndef EXPERIMENT_RUNNER_H
#define EXPERIMENT_RUNNER_H

#include "DataGenerator.h"
#include "BenchmarkResult.h"
#include <vector>
#include <unordered_map>

// This class handles all three experiments from the assignment
class ExperimentRunner {
private:
    DataGenerator dataGenerator_;
    std::vector<size_t> testSizes_;
    // Keep track of baseline times for comparison
    std::unordered_map<size_t, double> baselineBySize_;
    
    // Test std::merge with different execution policies
    void testMergeWithPolicies(size_t size);
    
    // Generate different K values to test
    std::vector<int> generateKValues(unsigned int cpuThreads);
    
    // Analyze results and find the best K value
    void analyzeResults(const std::vector<BenchmarkResult>& results, unsigned int cpuThreads);
    
public:
    explicit ExperimentRunner(std::vector<size_t> sizes);
    
    // Experiment 1: Test basic sequential merge (baseline)
    void runExperiment1_SequentialMerge();
    
    // Experiment 2: Test std::merge with different execution policies
    void runExperiment2_PolicyMerge();
    
    // Experiment 3: Test our custom parallel implementation with different K values
    void runExperiment3_KInvestigation(size_t testSize);
};

#endif // EXPERIMENT_RUNNER_H
