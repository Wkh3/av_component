#pragma once
#include <base/location.hpp>
#include <functional>

namespace component::base{

class Executor{
public:
    using Task = std::function<void()>;

    virtual ~Executor() = default;

    virtual void Post(Location location,Task task) = 0;
};

}