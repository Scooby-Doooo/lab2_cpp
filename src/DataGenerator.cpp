// DataGenerator.cpp
// Random data generation implementation

#include "../include/DataGenerator.h"
#include <algorithm>

DataGenerator::DataGenerator(int min, int max) 
    : generator_(std::random_device{}()),
      distribution_(min, max) {}

std::vector<int> DataGenerator::generateSortedData(size_t size) {
    std::vector<int> data;
    data.reserve(size);
    
    // Generate random numbers
    for (size_t i = 0; i < size; ++i) {
        data.push_back(distribution_(generator_));
    }
    
    // Sort them before returning
    std::sort(data.begin(), data.end());
    return data;
}

std::vector<int> DataGenerator::generateUnsortedData(size_t size) {
    std::vector<int> data;
    data.reserve(size);
    
    for (size_t i = 0; i < size; ++i) {
        data.push_back(distribution_(generator_));
    }
    
    return data;
}
