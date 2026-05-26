# Day 1 Runtime Model

## Purpose

This document summarizes the repository-level runtime architecture that the Day 1 bootstrap code is implementing.

## Runtime Ownership Model

The current Day 1 runtime model centers on six core boundaries:

1. `DBInstance`
2. `DatabaseRuntime`
3. `Session`
4. `Agent`
5. `EDU`
6. `MemoryContext`

These boundaries divide durable ownership from leased execution capacity:

1. `DBInstance` owns instance-scoped runtime coordination and the instance memory root.
2. `DatabaseRuntime` owns database-scoped lifecycle and the database memory root.
3. `Session` owns durable client context and remains alive without a permanently attached worker.
4. `Agent` represents pooled execution capacity that attaches to a session only while work is active.
5. `EDU` remains the common abstraction for executable engine work.
6. `MemoryContext` remains the common abstraction for scoped allocation, accounting, and cleanup.

## Lifecycle Model

The bootstrap code now exposes explicit state machines for:

1. `InstanceState`
2. `DatabaseState`
3. `AgentState`

Current skeletons use these flows:

1. `DBInstance`: `kCreated -> kInitializing -> kRecovering -> kRunning -> kQuiescing -> kStopping -> kStopped`
2. `DatabaseRuntime`: `kRegistered -> kMounting -> kRecovering -> kActive -> kQuiesced -> kStopping -> kStopped`
3. `Agent`: `kIdle -> kAssigned -> kRunning -> kFinished -> kIdle`

Failures during service startup or shutdown currently transition instance or database runtime state to `kFailed`.

## Session and Agent Separation

The repository now implements the first explicit attach and detach boundaries between sessions and agents.

Current semantics:

1. `Session` holds durable identity, session memory, and a root `InterruptToken`.
2. `Agent` holds execution identity, agent runtime memory, and attach state.
3. `Session` records the currently attached agent identity when present.
4. `Agent` records the currently attached session identity when present.
5. `Agent::ExecutePlaceholder()` marks the minimal active-work transition without introducing executor internals yet.

This matches the Day 1 goal of separating durable logical client state from leased foreground execution capacity.

## Memory Foundation

The repository now includes:

1. `MemoryContext` as the abstract allocation and accounting boundary
2. `HeapMemoryContext` as the bootstrap implementation
3. memory-pool classification types in `src/common`
4. memory-broker, grant, and pool-budget interfaces in `src/memory`

The current heap-backed implementation already enforces:

1. explicit alignment-aware allocation
2. outstanding allocation tracking
3. peak and used-byte accounting
4. hard-limit failure on over-allocation
5. scoped cleanup through `Reset()`

## Diagnostic Logging Direction

The repository now reserves a structured diagnostic path through:

1. shared diagnostic record metadata in `src/common/diagnostics.h`
2. runtime diagnostic service and flush policy boundaries in `src/runtime`
3. diagnostic buffer and emergency memory coverage in `src/memory`

This preserves the Day 1 direction that foreground publication remains separate from file flush work.

## Concurrency and Placement Boundaries

The repository now encodes three separate concurrency domains:

1. logical locks in `src/lock`
2. internal latches in `src/storage`
3. parallel exchange queues in `src/executor`

The repository also now exposes optimizer-visible placement vocabulary through:

1. storage identity aliases in `src/common`
2. tablespace, storage-class, and storage-cost profile structures in `src/storage`
3. `TablespaceCatalog` lookup boundaries in `src/catalog`

## Current Source Map

Current Day 1 runtime-related implementation lives in:

1. `src/common/`
2. `src/memory/`
3. `src/runtime/`
4. `src/storage/`
5. `src/catalog/`
6. `src/lock/`
7. `src/executor/`
8. `src/optimizer/`

Current bootstrap verification lives in `tests/unit/`.

## Relationship to Specifications

The source of truth for this runtime model remains:

1. `.monkeycode/specs/2026-05-24-day1-runtime-foundation/requirements.md`
2. `.monkeycode/specs/2026-05-24-day1-runtime-foundation/design.md`
3. `.monkeycode/specs/2026-05-24-day1-runtime-foundation/tasklist.md`
