#pragma once

namespace component::base{

class QueuedTask{
public:
    virtual ~QueuedTask() = default;

    virtual bool Run() = 0;
};

}