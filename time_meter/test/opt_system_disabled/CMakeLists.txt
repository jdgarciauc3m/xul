cmake_minimum_required (VERSION 2.8)

#include_directories(${CMAKE_SOURCE_DIR}/include)

add_definitions( -DXUL_TIME_METER_DISABLED)
add_executable(optsystemdis main.cpp)

add_test(optsystemdis1 optsystemdis 10)
set_tests_properties(optsystemdis1 PROPERTIES FAIL_REGULAR_EXPRESSION "milliseconds")
