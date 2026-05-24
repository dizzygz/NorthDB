# Project Documentation Index

## Overview

This repository currently contains the formal specification package for the Day 1 runtime foundation of a serious C++ relational database engine, plus the Day 1 repository scaffold for upcoming implementation work.

## Documents

1. [ARCHITECTURE.md](./ARCHITECTURE.md) - High-level repository and runtime-foundation architecture
2. [INTERFACES.md](./INTERFACES.md) - Current module boundaries and planned Day 1 interfaces
3. [DEVELOPER_GUIDE.md](./DEVELOPER_GUIDE.md) - Development workflow and Day 1 implementation notes

## Current Repository Structure

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

## Specification Packages

1. `../specs/2026-05-24-day1-runtime-foundation/` - Day 1 runtime foundation requirements, design, and task list
