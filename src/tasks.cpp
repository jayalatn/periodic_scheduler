#include "tasks.h"

#include <windows.h>

#include <iostream>

namespace periodic_task_scheduler
{

void SwapFileSizeTask::execute()
{
    auto tm = TaskMetric::clock_type_::now();
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof (statex);
    GlobalMemoryStatusEx (&statex);
    total_swap_file_size_.addMetric(tm, statex.ullTotalPageFile / MEGA_BYTES);
    cur_swap_file_usage_.addMetric(tm, statex.ullAvailPageFile / MEGA_BYTES);
}
    
const TaskMetric& SwapFileSizeTask::totalSwapFileSize() const noexcept
{
    return total_swap_file_size_;
}

const TaskMetric& SwapFileSizeTask::curSwapFileUsage() const noexcept
{
    return cur_swap_file_usage_;
}
}