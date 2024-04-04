project(Test_Lib)

# Make a lib containing all the .cpp
add_library(Test_Lib 
            Dataset/DataSet_Test.cpp
            PredictionModel/Trees/TreeNode_Test.cpp
            PredictionModel/Trees/DecisionTree_Test.cpp

)   

target_include_directories(Test_Lib 
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/Dataset
    ${CMAKE_SOURCE_DIR}/include/Criterias
    ${CMAKE_SOURCE_DIR}/include/Operators
    ${CMAKE_SOURCE_DIR}/include/PredictionModel
    ${CMAKE_SOURCE_DIR}/include/PredictionModel/Trees
)   