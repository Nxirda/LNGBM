project(Test_Lib)

# Make a lib containing all the .cpp
add_library(Test_Lib 
            DataSet_Test.cpp
            DecisionTree_Test.cpp
            TreeNode_Test.cpp)   
# Dont forget to include headers so cpp can compile
include_directories(${CMAKE_SOURCE_DIR}/include)

target_include_directories(Test_Lib 
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/Dataset
    ${CMAKE_SOURCE_DIR}/include/Operators
    ${CMAKE_SOURCE_DIR}/include/PredictionModel
    ${CMAKE_SOURCE_DIR}/include/Tree

)