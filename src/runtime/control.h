#pragma once

#include <atomic>
#include <cstddef>
#include <optional>
#include <unordered_map>
#include <unordered_set>

#include "common/ids.h"

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
    bool RegisterSession(northdb::common::SessionId session_id) {
        return sessions_.insert(session_id).second;
    }

    bool UnregisterSession(northdb::common::SessionId session_id) {
        session_to_agent_.erase(session_id);

        for (auto it = agent_to_session_.begin(); it != agent_to_session_.end();) {
            if (it->second == session_id) {
                it = agent_to_session_.erase(it);
            } else {
                ++it;
            }
        }

        for (auto it = transactions_.begin(); it != transactions_.end();) {
            if (it->second == session_id) {
                it = transactions_.erase(it);
            } else {
                ++it;
            }
        }

        for (auto it = queries_.begin(); it != queries_.end();) {
            if (it->second == session_id) {
                it = queries_.erase(it);
            } else {
                ++it;
            }
        }

        return sessions_.erase(session_id) > 0;
    }

    bool RegisterAgent(northdb::common::EduId agent_id) {
        return agents_.insert(agent_id).second;
    }

    bool UnregisterAgent(northdb::common::EduId agent_id) {
        auto session_it = agent_to_session_.find(agent_id);
        if (session_it != agent_to_session_.end()) {
            session_to_agent_.erase(session_it->second);
            agent_to_session_.erase(session_it);
        }

        return agents_.erase(agent_id) > 0;
    }

    bool RegisterTransaction(northdb::common::TransactionId transaction_id,
                             northdb::common::SessionId owner_session_id) {
        if (!HasSession(owner_session_id)) {
            return false;
        }

        return transactions_.emplace(transaction_id, owner_session_id).second;
    }

    bool UnregisterTransaction(northdb::common::TransactionId transaction_id) {
        return transactions_.erase(transaction_id) > 0;
    }

    bool RegisterQuery(northdb::common::QueryId query_id,
                       northdb::common::SessionId owner_session_id) {
        if (!HasSession(owner_session_id)) {
            return false;
        }

        return queries_.emplace(query_id, owner_session_id).second;
    }

    bool UnregisterQuery(northdb::common::QueryId query_id) {
        return queries_.erase(query_id) > 0;
    }

    bool RegisterUtilityJob(northdb::common::UtilityJobId utility_job_id,
                            northdb::common::EduId owner_edu_id) {
        if (!HasEdu(owner_edu_id)) {
            return false;
        }

        return utility_jobs_.emplace(utility_job_id, owner_edu_id).second;
    }

    bool UnregisterUtilityJob(northdb::common::UtilityJobId utility_job_id) {
        return utility_jobs_.erase(utility_job_id) > 0;
    }

    bool RegisterEdu(northdb::common::EduId edu_id) {
        return edus_.insert(edu_id).second;
    }

    bool UnregisterEdu(northdb::common::EduId edu_id) {
        for (auto it = utility_jobs_.begin(); it != utility_jobs_.end();) {
            if (it->second == edu_id) {
                it = utility_jobs_.erase(it);
            } else {
                ++it;
            }
        }

        return edus_.erase(edu_id) > 0;
    }

    bool AttachAgentToSession(northdb::common::SessionId session_id,
                              northdb::common::EduId agent_id) {
        if (!HasSession(session_id) || !HasAgent(agent_id)) {
            return false;
        }

        if (session_to_agent_.contains(session_id) || agent_to_session_.contains(agent_id)) {
            return false;
        }

        session_to_agent_.emplace(session_id, agent_id);
        agent_to_session_.emplace(agent_id, session_id);
        return true;
    }

    bool DetachAgentFromSession(northdb::common::SessionId session_id) {
        auto session_it = session_to_agent_.find(session_id);
        if (session_it == session_to_agent_.end()) {
            return false;
        }

        agent_to_session_.erase(session_it->second);
        session_to_agent_.erase(session_it);
        return true;
    }

    bool HasSession(northdb::common::SessionId session_id) const noexcept {
        return sessions_.contains(session_id);
    }

    bool HasAgent(northdb::common::EduId agent_id) const noexcept {
        return agents_.contains(agent_id);
    }

    bool HasTransaction(northdb::common::TransactionId transaction_id) const noexcept {
        return transactions_.contains(transaction_id);
    }

    bool HasQuery(northdb::common::QueryId query_id) const noexcept {
        return queries_.contains(query_id);
    }

    bool HasUtilityJob(northdb::common::UtilityJobId utility_job_id) const noexcept {
        return utility_jobs_.contains(utility_job_id);
    }

    bool HasEdu(northdb::common::EduId edu_id) const noexcept {
        return edus_.contains(edu_id);
    }

    std::optional<northdb::common::EduId> AttachedAgentForSession(
        northdb::common::SessionId session_id) const noexcept {
        const auto it = session_to_agent_.find(session_id);
        if (it == session_to_agent_.end()) {
            return std::nullopt;
        }

        return it->second;
    }

    std::optional<northdb::common::SessionId> AttachedSessionForAgent(
        northdb::common::EduId agent_id) const noexcept {
        const auto it = agent_to_session_.find(agent_id);
        if (it == agent_to_session_.end()) {
            return std::nullopt;
        }

        return it->second;
    }

    std::optional<northdb::common::SessionId> TransactionOwnerSession(
        northdb::common::TransactionId transaction_id) const noexcept {
        const auto it = transactions_.find(transaction_id);
        if (it == transactions_.end()) {
            return std::nullopt;
        }

        return it->second;
    }

    std::optional<northdb::common::SessionId> QueryOwnerSession(
        northdb::common::QueryId query_id) const noexcept {
        const auto it = queries_.find(query_id);
        if (it == queries_.end()) {
            return std::nullopt;
        }

        return it->second;
    }

    std::optional<northdb::common::EduId> UtilityJobOwnerEdu(
        northdb::common::UtilityJobId utility_job_id) const noexcept {
        const auto it = utility_jobs_.find(utility_job_id);
        if (it == utility_jobs_.end()) {
            return std::nullopt;
        }

        return it->second;
    }

    std::size_t SessionCount() const noexcept {
        return sessions_.size();
    }

    std::size_t AgentCount() const noexcept {
        return agents_.size();
    }

    std::size_t TransactionCount() const noexcept {
        return transactions_.size();
    }

    std::size_t QueryCount() const noexcept {
        return queries_.size();
    }

    std::size_t UtilityJobCount() const noexcept {
        return utility_jobs_.size();
    }

    std::size_t EduCount() const noexcept {
        return edus_.size();
    }

private:
    std::unordered_set<northdb::common::SessionId> sessions_;
    std::unordered_set<northdb::common::EduId> agents_;
    std::unordered_set<northdb::common::EduId> edus_;
    std::unordered_map<northdb::common::TransactionId, northdb::common::SessionId> transactions_;
    std::unordered_map<northdb::common::QueryId, northdb::common::SessionId> queries_;
    std::unordered_map<northdb::common::UtilityJobId, northdb::common::EduId> utility_jobs_;
    std::unordered_map<northdb::common::SessionId, northdb::common::EduId> session_to_agent_;
    std::unordered_map<northdb::common::EduId, northdb::common::SessionId> agent_to_session_;
};

}  // namespace northdb::runtime
