#include "../src/periodic_task_scheduler.h"

#include <iostream>

void test_frequency()
{
    periodic_task_scheduler::PeriodicTaskScheduler pts;
    auto swap_task = std::make_shared<periodic_task_scheduler::PeriodicTask>(std::make_unique<periodic_task_scheduler::SwapFileSizeTask>());
    swap_task->setExecutionFrq(std::chrono::microseconds(100));
    swap_task->setExecutionAt(std::chrono::steady_clock::now());
    pts.addTask(swap_task);
    for(auto i = 0; i < 100; ++i)
    {
        auto swap_task1 = std::make_shared<periodic_task_scheduler::PeriodicTask>(std::make_unique<periodic_task_scheduler::SwapFileSizeTask>());
        swap_task1->setExecutionFrq(std::chrono::microseconds(100));
        swap_task1->setExecutionAt(swap_task->executeAt() + std::chrono::microseconds(i * 10));
        pts.addTask(swap_task1);
    }

    pts.start();
    
    auto now = std::chrono::steady_clock::now();
    while(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - now) < std::chrono::seconds(5))
    {
        std::this_thread::yield();
    }
    pts.stop();

    auto swap_task_imp = dynamic_cast<periodic_task_scheduler::SwapFileSizeTask*>(swap_task->task());
    auto v1 = swap_task_imp->totalSwapFileSize();
    for(auto i = 1; i < v1.metrics().size(); ++i)
    {
        auto dt = v1.metrics().at(i).first - v1.metrics().at(i - 1).first;
        std::cout << std::chrono::duration_cast<std::chrono::microseconds>(dt).count() << std::endl;
    }

    // swap_task_imp = dynamic_cast<periodic_task_scheduler::SwapFileSizeTask*>(swap_task1->task());
    // v1 = swap_task_imp->totalSwapFileSize();
    // for(auto i = 1; i < v1.metrics().size(); ++i)
    // {
    //     auto dt = v1.metrics().at(i).first - v1.metrics().at(i - 1).first;
    //     //std::cout << std::chrono::duration_cast<std::chrono::microseconds>(dt).count() << std::endl;
    // }
}