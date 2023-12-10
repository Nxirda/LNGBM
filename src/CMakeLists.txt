project(Source_Lib)

# Get every .cpp file in subdirectories
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Make a lib containing all our .cpp
#add_library(Source_Lib  ${SOURCE_FILES})   
add_library(Source_Lib
            Dataset/DataSet.cpp
            Tree/TreeNode.cpp
            Tree/DecisionTree.cpp
            Operators/MAE.cpp
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