#pragma once

#include <memory>

#include "common/status.h"
#include "memory/memory_context.h"
#include "runtime/service.h"
#include "runtime/states.h"

namespace northdb::runtime {

class DatabaseRuntime {
public:
    using ServiceHandle = ServiceManager::ServiceHandle;

    explicit DatabaseRuntime(std::unique_ptr<northdb::memory::MemoryContext> memory_root)
        : memory_root_(std::move(memory_root)) {}

    northdb::common::Status RegisterService(ServiceHandle service) {
        return service_manager_.Register(std::move(service));
    }

    northdb::common::Status Activate() {
        return TransitionTo(DatabaseState::kMounting);
    }

    northdb::common::Status Recover() {
        auto status = TransitionTo(DatabaseState::kRecovering);
        if (!status.ok()) {
            return status;
        }

        status = service_manager_.StartAll();
        if (!status.ok()) {
            state_ = DatabaseState::kFailed;
            return status;
        }

        return TransitionTo(DatabaseState::kActive);
    }

    northdb::common::Status Quiesce() {
        return TransitionTo(DatabaseState::kQuiesced);
    }

    northdb::common::Status Deactivate() {
        auto status = TransitionTo(DatabaseState::kStopping);
        if (!status.ok()) {
            return status;
        }

        status = service_manager_.StopAll();
        if (!status.ok()) {
            state_ = DatabaseState::kFailed;
            return status;
        }

        return TransitionTo(DatabaseState::kStopped);
    }

    DatabaseState State() const noexcept {
        return state_;
    }

    northdb::memory::MemoryContext* MemoryRoot() const noexcept {
        return memory_root_.get();
    }

    std::size_t RegisteredServiceCount() const noexcept {
        return service_manager_.RegisteredServiceCount();
    }

private:
    northdb::common::Status TransitionTo(DatabaseState next_state) {
        if (!IsValidTransition(state_, next_state)) {
            return northdb::common::Status::Error("invalid database state transition");
        }

        state_ = next_state;
        return northdb::common::Status::Ok();
    }

    std::unique_ptr<northdb::memory::MemoryContext> memory_root_;
    ServiceManager service_manager_;
    DatabaseState state_ = DatabaseState::kRegistered;
};

}  // namespace northdb::runtime
