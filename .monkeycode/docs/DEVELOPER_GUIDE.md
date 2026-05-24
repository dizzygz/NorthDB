# Developer Guide

## Current Stage

This project is in the Day 1 runtime-foundation bootstrap stage.

## Implementation Workflow

1. Read the relevant specification package under `.monkeycode/specs/`
2. Implement one task at a time
3. Write and run tests for the task when the task introduces code requiring verification
4. Mark completed tasks as `[x]` in the task list
5. Refresh project documentation after task completion
6. Stop after each completed task for review

## Current Active Specification

1. `.monkeycode/specs/2026-05-24-day1-runtime-foundation/requirements.md`
2. `.monkeycode/specs/2026-05-24-day1-runtime-foundation/design.md`
3. `.monkeycode/specs/2026-05-24-day1-runtime-foundation/tasklist.md`

## Day 1 Task Scope

Task `1.1` creates the Day 1 repository scaffold:

```text
cmake/
docs/architecture/
src/common/
src/runtime/
src/memory/
src/network/
src/catalog/
src/storage/
src/txn/
src/lock/
src/wal/
src/recovery/
src/executor/
src/parser/
src/optimizer/
src/utility/
src/replication/
tests/unit/
tests/integration/
tools/
```

No runtime classes, types, build rules, or tests are part of task `1.1`.

## Implemented Day 1 Tasks

### Task `1.2`

Task `1.2` adds shared common types:

1. `src/common/ids.h`
2. `src/common/status.h`
3. `tests/unit/common_types_test.cpp`

The task is verified with a direct C++20 compile-and-run unit test.

## Concurrency Design Guidance

The current design baseline distinguishes three concurrency domains:

1. logical locks
2. internal latches
3. parallel exchange queues

Future implementation tasks should preserve this split in APIs, ownership, and diagnostics.

Future implementation tasks should also keep session wait state and agent occupancy as separate observable concepts.

## Optimizer and Memory Guidance

The current design baseline also reserves:

1. a statistics-driven cost-based optimizer architecture
2. a named memory-pool model for shared and transient consumers
3. a future memory-broker boundary for grant and budget management

Future implementation tasks should keep optimizer, statistics, plan-cache, and memory-governance concepts explicit rather than folding them into unrelated modules.

Future implementation tasks should also keep durable session memory, transaction memory, transient agent runtime memory, and future exchange-buffer memory distinct.

## Session and Agent Guidance

The current design baseline also reserves:

1. durable `Session` state independent of a continuously attached worker
2. pooled `Agent` capacity for active work
3. request-scoped coordinator roles carried by attached agents
4. future worker or subagent roles for local parallel or partition-local execution

Future implementation tasks should preserve explicit attach and detach boundaries between session state and execution workers.

## Storage Placement Guidance

The current design baseline also reserves:

1. tablespace abstractions for logical object placement
2. storage-class abstractions for summarized media behavior
3. storage cost profiles that can be consumed by the optimizer

Future implementation tasks should keep raw physical device topology inside the storage engine while exposing summarized cost parameters to planning and explain output.

## Heap and Index Guidance

The current design baseline also reserves:

1. stable tuple identity for future heap storage
2. page-oriented B+ tree secondary indexes
3. tuple revalidation on future index probe paths when row placement and index cleanup can diverge
4. clustering, fragmentation, and cleanup-backlog metadata as future planning and maintenance inputs

Future implementation tasks should preserve a clean separation between lookup correctness and physical cleanup so that background maintenance can evolve independently.

## Day 1 Storage Deferrals

The current Day 1 baseline explicitly defers:

1. MDC-style multidimensional clustered layouts
2. ITC-style insert-time clustered layouts
3. BID-oriented block indexes
4. global partition-spanning indexes
5. full online index defragmentation utilities
