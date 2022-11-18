#pragma once
#include "base/task_queue/queued_task.hpp"
#include <memory>

namespace component::base
{
    class TaskQueueBase{
    public:
        virtual ~TaskQueueBase() = default;

        virtual void Delete() = 0;

        virtual void PostTask(std::unique_ptr<QueuedTask> task) = 0; 

        static TaskQueueBase* Current();

        bool IsCurrent() const {return Current() == this;}
    protected:
        class CurrentTaskQueueSetter{
        public:
            explicit CurrentTaskQueueSetter(TaskQueueBase *task_queue);
            ~CurrentTaskQueueSetter();
        private:
            TaskQueueBase * const previous_;
        };
    };

    struct TaskQueueDeleter{
        void operator()(TaskQueueBase *task_queue) const{
            task_queue->Delete();
        }
    };
    
} // namespace component::base


