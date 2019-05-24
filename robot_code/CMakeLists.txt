cmake_minimum_required(VERSION 3.13)
project(robot_code)

set(CMAKE_CXX_STANDARD 14)
file(GLOB SOURCES "src/*.cpp")

add_executable(runMain exec/runMain.cpp ${SOURCES})
add_executable(runTests exec/runTests.cpp ${SOURCES})
include_directories(include E101/libtest)

find_library(LIB101 e101 E101)
target_link_libraries(runTests LINK_PUBLIC ${LIB101})