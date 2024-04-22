#pragma once

#include "queue.hpp"
#include "utility/types.hpp"

namespace sm::tools {
    class SendQueue final : public ExecutionQueue {
    public:
        SM_NCM(SendQueue)

        static SendQueue& instance();

        void sendRequestDone([[maybe_unused]] std::string requestId) noexcept {
            appendTask([]() {
                // TODO: send message to client (in future)
            });
        }

        void dump() noexcept override { /* TODO: dump to DB (in future) */ }
        void restore() noexcept override { /* TODO: restore from DB (in future) */ }

    private:
        SendQueue() = default;
        
        ~SendQueue() = default;
    };
}