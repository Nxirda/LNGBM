cmake_minimum_required(VERSION 3.10)

#Set the project name
project(PPN)

#Enable modern C++
set(CMAKE_CXX_STANDARD 23)

#Add compilation options 
add_compile_options(-std=c++23 -Wall -Wextra -Werror -Wpedantic -O3)

add_subdirectory(src)

#Add the executable
add_executable(Projet_run main.cpp)# src/DataSet.cpp src/DecisionTree.cpp src/TreeNode.cpp src/ReductionInVar.cpp)

#Add the headers
target_include_directories(Projet_run PUBLIC include/ )

target_link_libraries(Projet_run PUBLIC Source_Lib)





