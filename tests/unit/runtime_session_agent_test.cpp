#include <cassert>
#include <memory>

#include "memory/heap_memory_context.h"
#include "runtime/agent.h"

int main() {
    auto session_memory = std::make_unique<northdb::memory::HeapMemoryContext>(2048);
    auto* session_memory_ptr = session_memory.get();
    northdb::runtime::Session session(11, std::move(session_memory));
    assert(session.Id() == 11);
    assert(session.MemoryRoot() == session_memory_ptr);
    assert(!session.HasAttachedAgent());
    assert(!session.AttachedAgentId().has_value());
    assert(session.Interrupts().State() == northdb::runtime::InterruptState::kRunning);

    auto agent_memory = std::make_unique<northdb::memory::HeapMemoryContext>(1024);
    auto* agent_memory_ptr = agent_memory.get();
    northdb::runtime::Agent agent(22, std::move(agent_memory));
    assert(agent.Id() == 22);
    assert(agent.MemoryRoot() == agent_memory_ptr);
    assert(agent.State() == northdb::runtime::AgentState::kIdle);
    assert(!agent.AttachedSessionId().has_value());

    assert(agent.Attach(session).ok());
    assert(agent.State() == northdb::runtime::AgentState::kAssigned);
    assert(agent.AttachedSessionId().has_value());
    assert(agent.AttachedSessionId().value() == 11);
    assert(session.HasAttachedAgent());
    assert(session.AttachedAgentId().has_value());
    assert(session.AttachedAgentId().value() == 22);

    assert(agent.ExecutePlaceholder(session).ok());
    assert(agent.State() == northdb::runtime::AgentState::kRunning);

    assert(agent.Finish().ok());
    assert(agent.State() == northdb::runtime::AgentState::kFinished);

    assert(agent.Detach(session).ok());
    assert(agent.State() == northdb::runtime::AgentState::kIdle);
    assert(!agent.AttachedSessionId().has_value());
    assert(!session.HasAttachedAgent());
    assert(!session.AttachedAgentId().has_value());

    northdb::runtime::Session other_session(33, std::make_unique<northdb::memory::HeapMemoryContext>(512));
    northdb::runtime::Agent unattached_agent(44, std::make_unique<northdb::memory::HeapMemoryContext>(512));
    const auto execute_without_attach = unattached_agent.ExecutePlaceholder(other_session);
    assert(!execute_without_attach.ok());
    assert(execute_without_attach.message() == "agent is not attached to the session");

    northdb::runtime::Agent wrong_session_agent(55, std::make_unique<northdb::memory::HeapMemoryContext>(512));
    northdb::runtime::Session first_session(66, std::make_unique<northdb::memory::HeapMemoryContext>(512));
    northdb::runtime::Session second_session(77, std::make_unique<northdb::memory::HeapMemoryContext>(512));
    assert(wrong_session_agent.Attach(first_session).ok());
    const auto detach_wrong_session = wrong_session_agent.Detach(second_session);
    assert(!detach_wrong_session.ok());
    assert(detach_wrong_session.message() == "agent is not attached to the session");

    first_session.Interrupts().RequestCancel();
    assert(first_session.Interrupts().IsCancelled());

    return 0;
}
