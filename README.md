# NorthDB Day 1 Bootstrap

This repository contains the Day 1 runtime-foundation bootstrap for a C++ relational database engine.

## Current Focus

The current codebase implements the first repository scaffold and runtime-facing skeletons for:

1. instance lifecycle
2. database lifecycle
3. session and agent attachment
4. memory-context and heap-backed bootstrap allocation
5. diagnostic record and service boundaries
6. storage placement and optimizer-visible metadata boundaries

## Repository Notes

Repository-level architecture notes live in:

1. `docs/architecture/day1-runtime-model.md`
2. `docs/architecture/bootstrap.md`
3. `.monkeycode/docs/ARCHITECTURE.md`
4. `.monkeycode/docs/INTERFACES.md`
5. `.monkeycode/docs/DEVELOPER_GUIDE.md`

## Build And Test

Current bootstrap verification uses CMake and `ctest`.

Bootstrap steps are documented in `docs/architecture/bootstrap.md`.
Updated at: 2026-05-26 00:00:01
Updated at: 2026-05-27 00:00:01
Updated at: 2026-05-28 00:00:01
