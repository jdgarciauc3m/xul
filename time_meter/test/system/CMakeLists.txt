cmake_minimum_required (VERSION 2.8)

#include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(system main.cpp)

add_test(system1 system 10)
