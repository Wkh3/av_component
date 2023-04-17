#include <base/executor/manual_executor.hpp>

namespace component::base {

void ManualExecutor::Post(Location location, Task task) {
    std::unique_lock lock(mtx_);
    tasks_.emplace_back(std::move(location), std::move(task));
    cv_.notify_one();
}

void ManualExecutor::ScheduleAt(Location location, Task task, const TimePoint& point) {
    std::unique_lock lock(mtx_);
    scheduled_tasks_.emplace(QueuedTask(std::move(location), std::move(task)), point);
    cv_.notify_one();
}

size_t ManualExecutor::Run() {
    std::unique_lock lock(mtx_);

    ScheduleDelayTaskWithLock();

    if (tasks_.empty())
        return 0;

    std::list<QueuedTask> run_tasks(std::move(tasks_));

    lock.unlock();

    for (auto&& task : run_tasks) {
        task();
    }

    return run_tasks.size();
}

void ManualExecutor::ScheduleDelayTaskWithLock() {
    while (!scheduled_tasks_.empty()) {
        auto&& task = *scheduled_tasks_.begin();

        if (!task.Expired(Now()))
            break;

        tasks_.emplace_back(task.MoveTask());

        scheduled_tasks_.erase(scheduled_tasks_.begin());
    }
}

void ManualExecutor::Wait() {
    std::unique_lock lock(mtx_);

    do {
        ScheduleDelayTaskWithLock();

        if (!tasks_.empty())
            return;
        
        cv_.wait_for(lock,std::chrono::milliseconds(10));
    } while (1);
}

} // namespace component::base