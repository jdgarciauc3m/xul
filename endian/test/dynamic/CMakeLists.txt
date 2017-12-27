cmake_minimum_required (VERSION 2.8)
project (endian)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(writel writel.cpp)
add_executable(readl readl.cpp)

add_test(testreadl readl)
