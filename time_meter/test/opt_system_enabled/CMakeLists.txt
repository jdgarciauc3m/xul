cmake_minimum_required (VERSION 2.8)

add_definitions( -DXUL_TIME_METER_ENABLED)
add_executable(optsystemen main.cpp)

add_test(optsystemen1 optsystemen 10)
set_tests_properties(optsystemen1 PROPERTIES PASS_REGULAR_EXPRESSION "10 milliseconds")
