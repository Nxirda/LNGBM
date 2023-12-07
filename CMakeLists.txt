cmake_minimum_required(VERSION 3.10)

#Set the project name
project(PPN)

#Enable modern C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Add compilation options 
add_compile_options(-std=c++23 -Wall -Wextra -Werror -Wpedantic -O3)

#Add the subdirectory for the source files
add_subdirectory(src)

#Add the executable
add_executable(Projet_run main.cpp)

#Add the headers
target_include_directories(Projet_run PUBLIC include/ )

#Add the Libraries
target_link_libraries(Projet_run PUBLIC Source_Lib)