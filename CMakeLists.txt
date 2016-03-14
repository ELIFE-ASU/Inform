cmake_minimum_required(VERSION 3.0)
project(inform CXX C)

add_definitions("-std=c++14 -Wall -Wextra -pedantic")
include_directories(include)
add_subdirectory(src)
add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

enable_testing()
add_subdirectory(test)
