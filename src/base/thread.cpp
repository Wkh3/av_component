#include <base/thread.hpp>
#include <absl/synchronization/notification.h>

#include <memory>
namespace component::base {

Thread::Thread(std::string name)
        : name_(name) {

    thread_ = std::thread([this](){
        Run();
    });
}

Thread::~Thread() {
    Close();

    if (thread_.joinable())
        thread_.join();
}

bool Thread::PostAsyncTask(Location location, Task task) {
    std::unique_lock<std::mutex> lock(mtx_);

    if (exit_.load(std::memory_order_acquire))
        return false;

    tasks_.emplace_back(std::move(location), std::move(task));

    if (wait_.load(std::memory_order_acquire))
        cv_.notify_one();

    return true;
}

bool Thread::PostSyncTask(Location location, Task task,absl::Duration wait_time) {
    auto event = std::make_shared<absl::Notification>();

    PostAsyncTask(location,[wrapper_task = std::move(task),event](){
        wrapper_task();
        event->Notify();
    });

    event->WaitForNotificationWithTimeout(wait_time);
    return true;
        
}

void Thread::Run() {
    do {
        std::unique_lock<std::mutex> lock(mtx_);

        cv_.wait(lock, [this]() {
            if (exit_.load(std::memory_order_acquire))
                return true;

            if (!tasks_.empty())
                return true;

            wait_.store(true, std::memory_order_release);
            return false;
        });

        if (exit_.load(std::memory_order_acquire))
            break;

        auto task(std::move(tasks_.front()));

        tasks_.pop_front();

        lock.unlock();

        task();

    } while (1);
}

} // namespace component::base