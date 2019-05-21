cmake_minimum_required(VERSION 3.13)
project(robot_code)

set(CMAKE_CXX_STANDARD 14)
file(GLOB SOURCES "src/*.cpp")

add_executable(runTests runTests.cpp ${SOURCES})
include_directories(include E101/libtest)