cmake_minimum_required(VERSION 3.10)


project(PPN)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_executable(data_loading.exe data_loading.cpp decision_tree.cpp)






