#include "periodic_task.h"
#include <iostream>
namespace periodic_task_scheduler
{
PeriodicTask::PeriodicTask(std::unique_ptr<Task> task):task_(std::move(task))
{
    execute_at_ = std::chrono::steady_clock::now();
}

void PeriodicTask::setExecutionFrq(const std::chrono::microseconds& execution_frq) 
{
    execution_frq_ = execution_frq;
}

std::chrono::microseconds PeriodicTask::executionFrq() const 
{
    return execution_frq_;
}

void PeriodicTask::execute()
{
    std::scoped_lock<std::mutex> lock(execution_busy_);
    task_->execute();
}

Task* PeriodicTask::task() const  
{
    return task_.get();
}

void PeriodicTask::disable() 
{
    disabled_.store(true);
}

void PeriodicTask::enable() 
{
    disabled_.store(false);
}

bool PeriodicTask::enabled() 
{
    return !disabled_;
}

void PeriodicTask::updateExecutionTime() 
{
    execute_at_ += execution_frq_;
}

bool PeriodicTask::executeNow() const 
{
    return (execute_at_ <= std::chrono::steady_clock::now());
}

void PeriodicTask::setExecutionAt(const std::chrono::steady_clock::time_point& at) 
{
    execute_at_ = at;
}

const std::chrono::steady_clock::time_point& PeriodicTask::executeAt() const 
{
    return execute_at_;
}

bool operator<(const std::shared_ptr<PeriodicTask>& t1,  const std::shared_ptr<PeriodicTask>& t2)
{
    return (t1->executeAt() > t2->executeAt());
}

}