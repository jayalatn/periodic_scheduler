#include "task_metric.h"

namespace periodic_task_scheduler
{
TaskMetric::TaskMetric(const std::string& name):metric_name_(name)
{
}

void TaskMetric::addMetric(const clock_type_::time_point& at, double val)
{
    metrics_.push_back({at, val});
    avg_metric_ = (avg_metric_ + val) / metrics_.size();
    if(val < min_metric_)
    {
        min_metric_ = val;
    }
    if(val > max_metric_)
    {
        max_metric_ = val;
    }
}

const std::vector<std::pair<TaskMetric::clock_type_::time_point, double>>& TaskMetric::metrics() const
{
    return metrics_;
}

double TaskMetric::averageMetric() const
{
    return avg_metric_;
}

double TaskMetric::minMetric() const
{
    return min_metric_;
}

double TaskMetric::maxMetric() const
{
    return max_metric_;
}

const std::string& TaskMetric::name() const
{
    return metric_name_;
}

}