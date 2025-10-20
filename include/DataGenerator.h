// DataGenerator.h
// Generates random test data

#ifndef DATA_GENERATOR_H
#define DATA_GENERATOR_H

#include <vector>
#include <random>

// Handles generation of random test data
class DataGenerator {
private:
    std::mt19937 generator_;
    std::uniform_int_distribution<int> distribution_;
    
public:
    explicit DataGenerator(int min = 1, int max = 1'000'000);
    
    // Generate a sorted vector of random numbers
    std::vector<int> generateSortedData(size_t size);
    
    // Generate unsorted random numbers
    std::vector<int> generateUnsortedData(size_t size);
};

#endif // DATA_GENERATOR_H
