# CMakeLists.txt
# CMake build configuration for Merge Algorithm Performance Analysis
# Usage:
#   mkdir build
#   cd build
#   cmake ..
#   cmake --build .

cmake_minimum_required(VERSION 3.15)

project(MergeAlgorithmAnalysis 
    VERSION 1.0
    DESCRIPTION "Performance analysis of merge algorithms with OOP design"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Collect all source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(merge_benchmark ${SOURCES})

# Enable threading support
find_package(Threads REQUIRED)
target_link_libraries(merge_benchmark PRIVATE Threads::Threads)

# Compiler-specific optimizations
if(MSVC)
    target_compile_options(merge_benchmark PRIVATE /W4 /O2)
else()
    target_compile_options(merge_benchmark PRIVATE -Wall -Wextra -O3 -pthread)
endif()

# Installation
install(TARGETS merge_benchmark DESTINATION bin)

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
