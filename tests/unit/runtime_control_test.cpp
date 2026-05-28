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
    assert(!registry.HasSession(11));
    assert(!registry.HasAgent(22));
    assert(!registry.HasTransaction(33));
    assert(!registry.HasQuery(44));
    assert(!registry.HasUtilityJob(55));
    assert(!registry.HasEdu(66));

    assert(registry.RegisterSession(11));
    assert(registry.RegisterAgent(22));
    assert(registry.RegisterEdu(66));
    assert(registry.RegisterTransaction(33, 11));
    assert(registry.RegisterQuery(44, 11));
    assert(registry.RegisterUtilityJob(55, 66));

    assert(registry.SessionCount() == 1);
    assert(registry.AgentCount() == 1);
    assert(registry.TransactionCount() == 1);
    assert(registry.QueryCount() == 1);
    assert(registry.UtilityJobCount() == 1);
    assert(registry.EduCount() == 1);
    assert(registry.HasSession(11));
    assert(registry.HasAgent(22));
    assert(registry.HasTransaction(33));
    assert(registry.HasQuery(44));
    assert(registry.HasUtilityJob(55));
    assert(registry.HasEdu(66));
    assert(registry.TransactionOwnerSession(33).has_value());
    assert(registry.TransactionOwnerSession(33).value() == 11);
    assert(registry.QueryOwnerSession(44).has_value());
    assert(registry.QueryOwnerSession(44).value() == 11);
    assert(registry.UtilityJobOwnerEdu(55).has_value());
    assert(registry.UtilityJobOwnerEdu(55).value() == 66);

    assert(registry.AttachAgentToSession(11, 22));
    assert(registry.AttachedAgentForSession(11).has_value());
    assert(registry.AttachedAgentForSession(11).value() == 22);
    assert(registry.AttachedSessionForAgent(22).has_value());
    assert(registry.AttachedSessionForAgent(22).value() == 11);

    assert(!registry.RegisterSession(11));
    assert(!registry.RegisterAgent(22));
    assert(!registry.RegisterEdu(66));
    assert(!registry.RegisterTransaction(33, 11));
    assert(!registry.RegisterQuery(44, 11));
    assert(!registry.RegisterUtilityJob(55, 66));
    assert(!registry.AttachAgentToSession(11, 22));
    assert(!registry.RegisterTransaction(77, 999));
    assert(!registry.RegisterQuery(88, 999));
    assert(!registry.RegisterUtilityJob(99, 999));

    assert(registry.DetachAgentFromSession(11));
    assert(!registry.AttachedAgentForSession(11).has_value());
    assert(!registry.AttachedSessionForAgent(22).has_value());
    assert(!registry.DetachAgentFromSession(11));

    assert(registry.UnregisterTransaction(33));
    assert(registry.UnregisterQuery(44));
    assert(registry.UnregisterUtilityJob(55));
    assert(registry.UnregisterAgent(22));
    assert(registry.UnregisterEdu(66));
    assert(registry.UnregisterSession(11));

    assert(registry.SessionCount() == 0);
    assert(registry.AgentCount() == 0);
    assert(registry.TransactionCount() == 0);
    assert(registry.QueryCount() == 0);
    assert(registry.UtilityJobCount() == 0);
    assert(registry.EduCount() == 0);
    assert(!registry.HasSession(11));
    assert(!registry.HasAgent(22));
    assert(!registry.HasTransaction(33));
    assert(!registry.HasQuery(44));
    assert(!registry.HasUtilityJob(55));
    assert(!registry.HasEdu(66));

    assert(!registry.UnregisterTransaction(33));
    assert(!registry.UnregisterQuery(44));
    assert(!registry.UnregisterUtilityJob(55));
    assert(!registry.UnregisterAgent(22));
    assert(!registry.UnregisterEdu(66));
    assert(!registry.UnregisterSession(11));

    assert(registry.SessionCount() == 0);
    assert(registry.AgentCount() == 0);
    assert(registry.TransactionCount() == 0);
    assert(registry.QueryCount() == 0);
    assert(registry.UtilityJobCount() == 0);
    assert(registry.EduCount() == 0);

    assert(registry.RegisterSession(101));
    assert(registry.RegisterAgent(202));
    assert(registry.RegisterEdu(303));
    assert(registry.RegisterTransaction(404, 101));
    assert(registry.RegisterQuery(505, 101));
    assert(registry.RegisterUtilityJob(606, 303));
    assert(registry.AttachAgentToSession(101, 202));

    assert(registry.UnregisterSession(101));
    assert(registry.SessionCount() == 0);
    assert(registry.TransactionCount() == 0);
    assert(registry.QueryCount() == 0);
    assert(!registry.AttachedAgentForSession(101).has_value());
    assert(!registry.AttachedSessionForAgent(202).has_value());

    assert(registry.UnregisterEdu(303));
    assert(registry.EduCount() == 0);
    assert(registry.UtilityJobCount() == 0);

    return 0;
}
