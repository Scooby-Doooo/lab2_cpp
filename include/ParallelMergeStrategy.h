// ParallelMergeStrategy.h
// Our custom parallel merge implementation

#ifndef PARALLEL_MERGE_STRATEGY_H
#define PARALLEL_MERGE_STRATEGY_H

#include "IMergeStrategy.h"

// Implementation of parallel merge using K threads
// Algorithm:
// 1. Split vec1 into K equal parts
// 2. For each part, find where it should split vec2 (using binary search)
// 3. Merge each pair of parts in parallel with std::merge
// 4. Concatenate results (they're already in order)
class ParallelMergeStrategy : public IMergeStrategy {
private:
    int numThreads_;
    
public:
    explicit ParallelMergeStrategy(int K);
    
    // Merge two sorted vectors using K threads
    std::vector<int> merge(const std::vector<int>& vec1, 
                          const std::vector<int>& vec2) override;
    
    std::string getName() const override;
    
    int getThreadCount() const;
};

#endif // PARALLEL_MERGE_STRATEGY_H
