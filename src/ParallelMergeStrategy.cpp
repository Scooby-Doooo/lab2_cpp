// ParallelMergeStrategy.cpp
// Parallel merge implementation

#include "../include/ParallelMergeStrategy.h"
#include "../include/SequentialMergeStrategy.h"
#include <algorithm>
#include <thread>
#include <iterator>

ParallelMergeStrategy::ParallelMergeStrategy(int K) 
    : numThreads_(K > 0 ? K : 1) {}

std::vector<int> ParallelMergeStrategy::merge(const std::vector<int>& vec1, 
                                               const std::vector<int>& vec2) {
    if (numThreads_ == 1) {
        // Just use sequential merge if K=1
        return SequentialMergeStrategy().merge(vec1, vec2);
    }
    
    size_t n1 = vec1.size();
    size_t n2 = vec2.size();
    
    // Each thread stores its result here
    std::vector<std::vector<int>> partialResults(numThreads_);
    std::vector<std::thread> threads;
    
    // Create K threads, each handling one part
    for (int i = 0; i < numThreads_; ++i) {
        threads.emplace_back([&, i]() {
            // Figure out which part of vec1 this thread handles
            size_t start1 = (n1 * i) / numThreads_;
            size_t end1 = (n1 * (i + 1)) / numThreads_;
            
            // Find the corresponding split in vec2 using binary search
            size_t start2, end2;
            
            if (start1 < n1) {
                start2 = std::lower_bound(vec2.begin(), vec2.end(), vec1[start1]) - vec2.begin();
            } else {
                start2 = n2;
            }
            
            if (end1 < n1) {
                end2 = std::lower_bound(vec2.begin(), vec2.end(), vec1[end1]) - vec2.begin();
            } else {
                end2 = n2;
            }
            
            // Merge these two ranges
            size_t resultSize = (end1 - start1) + (end2 - start2);
            partialResults[i].resize(resultSize);
            
            std::merge(vec1.begin() + start1, vec1.begin() + end1,
                      vec2.begin() + start2, vec2.begin() + end2,
                      partialResults[i].begin());
        });
    }
    
    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }
    
    // Put all partial results together (they're already sorted relative to each other)
    size_t totalSize = 0;
    for (const auto& partial : partialResults) {
        totalSize += partial.size();
    }
    
    std::vector<int> result;
    result.reserve(totalSize);
    
    for (const auto& partial : partialResults) {
        result.insert(result.end(), partial.begin(), partial.end());
    }
    
    return result;
}

std::string ParallelMergeStrategy::getName() const {
    return "Parallel merge (K=" + std::to_string(numThreads_) + ")";
}

int ParallelMergeStrategy::getThreadCount() const {
    return numThreads_;
}
