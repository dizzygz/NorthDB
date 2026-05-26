# Architecture

## Current State

The repository is in the Day 1 bootstrap stage for a C++ relational database engine. The implemented repository structure now matches the Day 1 directory-layout scaffold required by task `1.1` of the Day 1 runtime-foundation specification.

Repository-level architecture notes are now also published in `docs/architecture/day1-runtime-model.md`.

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

The runtime direction now also treats `Session` and `Agent` as different resource classes:

1. `Session` is durable logical client state
2. `Agent` is pooled execution capacity attached only while work is active
3. future coordinator and subagent roles are execution roles carried by agents rather than permanent client-thread ownership

The runtime direction now also reserves a dedicated asynchronous diagnostic logging subsystem.

## Concurrency Domains

The architecture now distinguishes three different concurrency domains:

1. **Logical locks** in `src/lock` for user-visible data protection and transaction isolation
2. **Internal latches** in future `src/storage` interfaces for short critical sections over shared engine memory
3. **Parallel exchange queues** in future `src/executor` interfaces for coordinator and worker communication

This split is based on DB2 lock and latch design lessons:

1. locks protect logical data and may wait for transaction progress
2. latches protect internal engine structures and must stay short-lived
3. parallel execution queues can become their own contention domain and must be treated explicitly by the executor and optimizer

The wait model also needs room for client-idle and future remote or partition-coordination waits so that session state and worker occupancy remain measurable as different resources.

The wait and observability model now also needs room for diagnostic backlog and suppression state so that logging storms remain visible without blocking foreground work.

## Optimization and Memory Governance

The architecture now also treats optimization and memory governance as core design domains.

### Optimizer direction

The future optimizer is intended to be:

1. cost-based
2. statistics-driven
3. sensitive to query shape
4. aware of access-path alternatives such as scans, probes, joins, exchange operators, and materialization boundaries

Planned optimizer subcomponents include:

1. statistics catalog
2. cardinality estimator
3. access-path enumerator
4. cost model
5. plan cache
6. explain formatter

### Memory-governance direction

The future memory model is intended to distinguish:

1. instance-shared memory
2. database-shared memory
3. session-private memory
4. query-work memory
5. utility-work memory

The instance will later need a memory broker or governor, and each database runtime will later own named shared pools such as plan cache, catalog cache, lock memory, and buffer-pool bindings.

The memory model now also reserves separate durable session memory, transaction memory, transient agent runtime memory, and future exchange-buffer memory.

The memory model now also reserves bounded diagnostic buffer memory and crash-path emergency logging memory.

## Storage Placement and I/O Costing

The architecture now also reserves optimizer-visible storage placement concepts.

### Storage direction

The future storage design should distinguish:

1. logical tablespaces for object placement
2. summarized storage classes for optimizer-visible media behavior
3. storage cost profiles that feed scan, probe, and spill costing

This follows a DB2-style lesson that table-space characteristics and I/O cost parameters can affect access-plan selection directly.

## Heap and Index Direction

The architecture now also reserves a baseline physical-organization model for tables and indexes.

### Baseline storage structures

The future engine should converge on:

1. heap-organized tables with stable tuple identity
2. page-oriented B+ tree secondary indexes
3. tuple revalidation on index probe paths when row relocation or stale index entries are possible

This direction supports deferred maintenance without sacrificing correctness.

### Maintenance and clustering direction

The future engine should also model physical organization as observable state:

1. clustering quality should be visible to the optimizer
2. index fragmentation should be visible to the optimizer and utility framework
3. stale-index-entry cleanup backlog should be visible to background maintenance and diagnostics
4. partitioned tables should support local partitioned indexes before global partition-spanning indexes

Specialized layouts inspired by MDC and ITC are reserved for later milestones after the baseline heap and B+ tree path is stable.

## Client-Server and Agent Direction

The runtime now reserves a DB2-inspired client-server processing model.

### Session and agent separation

The future engine should support:

1. many connected sessions with lower durable per-session overhead
2. a bounded pool of foreground agents
3. explicit session-to-agent attachment during active work
4. detach-capable waits and post-response idle behavior

### Coordinator and worker direction

The future engine should support:

1. a request-scoped coordinator role carried by an attached agent
2. future worker or subagent roles for local parallel execution
3. future partition-local agent pools for partitioned database execution
4. fragment-oriented execution APIs that can evolve from single-node to partitioned routing

## Diagnostic Logging Direction

The runtime now reserves a Db2-inspired diagnostic history stream with async write isolation.

### Logging service direction

The future engine should support:

1. a dedicated `DiagnosticLogService`
2. bounded in-memory publication buffers for diagnostic records
3. a single member-local flusher worker per runtime logging domain
4. append-only member-local diagnostic log files kept open during normal service

### Operational resilience direction

The future engine should support:

1. structured record metadata including sequence, time, severity, component, worker identity, and probe site
2. suppression and rate limiting for repeated client, SSL, and network failure storms
3. explicit drop accounting when buffers overflow or records are sampled away
4. crash-path emergency flush behavior using preallocated memory only

This direction keeps troubleshooting value high while protecting critical code paths from file-lock and open-close logging stalls.

## Module Responsibilities

### `src/common`

This module now holds the first shared foundational types used across the engine:

1. shared identity aliases in `src/common/ids.h`
2. shared status handling in `src/common/status.h`

These interfaces are intended to be reused by runtime, memory, transaction, and storage modules in later tasks.

### `src/memory`

This module will hold the memory-context abstractions and related allocation infrastructure for instance, database, session, query, and operator-scoped memory ownership.

It will later also host memory-governance interfaces such as shared memory-pool classes and memory-broker boundaries.

It will later also host bounded diagnostic-buffer and emergency-buffer memory policy boundaries.

### `src/runtime`

This module will hold instance lifecycle, database lifecycle, EDU abstractions, session state, agent execution state, agent-pool boundaries, service management, interrupt handling, and runtime registries.

It will later also host diagnostic-log service coordination, structured publish APIs, flusher lifecycle hooks, and log-health observability.

### `src/lock`

This module is reserved for logical concurrency control only. It will later hold lock modes, lock requests, lock management, escalation policy, and deadlock-aware transaction wait handling.

### `src/storage`

This module is reserved for persistent data structures and internal synchronization primitives such as page and metadata latches. Buffer-pool metadata contention and page-level latch design will be handled here, separately from logical locking.

It will later also host tablespace, storage-class, and storage-cost-profile abstractions used by both the storage engine and the optimizer.

It will later also host stable tuple and page identity concepts, heap-row indirection or forwarding semantics, B+ tree index structures, and maintenance hooks for cleanup and defragmentation.

### `src/executor`

This module will later contain explicit parallel exchange structures such as table queues and parallel execution policy objects. Queue contention is part of execution design and optimizer cost modeling.

It will later also host coordinator-facing execution-fragment boundaries and future worker-fragment control vocabulary.

It may later emit structured diagnostic storm summaries for queue overload and execution-fragment failures through the shared logging service.

### `src/catalog`

This module will later continue to host catalog metadata and may also participate in member-local diagnostic metadata lookup for database, member, and object identity decoration.

### `src/optimizer`

This module is reserved for statistics-driven access-path optimization, including cardinality estimation, cost modeling, access-path enumeration, plan caching, and explain output.

The cost model will later also consume tablespace and storage-cost metadata for I/O-sensitive plan selection.

It will later also consume clustering, fragmentation, and partition-local index metadata for heap and index access planning.

It will later also consume fragment-placement and exchange-cost inputs for coordinator-to-worker planning.

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
