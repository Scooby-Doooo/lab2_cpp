// ExperimentRunner.cpp
// Implementation of experiment runner

#include "../include/ExperimentRunner.h"
#include "../include/OutputFormatter.h"
#include "../include/SystemInfo.h"
#include "../include/Timer.h"
#include "../include/BenchmarkRunner.h"
#include "../include/SequentialMergeStrategy.h"
#include "../include/ParallelMergeStrategy.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <execution>

namespace {
	// How many times we run each test to get average
	constexpr int DEFAULT_NUM_RUNS = 5;
	constexpr int SEPARATOR_WIDTH_NARROW = 60;
	constexpr int SEPARATOR_WIDTH_STANDARD = 70;
	constexpr int PRECISION_TIME = 3;
	constexpr int PRECISION_RATIO = 2;
	
	// Constants for generating K values to test
	constexpr int K_BASE_VALUE_1 = 1;
	constexpr int K_BASE_VALUE_2 = 2;
	constexpr int K_BASE_VALUE_4 = 4;
	constexpr int K_MULTIPLIER_2X = 2;
	constexpr int K_MULTIPLIER_4X = 4;
	constexpr unsigned int MIN_THREADS_FOR_HALF_K = 6;
}

ExperimentRunner::ExperimentRunner(std::vector<size_t> sizes)
	: testSizes_(std::move(sizes)) {}

void ExperimentRunner::runExperiment1_SequentialMerge() {
	OutputFormatter::printSectionHeader("EXPERIMENT 1: Sequential std::merge (baseline)");
	std::cout << "\nThis experiment is integrated into Experiment 2.\n";
	std::cout << "Check Experiment 2 results for baseline measurements.\n\n";
}

void ExperimentRunner::runExperiment2_PolicyMerge() {
	OutputFormatter::printSectionHeader("EXPERIMENT 2: Testing std::merge with Execution Policies");
	std::cout << "\nAll tests use the same structure for fair comparison.\n";
	std::cout << "Fresh data is generated for each test, measuring only merge time.\n";
	
	for (size_t size : testSizes_) {
		std::cout << "\nTest Size: " << size << " elements\n";
		std::cout << std::string(SEPARATOR_WIDTH_STANDARD, '-') << "\n";
		testMergeWithPolicies(size);
	}
}

void ExperimentRunner::runExperiment3_KInvestigation(size_t testSize) {
	OutputFormatter::printSectionHeader("EXPERIMENT 3: Custom Parallel Merge - Finding Optimal K");

	std::cout << "\nTesting K parameter for data size: " << testSize << " elements\n\n";

	unsigned int cpuThreads = SystemInfo::getHardwareThreads();
	std::cout << "CPU Hardware Threads: " << cpuThreads << "\n\n";

	// Generate test data once
	std::cout << "Generating test data... ";
	std::cout.flush();
	size_t halfSize = testSize / 2;
	auto vec1 = dataGenerator_.generateSortedData(halfSize);
	auto vec2 = dataGenerator_.generateSortedData(halfSize);
	std::cout << "Done\n\n";

	// Decide which K values to test
	std::vector<int> kValues = generateKValues(cpuThreads);

	// Run benchmarks for each K
	std::vector<BenchmarkResult> results;
	BenchmarkRunner runner(dataGenerator_, testSize);

	OutputFormatter::printTableHeader();

	double baselineTime = 0.0;

	for (int K : kValues) {
		ParallelMergeStrategy strategy(K);
		auto result = runner.runBenchmark(strategy, baselineTime);

		if (K == 1) {
			baselineTime = result.averageTime;
			result.speedup = 1.0;
		}

		results.push_back(result);

		double ratio = static_cast<double>(K) / cpuThreads;
		OutputFormatter::printTableRow(K, result.averageTime, result.speedup, ratio);
	}

	std::cout << std::string(SEPARATOR_WIDTH_NARROW, '-') << "\n\n";

	// Find and print the best K value
	analyzeResults(results, cpuThreads);
}

void ExperimentRunner::testMergeWithPolicies(size_t size) {
	std::cout << "\n  Testing std::merge with different execution policies.\n";
	std::cout << "  Using two separate sorted vectors merged into output buffer.\n\n";

	const int numRuns = DEFAULT_NUM_RUNS;
	size_t halfSize = size / 2;

	// [1] Sequential merge without policy - this is our baseline
	std::cout << "  [1] std::merge (sequential, no policy):\n";
	double totalSeq = 0.0;
	for (int run = 0; run < numRuns; ++run) {
		auto vec1 = dataGenerator_.generateSortedData(halfSize);
		auto vec2 = dataGenerator_.generateSortedData(halfSize);
		std::vector<int> output(vec1.size() + vec2.size());

		double time = Timer::measure([&]() {
			std::merge(vec1.begin(), vec1.end(),
					   vec2.begin(), vec2.end(),
					   output.begin());
		});
		totalSeq += time;
	}
	double avgSeq = totalSeq / numRuns;
	std::cout << "      Average: " << std::fixed << std::setprecision(PRECISION_TIME)
			  << avgSeq << " ms (baseline)\n\n";

	// Save baseline for later comparisons
	baselineBySize_[size] = avgSeq;

	// Try testing with execution policies if available
	#ifndef __cpp_lib_parallel_algorithm
	std::cout << "  Note: Execution policies not supported by compiler/library.\n";
	std::cout << "        Tests [2-4] will be skipped.\n\n";
	#else
	
	// [2] std::execution::seq - sequential policy
	std::cout << "  [2] std::merge with std::execution::seq:\n";
	double totalExecSeq = 0.0;
	for (int run = 0; run < numRuns; ++run) {
		auto vec1 = dataGenerator_.generateSortedData(halfSize);
		auto vec2 = dataGenerator_.generateSortedData(halfSize);
		std::vector<int> output(vec1.size() + vec2.size());

		double time = Timer::measure([&]() {
			std::merge(std::execution::seq,
					   vec1.begin(), vec1.end(),
					   vec2.begin(), vec2.end(),
					   output.begin());
		});
		totalExecSeq += time;
	}
	double avgExecSeq = totalExecSeq / numRuns;
	double ratioExecSeq = avgSeq / avgExecSeq;
	std::cout << "      Average: " << std::fixed << std::setprecision(PRECISION_TIME)
			  << avgExecSeq << " ms";
	std::cout << " (vs baseline: " << std::setprecision(PRECISION_RATIO)
			  << ratioExecSeq << "x)\n\n";

	// [3] std::execution::par - parallel policy
	std::cout << "  [3] std::merge with std::execution::par:\n";
	double totalPar = 0.0;
	for (int run = 0; run < numRuns; ++run) {
		auto vec1 = dataGenerator_.generateSortedData(halfSize);
		auto vec2 = dataGenerator_.generateSortedData(halfSize);
		std::vector<int> output(vec1.size() + vec2.size());

		double time = Timer::measure([&]() {
			std::merge(std::execution::par,
					   vec1.begin(), vec1.end(),
					   vec2.begin(), vec2.end(),
					   output.begin());
		});
		totalPar += time;
	}
	double avgPar = totalPar / numRuns;
	double ratioPar = avgSeq / avgPar;
	std::cout << "      Average: " << std::fixed << std::setprecision(PRECISION_TIME)
			  << avgPar << " ms";
	std::cout << " (vs baseline: " << std::setprecision(PRECISION_RATIO)
			  << ratioPar << "x)\n\n";

	// [4] std::execution::par_unseq - parallel + vectorization
	std::cout << "  [4] std::merge with std::execution::par_unseq:\n";
	double totalParUnseq = 0.0;
	for (int run = 0; run < numRuns; ++run) {
		auto vec1 = dataGenerator_.generateSortedData(halfSize);
		auto vec2 = dataGenerator_.generateSortedData(halfSize);
		std::vector<int> output(vec1.size() + vec2.size());

		double time = Timer::measure([&]() {
			std::merge(std::execution::par_unseq,
					   vec1.begin(), vec1.end(),
					   vec2.begin(), vec2.end(),
					   output.begin());
		});
		totalParUnseq += time;
	}
	double avgParUnseq = totalParUnseq / numRuns;
	double ratioParUnseq = avgSeq / avgParUnseq;
	std::cout << "      Average: " << std::fixed << std::setprecision(PRECISION_TIME)
			  << avgParUnseq << " ms";
	std::cout << " (vs baseline: " << std::setprecision(PRECISION_RATIO)
			  << ratioParUnseq << "x)\n\n";
	
	#endif
}

std::vector<int> ExperimentRunner::generateKValues(unsigned int cpuThreads) {
	std::vector<int> kValues;
	// Start with small values
	kValues.push_back(K_BASE_VALUE_1);
	kValues.push_back(K_BASE_VALUE_2);
	kValues.push_back(K_BASE_VALUE_4);

	// Add half of CPU threads if we have enough
	if (cpuThreads >= MIN_THREADS_FOR_HALF_K) {
		kValues.push_back(cpuThreads / 2);
	}
	// Add CPU thread count and multiples of it
	kValues.push_back(cpuThreads);
	kValues.push_back(cpuThreads * K_MULTIPLIER_2X);
	kValues.push_back(cpuThreads * K_MULTIPLIER_4X);

	return kValues;
}

void ExperimentRunner::analyzeResults(const std::vector<BenchmarkResult>& results, unsigned int cpuThreads) {
	// Find which K gave us the best performance
	auto bestIt = std::min_element(results.begin(), results.end(),
		[](const BenchmarkResult& a, const BenchmarkResult& b) {
			return a.averageTime < b.averageTime;
		});

	const auto& best = *bestIt;
	double bestRatio = static_cast<double>(best.threadCount) / cpuThreads;

	std::cout << "RESULTS SUMMARY:\n";
	std::cout << "  Optimal K value: " << best.threadCount << "\n";
	std::cout << "  Best time: " << std::fixed << std::setprecision(PRECISION_TIME)
			  << best.averageTime << " ms\n";
	std::cout << "  Speedup vs K=1: " << std::fixed << std::setprecision(PRECISION_RATIO)
			  << best.speedup << "x\n";
	std::cout << "  K / CPU_threads: " << std::fixed << std::setprecision(PRECISION_RATIO)
			  << bestRatio << "\n";

	// Try to understand what we're seeing
	std::cout << "\n  Observations:\n";
	if (best.threadCount < static_cast<int>(cpuThreads)) {
		std::cout << "    - Optimal K is below CPU thread count\n";
		std::cout << "    - Thread management overhead limits scaling\n";
	} else if (best.threadCount == static_cast<int>(cpuThreads)) {
		std::cout << "    - Optimal K matches CPU thread count\n";
		std::cout << "    - This is typically the sweet spot\n";
	} else {
		std::cout << "    - Optimal K exceeds CPU thread count\n";
		std::cout << "    - Possible reasons: I/O wait or cache effects\n";
	}

	// Check if performance gets worse at high K
	if (results.size() > 1 && results.back().averageTime > best.averageTime) {
		double degradation = ((results.back().averageTime / best.averageTime) - 1.0) * 100.0;
		std::cout << "    - Performance drops by " << std::fixed
				  << std::setprecision(1) << degradation << "% at highest K\n";
		std::cout << "    - Too many threads cause overhead\n";
	}
}
