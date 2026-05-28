# Interfaces

## Current Repository Interfaces

The repository currently exposes repository-level and module-level boundaries through the Day 1 scaffold.

## Module Boundaries

### `src/common`

Current interfaces:

1. `ids.h` for shared identity aliases
2. `status.h` for shared status types
3. `wait_events.h` for shared wait-event classification
4. `memory_pools.h` for shared memory-pool classification
5. `diagnostics.h` for shared diagnostic record vocabulary

Defined identity aliases:

1. `InstanceId`
2. `DatabaseId`
3. `SessionId`
4. `TransactionId`
5. `QueryId`
6. `UtilityJobId`
7. `EduId`
8. `TablespaceId`
9. `StorageClassId`
10. `StorageCostProfileId`
11. `TupleId`
12. `PageId`

Defined shared status type:

1. `Status::Ok()`
2. `Status::Error(std::string_view)`
3. `Status::ok()`
4. `Status::message()`

### `src/memory`

Current reserved interfaces:

1. `memory_pool_budget.h`
2. `memory_grant.h`
3. `memory_broker.h`
4. `diagnostic_buffer_memory.h`
5. `memory_context.h`
6. `heap_memory_context.h`
7. future arena and slab allocators

Current memory-broker boundary direction includes:

1. named pool budget configuration
2. query work-memory grant requests
3. spill-aware grant results
4. grant release boundaries

Current diagnostic memory coverage includes:

1. `DiagnosticBufferMemory`
2. `DiagnosticEmergencyMemory`

Current memory-context boundary includes:

1. allocation with explicit alignment
2. per-context deallocation
3. reset for scoped cleanup
4. usage, peak, and configured-limit introspection

Current bootstrap implementation includes:

1. heap-backed allocation using aligned operator new
2. outstanding-allocation tracking for `Reset()` cleanup
3. hard-limit enforcement through allocation failure
4. usage and peak tracking across allocate, deallocate, and reset

Planned memory-class direction also includes:

1. durable session memory
2. durable transaction memory
3. transient agent runtime memory
4. future exchange-buffer memory
5. bounded diagnostic-buffer memory
6. crash-path diagnostic emergency memory

### `src/runtime`

Current interfaces:

1. `db_instance.h`
2. `database_runtime.h`
3. `edu.h`
4. `agent.h`
5. `session.h`
6. `service.h`
7. `control.h`
8. `states.h`
9. `diagnostic_flush_policy.h`
10. `diagnostic_log_service.h`
11. `wait_post.h`
12. `process_local_wait_post.h`

Planned Day 2 runtime interfaces also include:

1. `wait_post.h`
2. `SessionState`
3. `ExecutionRole`

Current reserved diagnostic runtime direction includes:

1. publish boundaries for structured diagnostic headers
2. explicit flush boundaries for member-local writers
3. pluggable flush-decision policy boundaries

Planned runtime direction also includes:

1. future `AgentPool`
2. future `CoordinatorContext`
3. future session-state and execution-role types
4. future `DiagnosticLogService`
5. future structured diagnostic publish and flush policy types
6. event-style `WaitPost` coordination channels for service, session, agent, and diagnostic wakeup flows

Current bootstrap waitpost implementation includes:

1. `ProcessLocalWaitPost`
2. process-local blocking waits via standard C++ mutex and condition-variable primitives
3. immediate wakeup after posted state becomes visible within one process

Current instance boundary includes:

1. root memory ownership
2. instance service registration and lifecycle coordination
3. instance state transitions from create through stop

Current database runtime boundary includes:

1. database memory-root ownership
2. database service registration and lifecycle coordination
3. database state transitions from register through deactivate

Current session and agent boundary includes:

1. durable session identity and memory ownership
2. session-root interrupt token access
3. explicit agent attach and detach boundaries
4. agent execution placeholder state transitions

Current supporting runtime vocabulary includes:

1. `InstanceState`
2. `DatabaseState`
3. `AgentState`
4. `SessionState`
5. `ExecutionRole`
6. `InterruptState`
7. `EDUType`
8. `WaitPost::Mode`

Current runtime-registry direction includes:

1. explicit session, agent, EDU, transaction, query, and utility-job handles
2. ownership lookup for transaction-to-session and query-to-session relationships
3. ownership lookup for utility-job-to-EDU relationships
4. explicit session-to-agent attachment tracking

### `src/lock`

Planned interfaces:

1. `lock_mode.h`
2. `lock_request.h`
3. `lock_manager.h`
4. `lock_escalation_policy.h`

### `src/storage`

Planned concurrency interfaces:

1. `latch.h`
2. `latch_guard.h`

Planned physical-organization interfaces:

1. stable tuple and page identity support
2. future heap-row indirection or forwarding support
3. future B+ tree index support
4. future background cleanup and defragmentation hooks

Current reserved placement and costing interfaces:

1. `tablespace.h`
2. `storage_class.h`
3. `storage_cost_profile.h`

Current storage-placement boundary direction includes:

1. logical tablespace identity
2. summarized storage-class identity
3. optimizer-visible storage-cost profile identity and metrics

### `src/executor`

Planned parallel coordination interfaces:

1. `table_queue.h`
2. `parallel_execution_policy.h`

Planned execution-direction interfaces also include:

1. future execution-fragment boundaries
2. future coordinator-to-worker control vocabulary

### `src/optimizer`

Current reserved interfaces:

1. `statistics_catalog.h`
2. `cardinality_estimator.h`
3. `access_path_enumerator.h`
4. `cost_model.h`
5. `plan_cache.h`
6. `explain_formatter.h`

Current optimizer boundary direction includes:

1. statistics refresh boundaries
2. cardinality estimation boundaries
3. access-path enumeration boundaries
4. costing boundaries
5. plan-cache lookup boundaries
6. explain-formatting boundaries

### `src/catalog`

Current reserved storage-metadata interface:

1. `tablespace_catalog.h`

Current catalog boundary direction includes:

1. tablespace lookup by logical identity
2. tablespace lookup by logical name
3. catalog refresh boundaries for placement metadata

### `src/common`

Shared concurrency support will later also include wait-event classification for lock, latch, queue, I/O, and log-flush waits.

Shared concurrency support will later also include client and remote wait classes.

Shared concurrency support will later also include diagnostic wait or backpressure classification.

Shared memory-governance support will later also include memory-pool classification types.

Current shared memory-pool classification includes:

1. `kInstanceShared`
2. `kDatabaseShared`
3. `kSessionPrivate`
4. `kTransactionPrivate`
5. `kAgentRuntime`
6. `kQueryWork`
7. `kUtilityWork`
8. `kExchangeBuffer`
9. `kPlanCache`
10. `kCatalogCache`
11. `kLockMemory`
12. `kBufferPool`
13. `kDiagnosticBuffer`
14. `kDiagnosticEmergency`

Shared runtime support will later also include session-state and execution-role classification types.

Current shared diagnostic support includes:

1. `DiagnosticSequence`
2. `DiagnosticSeverity`
3. `DiagnosticRecordHeader`

Current shared storage support includes:

1. `TablespaceId`
2. `StorageClassId`
3. `StorageCostProfileId`
4. `TupleId`
5. `PageId`

### Reserved Day 1 module boundaries

These directories are created in the Day 1 scaffold and will receive interfaces in later tasks:

1. `src/network`
2. `src/catalog`
3. `src/storage`
4. `src/txn`
5. `src/lock`
6. `src/wal`
7. `src/recovery`
8. `src/executor`
9. `src/parser`
10. `src/optimizer`
11. `src/utility`
12. `src/replication`

## Source of Truth

The authoritative interface plan for Day 1 is defined in:

1. `.monkeycode/specs/2026-05-24-day1-runtime-foundation/design.md`
2. `.monkeycode/specs/2026-05-24-day1-runtime-foundation/tasklist.md`
