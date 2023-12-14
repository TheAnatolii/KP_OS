cmake_minimum_required(VERSION 3.10)
project(Dynamic_Libraries)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(yaml-cpp-master)

add_executable(check checker.cpp)
target_link_libraries(check yaml-cpp)