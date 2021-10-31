#pragma once

#include <limits>
#include <chrono>
#include <vector>
#include <string>

namespace periodic_task_scheduler
{
    ///TaskMetric class is a container for a single metric for a given task, the metric agregates are 
    ///computed on @addMetric().  
    class TaskMetric
    {
    public:
        using clock_type_ = std::chrono::steady_clock;

        explicit TaskMetric(const std::string& name);

        void addMetric(const clock_type_::time_point& at, double val);

        const std::vector<std::pair<clock_type_::time_point, double>>& metrics() const;

        double averageMetric() const;

        double minMetric() const;

        double maxMetric() const;

        const std::string& name() const;

    private:
        std::string metric_name_;
        std::vector<std::pair<clock_type_::time_point, double>> metrics_;
        double min_metric_;// = std::numeric_limits<double>::max();
        double max_metric_;// = std::numeric_limits<double>::min();
        double avg_metric_ = 0.;
    };
}