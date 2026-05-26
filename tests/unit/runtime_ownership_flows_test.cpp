#include <cassert>
#include <memory>

#include "memory/heap_memory_context.h"
#include "runtime/agent.h"
#include "runtime/database_runtime.h"
#include "runtime/db_instance.h"

int main() {
    northdb::runtime::DBInstance instance(std::make_unique<northdb::memory::HeapMemoryContext>(4096));
    assert(instance.State() == northdb::runtime::InstanceState::kCreated);
    assert(instance.Initialize().ok());
    assert(instance.State() == northdb::runtime::InstanceState::kInitializing);
    assert(instance.Start().ok());
    assert(instance.State() == northdb::runtime::InstanceState::kRunning);
    assert(instance.Quiesce().ok());
    assert(instance.State() == northdb::runtime::InstanceState::kQuiescing);
    assert(instance.Stop().ok());
    assert(instance.State() == northdb::runtime::InstanceState::kStopped);

    const auto instance_restart_status = instance.Start();
    assert(!instance_restart_status.ok());
    assert(instance_restart_status.message() == "invalid instance state transition");
    assert(instance.State() == northdb::runtime::InstanceState::kStopped);

    northdb::runtime::DatabaseRuntime database_runtime(std::make_unique<northdb::memory::HeapMemoryContext>(2048));
    assert(database_runtime.State() == northdb::runtime::DatabaseState::kRegistered);
    assert(database_runtime.Activate().ok());
    assert(database_runtime.State() == northdb::runtime::DatabaseState::kMounting);
    assert(database_runtime.Recover().ok());
    assert(database_runtime.State() == northdb::runtime::DatabaseState::kActive);
    assert(database_runtime.Quiesce().ok());
    assert(database_runtime.State() == northdb::runtime::DatabaseState::kQuiesced);
    assert(database_runtime.Deactivate().ok());
    assert(database_runtime.State() == northdb::runtime::DatabaseState::kStopped);

    const auto database_recover_status = database_runtime.Recover();
    assert(!database_recover_status.ok());
    assert(database_recover_status.message() == "invalid database state transition");
    assert(database_runtime.State() == northdb::runtime::DatabaseState::kStopped);

    northdb::runtime::Session session(101, std::make_unique<northdb::memory::HeapMemoryContext>(1024));
    northdb::runtime::Agent agent(202, std::make_unique<northdb::memory::HeapMemoryContext>(1024));
    assert(agent.State() == northdb::runtime::AgentState::kIdle);
    assert(agent.Attach(session).ok());
    assert(agent.State() == northdb::runtime::AgentState::kAssigned);
    assert(agent.ExecutePlaceholder(session).ok());
    assert(agent.State() == northdb::runtime::AgentState::kRunning);
    assert(agent.Finish().ok());
    assert(agent.State() == northdb::runtime::AgentState::kFinished);
    assert(agent.Detach(session).ok());
    assert(agent.State() == northdb::runtime::AgentState::kIdle);

    northdb::runtime::Session invalid_session(303, std::make_unique<northdb::memory::HeapMemoryContext>(512));
    northdb::runtime::Agent invalid_agent(404, std::make_unique<northdb::memory::HeapMemoryContext>(512));
    assert(invalid_agent.Attach(invalid_session).ok());
    assert(invalid_agent.ExecutePlaceholder(invalid_session).ok());
    const auto second_execute_status = invalid_agent.ExecutePlaceholder(invalid_session);
    assert(!second_execute_status.ok());
    assert(second_execute_status.message() == "invalid agent state transition");
    assert(invalid_agent.State() == northdb::runtime::AgentState::kRunning);

    return 0;
}
