cmake_minimum_required(VERSION 3.10)

# ===== Testing Part =====

#Set the project name
project(PPN)

#Enable modern C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Add the subdirectory for the source files
add_subdirectory(src)

#Add the executable
add_executable(main main.cpp)

# Set MPI 
find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})
target_link_libraries(main PUBLIC ${MPI_C_LIBRARIES})
set(CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})

# Set Boost
find_package(Boost REQUIRED COMPONENTS serialization mpi)
target_link_libraries(main PUBLIC Boost::serialization Boost::mpi)

#Add the Libraries
target_link_libraries(main PUBLIC Source_Lib)

#Add compilation options    
add_compile_options(-std=c++23 -Wall -Wextra -Werror -Wpedantic -O3 -march=native)

# ===== Testing Part =====

#Find Google test package
find_package(GTest REQUIRED)

#To be able to launch tests with ctest
enable_testing()

add_executable(Tests test/DataSet_Test.cpp 
                    test/DecisionTree_Test.cpp 
                    test/TreeNode_Test.cpp 
                    src/
)

add_test(myTest Tests)

add_subdirectory(test)

target_include_directories(Tests PUBLIC include/ )

target_link_libraries(Tests PUBLIC Test_Lib)
target_link_libraries(Tests PUBLIC GTest::GTest GTest::Main)
target_link_libraries(Tests PUBLIC Source_Lib)

gtest_discover_tests(Tests)

# ===== Benchmark Part =====

add_executable(Benchmarks benchmark/mainBenchmark.cpp)

add_subdirectory(benchmark)

target_link_libraries(Benchmarks PUBLIC Benchmark_Lib)



