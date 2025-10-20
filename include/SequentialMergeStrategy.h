// SequentialMergeStrategy.h
// Simple sequential merge using std::merge

#ifndef SEQUENTIAL_MERGE_STRATEGY_H
#define SEQUENTIAL_MERGE_STRATEGY_H

#include "IMergeStrategy.h"

// Basic sequential merge - just wraps std::merge
class SequentialMergeStrategy : public IMergeStrategy {
public:
    std::vector<int> merge(const std::vector<int>& vec1, 
                          const std::vector<int>& vec2) override;
    
    std::string getName() const override;
};

#endif // SEQUENTIAL_MERGE_STRATEGY_H
