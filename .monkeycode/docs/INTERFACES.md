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

### `src/executor`

Planned parallel coordination interfaces:

1. `table_queue.h`
2. `parallel_execution_policy.h`

### `src/common`

Shared concurrency support will later also include wait-event classification for lock, latch, queue, I/O, and log-flush waits.

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
