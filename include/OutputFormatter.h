// OutputFormatter.h
// Handles formatted console output

#ifndef OUTPUT_FORMATTER_H
#define OUTPUT_FORMATTER_H

#include <string>

// Helper class for printing nice-looking output
class OutputFormatter {
public:
    // Print a big section header
    static void printSectionHeader(const std::string& title);
    
    // Print a smaller subsection header
    static void printSubsectionHeader(const std::string& title);
    
    // Print table header for results
    static void printTableHeader();
    
    // Print one row of results
    static void printTableRow(int K, double time, double speedup, double ratio);
};

#endif // OUTPUT_FORMATTER_H
