#include <cassert>
#include <type_traits>

#include "runtime/control.h"

int main() {
    static_assert(std::is_enum_v<northdb::runtime::InterruptState>);

    northdb::runtime::InterruptToken interrupt_token;
    assert(interrupt_token.State() == northdb::runtime::InterruptState::kRunning);
    assert(!interrupt_token.IsCancelled());
    assert(!interrupt_token.IsTimedOut());
    assert(!interrupt_token.IsStopRequested());

    interrupt_token.RequestCancel();
    assert(interrupt_token.State() == northdb::runtime::InterruptState::kCancelled);
    assert(interrupt_token.IsCancelled());
    assert(!interrupt_token.IsTimedOut());
    assert(interrupt_token.IsStopRequested());

    northdb::runtime::InterruptToken timeout_token;
    timeout_token.RequestTimeout();
    assert(timeout_token.State() == northdb::runtime::InterruptState::kTimedOut);
    assert(!timeout_token.IsCancelled());
    assert(timeout_token.IsTimedOut());
    assert(timeout_token.IsStopRequested());

    northdb::runtime::InterruptToken override_token;
    override_token.RequestCancel();
    assert(override_token.IsCancelled());
    override_token.RequestTimeout();
    assert(override_token.State() == northdb::runtime::InterruptState::kTimedOut);
    assert(!override_token.IsCancelled());
    assert(override_token.IsTimedOut());
    assert(override_token.IsStopRequested());

    northdb::runtime::RuntimeRegistry registry;
    assert(registry.SessionCount() == 0);
    assert(registry.AgentCount() == 0);
    assert(registry.TransactionCount() == 0);
    assert(registry.QueryCount() == 0);
    assert(registry.UtilityJobCount() == 0);
    assert(registry.EduCount() == 0);

    registry.RegisterSession();
    registry.RegisterAgent();
    registry.RegisterTransaction();
    registry.RegisterQuery();
    registry.RegisterUtilityJob();
    registry.RegisterEdu();

    assert(registry.SessionCount() == 1);
    assert(registry.AgentCount() == 1);
    assert(registry.TransactionCount() == 1);
    assert(registry.QueryCount() == 1);
    assert(registry.UtilityJobCount() == 1);
    assert(registry.EduCount() == 1);

    registry.UnregisterSession();
    registry.UnregisterAgent();
    registry.UnregisterTransaction();
    registry.UnregisterQuery();
    registry.UnregisterUtilityJob();
    registry.UnregisterEdu();

    assert(registry.SessionCount() == 0);
    assert(registry.AgentCount() == 0);
    assert(registry.TransactionCount() == 0);
    assert(registry.QueryCount() == 0);
    assert(registry.UtilityJobCount() == 0);
    assert(registry.EduCount() == 0);

    registry.UnregisterSession();
    registry.UnregisterAgent();
    registry.UnregisterTransaction();
    registry.UnregisterQuery();
    registry.UnregisterUtilityJob();
    registry.UnregisterEdu();

    assert(registry.SessionCount() == 0);
    assert(registry.AgentCount() == 0);
    assert(registry.TransactionCount() == 0);
    assert(registry.QueryCount() == 0);
    assert(registry.UtilityJobCount() == 0);
    assert(registry.EduCount() == 0);

    return 0;
}
