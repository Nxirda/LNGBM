# Make a lib containing all the .cpp
add_library(Source_Lib
    #Dataset part
    Dataset/DataSet.cpp
    #Operator part
    Operators/EnumOperator.cpp
    Operators/RIV.cpp
    Operators/MAE.cpp
    Operators/MAPE.cpp
    Operators/RMSE.cpp
    #Criteria part
    Criterias/EnumCriteria.cpp
    Criterias/Histogram.cpp
    Criterias/Quartiles.cpp
    Criterias/Percentiles.cpp
    Criterias/RandomValues.cpp
    Criterias/UniqueValues.cpp
    #Model part
    PredictionModel/Trees/TreeNode.cpp
    PredictionModel/Trees/DecisionTree.cpp
    PredictionModel/Trees/TrainingElement.cpp
    PredictionModel/RandomForest.cpp
    PredictionModel/BaggingModel.cpp
    #Validation
    Validation/Answers.cpp
    Validation/Validation.cpp
    Validation/CrossValidation.cpp 
    #MPI Wrapper
    MpiWrapper/MPIWrapper.cpp
    #Tools part
    Tools/Timer.cpp
    Tools/Tools.cpp
    Tools/Serializer.cpp 
)   

include_directories(${CMAKE_SOURCE_DIR}/include)

# Dont forget to include headers so cpp can compile
target_include_directories(Source_Lib
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/Tools/
    ${CMAKE_SOURCE_DIR}/include/Dataset/
    ${CMAKE_SOURCE_DIR}/include/Operators/
    ${CMAKE_SOURCE_DIR}/include/Criterias/
    ${CMAKE_SOURCE_DIR}/include/Validation/
    ${CMAKE_SOURCE_DIR}/include/MpiWrapper/
    ${CMAKE_SOURCE_DIR}/include/PredictionModel/
    ${CMAKE_SOURCE_DIR}/include/PredictionModel/Trees
)

target_link_libraries(Source_Lib 
    PUBLIC 
    MPI::MPI_CXX 
    Boost::mpi 
    Boost::serialization 
    Threads::Threads 
    BLAS::BLAS
    OpenMP::OpenMP_CXX
    TBB::tbb
)