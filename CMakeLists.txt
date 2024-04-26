cmake_minimum_required(VERSION 3.10)

# ===== Main Part =====

#Set the project name
project(PPN LANGUAGES CXX DESCRIPTION "Machine Learning Tool for Regression Problems")

#Enable modern C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

# Required dependencies 
find_package(MPI REQUIRED)
if(MPI_FOUND)
    include_directories(SYSTEM ${MPI_INCLUDE_PATH})
    set(CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})
endif()

find_package(BLAS REQUIRED)

find_package(OpenMP REQUIRED)

find_package(Boost REQUIRED COMPONENTS serialization mpi)

# Set compile options for Release build -DNDEBUG
set(FLAGS_RELEASE -std=c++23 -O3 -march=native)

# Set compile options for Debug build
set(FLAGS_DEBUG -std=c++23 -g3 -O0 -Wextra -Werror -Wpedantic)

# Add compilation options | -Wconversion
if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_options(${FLAGS_DEBUG})
else()
    add_compile_options(${FLAGS_RELEASE})
endif()

# Add the subdirectory for the source files
add_subdirectory(src)

# Add the executable
add_executable(lngbm main.cpp)

target_link_libraries(lngbm PRIVATE Source_Lib)

# ===== Testing Part =====

#Find Google test package
find_package(GTest REQUIRED)

#To be able to launch tests with ctest
enable_testing()

add_subdirectory(test)

add_executable(tests test/main_Test.cpp) 

add_test(myTest tests)

target_include_directories(tests PUBLIC include/ )

target_link_libraries(tests PRIVATE Test_Lib)
target_link_libraries(tests PRIVATE GTest::GTest GTest::Main)
target_link_libraries(tests PRIVATE Source_Lib)
target_link_libraries(tests PRIVATE BLAS::BLAS)

gtest_discover_tests(tests)

# ===== Benchmark Part =====

add_executable(Benchmarks benchmark/mainBenchmark.cpp)

add_subdirectory(benchmark)

target_link_libraries(Benchmarks PRIVATE Source_Lib)
