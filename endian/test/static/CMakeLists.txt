cmake_minimum_required (VERSION 2.8)
project (endian)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(swritel writel.cpp)
add_executable(sreadl readl.cpp)

add_test(testswritel swritel)
add_test(testsreadl sreadl)
