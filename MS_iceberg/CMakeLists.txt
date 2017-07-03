cmake_minimum_required(VERSION 3.7)
project(MS_iceberg)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Time_Order.h Time_Order.cpp)
add_executable(MS_iceberg ${SOURCE_FILES})