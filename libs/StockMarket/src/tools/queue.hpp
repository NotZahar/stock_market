#pragma once

#include <mutex>

#include "utility/types.hpp"

namespace sm::tools {
    class ExecutionQueue {
    public:
        virtual void dump() noexcept = 0;
        virtual void restore() noexcept = 0; 

        template <class Task>
        void appendTask(Task&& function) {
            std::lock_guard lock{ _queueMutex };
            asio::post(_taskQueue, std::move(function));
        }

    protected:
        ExecutionQueue() = default;
        
        ~ExecutionQueue() = default;

    private:
        std::mutex _queueMutex; 
        asio::thread_pool _taskQueue{ 1 };
    };
}