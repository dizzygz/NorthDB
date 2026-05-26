#pragma once

#include <memory>

#include "common/status.h"
#include "memory/memory_context.h"
#include "runtime/service.h"
#include "runtime/states.h"

namespace northdb::runtime {

class DBInstance {
public:
    using ServiceHandle = ServiceManager::ServiceHandle;

    explicit DBInstance(std::unique_ptr<northdb::memory::MemoryContext> memory_root)
        : memory_root_(std::move(memory_root)) {}

    northdb::common::Status RegisterService(ServiceHandle service) {
        return service_manager_.Register(std::move(service));
    }

    northdb::common::Status Initialize() {
        return TransitionTo(InstanceState::kInitializing);
    }

    northdb::common::Status Start() {
        auto status = TransitionTo(InstanceState::kRecovering);
        if (!status.ok()) {
            return status;
        }

        status = service_manager_.StartAll();
        if (!status.ok()) {
            state_ = InstanceState::kFailed;
            return status;
        }

        return TransitionTo(InstanceState::kRunning);
    }

    northdb::common::Status Quiesce() {
        return TransitionTo(InstanceState::kQuiescing);
    }

    northdb::common::Status Stop() {
        auto status = TransitionTo(InstanceState::kStopping);
        if (!status.ok()) {
            return status;
        }

        status = service_manager_.StopAll();
        if (!status.ok()) {
            state_ = InstanceState::kFailed;
            return status;
        }

        return TransitionTo(InstanceState::kStopped);
    }

    InstanceState State() const noexcept {
        return state_;
    }

    northdb::memory::MemoryContext* MemoryRoot() const noexcept {
        return memory_root_.get();
    }

    std::size_t RegisteredServiceCount() const noexcept {
        return service_manager_.RegisteredServiceCount();
    }

private:
    northdb::common::Status TransitionTo(InstanceState next_state) {
        if (!IsValidTransition(state_, next_state)) {
            return northdb::common::Status::Error("invalid instance state transition");
        }

        state_ = next_state;
        return northdb::common::Status::Ok();
    }

    std::unique_ptr<northdb::memory::MemoryContext> memory_root_;
    ServiceManager service_manager_;
    InstanceState state_ = InstanceState::kCreated;
};

}  // namespace northdb::runtime
