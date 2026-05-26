# Requirements Document

## Introduction

This specification defines the Day 1 runtime foundation for a serious C++ relational database engine. The Day 1 scope establishes the runtime architecture, execution model, lifecycle state machines, and memory-domain model that all later storage, SQL, transaction, recovery, and cluster features will build upon.

## Glossary

- **Instance**: The top-level database engine runtime that owns global services, background workers, and database registries.
- **Database Runtime**: The in-memory runtime container for one database under an instance.
- **EDU**: Engine Dispatchable Unit, the scheduling abstraction for foreground and background database work.
- **Agent**: A foreground execution resource that services one application request at a time while attached to a session.
- **Session**: The long-lived client attachment point that owns session-scoped state independently of a continuously attached execution worker.
- **Memory Context**: A hierarchical memory ownership and accounting scope.
- **Consistency Point**: A durable transaction or checkpoint boundary used by runtime, recovery, and storage subsystems.
- **Interrupt Token**: A runtime control object that carries cancel and timeout signals to long-running work.

## Requirements

### Requirement 1

**User Story:** AS a database kernel engineer, I want a formal instance runtime model, so that all later components share a stable lifecycle contract.

#### Acceptance Criteria

1. THE runtime specification SHALL define an instance state machine with explicit startup, running, quiesce, stop, and failure states.
2. WHEN the instance transitions between lifecycle states, the runtime specification SHALL define the valid transition order.
3. IF an invalid lifecycle transition is requested, the runtime specification SHALL require the implementation to return an explicit failure status.

### Requirement 2

**User Story:** AS a database kernel engineer, I want a database activation model, so that multiple databases can be managed safely under one instance.

#### Acceptance Criteria

1. THE runtime specification SHALL define a database runtime state machine with registered, mounting, recovering, active, quiesced, stopping, stopped, and failed states.
2. WHEN a database is activated, the runtime specification SHALL define activation, recovery, and active-service phases.
3. IF database activation fails, the runtime specification SHALL require the runtime to place the database in a failed state.

### Requirement 3

**User Story:** AS a database kernel engineer, I want an EDU execution model, so that foreground and background work share one scheduling vocabulary.

#### Acceptance Criteria

1. THE runtime specification SHALL define EDU as the execution abstraction for engine work.
2. THE runtime specification SHALL identify foreground EDU categories and background EDU categories.
3. WHEN a runtime service starts a background worker, the runtime specification SHALL require the worker to be represented as an EDU.

### Requirement 4

**User Story:** AS a database kernel engineer, I want an agent-based application servicing model, so that sessions, scheduling, and request execution remain decoupled.

#### Acceptance Criteria

1. THE runtime specification SHALL define the execution path from connection to session to agent to query execution.
2. THE runtime specification SHALL define an agent lifecycle with explicit states.
3. WHEN an application request is executed, the runtime specification SHALL require the agent to own query-scoped execution state until completion.
4. THE runtime specification SHALL define sessions as durable logical client state that can exist without a continuously attached execution agent.
5. THE runtime specification SHALL reserve attach and detach semantics between sessions and agents around active work and wait states.

### Requirement 5

**User Story:** AS a database kernel engineer, I want hierarchical memory domains, so that instance, database, session, query, and operator memory can be owned and measured correctly.

#### Acceptance Criteria

1. THE runtime specification SHALL define a memory context hierarchy rooted at the instance.
2. THE runtime specification SHALL define ownership rules for instance, database, session, query, and operator memory scopes.
3. WHEN allocations occur inside a runtime scope, the runtime specification SHALL require the scope to expose usage, peak usage, and limit information.
4. IF a scope reaches a configured memory limit, the runtime specification SHALL require the implementation to surface an explicit failure or spill decision path.

### Requirement 6

**User Story:** AS a database kernel engineer, I want explicit C++ ownership rules, so that lifecycle safety and memory cleanup are deterministic.

#### Acceptance Criteria

1. THE runtime specification SHALL define ownership conventions for `std::unique_ptr`, non-owning pointers or references, and scoped RAII guards.
2. THE runtime specification SHALL require every long-lived subsystem owner to be represented with explicit ownership.
3. WHEN a scoped runtime resource is acquired, the runtime specification SHALL require release through RAII.

### Requirement 7

**User Story:** AS a database kernel engineer, I want a runtime control and interruption model, so that active work can be cancelled, timed out, or quiesced safely.

#### Acceptance Criteria

1. THE runtime specification SHALL define an interrupt token abstraction.
2. WHEN a long-running operation executes, the runtime specification SHALL require that operation to poll the interrupt token.
3. THE runtime specification SHALL define a runtime registry for active sessions, agents, transactions, queries, utilities, and EDUs.
4. THE runtime specification SHALL distinguish connected sessions, runnable sessions, waiting sessions, active agents, and idle pooled agents in runtime observability.
5. THE runtime specification SHALL reserve structured diagnostic-log metadata sufficient to correlate runtime events by time, worker identity, subsystem, and probe site.

### Requirement 8

**User Story:** AS a database kernel engineer, I want a formal concurrency vocabulary that separates logical locks, internal latches, and parallel exchange coordination, so that future subsystems use the correct synchronization model.

#### Acceptance Criteria

1. THE runtime specification SHALL define logical locks as user-visible concurrency controls tied to transaction semantics.
2. THE runtime specification SHALL define internal latches as short critical-section guards for shared engine memory structures.
3. THE runtime specification SHALL define parallel exchange coordination as a separate execution-domain concern for worker and coordinator communication.
4. THE runtime specification SHALL require wait observability to distinguish lock waits, latch waits, and queue waits.
5. THE runtime specification SHALL reserve additional wait and suspend vocabulary for client-idle waits and future remote or partition-coordination waits.

### Requirement 9

**User Story:** AS a database kernel engineer, I want an asynchronous diagnostic logging model, so that troubleshooting fidelity remains high without blocking critical execution paths during log storms or I/O stalls.

#### Acceptance Criteria

1. THE runtime specification SHALL define diagnostic logging as an asynchronous service rather than a foreground open-lock-write-close path.
2. THE runtime specification SHALL require foreground publishers to enqueue into bounded preallocated buffers without performing diagnostic file open or close operations.
3. THE runtime specification SHALL reserve a dedicated flusher service that writes diagnostic records by time, space, or severity-triggered policy.
4. THE runtime specification SHALL require structured diagnostic records to carry sequence, timestamp, severity, component, worker identity, and probe-site metadata.
5. THE runtime specification SHALL require overflow, suppression, and drop-accounting behavior to be explicit and observable.

### Requirement 10

**User Story:** AS a database kernel engineer, I want a formal optimizer vocabulary that includes statistics, access-path selection, and plan reuse, so that future query planning can be both explainable and cost-driven.

#### Acceptance Criteria

1. THE runtime specification SHALL reserve optimizer architecture concepts for statistics, cardinality estimation, access-path enumeration, cost modeling, and plan caching.
2. THE runtime specification SHALL define optimizer dependence on catalog statistics and query shape.
3. THE runtime specification SHALL require future plan diagnostics to expose enough information to explain access-path choices.

### Requirement 11

**User Story:** AS a database kernel engineer, I want named shared and private memory classes, so that memory governance can distinguish durable engine pools from transient query work areas.

#### Acceptance Criteria

1. THE runtime specification SHALL define separate memory classes for instance-shared, database-shared, session-private, query-work, and utility-work memory.
2. THE runtime specification SHALL reserve named shared consumers such as plan cache, catalog cache, lock memory, and buffer pool memory.
3. THE runtime specification SHALL require shared memory pools to have observable budgets.
4. THE runtime specification SHALL require future query work-memory consumers to execute under explicit grant or spill policies.
5. THE runtime specification SHALL reserve durable session memory separately from transient agent runtime memory.

### Requirement 12

**User Story:** AS a database kernel engineer, I want optimizer-visible storage placement metadata, so that access-path costing can account for I/O behavior and storage layout.

#### Acceptance Criteria

1. THE runtime specification SHALL reserve abstractions for tablespaces, storage classes, and storage cost profiles.
2. THE runtime specification SHALL define storage-cost metadata as an optimizer input rather than a storage-only concern.
3. THE runtime specification SHALL require future plan diagnostics to expose storage-derived costing inputs.
4. THE runtime specification SHALL require future storage metadata changes to participate in plan review or invalidation workflows.

### Requirement 13

**User Story:** AS a database kernel engineer, I want a coordinator and subagent execution vocabulary, so that single-node execution can evolve cleanly into pooled parallel and partitioned execution.

#### Acceptance Criteria

1. THE runtime specification SHALL reserve a coordinator role for request-level execution control.
2. THE runtime specification SHALL reserve subagent or worker execution roles for future parallel or partition-local execution.
3. THE runtime specification SHALL define the coordinator role independently from permanent thread ownership.
4. THE runtime specification SHALL reserve partition-local agent-pool concepts for future partitioned database execution.

### Requirement 14

**User Story:** AS a database kernel engineer, I want stable row and index identity concepts, so that future heap and index structures can support deferred maintenance without breaking lookup correctness.

#### Acceptance Criteria

1. THE runtime specification SHALL reserve stable tuple-identity concepts for heap storage and secondary-index references.
2. THE runtime specification SHALL define future secondary indexes as page-oriented B+ tree structures.
3. THE runtime specification SHALL require future index probes to support tuple revalidation when row indirection, relocation, or stale entries are possible.
4. THE runtime specification SHALL reserve clustering and fragmentation metadata as optimizer-visible inputs.

### Requirement 15

**User Story:** AS a database kernel engineer, I want a Day 1 implementation breakdown, so that the runtime foundation can be built incrementally and tested.

#### Acceptance Criteria

1. THE Day 1 specification SHALL include an implementation task list.
2. THE Day 1 specification SHALL identify code skeletons that are in scope for implementation.
3. THE Day 1 specification SHALL identify tests required for lifecycle, memory, service management, and interruption behavior.
4. THE Day 1 specification SHALL define the Day 1 repository layout for code, tests, tools, and architecture documentation.
5. THE Day 1 specification SHALL reserve interface boundaries for future lock, latch, and exchange-queue components.
6. THE Day 1 specification SHALL reserve interface boundaries for future optimizer and memory-governance components.
7. THE Day 1 specification SHALL reserve interface boundaries for future tablespace and storage-cost components.
8. THE Day 1 specification SHALL identify Day 1 deferrals for specialized storage and indexing features including MDC-style layouts, ITC-style layouts, block indexes, global partition-spanning indexes, and full online defragmentation.
9. THE Day 1 specification SHALL reserve interface and policy boundaries for asynchronous diagnostic logging, including buffering, flushing, suppression, and member-local log partitioning.
