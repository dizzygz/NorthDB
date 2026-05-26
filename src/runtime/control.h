#pragma once

#include <atomic>
#include <cstddef>

namespace northdb::runtime {

enum class InterruptState {
    kRunning,
    kCancelled,
    kTimedOut
};

class InterruptToken {
public:
    InterruptToken() = default;

    void RequestCancel() noexcept {
        state_.store(InterruptState::kCancelled, std::memory_order_relaxed);
    }

    void RequestTimeout() noexcept {
        state_.store(InterruptState::kTimedOut, std::memory_order_relaxed);
    }

    InterruptState State() const noexcept {
        return state_.load(std::memory_order_relaxed);
    }

    bool IsCancelled() const noexcept {
        return State() == InterruptState::kCancelled;
    }

    bool IsTimedOut() const noexcept {
        return State() == InterruptState::kTimedOut;
    }

    bool IsStopRequested() const noexcept {
        return State() != InterruptState::kRunning;
    }

private:
    std::atomic<InterruptState> state_{InterruptState::kRunning};
};

class RuntimeRegistry {
public:
    void RegisterSession() noexcept {
        ++session_count_;
    }

    void UnregisterSession() noexcept {
        if (session_count_ > 0) {
            --session_count_;
        }
    }

    void RegisterAgent() noexcept {
        ++agent_count_;
    }

    void UnregisterAgent() noexcept {
        if (agent_count_ > 0) {
            --agent_count_;
        }
    }

    void RegisterTransaction() noexcept {
        ++transaction_count_;
    }

    void UnregisterTransaction() noexcept {
        if (transaction_count_ > 0) {
            --transaction_count_;
        }
    }

    void RegisterQuery() noexcept {
        ++query_count_;
    }

    void UnregisterQuery() noexcept {
        if (query_count_ > 0) {
            --query_count_;
        }
    }

    void RegisterUtilityJob() noexcept {
        ++utility_job_count_;
    }

    void UnregisterUtilityJob() noexcept {
        if (utility_job_count_ > 0) {
            --utility_job_count_;
        }
    }

    void RegisterEdu() noexcept {
        ++edu_count_;
    }

    void UnregisterEdu() noexcept {
        if (edu_count_ > 0) {
            --edu_count_;
        }
    }

    std::size_t SessionCount() const noexcept {
        return session_count_;
    }

    std::size_t AgentCount() const noexcept {
        return agent_count_;
    }

    std::size_t TransactionCount() const noexcept {
        return transaction_count_;
    }

    std::size_t QueryCount() const noexcept {
        return query_count_;
    }

    std::size_t UtilityJobCount() const noexcept {
        return utility_job_count_;
    }

    std::size_t EduCount() const noexcept {
        return edu_count_;
    }

private:
    std::size_t session_count_ = 0;
    std::size_t agent_count_ = 0;
    std::size_t transaction_count_ = 0;
    std::size_t query_count_ = 0;
    std::size_t utility_job_count_ = 0;
    std::size_t edu_count_ = 0;
};

}  // namespace northdb::runtime
