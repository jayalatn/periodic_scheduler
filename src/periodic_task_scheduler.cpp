#include "periodic_task_scheduler.h"

#include <iostream>

namespace periodic_task_scheduler
{

PeriodicTaskScheduler::PeriodicTaskScheduler(size_t thread_pool_size):thread_pool_size_(thread_pool_size)
{
}

PeriodicTaskScheduler::~PeriodicTaskScheduler()
{
    stop();
}

void PeriodicTaskScheduler::addTask(std::shared_ptr<PeriodicTask> task) 
{
    std::scoped_lock<std::mutex> lock(task_queue_mutex_);
    task->enable();
    task_queue_.emplace(task);
}

void removeTask(std::shared_ptr<PeriodicTask> task)
{
    task->disable();
}

void PeriodicTaskScheduler::start()
{
    if(task_queue_.size())
    {
       recalculateTaskExecutionTimes();
    }
    running_.store(true);
    for(auto i = 0u; i < thread_pool_size_; ++i)
    {
        thread_pool_.emplace_back(std::thread(&PeriodicTaskScheduler::processTaskQueue, this));
    }
}

void PeriodicTaskScheduler::stop()
{
    using namespace std::chrono_literals;
    running_.store(false);
    for(auto& th: thread_pool_)
    {
        if(th.joinable())
        {
            th.join();
        }
    }
    thread_pool_.clear();
}

void PeriodicTaskScheduler::recalculateTaskExecutionTimes()
{
    std::scoped_lock<std::mutex> lock(task_queue_mutex_);
    std::vector<std::shared_ptr<PeriodicTask>> tasks;
    while(!task_queue_.empty())
    {
        tasks.push_back(task_queue_.top());
        task_queue_.pop();
    }
    auto diff = std::chrono::steady_clock::now() - tasks.front()->executeAt();
    for(auto task: tasks)
    {
        task->setExecutionAt(task->executeAt() + diff);
        task_queue_.push(task);
    }
}

void PeriodicTaskScheduler::processTaskQueue()
{
    while(running_.load())
    {
        std::shared_ptr<PeriodicTask> task = nullptr;
        {
            std::unique_lock<std::mutex> lock(task_queue_mutex_);
            if(task_queue_.empty())
            {
                continue_process_queue.wait(lock);
            }

            task = task_queue_.top();
            if(!task->enabled())
            {
                task_queue_.pop();
                continue;
            }

            if(!task->executeNow())
            {
                continue;
            }
        
            task_queue_.pop();
            task->updateExecutionTime();
            task_queue_.push(task);
        }

        continue_process_queue.notify_one();
        if(task)
        {
            task->execute(); 
        }
    }
}
}