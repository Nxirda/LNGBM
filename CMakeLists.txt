cmake_minimum_required(VERSION 3.10)

#Set the project name
project(PPN)

#Enable modern C++
set(CMAKE_CXX_STANDARD 23)

#Add compilation options 
add_compile_options(-std=c++23 -Wall -Wextra -Werror -Wpedantic -O3)

#Add the executable
add_executable(Projet_run main.cpp src/DataSet.cpp src/DecisionTree.cpp src/TreeNode.cpp)

#Add the headers
target_include_directories(Projet_run PUBLIC include/ )





