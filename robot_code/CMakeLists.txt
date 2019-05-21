cmake_minimum_required(VERSION 3.13)
project(try2)

set(CMAKE_CXX_STANDARD 14)
file(GLOB SOURCES "src/*.cpp")

add_executable(runTests ${SOURCES})
include_directories(include E101)