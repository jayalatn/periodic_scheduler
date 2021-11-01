cmake_minimum_required(VERSION 3.5)
project(PeriodicTaskScheduler)

set (CMAKE_CXX_STANDARD 17)

add_library(
    PeriodicTaskSchedulerLib
    periodic_task_scheduler.cpp
    periodic_task.cpp
    task_metric.cpp
    tasks.cpp
)