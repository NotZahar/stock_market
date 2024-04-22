#pragma once

#include "queue.hpp"
#include "utility/types.hpp"

namespace sm::tools {
    class RequestQueue final : public ExecutionQueue {
    public:
        SM_NCM(RequestQueue)

        static RequestQueue& instance();

        void dump() noexcept override { /* TODO: dump to DB (in future) */ }
        void restore() noexcept override { /* TODO: restore from DB (in future) */ }

    private:
        RequestQueue() = default;
        
        ~RequestQueue() = default;
    };
}