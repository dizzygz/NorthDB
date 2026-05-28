#pragma once

namespace northdb::runtime {

enum class InstanceState {
    kCreated,
    kInitializing,
    kRecovering,
    kRunning,
    kQuiescing,
    kStopping,
    kStopped,
    kFailed
};

enum class DatabaseState {
    kRegistered,
    kMounting,
    kRecovering,
    kActive,
    kQuiesced,
    kStopping,
    kStopped,
    kFailed
};

enum class AgentState {
    kIdle,
    kAssigned,
    kRunning,
    kWaiting,
    kDetachedWaiting,
    kCancelling,
    kFinished,
    kFailed
};

enum class SessionState {
    kConnected,
    kIdle,
    kWaiting,
    kDetachedWaiting,
    kActive,
    kCancelling,
    kClosed,
    kFailed
};

enum class ExecutionRole {
    kForeground,
    kCoordinator,
    kSubagent,
    kUtility,
    kLogFlusher,
    kRecovery,
    kRemote
};

constexpr bool IsValidTransition(InstanceState from, InstanceState to) noexcept {
    switch (from) {
    case InstanceState::kCreated:
        return to == InstanceState::kInitializing || to == InstanceState::kFailed;
    case InstanceState::kInitializing:
        return to == InstanceState::kRecovering || to == InstanceState::kFailed;
    case InstanceState::kRecovering:
        return to == InstanceState::kRunning || to == InstanceState::kFailed;
    case InstanceState::kRunning:
        return to == InstanceState::kQuiescing || to == InstanceState::kFailed;
    case InstanceState::kQuiescing:
        return to == InstanceState::kStopping || to == InstanceState::kFailed;
    case InstanceState::kStopping:
        return to == InstanceState::kStopped || to == InstanceState::kFailed;
    case InstanceState::kStopped:
    case InstanceState::kFailed:
        return false;
    }

    return false;
}

constexpr bool IsValidTransition(DatabaseState from, DatabaseState to) noexcept {
    switch (from) {
    case DatabaseState::kRegistered:
        return to == DatabaseState::kMounting || to == DatabaseState::kFailed;
    case DatabaseState::kMounting:
        return to == DatabaseState::kRecovering || to == DatabaseState::kFailed;
    case DatabaseState::kRecovering:
        return to == DatabaseState::kActive || to == DatabaseState::kFailed;
    case DatabaseState::kActive:
        return to == DatabaseState::kQuiesced || to == DatabaseState::kFailed;
    case DatabaseState::kQuiesced:
        return to == DatabaseState::kStopping || to == DatabaseState::kFailed;
    case DatabaseState::kStopping:
        return to == DatabaseState::kStopped || to == DatabaseState::kFailed;
    case DatabaseState::kStopped:
    case DatabaseState::kFailed:
        return false;
    }

    return false;
}

constexpr bool IsValidTransition(AgentState from, AgentState to) noexcept {
    switch (from) {
    case AgentState::kIdle:
        return to == AgentState::kAssigned || to == AgentState::kFailed;
    case AgentState::kAssigned:
        return to == AgentState::kRunning || to == AgentState::kCancelling ||
               to == AgentState::kFailed;
    case AgentState::kRunning:
        return to == AgentState::kWaiting || to == AgentState::kFinished ||
               to == AgentState::kCancelling || to == AgentState::kFailed;
    case AgentState::kWaiting:
        return to == AgentState::kDetachedWaiting || to == AgentState::kRunning ||
               to == AgentState::kCancelling || to == AgentState::kFailed;
    case AgentState::kDetachedWaiting:
        return to == AgentState::kAssigned || to == AgentState::kCancelling ||
               to == AgentState::kFailed;
    case AgentState::kCancelling:
        return to == AgentState::kFinished || to == AgentState::kFailed;
    case AgentState::kFinished:
        return to == AgentState::kIdle || to == AgentState::kFailed;
    case AgentState::kFailed:
        return false;
    }

    return false;
}

constexpr bool IsValidTransition(SessionState from, SessionState to) noexcept {
    switch (from) {
    case SessionState::kConnected:
        return to == SessionState::kIdle || to == SessionState::kClosed ||
               to == SessionState::kFailed;
    case SessionState::kIdle:
        return to == SessionState::kWaiting || to == SessionState::kActive ||
               to == SessionState::kClosed || to == SessionState::kFailed;
    case SessionState::kWaiting:
        return to == SessionState::kDetachedWaiting || to == SessionState::kActive ||
               to == SessionState::kCancelling || to == SessionState::kClosed ||
               to == SessionState::kFailed;
    case SessionState::kDetachedWaiting:
        return to == SessionState::kActive || to == SessionState::kCancelling ||
               to == SessionState::kClosed || to == SessionState::kFailed;
    case SessionState::kActive:
        return to == SessionState::kIdle || to == SessionState::kWaiting ||
               to == SessionState::kCancelling || to == SessionState::kClosed ||
               to == SessionState::kFailed;
    case SessionState::kCancelling:
        return to == SessionState::kIdle || to == SessionState::kClosed ||
               to == SessionState::kFailed;
    case SessionState::kClosed:
    case SessionState::kFailed:
        return false;
    }

    return false;
}

}  // namespace northdb::runtime
