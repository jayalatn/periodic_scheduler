#include "../src/periodic_task_scheduler.h"

#include <iostream>

int main()
{
    using namespace std::chrono_literals;
    using namespace periodic_task_scheduler;
    PeriodicTaskScheduler pts;
    auto swap_task = std::make_shared<PeriodicTask>(std::make_unique<SwapFileSizeTask>());
    swap_task->setExecutionFrq(1000us);
    swap_task->setExecutionAt(std::chrono::steady_clock::now());
    pts.addTask(swap_task);
    for(auto i = 1; i < 99; ++i) //add more tasks
    {
        auto swap_task1 = std::make_shared<PeriodicTask>(std::make_unique<SwapFileSizeTask>());
        swap_task1->setExecutionFrq(1000us);
        auto start_time = swap_task->executeAt() + std::chrono::microseconds(10 * i);
        swap_task1->setExecutionAt(start_time);
        pts.addTask(swap_task1);
    }

    //start the scheduler
    {
        pts.start();
        
        auto now = std::chrono::steady_clock::now();
        while(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - now) < 5s)
        {
            std::this_thread::sleep_for(1ms);
        }
        pts.stop();    
        
        //use metrics
        auto task = dynamic_cast<SwapFileSizeTask*>(swap_task->task());
        if(task)
        {
            auto metrics = task->totalSwapFileSize();
            std::cout << "Average total swap file size " << metrics.averageMetric() << std::endl;
            std::cout << "Min total swap file size " << metrics.minMetric() << std::endl;
            std::cout << "Max total swap file size " << metrics.maxMetric() << std::endl;

            metrics = task->curSwapFileUsage();
            std::cout << "Average cur swap file usage " << metrics.averageMetric() << std::endl;
            std::cout << "Min cur swap file usage " << metrics.minMetric() << std::endl;
            std::cout << "Max cur swap file usage " << metrics.maxMetric() << std::endl;
        }
    }

    //restart the scheduler
    pts.start();
    {
        pts.start();
        
        auto now = std::chrono::steady_clock::now();
        while(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - now) < 5s)
        {
            std::this_thread::sleep_for(1ms);
        }
        pts.stop();    
        
        //use metrics
        auto task = dynamic_cast<SwapFileSizeTask*>(swap_task->task());
        if(task)
        {
            auto metrics = task->totalSwapFileSize();
            std::cout << "Average total swap file size " << metrics.averageMetric() << std::endl;
            std::cout << "Min total swap file size " << metrics.minMetric() << std::endl;
            std::cout << "Max total swap file size " << metrics.maxMetric() << std::endl;

            metrics = task->curSwapFileUsage();
            std::cout << "Average cur swap file usage " << metrics.averageMetric() << std::endl;
            std::cout << "Min cur swap file usage " << metrics.minMetric() << std::endl;
            std::cout << "Max cur swap file usage " << metrics.maxMetric() << std::endl;
        }
    }   
    
    return 0;
}