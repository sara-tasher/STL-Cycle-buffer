cmake_minimum_required(VERSION 3.25)
project(STL_Cycle_buffer)

set(CMAKE_CXX_STANDARD 20)

add_executable(STL_Cycle_buffer main.cpp)
add_subdirectory(lib)

enable_testing()
add_subdirectory(tests)