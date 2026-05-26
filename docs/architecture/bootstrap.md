# Developer Bootstrap Notes

## Purpose

This document explains how to configure, build, and test the Day 1 runtime-foundation bootstrap.

## Current Toolchain Assumptions

The current repository expects:

1. a C++20-capable compiler
2. CMake 3.20 or newer
3. a working `ctest` installation

## Repository Layout

The bootstrap implementation currently centers on:

1. `src/common/`
2. `src/memory/`
3. `src/runtime/`
4. `tests/unit/`

Additional reserved modules already exist for later phases under `src/`.

## Configure

Run the bootstrap configure step from the repository root.

```bash
cmake -S . -B /tmp/opencode/northdb-build
```

## Build

Build the current Day 1 test targets with:

```bash
cmake --build /tmp/opencode/northdb-build
```

## Test

Run the full bootstrap test suite with:

```bash
ctest --test-dir /tmp/opencode/northdb-build --output-on-failure
```

## Current Verification Scope

The existing tests currently cover:

1. shared foundational types
2. runtime lifecycle enums and transitions
3. service and interrupt primitives
4. concurrency-domain placeholder interfaces
5. optimizer and memory-governance placeholder interfaces
6. storage-placement placeholder interfaces
7. diagnostic placeholder interfaces
8. memory-context and heap-backed bootstrap allocation
9. instance, database, session, and agent skeleton flows

## Recommended Workflow

For Day 1 development work, use this sequence:

1. read `.monkeycode/specs/2026-05-24-day1-runtime-foundation/`
2. implement one task at a time
3. update or add focused unit tests for that task
4. rebuild with CMake
5. rerun `ctest`
6. refresh the repository-facing docs when interfaces change

## Current Build Targets

The repository currently builds interface-oriented test executables only.

There is no production server binary yet. Day 1 remains focused on:

1. interface shape
2. lifecycle state flow
3. ownership boundaries
4. bootstrap verification

## Related Documents

Use these documents together:

1. `docs/architecture/day1-runtime-model.md`
2. `.monkeycode/docs/ARCHITECTURE.md`
3. `.monkeycode/docs/INTERFACES.md`
4. `.monkeycode/docs/DEVELOPER_GUIDE.md`
5. `.monkeycode/specs/2026-05-24-day1-runtime-foundation/design.md`
