#pragma once

#include <optional>

#include "common/ids.h"
#include "memory/memory_context.h"
#include "runtime/control.h"

namespace northdb::runtime {

class Session {
public:
    Session(northdb::common::SessionId id,
            std::unique_ptr<northdb::memory::MemoryContext> memory_root)
        : id_(id), memory_root_(std::move(memory_root)) {}

    northdb::common::SessionId Id() const noexcept {
        return id_;
    }

    northdb::memory::MemoryContext* MemoryRoot() const noexcept {
        return memory_root_.get();
    }

    InterruptToken& Interrupts() noexcept {
        return interrupt_token_;
    }

    const InterruptToken& Interrupts() const noexcept {
        return interrupt_token_;
    }

    bool HasAttachedAgent() const noexcept {
        return attached_agent_id_.has_value();
    }

    std::optional<northdb::common::EduId> AttachedAgentId() const noexcept {
        return attached_agent_id_;
    }

    void AttachAgent(northdb::common::EduId agent_id) noexcept {
        attached_agent_id_ = agent_id;
    }

    void DetachAgent() noexcept {
        attached_agent_id_.reset();
    }

private:
    northdb::common::SessionId id_ = 0;
    std::unique_ptr<northdb::memory::MemoryContext> memory_root_;
    InterruptToken interrupt_token_;
    std::optional<northdb::common::EduId> attached_agent_id_;
};

}  // namespace northdb::runtime
