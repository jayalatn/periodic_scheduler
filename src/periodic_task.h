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
        void setExecutionFrq(const std::chrono::microseconds& execution_frq);

        std::chrono::microseconds executionFrq() const;

        ///set the execution time at which task shall be executed
        void setExecutionAt(const std::chrono::steady_clock::time_point& at);

        const std::chrono::steady_clock::time_point& executeAt() const;

        ///executes actual task, Task::execute() is called, thereby concrete implementation
        void execute();

        ///disables executing of this task, disabled tasks shall be removed from @PeriodicTaskScheduler
        void disable();

        ///enables executing of this task
        void enable();

        bool enabled();

        ///updates the execution time for the next execution of the task, i.e. execute_at_ += execution_frq_
        void updateExecutionTime();

        ///returns underline @Task
        Task* task() const;

        ///returns if the task shall be executed now
        bool executeNow() const;

    private:
        std::atomic<bool> disabled_{false};
        std::mutex execution_busy_;
        std::chrono::microseconds execution_frq_;
        std::unique_ptr<Task> task_;
        std::chrono::steady_clock::time_point execute_at_;
    };

    ///compare two tasks w.r.t to the execution times @PeriodicTask::executeAt()
    bool operator<(const std::shared_ptr<PeriodicTask>&, const std::shared_ptr<PeriodicTask>&);
}