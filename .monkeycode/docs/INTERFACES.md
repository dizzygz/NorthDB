# Interfaces

## Current Repository Interfaces

The repository currently exposes repository-level and module-level boundaries through the Day 1 scaffold.

## Module Boundaries

### `src/common`

Current interfaces:

1. `ids.h` for shared identity aliases
2. `status.h` for shared status types

Defined identity aliases:

1. `InstanceId`
2. `DatabaseId`
3. `SessionId`
4. `TransactionId`
5. `QueryId`
6. `EduId`

Defined shared status type:

1. `Status::Ok()`
2. `Status::Error(std::string_view)`
3. `Status::ok()`
4. `Status::message()`

### `src/memory`

Planned interfaces:

1. `MemoryContext`
2. bootstrap heap-backed memory context
3. future arena and slab allocators
4. future memory-broker and pool-class interfaces

Planned memory-class direction also includes:

1. durable session memory
2. durable transaction memory
3. transient agent runtime memory
4. future exchange-buffer memory

### `src/runtime`

Planned interfaces:

1. `DBInstance`
2. `DatabaseRuntime`
3. `EDU`
4. `Agent`
5. `Session`
6. `ServiceManager`
7. `RuntimeRegistry`
8. `InterruptToken`

Planned runtime direction also includes:

1. future `AgentPool`
2. future `CoordinatorContext`
3. future session-state and execution-role types

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

### `src/executor`

Planned parallel coordination interfaces:

1. `table_queue.h`
2. `parallel_execution_policy.h`

Planned execution-direction interfaces also include:

1. future execution-fragment boundaries
2. future coordinator-to-worker control vocabulary

### `src/optimizer`

Planned interfaces:

1. `statistics_catalog.h`
2. `cardinality_estimator.h`
3. `access_path_enumerator.h`
4. `cost_model.h`
5. `plan_cache.h`
6. `explain_formatter.h`

### `src/storage`

Planned placement and costing interfaces:

1. `tablespace.h`
2. `storage_class.h`
3. `storage_cost_profile.h`

### `src/catalog`

Planned storage-metadata interface:

1. `tablespace_catalog.h`

### `src/common`

Shared concurrency support will later also include wait-event classification for lock, latch, queue, I/O, and log-flush waits.

Shared concurrency support will later also include client and remote wait classes.

Shared memory-governance support will later also include memory-pool classification types.

Shared runtime support will later also include session-state and execution-role classification types.

Shared storage support will later also include tablespace and storage-cost identity aliases.

Shared storage support will later also include `TupleId` and `PageId` aliases.

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
