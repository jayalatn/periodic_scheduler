cmake_minimum_required(VERSION 3.5)
project(PeriodicTaskSchedulerTest)

set(CMAKE_CXX_STANDARD 17)

add_executable(
    PeriodicTaskSchedulerTest
    main.cpp
)

target_link_libraries(PeriodicTaskSchedulerTest PRIVATE PeriodicTaskScheduler)