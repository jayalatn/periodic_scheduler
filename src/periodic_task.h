#pragma once

#include <chrono>
#include <functional>
#include <mutex>
#include <limits>

#include "tasks.h"

namespace periodic_task_scheduler
{
    ///PeriodicTask defines tasks that can be periodically executed via @PeriodicTaskScheduler. The PeriodicTask
    ///is an extention of any @Task and created by containing a @Task object.
    class PeriodicTask
    {
    public:
        explicit PeriodicTask(std::unique_ptr<Task> task);
        ~PeriodicTask() = default;
        PeriodicTask(const PeriodicTask&) = delete;
    	
        ///set the execution frequency of the task
        void setExecutionFrq(const std::chrono::microseconds& execution_frq) noexcept;

        std::chrono::microseconds executionFrq() const noexcept;

        ///set the execution time at which task shall be executed
        void setExecutionAt(const std::chrono::steady_clock::time_point& at) noexcept;

        const std::chrono::steady_clock::time_point& executeAt() const noexcept;

        ///executes actual task, Task::execute() is called, thereby concrete implementation
        void execute();

        ///disables executing of this task, disabled tasks shall be removed from @PeriodicTaskScheduler
        void disable() noexcept;

        ///enables executing of this task
        void enable() noexcept;

        bool enabled() noexcept;

        ///updates the execution time for the next execution of the task, i.e. execute_at_ += execution_frq_
        void updateExecutionTime() noexcept;

        ///returns underline @Task
        Task* task() const noexcept;

        ///returns if the task shall be executed now
        bool executeNow() const noexcept;

    private:
        bool thread_safe_ = true;
        std::atomic<bool> disabled_{false};
        std::mutex execution_busy_;
        std::chrono::microseconds execution_frq_;
        std::unique_ptr<Task> task_;
        std::chrono::steady_clock::time_point execute_at_;
    };

    ///compare two tasks w.r.t to the execution times @PeriodicTask::executeAt()
    bool operator<(const std::shared_ptr<PeriodicTask>&, const std::shared_ptr<PeriodicTask>&);
}