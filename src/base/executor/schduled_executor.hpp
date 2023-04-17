#pragma once
#include <absl/time/clock.h>
#include <absl/time/time.h>
#include <base/location.hpp>
#include <base/executor/executor.hpp>

namespace component::base {

class SchduledExecutor : public Executor {
public:
    using Duration  = absl::Duration;
    using TimePoint = absl::Time;

    void Schedule(Location location,Task task,const Duration &dur) {
         ScheduleAt(std::move(location),std::move(task),Now() + dur);
    }

    virtual void ScheduleAt(Location location,Task task,const TimePoint &point) = 0;

    virtual TimePoint Now(){
         return absl::Now();
    }
};

} // namespace component::base