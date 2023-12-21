project(Bench_Lib)

# Make a lib 
add_library(Benchmark_Lib
    src/Benchmark.cpp
    src/Timer.cpp

    ${CMAKE_SOURCE_DIR}/src/Dataset/DataSet.cpp
    ${CMAKE_SOURCE_DIR}/src/Tree/TreeNode.cpp
    ${CMAKE_SOURCE_DIR}/src/Tree/DecisionTree.cpp
    ${CMAKE_SOURCE_DIR}/src/Operators/MAE.cpp
    ${CMAKE_SOURCE_DIR}/src/Operators/MAPE.cpp
    ${CMAKE_SOURCE_DIR}/src/Operators/ReductionInVar.cpp
    ${CMAKE_SOURCE_DIR}/src/Operators/RMSE.cpp
    ${CMAKE_SOURCE_DIR}/src/PredictionModel/RandomForest.cpp
    ${CMAKE_SOURCE_DIR}/src/PredictionModel/BaggingModel.cpp
    ${CMAKE_SOURCE_DIR}/src/PredictionModel/TrainingElement.cpp
)   


# Dont forget to include headers so cpp can compile
target_include_directories(Benchmark_Lib
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/Dataset
    ${CMAKE_SOURCE_DIR}/include/Operators
    ${CMAKE_SOURCE_DIR}/include/PredictionModel
    ${CMAKE_SOURCE_DIR}/include/Tree
    ${CMAKE_SOURCE_DIR}/include/Validation
    ${CMAKE_SOURCE_DIR}/Benchmark/include/
)