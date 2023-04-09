#pragma once
#include <absl/log/log.h>
#include <absl/time/clock.h>
#include <absl/time/time.h>

#include <type_traits>
#include <iostream>
#include <atomic>
#include <base/location.hpp>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <thread>
#include <utility>

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

        std::cout << "task from" << location
                   << " time " << absl::FormatTime(time)
                   << "run elapsed" << absl::ToInt64Milliseconds(duration) << " ms";
    }

    Location location;
    Task task;
    Time time;
};

class Thread final {
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

    template <typename Fn>
    auto PostSyncTask(Location location, Fn &&task, absl::Duration wait_time = absl::Seconds(10)) {
        auto event  = std::make_shared<std::promise<std::invoke_result<Fn>>>();
        auto result = event->get_future();

        PostAsyncTask(location, [wrapper_task = std::forward<Fn>(task), event]() {
            event->set_value(wrapper_task());
        });
        CHECK()
            << "task from" << location << "wait "
            << absl::ToInt64Seconds(wait_time) << " s timeout,"
            << "deadlock may occur!";
    }

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