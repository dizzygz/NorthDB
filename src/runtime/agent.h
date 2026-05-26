#pragma once

#include <optional>

#include "common/ids.h"
#include "common/status.h"
#include "memory/memory_context.h"
#include "runtime/session.h"
#include "runtime/states.h"

namespace northdb::runtime {

class Agent {
public:
    Agent(northdb::common::EduId id,
          std::unique_ptr<northdb::memory::MemoryContext> memory_root)
        : id_(id), memory_root_(std::move(memory_root)) {}

    northdb::common::EduId Id() const noexcept {
        return id_;
    }

    AgentState State() const noexcept {
        return state_;
    }

    northdb::memory::MemoryContext* MemoryRoot() const noexcept {
        return memory_root_.get();
    }

    std::optional<northdb::common::SessionId> AttachedSessionId() const noexcept {
        return attached_session_id_;
    }

    northdb::common::Status Attach(Session& session) {
        if (!IsValidTransition(state_, AgentState::kAssigned)) {
            return northdb::common::Status::Error("invalid agent state transition");
        }

        state_ = AgentState::kAssigned;
        attached_session_id_ = session.Id();
        session.AttachAgent(id_);
        return northdb::common::Status::Ok();
    }

    northdb::common::Status ExecutePlaceholder(Session& session) {
        if (!attached_session_id_.has_value() || attached_session_id_.value() != session.Id()) {
            return northdb::common::Status::Error("agent is not attached to the session");
        }

        if (!IsValidTransition(state_, AgentState::kRunning)) {
            return northdb::common::Status::Error("invalid agent state transition");
        }

        state_ = AgentState::kRunning;
        return northdb::common::Status::Ok();
    }

    northdb::common::Status Finish() {
        if (!IsValidTransition(state_, AgentState::kFinished)) {
            return northdb::common::Status::Error("invalid agent state transition");
        }

        state_ = AgentState::kFinished;
        return northdb::common::Status::Ok();
    }

    northdb::common::Status Detach(Session& session) {
        if (!attached_session_id_.has_value() || attached_session_id_.value() != session.Id()) {
            return northdb::common::Status::Error("agent is not attached to the session");
        }

        if (!IsValidTransition(state_, AgentState::kIdle)) {
            return northdb::common::Status::Error("invalid agent state transition");
        }

        state_ = AgentState::kIdle;
        attached_session_id_.reset();
        session.DetachAgent();
        return northdb::common::Status::Ok();
    }

private:
    northdb::common::EduId id_ = 0;
    std::unique_ptr<northdb::memory::MemoryContext> memory_root_;
    AgentState state_ = AgentState::kIdle;
    std::optional<northdb::common::SessionId> attached_session_id_;
};

}  // namespace northdb::runtime
