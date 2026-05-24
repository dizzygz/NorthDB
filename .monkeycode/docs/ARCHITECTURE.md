# Architecture

## Current State

The repository is in the Day 1 bootstrap stage for a C++ relational database engine. The implemented repository structure now matches the Day 1 directory-layout scaffold required by task `1.1` of the Day 1 runtime-foundation specification.

## Active Repository Layout

```text
cmake/
docs/
  architecture/
src/
  common/
  memory/
  network/
  catalog/
  storage/
  txn/
  lock/
  wal/
  recovery/
  executor/
  parser/
  optimizer/
  utility/
  replication/
  runtime/
tests/
  unit/
  integration/
tools/
```

## Intended Day 1 Runtime Direction

The Day 1 specification defines the following runtime boundaries:

1. `DBInstance`
2. `DatabaseRuntime`
3. `Session`
4. `Agent`
5. `EDU`
6. `MemoryContext`

## Concurrency Domains

The architecture now distinguishes three different concurrency domains:

1. **Logical locks** in `src/lock` for user-visible data protection and transaction isolation
2. **Internal latches** in future `src/storage` interfaces for short critical sections over shared engine memory
3. **Parallel exchange queues** in future `src/executor` interfaces for coordinator and worker communication

This split is based on DB2 lock and latch design lessons:

1. locks protect logical data and may wait for transaction progress
2. latches protect internal engine structures and must stay short-lived
3. parallel execution queues can become their own contention domain and must be treated explicitly by the executor and optimizer

## Module Responsibilities

### `src/common`

This module now holds the first shared foundational types used across the engine:

1. shared identity aliases in `src/common/ids.h`
2. shared status handling in `src/common/status.h`

These interfaces are intended to be reused by runtime, memory, transaction, and storage modules in later tasks.

### `src/memory`

This module will hold the memory-context abstractions and related allocation infrastructure for instance, database, session, query, and operator-scoped memory ownership.

### `src/runtime`

This module will hold instance lifecycle, database lifecycle, EDU abstractions, agent execution state, service management, interrupt handling, and runtime registries.

### `src/lock`

This module is reserved for logical concurrency control only. It will later hold lock modes, lock requests, lock management, escalation policy, and deadlock-aware transaction wait handling.

### `src/storage`

This module is reserved for persistent data structures and internal synchronization primitives such as page and metadata latches. Buffer-pool metadata contention and page-level latch design will be handled here, separately from logical locking.

### `src/executor`

This module will later contain explicit parallel exchange structures such as table queues and parallel execution policy objects. Queue contention is part of execution design and optimizer cost modeling.

### Supporting modules reserved by Day 1 scaffold

The following directories are now present to stabilize repository boundaries for later tasks:

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
13. `tests/unit`
14. `tests/integration`
15. `cmake`
16. `docs/architecture`
17. `tools`

## Notes

The runtime and API names above are derived from `.monkeycode/specs/2026-05-24-day1-runtime-foundation/design.md`.
