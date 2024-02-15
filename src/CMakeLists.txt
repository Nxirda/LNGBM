project(Source_Lib)

# Make a lib containing all the .cpp
add_library(Source_Lib
            #Tools part
            Tools/Timer.cpp
            Tools/Tools.cpp
            Dataset/DataSet.cpp
            Tree/TreeNode.cpp
            Tree/DecisionTree.cpp
            #Operator part
            Operators/MAE.cpp
            Operators/MAPE.cpp
            Operators/RIV.cpp
            Operators/RMSE.cpp
            #Criteria part
            Criterias/Quartiles.cpp
            Criterias/Percentiles.cpp
            Criterias/RandomValues.cpp
            Criterias/UniqueValues.cpp
            Criterias/Histogram.cpp
            #Model part
            PredictionModel/RandomForest.cpp
            PredictionModel/BaggingModel.cpp
            PredictionModel/TrainingElement.cpp
            #Validation
            Validation/Answers.cpp
            #MPI Wrapper
            MpiWrapper/MPIWrapper.cpp
)   

include_directories(${CMAKE_SOURCE_DIR}/include)

# Dont forget to include headers so cpp can compile
target_include_directories(Source_Lib
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/Dataset/
    ${CMAKE_SOURCE_DIR}/include/Operators/
    ${CMAKE_SOURCE_DIR}/include/Criterias/
    ${CMAKE_SOURCE_DIR}/include/PredictionModel/
    ${CMAKE_SOURCE_DIR}/include/Tree/
    ${CMAKE_SOURCE_DIR}/include/Validation/
    ${CMAKE_SOURCE_DIR}/include/Tools/
    ${CMAKE_SOURCE_DIR}/include/MpiWrapper/

)