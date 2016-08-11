cmake_minimum_required(VERSION 2.8.12)
project(bin2las)

find_package(PDAL 1.0.0 REQUIRED CONFIG)
include_directories(${PDAL_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "-std=c++11")
add_executable(bin2las bin2las.cpp)
target_link_libraries(bin2las ${PDAL_LIBRARIES})
