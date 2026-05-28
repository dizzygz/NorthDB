#include <cassert>
#include <initializer_list>
#include <type_traits>

#include "runtime/states.h"

int main() {
    static_assert(std::is_enum_v<northdb::runtime::InstanceState>);
    static_assert(std::is_enum_v<northdb::runtime::DatabaseState>);
    static_assert(std::is_enum_v<northdb::runtime::AgentState>);
    static_assert(std::is_enum_v<northdb::runtime::SessionState>);
    static_assert(std::is_enum_v<northdb::runtime::ExecutionRole>);

    const auto instance_state = northdb::runtime::InstanceState::kRunning;
    const auto database_state = northdb::runtime::DatabaseState::kActive;
    const auto agent_state = northdb::runtime::AgentState::kDetachedWaiting;
    const auto session_state = northdb::runtime::SessionState::kDetachedWaiting;
    const auto execution_role = northdb::runtime::ExecutionRole::kCoordinator;

    assert(instance_state == northdb::runtime::InstanceState::kRunning);
    assert(database_state == northdb::runtime::DatabaseState::kActive);
    assert(agent_state == northdb::runtime::AgentState::kDetachedWaiting);
    assert(session_state == northdb::runtime::SessionState::kDetachedWaiting);
    assert(execution_role == northdb::runtime::ExecutionRole::kCoordinator);

    static_assert(northdb::runtime::IsValidTransition(
        northdb::runtime::InstanceState::kCreated,
        northdb::runtime::InstanceState::kInitializing));
    static_assert(!northdb::runtime::IsValidTransition(
        northdb::runtime::InstanceState::kCreated,
        northdb::runtime::InstanceState::kRunning));
    static_assert(northdb::runtime::IsValidTransition(
        northdb::runtime::DatabaseState::kRecovering,
        northdb::runtime::DatabaseState::kActive));
    static_assert(!northdb::runtime::IsValidTransition(
        northdb::runtime::DatabaseState::kActive,
        northdb::runtime::DatabaseState::kStopped));
    static_assert(northdb::runtime::IsValidTransition(
        northdb::runtime::AgentState::kRunning,
        northdb::runtime::AgentState::kWaiting));
    static_assert(northdb::runtime::IsValidTransition(
        northdb::runtime::AgentState::kDetachedWaiting,
        northdb::runtime::AgentState::kAssigned));
    static_assert(!northdb::runtime::IsValidTransition(
        northdb::runtime::AgentState::kIdle,
        northdb::runtime::AgentState::kFinished));
    static_assert(northdb::runtime::IsValidTransition(
        northdb::runtime::SessionState::kConnected,
        northdb::runtime::SessionState::kIdle));
    static_assert(northdb::runtime::IsValidTransition(
        northdb::runtime::SessionState::kWaiting,
        northdb::runtime::SessionState::kDetachedWaiting));
    static_assert(!northdb::runtime::IsValidTransition(
        northdb::runtime::SessionState::kIdle,
        northdb::runtime::SessionState::kDetachedWaiting));

    const auto assert_instance_flow = [](std::initializer_list<northdb::runtime::InstanceState> states) {
        auto it = states.begin();
        auto current = *it;
        ++it;
        for (; it != states.end(); ++it) {
            assert(northdb::runtime::IsValidTransition(current, *it));
            current = *it;
        }
    };

    const auto assert_database_flow = [](std::initializer_list<northdb::runtime::DatabaseState> states) {
        auto it = states.begin();
        auto current = *it;
        ++it;
        for (; it != states.end(); ++it) {
            assert(northdb::runtime::IsValidTransition(current, *it));
            current = *it;
        }
    };

    const auto assert_agent_flow = [](std::initializer_list<northdb::runtime::AgentState> states) {
        auto it = states.begin();
        auto current = *it;
        ++it;
        for (; it != states.end(); ++it) {
            assert(northdb::runtime::IsValidTransition(current, *it));
            current = *it;
        }
    };

    const auto assert_session_flow = [](std::initializer_list<northdb::runtime::SessionState> states) {
        auto it = states.begin();
        auto current = *it;
        ++it;
        for (; it != states.end(); ++it) {
            assert(northdb::runtime::IsValidTransition(current, *it));
            current = *it;
        }
    };

    assert_instance_flow({northdb::runtime::InstanceState::kCreated,
                          northdb::runtime::InstanceState::kInitializing,
                          northdb::runtime::InstanceState::kRecovering,
                          northdb::runtime::InstanceState::kRunning,
                          northdb::runtime::InstanceState::kQuiescing,
                          northdb::runtime::InstanceState::kStopping,
                          northdb::runtime::InstanceState::kStopped});

    assert_database_flow({northdb::runtime::DatabaseState::kRegistered,
                          northdb::runtime::DatabaseState::kMounting,
                          northdb::runtime::DatabaseState::kRecovering,
                          northdb::runtime::DatabaseState::kActive,
                          northdb::runtime::DatabaseState::kQuiesced,
                          northdb::runtime::DatabaseState::kStopping,
                          northdb::runtime::DatabaseState::kStopped});

    assert_agent_flow({northdb::runtime::AgentState::kIdle,
                       northdb::runtime::AgentState::kAssigned,
                       northdb::runtime::AgentState::kRunning,
                       northdb::runtime::AgentState::kWaiting,
                       northdb::runtime::AgentState::kDetachedWaiting,
                       northdb::runtime::AgentState::kAssigned,
                       northdb::runtime::AgentState::kRunning,
                       northdb::runtime::AgentState::kFinished,
                       northdb::runtime::AgentState::kIdle});

    assert_session_flow({northdb::runtime::SessionState::kConnected,
                         northdb::runtime::SessionState::kIdle,
                         northdb::runtime::SessionState::kActive,
                         northdb::runtime::SessionState::kWaiting,
                         northdb::runtime::SessionState::kDetachedWaiting,
                         northdb::runtime::SessionState::kActive,
                         northdb::runtime::SessionState::kCancelling,
                         northdb::runtime::SessionState::kIdle,
                         northdb::runtime::SessionState::kClosed});

    for (const auto state : {northdb::runtime::InstanceState::kCreated,
                             northdb::runtime::InstanceState::kInitializing,
                             northdb::runtime::InstanceState::kRecovering,
                             northdb::runtime::InstanceState::kRunning,
                             northdb::runtime::InstanceState::kQuiescing,
                             northdb::runtime::InstanceState::kStopping}) {
        assert(northdb::runtime::IsValidTransition(state, northdb::runtime::InstanceState::kFailed));
    }

    for (const auto state : {northdb::runtime::DatabaseState::kRegistered,
                             northdb::runtime::DatabaseState::kMounting,
                             northdb::runtime::DatabaseState::kRecovering,
                             northdb::runtime::DatabaseState::kActive,
                             northdb::runtime::DatabaseState::kQuiesced,
                             northdb::runtime::DatabaseState::kStopping}) {
        assert(northdb::runtime::IsValidTransition(state, northdb::runtime::DatabaseState::kFailed));
    }

    for (const auto state : {northdb::runtime::AgentState::kIdle,
                             northdb::runtime::AgentState::kAssigned,
                             northdb::runtime::AgentState::kRunning,
                             northdb::runtime::AgentState::kWaiting,
                             northdb::runtime::AgentState::kDetachedWaiting,
                             northdb::runtime::AgentState::kCancelling,
                             northdb::runtime::AgentState::kFinished}) {
        assert(northdb::runtime::IsValidTransition(state, northdb::runtime::AgentState::kFailed));
    }

    for (const auto state : {northdb::runtime::SessionState::kConnected,
                             northdb::runtime::SessionState::kIdle,
                             northdb::runtime::SessionState::kWaiting,
                             northdb::runtime::SessionState::kDetachedWaiting,
                             northdb::runtime::SessionState::kActive,
                             northdb::runtime::SessionState::kCancelling}) {
        assert(northdb::runtime::IsValidTransition(state, northdb::runtime::SessionState::kFailed));
    }

    assert(!northdb::runtime::IsValidTransition(
        northdb::runtime::InstanceState::kCreated,
        northdb::runtime::InstanceState::kRunning));
    assert(!northdb::runtime::IsValidTransition(
        northdb::runtime::InstanceState::kStopped,
        northdb::runtime::InstanceState::kRunning));
    assert(!northdb::runtime::IsValidTransition(
        northdb::runtime::InstanceState::kFailed,
        northdb::runtime::InstanceState::kStopped));

    assert(!northdb::runtime::IsValidTransition(
        northdb::runtime::DatabaseState::kRegistered,
        northdb::runtime::DatabaseState::kActive));
    assert(!northdb::runtime::IsValidTransition(
        northdb::runtime::DatabaseState::kActive,
        northdb::runtime::DatabaseState::kStopped));
    assert(!northdb::runtime::IsValidTransition(
        northdb::runtime::DatabaseState::kFailed,
        northdb::runtime::DatabaseState::kRecovering));

    assert(!northdb::runtime::IsValidTransition(
        northdb::runtime::AgentState::kIdle,
        northdb::runtime::AgentState::kFinished));
    assert(!northdb::runtime::IsValidTransition(
        northdb::runtime::AgentState::kDetachedWaiting,
        northdb::runtime::AgentState::kRunning));
    assert(!northdb::runtime::IsValidTransition(
        northdb::runtime::AgentState::kFailed,
        northdb::runtime::AgentState::kIdle));

    assert(!northdb::runtime::IsValidTransition(
        northdb::runtime::SessionState::kIdle,
        northdb::runtime::SessionState::kDetachedWaiting));
    assert(!northdb::runtime::IsValidTransition(
        northdb::runtime::SessionState::kClosed,
        northdb::runtime::SessionState::kIdle));
    assert(!northdb::runtime::IsValidTransition(
        northdb::runtime::SessionState::kFailed,
        northdb::runtime::SessionState::kConnected));

    for (const auto role : {northdb::runtime::ExecutionRole::kForeground,
                            northdb::runtime::ExecutionRole::kCoordinator,
                            northdb::runtime::ExecutionRole::kSubagent,
                            northdb::runtime::ExecutionRole::kUtility,
                            northdb::runtime::ExecutionRole::kLogFlusher,
                            northdb::runtime::ExecutionRole::kRecovery,
                            northdb::runtime::ExecutionRole::kRemote}) {
        assert(role == role);
    }

    return 0;
}
