#pragma once

#include "task_metric.h"

namespace periodic_task_scheduler
{
///Base class for implementing tasks
class Task
{
public:
    ///the derived tasks shall implement task execution in @execute() function
    virtual void execute() = 0;
};

class SwapFileSizeTask: public Task
{
public:
    void execute() override;
    
    const TaskMetric& totalSwapFileSize() const;

    const TaskMetric& curSwapFileUsage() const;

private:
    TaskMetric total_swap_file_size_{"total_swap_file_size"};
    TaskMetric cur_swap_file_usage_{"cur_swap_file_usage"};

    static constexpr double MEGA_BYTES = 1024*1024;
};
}