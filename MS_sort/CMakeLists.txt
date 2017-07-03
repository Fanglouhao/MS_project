cmake_minimum_required(VERSION 3.7)
project(Ms_project)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        for_MS.cpp
        for_MS.h
        main.cpp
        std_lib_facilities.h)

add_executable(Ms_project ${SOURCE_FILES})