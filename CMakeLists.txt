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
target_link_libraries(main PRIVATE ${MPI_C_LIBRARIES})
set(CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})

# Set Boost
find_package(Boost REQUIRED COMPONENTS serialization mpi)
target_link_libraries(main PRIVATE Boost::serialization Boost::mpi)

# Set OpenMP 
find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

target_link_libraries(main PRIVATE OpenMP::OpenMP_CXX)

find_package(TBB REQUIRED)

# Find BLAS library
find_package(BLAS REQUIRED)

target_include_directories(main PRIVATE ${BLAS_INCLUDE_DIRS})
# Link with BLAS (CBLAS)
target_link_libraries(main PRIVATE BLAS::BLAS)

#Add our Libraries
target_link_libraries(main PRIVATE Source_Lib)

#For parallel execution policies
target_link_libraries(main PRIVATE TBB::tbb)

#Add compilation options    
add_compile_options(-std=c++23 -fopenmp -Wall -Wextra -Werror -Wpedantic -O3 -march=native)

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

target_link_libraries(Tests PRIVATE Test_Lib)
target_link_libraries(Tests PRIVATE GTest::GTest GTest::Main)
target_link_libraries(Tests PRIVATE Source_Lib)
target_link_libraries(Tests PRIVATE TBB::tbb)
target_link_libraries(Tests PRIVATE BLAS::BLAS)


gtest_discover_tests(Tests)

# ===== Benchmark Part =====

add_executable(Benchmarks benchmark/mainBenchmark.cpp)

add_subdirectory(benchmark)

target_link_libraries(Benchmarks PRIVATE Benchmark_Lib)
target_link_libraries(Benchmarks PRIVATE OpenMP::OpenMP_CXX)
target_link_libraries(Benchmarks PRIVATE TBB::tbb)
target_link_libraries(Benchmarks PRIVATE BLAS::BLAS)

