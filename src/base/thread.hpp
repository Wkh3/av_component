#pragma once
#include <absl/time/clock.h>
#include <absl/time/time.h>
#include <absl/log/log.h>
#include <atomic>
#include <base/location.hpp>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

namespace component::base {

using Task = std::function<void()>;
using Time = absl::Time;
class QueuedTask {
public:
    QueuedTask() = default;

    QueuedTask(Location location, Task task,
               Time time = absl::Now())
            : location(std::move(location)),
              task(std::move(task)),
              time(time) {
        
    }

    QueuedTask(const QueuedTask& rhs)
            : location(rhs.location),
              task(rhs.task),
              time(rhs.time) {
    }

    QueuedTask(QueuedTask&& rhs)
            : location(std::move(rhs.location)),
              task(std::move(rhs.task)),
              time(rhs.time) {
    }

    void operator()() {
        auto start_time = absl::Now();
        task();
        auto duration = absl::Now() - start_time;
        // spdlog::debug("task from {0} time {1} run {2} millseconds", location.ToString(), absl::FormatTime(time), absl::ToInt64Milliseconds(duration));
    }

    Location location;
    Task task;
    Time time;
};

class Thread {
public:
    Thread(std::string name = "");

    ~Thread();

    const std::string& name() const {
        return name_;
    }


    bool IsRunning() const {
        return !exit_.load(std::memory_order_acquire);
    }

    void Close() {
        exit_.store(std::memory_order_release);
    }

    bool PostAsyncTask(Location location, Task task);

    bool PostSyncTask(Location location, Task task, absl::Duration wait_time = absl::Seconds(10));

private:
    void Run();

private:
    std::thread thread_;
    std::string name_;
    std::list<QueuedTask> tasks_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::atomic<bool> exit_{false};
    std::atomic<bool> wait_{false};
};

} // namespace component::base