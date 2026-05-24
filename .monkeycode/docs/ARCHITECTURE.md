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

## Module Responsibilities

### `src/common`

This module will hold shared foundational types used across the engine, including identifiers, status types, and common utilities.

### `src/memory`

This module will hold the memory-context abstractions and related allocation infrastructure for instance, database, session, query, and operator-scoped memory ownership.

### `src/runtime`

This module will hold instance lifecycle, database lifecycle, EDU abstractions, agent execution state, service management, interrupt handling, and runtime registries.

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
