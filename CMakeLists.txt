cmake_minimum_required(VERSION 3.6)
project(metric_view)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")


add_custom_target(metric_view ALL COMMAND make WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

