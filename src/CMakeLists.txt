project(Source_Lib)

# Make a lib containing all the .cpp
add_library(Source_Lib
            Dataset/DataSet.cpp
            Tree/TreeNode.cpp
            Tree/DecisionTree.cpp
            Operators/MAE.cpp
            Operators/MAPE.cpp
            Operators/ReductionInVar.cpp
            Operators/RMSE.cpp
            PredictionModel/TrainingElement.cpp
            PredictionModel/RandomForest.cpp)   

include_directories(${CMAKE_SOURCE_DIR}/include)

# Dont forget to include headers so cpp can compile
target_include_directories(Source_Lib
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/Dataset
    ${CMAKE_SOURCE_DIR}/include/Operators
    ${CMAKE_SOURCE_DIR}/include/PredictionModel
    ${CMAKE_SOURCE_DIR}/include/Tree
)