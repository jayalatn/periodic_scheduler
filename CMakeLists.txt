cmake_minimum_required(VERSION 3.5)
project(hellotest)

set(CMAKE_CXX_STANDARD 14)

set(INSTALL_GTEST OFF)
add_subdirectory(googletest)

add_executable(hellotest main.cpp)
target_link_libraries(hellotest gtest)
