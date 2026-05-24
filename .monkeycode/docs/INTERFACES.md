# Interfaces

## Current Repository Interfaces

The repository currently exposes repository-level and module-level boundaries through the Day 1 scaffold.

## Module Boundaries

### `src/common`

Planned interfaces:

1. shared identity aliases
2. shared status types
3. common utility helpers

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
