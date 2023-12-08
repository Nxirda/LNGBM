project(Test_Lib)

# Get every .cpp file in subdirectories
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Make a lib containing all our .cpp
add_library(Test_Lib  ${SOURCE_FILES})   

# Dont forget to include headers so cpp can compile
include_directories(${CMAKE_SOURCE_DIR}/include)

target_include_directories(Test_Lib 
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/Dataset
    ${CMAKE_SOURCE_DIR}/include/Operators
    ${CMAKE_SOURCE_DIR}/include/PredictionModel
    ${CMAKE_SOURCE_DIR}/include/Tree

)