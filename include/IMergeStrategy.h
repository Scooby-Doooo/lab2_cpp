// IMergeStrategy.h
// Interface for different merge implementations

#ifndef IMERGE_STRATEGY_H
#define IMERGE_STRATEGY_H

#include <vector>
#include <string>

// Base interface for merge strategies
// Allows us to swap between different implementations easily
class IMergeStrategy {
public:
    virtual ~IMergeStrategy() = default;
    
    // Merge two sorted vectors into one sorted vector
    virtual std::vector<int> merge(const std::vector<int>& vec1, 
                                   const std::vector<int>& vec2) = 0;
    
    // Get a descriptive name for this strategy
    virtual std::string getName() const = 0;
};

#endif // IMERGE_STRATEGY_H
