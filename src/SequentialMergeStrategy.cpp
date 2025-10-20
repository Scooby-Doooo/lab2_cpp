// SequentialMergeStrategy.cpp
// Basic sequential merge

#include "../include/SequentialMergeStrategy.h"
#include <algorithm>

std::vector<int> SequentialMergeStrategy::merge(const std::vector<int>& vec1, 
                                                 const std::vector<int>& vec2) {
    // Simple wrapper around std::merge
    std::vector<int> result(vec1.size() + vec2.size());
    std::merge(vec1.begin(), vec1.end(), 
              vec2.begin(), vec2.end(), 
              result.begin());
    return result;
}

std::string SequentialMergeStrategy::getName() const {
    return "Sequential std::merge";
}
