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
