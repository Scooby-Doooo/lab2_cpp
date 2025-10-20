// OutputFormatter.cpp
// Helper functions for nice output formatting

#include "../include/OutputFormatter.h"
#include <iostream>
#include <iomanip>
#include <string>

namespace {
	constexpr int SECTION_HEADER_WIDTH = 80;
	constexpr int TABLE_SEPARATOR_WIDTH = 60;
	constexpr int TABLE_COL_K_WIDTH = 8;
	constexpr int TABLE_COL_TIME_WIDTH = 15;
	constexpr int TABLE_COL_SPEEDUP_WIDTH = 15;
	constexpr int TABLE_COL_RATIO_WIDTH = 22;
	constexpr int PRECISION_TIME = 3;
	constexpr int PRECISION_RATIO = 2;
}

void OutputFormatter::printSectionHeader(const std::string& title) {
    std::cout << "\n" << std::string(SECTION_HEADER_WIDTH, '=') << "\n";
    std::cout << title << "\n";
    std::cout << std::string(SECTION_HEADER_WIDTH, '=') << "\n";
}

void OutputFormatter::printSubsectionHeader(const std::string& title) {
    std::cout << "\n--- " << title << " ---\n";
}

void OutputFormatter::printTableHeader() {
    std::cout << std::string(TABLE_SEPARATOR_WIDTH, '-') << "\n";
    std::cout << std::setw(TABLE_COL_K_WIDTH) << "K" 
              << std::setw(TABLE_COL_TIME_WIDTH) << "Time (ms)" 
              << std::setw(TABLE_COL_SPEEDUP_WIDTH) << "Speedup"
              << std::setw(TABLE_COL_RATIO_WIDTH) << "K / CPU_threads\n";
    std::cout << std::string(TABLE_SEPARATOR_WIDTH, '-') << "\n";
}

void OutputFormatter::printTableRow(int K, double time, double speedup, double ratio) {
    std::cout << std::setw(TABLE_COL_K_WIDTH) << K
              << std::setw(TABLE_COL_TIME_WIDTH) << std::fixed << std::setprecision(PRECISION_TIME) << time
              << std::setw(TABLE_COL_SPEEDUP_WIDTH) << std::fixed << std::setprecision(PRECISION_RATIO) << speedup << "x"
              << std::setw(TABLE_COL_RATIO_WIDTH) << std::fixed << std::setprecision(PRECISION_RATIO) << ratio << "\n";
}
