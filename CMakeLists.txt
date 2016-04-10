cmake_minimum_required(VERSION 3.0)
project(inform CXX C)

add_definitions("-std=c++14 -Wall -Wextra -pedantic")
include_directories(include)
add_subdirectory(src)
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SOURCES})

enable_testing()
add_subdirectory(test)

add_subdirectory(examples)
