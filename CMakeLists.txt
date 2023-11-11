cmake_minimum_required(VERSION 3.10)

#Set the project name
project(PPN)

#Enable modern C++
set(CMAKE_CXX_STANDARD 17)

#Add compilation options 
add_compile_options(-Wall -Wextra -Werror -Wpedantic -O3)

#Add the executable
add_executable(Projet_run main.cpp src/data_loading.cpp src/decision_tree.cpp)

#Add the headers
target_include_directories(Projet_run PUBLIC include/)





