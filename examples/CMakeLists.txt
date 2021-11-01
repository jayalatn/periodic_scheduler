cmake_minimum_required(VERSION 3.5)
project(PeriodicTaskScheduler)

set (CMAKE_CXX_STANDARD 17)

add_executable(
    PeriodicTaskSchedulerExample
    main.cpp
)

target_link_libraries(PeriodicTaskSchedulerExample PRIVATE PeriodicTaskSchedulerLib)