cmake_minimum_required (VERSION 2.8)
project (endian)

include_directories("${CMAKE_SOURCE_DIR}/include")
list(APPEND CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wno-deprecated -Werror -pedantic-errors")

enable_testing()

add_subdirectory(test)
#add_subdirectory(doxy)
#add_subdirectory(unit_test)

FILE(GLOB headers "${CMAKE_SOURCE_DIR}/include/*.h")
INSTALL(FILES ${headers} DESTINATION include/xul/endian)
