#pragma once
#include <base/executor/schduled_executor.hpp>
#include <base/queued_task.hpp>
#include <condition_variable>
#include <list>
#include <mutex>
#include <set>

namespace component::base {

class ManualExecutor : public SchduledExecutor {
public:
    ~ManualExecutor() override;

    void Post(Location location, Task task) override;

    void ScheduleAt(Location location, Task task, const TimePoint& point) override;

    size_t Run();

    void Wait();

private:
     void ScheduleDelayTaskWithLock();

private:
    class SchduledTask {
    public:
        SchduledTask(QueuedTask task, const TimePoint& point)
                : task_(std::move(task)),
                  point_(point) {
            static size_t seq = 0;
            ordinal_          = seq++;
        }

        bool operator<(const SchduledTask& rhs) const {
            return point_ == rhs.point_ ? ordinal_ < rhs.ordinal_ : point_ < rhs.point_;
        }

        QueuedTask&& MoveTask() const {
            return std::move(task_);
        }

        bool Expired(TimePoint point) const {
            return point_ < point;
        }

    private:
        TimePoint point_;
        size_t ordinal_;
        mutable QueuedTask task_;
    };

private:
    std::mutex mtx_;
    std::condition_variable cv_;
    std::list<QueuedTask> tasks_;
    std::set<SchduledTask> scheduled_tasks_;
};
} // namespace component::base