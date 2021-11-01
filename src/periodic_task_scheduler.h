#pragma once

#include <queue>
#include <thread>
#include <mutex>

#include "periodic_task.h"

namespace periodic_task_scheduler
{
    ///Periodic task scheduler supports execution of periodic tasks added to the periodic task 
    ///scheduler. The scheduler uses std::thread::hardware_concurrency() to initialize the internal 
    ///thread pool to process the task queue and execute the @Task::execute(). Tasks are kept on a 
    ///@std::priority_queue and priority is the execution time.
    class PeriodicTaskScheduler
    {
    public:
        explicit PeriodicTaskScheduler(size_t thread_pool_size = std::thread::hardware_concurrency());
        ~PeriodicTaskScheduler();
        PeriodicTaskScheduler(const PeriodicTaskScheduler&) = delete;

        ///add @PeriodicTask to the scheduler
        void addTask(std::shared_ptr<PeriodicTask> task);
        
        ///removes @PeriodicTask from task scheduler, the task is not instantly removed from queue
        void removeTask(std::shared_ptr<PeriodicTask> task);

        ///starts the scheduler, if tasks queue has tasks, the execution times will be adjusted to make sure tasks 
        ///are processed @recalculateTaskExecutionTimes()
        void start();

        ///stops the scheduler
        void stop();

    private:
        ///thread function processing the task queue
        void processTaskQueue();

        ///the execution times of the tasks are adjusted to make sure relative times between tasks are respected even when the 
        ///@start() is called after @stop() or cosiderable duration after tasks are addded
        void recalculateTaskExecutionTimes();

        size_t thread_pool_size_;
        std::priority_queue<std::shared_ptr<PeriodicTask>> task_queue_;
        std::mutex task_queue_mutex_;
        std::condition_variable continue_process_queue;
        std::atomic<bool> running_ = {false};
        std::vector<std::thread> thread_pool_;
    };
}