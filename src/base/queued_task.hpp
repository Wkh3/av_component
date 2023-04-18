#pragma once
#include <absl/log/log.h>
#include <absl/time/clock.h>
#include <absl/time/time.h>

#include <functional>
namespace component::base {

class QueuedTask {
public:
    using Task   = std::function<void()>;
    using Time   = absl::Time;
    QueuedTask() = default;

    QueuedTask(Location location, Task task)
            : location(std::move(location)),
              task(std::move(task)) {
    }

    QueuedTask(const QueuedTask& rhs)
            : location(rhs.location),
              task(rhs.task) {
    }

    QueuedTask(QueuedTask&& rhs)
            : location(std::move(rhs.location)),
              task(std::move(rhs.task)) {
    }

    void operator()() {
        auto start_time = absl::Now();
        task();
        auto duration = absl::Now() - start_time;
        LOG(INFO) << "task from" << location.ToString()
                  << "run elapsed" << absl::ToInt64Milliseconds(duration) << " ms";
    }

private:
    Location location;
    Task task;
};

} // namespace component::base