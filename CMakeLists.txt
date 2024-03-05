cmake_minimum_required(VERSION 3.10)

# ===== Main Part =====

#Set the project name
project(PPN)

#Enable modern C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Add the executable
add_executable(main main.cpp)

#Required dependencies 
find_package(MPI REQUIRED)
find_package(BLAS REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)
find_package(Boost REQUIRED COMPONENTS serialization mpi)

#
include_directories(SYSTEM ${MPI_INCLUDE_PATH})
set(CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})

#Add compilation options 
add_compile_options(-std=c++23 -Wall -Wextra -Werror -Wpedantic -O3 -march=native)

#Add the subdirectory for the source files
add_subdirectory(src)

target_link_libraries(main PRIVATE Source_Lib)

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

#add_executable(Benchmarks benchmark/mainBenchmark.cpp)

#add_subdirectory(benchmark)

#target_link_libraries(Benchmarks PRIVATE Benchmark_Lib)
#target_link_libraries(Benchmarks PRIVATE OpenMP::OpenMP_CXX)
#target_link_libraries(Benchmarks PRIVATE TBB::tbb)
#target_link_libraries(Benchmarks PRIVATE BLAS::BLAS)
#target_link_libraries(Benchmarks PRIVATE Boost::serialization Boost::mpi)


