# Day 1 Runtime Foundation

Feature Name: day1-runtime-foundation
Updated: 2026-05-24

## Description

This design defines the first formal implementation milestone for a serious C++ relational database engine. The goal of Day 1 is to establish the engine runtime foundation before SQL, storage, recovery, or query processing internals are implemented.

The Day 1 runtime foundation introduces:

1. A top-level instance runtime model
2. A database activation model
3. An EDU-based execution framework
4. An agent-based application servicing model
5. Hierarchical memory domains
6. Explicit C++ ownership rules
7. Runtime registry and interruption control
8. A minimal implementation and test plan

## Architecture

```mermaid
graph TD
    A["Client Connection"] --> B["Session Manager"]
    B --> C["Session"]
    C --> D["Agent"]
    D --> E["Query Execution"]
    F["DBInstance"] --> G["EDU Manager"]
    F --> H["Service Manager"]
    F --> I["Database Registry"]
    F --> J["Global Memory Manager"]
    I --> K["Database Runtime"]
    K --> L["Transaction Manager"]
    K --> M["Lock Manager"]
    K --> N["Catalog Runtime"]
    K --> O["Buffer Pool Binding"]
    H --> P["Background EDUs"]
    G --> D
    G --> P
```

### Runtime hierarchy

The Day 1 runtime model is organized around four major scope boundaries:

1. `DBInstance`
2. `DatabaseRuntime`
3. `Session` or `ApplicationContext`
4. `QueryExecution`

The system follows a DB2-inspired runtime model where the instance is the root operational boundary, database runtimes are activatable children, and both foreground and background engine work are represented through EDUs.

## Repository Layout

The Day 1 repository layout must match the Day 1 runtime bootstrap plan and expose the major code, test, tool, and documentation boundaries from the start.

```text
/northdb
  /cmake
  /docs
    /architecture
  /src
    /common
    /runtime
    /memory
    /network
    /catalog
    /storage
    /txn
    /lock
    /wal
    /recovery
    /executor
    /parser
    /optimizer
    /utility
    /replication
  /tests
    /unit
    /integration
  /tools
```

The implementation scope for Day 1 task `1.1` is the creation of this repository scaffold only. The task does not yet require class skeletons, build rules, or executable tests.

## Components and Interfaces

### 1. `DBInstance`

`DBInstance` is the top-level owner of the database engine runtime.

Responsibilities:

1. Own global configuration
2. Own the global memory root
3. Own runtime registries and managers
4. Start and stop core services
5. Coordinate database activation and shutdown
6. Enter quiesce state for controlled shutdown or maintenance

Core API:

```cpp
class DBInstance {
public:
    Status Initialize();
    Status Start();
    Status Quiesce();
    Status Stop();
    InstanceState State() const noexcept;
};
```

### 2. `DatabaseRuntime`

`DatabaseRuntime` is the in-memory runtime container for one database.

Responsibilities:

1. Activate database-local services
2. Coordinate mount and recovery stages
3. Expose database-level transaction and lock systems
4. Own the database memory root
5. Support quiesce and deactivate flows

Core API:

```cpp
class DatabaseRuntime {
public:
    Status Activate();
    Status Recover();
    Status Quiesce();
    Status Deactivate();
    DatabaseState State() const noexcept;
};
```

### 3. `EDU`

`EDU` is the common execution abstraction for all engine work.

Foreground EDU examples:

1. `AgentEDU`
2. `UtilityJobEDU`
3. `ParallelQueryWorkerEDU` in later phases

Background EDU examples:

1. `LogWriterEDU`
2. `PageCleanerEDU`
3. `CheckpointEDU`
4. `PrefetchEDU`
5. `DeadlockDetectorEDU`
6. `RecoveryMasterEDU`
7. `RecoveryWorkerEDU`

Core interface:

```cpp
class EDU {
public:
    virtual ~EDU() = default;
    virtual EduId Id() const noexcept = 0;
    virtual EDUType Type() const noexcept = 0;
    virtual std::string_view Name() const noexcept = 0;
    virtual void Run() = 0;
    virtual void RequestStop() = 0;
};
```

### 4. `Agent`

`Agent` is the foreground runtime unit that services an application request.

Responsibilities:

1. Bind to a session for request execution
2. Own query-scoped execution state while active
3. Attach to transaction state
4. Enforce cancellation and timeout checks
5. Report execution metrics and status

Execution path:

```text
Connection -> Session -> Agent -> Transaction Context -> QueryExecution
```

### 5. `Session`

`Session` is the long-lived client runtime context.

Responsibilities:

1. Authentication state
2. Session variables
3. Prepared statement cache later
4. Session memory context
5. Current transaction binding
6. Interrupt token root

### 6. `MemoryContext`

`MemoryContext` is the fundamental allocation, accounting, and cleanup abstraction.

Core interface:

```cpp
class MemoryContext {
public:
    virtual ~MemoryContext() = default;
    virtual void* Allocate(size_t bytes,
                           size_t alignment = alignof(std::max_align_t)) = 0;
    virtual void Deallocate(void* ptr, size_t bytes) = 0;
    virtual void Reset() = 0;
    virtual size_t UsedBytes() const noexcept = 0;
    virtual size_t PeakBytes() const noexcept = 0;
    virtual size_t LimitBytes() const noexcept = 0;
};
```

### 7. `ServiceManager`

`ServiceManager` is responsible for lifecycle coordination of always-on engine services.

Responsibilities:

1. Register services
2. Start services in a defined order
3. Stop services in a defined order
4. Propagate failures into runtime state

### 8. `RuntimeRegistry` and `InterruptToken`

`RuntimeRegistry` tracks active engine objects.

Tracked entity classes:

1. Session
2. Agent
3. Transaction
4. Query
5. Utility job
6. EDU

`InterruptToken` carries runtime cancel and timeout requests.

All long-running operations must be designed to poll this token.

## Data Models

### Runtime identity types

```cpp
using InstanceId = uint64_t;
using DatabaseId = uint64_t;
using SessionId = uint64_t;
using TransactionId = uint64_t;
using QueryId = uint64_t;
using EduId = uint64_t;
```

### Instance state machine

```cpp
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
```

Valid transition flow:

```text
kCreated -> kInitializing -> kRecovering -> kRunning
kRunning -> kQuiescing -> kStopping -> kStopped
Any operational state -> kFailed
```

### Database state machine

```cpp
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
```

Valid transition flow:

```text
kRegistered -> kMounting -> kRecovering -> kActive
kActive -> kQuiesced -> kStopping -> kStopped
Any operational state -> kFailed
```

### Agent state machine

```cpp
enum class AgentState {
    kIdle,
    kAssigned,
    kRunning,
    kWaiting,
    kCancelling,
    kFinished,
    kFailed
};
```

### Memory context hierarchy

```text
InstanceMemoryContext
  |- ServiceMemoryContext
  |- AgentPoolMemoryContext
  |- BufferPoolMemoryContext
  |- WALMemoryContext
  |- RecoveryMemoryContext
  |- CatalogMemoryContext
  |- DatabaseMemoryContext(db1)
  |    |- TxnSystemMemoryContext
  |    |- LockSystemMemoryContext
  |    |- SessionMemoryContext(s1)
  |    |    |- QueryMemoryContext(q1)
  |    |    |    |- OperatorMemoryContext(scan)
  |    |    |    |- OperatorMemoryContext(hash_join)
  |    |    |- QueryMemoryContext(q2)
  |    |- SessionMemoryContext(s2)
  |- DatabaseMemoryContext(db2)
```

## Correctness Properties

### Ownership and lifecycle invariants

1. Every runtime object must have one clear lifecycle owner.
2. Every memory scope must have one clear parent scope except the instance root.
3. Every active query must execute under exactly one agent.
4. Every active agent must be associated with at most one active request.
5. Every database runtime must belong to exactly one instance.
6. Every long-running operation must have an interrupt token.

### C++ ownership rules

1. Exclusive subsystem ownership uses `std::unique_ptr`.
2. Non-owning object references use raw pointers or references.
3. Scoped resource release must use RAII guards.
4. Query-local containers should be compatible with `std::pmr` or equivalent scoped allocation strategies.

### Runtime control invariants

1. Invalid lifecycle transitions return explicit failure status.
2. Quiesce state stops admission of new work before shutdown continues.
3. Session teardown must release session memory and detach active request state.
4. Query teardown must release query and operator memory contexts.

## Error Handling

### Startup and lifecycle errors

1. If instance initialization fails, the instance enters `kFailed`.
2. If database activation fails, the database enters `kFailed`.
3. If service startup fails, the owning runtime returns an error and records the failure state.

### Memory errors

1. If a memory context exceeds a configured hard limit, the allocator returns an explicit failure.
2. Query-scoped memory-limit breaches should later integrate with spill policies for sort and hash operators.

### Interruption errors

1. If an interrupt token is set to cancelled, active work returns a cancellation status.
2. If an interrupt token is set to timeout, active work returns a timeout status.

## Test Strategy

### Unit tests

Required Day 1 unit-test targets:

1. Instance lifecycle transitions
2. Database lifecycle transitions
3. Agent lifecycle transitions
4. Memory context usage and peak tracking
5. Interrupt token behavior
6. Service manager start and stop ordering

### Example test names

1. `DBInstance_TransitionsToRunning`
2. `DBInstance_RejectsInvalidStopBeforeStart`
3. `DatabaseRuntime_ActivateRecoverDeactivateFlow`
4. `MemoryContext_TracksUsageAndPeak`
5. `InterruptToken_RequestsCancel`
6. `Agent_TransitionsIdleAssignedRunningFinished`
7. `ServiceManager_StartsServicesInOrder`

### Scope exclusions for Day 1

The following are intentionally out of scope for Day 1 implementation:

1. SQL parsing
2. Storage engine internals
3. WAL record format
4. Query optimizer internals
5. Lock table implementation
6. Executor operator implementation

## References

[^1]: (Website) - [DB2 process model](https://www.ibm.com/docs/en/db2/11.5.x?topic=architecture-db2-process-model)
[^2]: (Website) - [DB2 process overview](https://www.ibm.com/docs/en/db2/11.5.x?topic=architecture-db2-process-overview)
[^3]: (Website) - [DB2 database agents](https://www.ibm.com/docs/en/db2/11.5.x?topic=architecture-database-agents)
[^4]: (Website) - [DB2 application processes, concurrency, and recovery](https://www.ibm.com/docs/en/db2/11.5.x?topic=design-application-processes-concurrency-recovery)
