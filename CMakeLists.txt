# CMakeLists.txt
# CMake build configuration for Merge Algorithm Performance Analysis
# Compiler: Visual Studio 2022 (MSVC 19.x)
# Usage:
#   Open folder in Visual Studio 2022

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


# OPTIMIZATION LEVEL CONFIGURATION

# Specify the optimization level: O0 (no optimization), O2 (standard), O3 (maximum)
set(OPTIMIZATION_LEVEL "O3" CACHE STRING "Optimization level: O0, O2, O3")
set_property(CACHE OPTIMIZATION_LEVEL PROPERTY STRINGS O0 O2 O3)

message(STATUS "")
message(STATUS "========================================")
message(STATUS "Optimization Level: ${OPTIMIZATION_LEVEL}")
message(STATUS "========================================")

# Apply optimization level for MSVC
if(MSVC)
    # Remove default flags
    string(REGEX REPLACE "/O[0-2]" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REGEX REPLACE "/O[0-2]" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    
    if(OPTIMIZATION_LEVEL STREQUAL "O0")
        # No optimization
        add_compile_options(/Od /Zi)
        message(STATUS "MSVC: No optimization (/Od)")
        
    elseif(OPTIMIZATION_LEVEL STREQUAL "O2")
        # Standard optimization
        add_compile_options(/O2)
        message(STATUS "MSVC: Standard optimization (/O2)")
        
    elseif(OPTIMIZATION_LEVEL STREQUAL "O3")
        # Maximum optimization
        add_compile_options(/O2 /GL /Oi /Ot)
        add_link_options(/LTCG)
        message(STATUS "MSVC: Maximum optimization (/O2 /GL /Oi /Ot + LTCG)")
    endif()

    # Additional flags
    add_compile_options(/W4 /MP)
endif()

message(STATUS "========================================")
message(STATUS "")


# PROJECT SETUP


# Add include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Collect all source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(merge_benchmark ${SOURCES})

# Enable threading support
find_package(Threads REQUIRED)
target_link_libraries(merge_benchmark PRIVATE Threads::Threads)

# Installation
install(TARGETS merge_benchmark DESTINATION bin)

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
