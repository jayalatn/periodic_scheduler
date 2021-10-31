#include "periodic_task.h"
#include <iostream>
namespace periodic_task_scheduler
{
PeriodicTask::PeriodicTask(std::unique_ptr<Task> task):task_(std::move(task))
{
    execute_at_ = std::chrono::steady_clock::now();
}

void PeriodicTask::setExecutionFrq(const std::chrono::microseconds& execution_frq) noexcept
{
    execution_frq_ = execution_frq;
}

std::chrono::microseconds PeriodicTask::executionFrq() const noexcept
{
    return execution_frq_;
}

void PeriodicTask::execute()
{
    std::scoped_lock<std::mutex> lock(execution_busy_);
    task_->execute();
}

Task* PeriodicTask::task() const  noexcept
{
    return task_.get();
}

void PeriodicTask::disable() noexcept
{
    disabled_.store(true);
}

void PeriodicTask::enable() noexcept
{
    disabled_.store(false);
}

bool PeriodicTask::enabled() noexcept
{
    return !disabled_;
}

void PeriodicTask::updateExecutionTime() noexcept
{
    execute_at_ += execution_frq_;
}

void PeriodicTask::setExecutionAt(const std::chrono::steady_clock::time_point& at) noexcept
{
    execute_at_ = at;
}

const std::chrono::steady_clock::time_point& PeriodicTask::executeAt() const noexcept
{
    return execute_at_;
}

bool operator<(const std::shared_ptr<PeriodicTask>& t1,  const std::shared_ptr<PeriodicTask>& t2)
{
    return (t1->executeAt() > t2->executeAt());
}

}