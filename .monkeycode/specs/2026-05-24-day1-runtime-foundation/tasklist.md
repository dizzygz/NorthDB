# Task List

## Day 1 Runtime Foundation

- [ ] 1. Create repository runtime skeleton
  - [x] 1.1 Create the Day 1 repository layout: `cmake`, `docs/architecture`, `src/{common,runtime,memory,network,catalog,storage,txn,lock,wal,recovery,executor,parser,optimizer,utility,replication}`, `tests/{unit,integration}`, and `tools`
  - [x] 1.2 Add core identity aliases and shared status types
  - [x] 1.3 Add initial CMake targets for runtime and memory modules

- [ ] 2. Implement lifecycle state definitions
  - [x] 2.1 Add `InstanceState`, `DatabaseState`, and `AgentState` enums
  - [x] 2.2 Implement lifecycle transition validation helpers
  - [x] 2.3 Add unit tests for valid and invalid lifecycle transitions

- [ ] 3. Implement runtime control interfaces
  - [x] 3.1 Add the `EDU` base interface and `EDUType` definitions
  - [x] 3.2 Add the `Service` base interface and `ServiceManager` skeleton
  - [x] 3.3 Add the `InterruptToken` and `RuntimeRegistry` skeletons
  - [x] 3.4 Add unit tests for service ordering and interruption behavior

- [ ] 3A. Define concurrency-domain interfaces
  - [x] 3A.1 Add shared wait-event classification types in `src/common`
  - [x] 3A.2 Add reserved logical lock interface headers in `src/lock`
  - [x] 3A.3 Add reserved latch interface headers in `src/storage`
  - [x] 3A.4 Add reserved parallel exchange interface headers in `src/executor`
  - [x] 3A.5 Add unit tests for wait-event type stability and header-level compile coverage

- [ ] 3B. Define optimizer and memory-governance interfaces
  - [x] 3B.1 Add shared memory-pool classification types in `src/common`
  - [x] 3B.2 Add reserved optimizer interface headers in `src/optimizer`
  - [x] 3B.3 Add reserved memory-broker interface headers in `src/memory`
  - [x] 3B.4 Add unit tests for memory-pool type stability and header-level compile coverage

- [ ] 3C. Define storage-placement interfaces
  - [x] 3C.1 Add shared storage identity aliases in `src/common`
  - [x] 3C.2 Add reserved tablespace and storage-cost interface headers in `src/storage`
  - [x] 3C.3 Add reserved tablespace catalog interface headers in `src/catalog`
  - [x] 3C.4 Add unit tests for storage-identity type stability and header-level compile coverage

- [ ] 3D. Define diagnostic-logging interfaces
  - [x] 3D.1 Add shared diagnostic sequence, severity, and record-header types in `src/common`
  - [x] 3D.2 Add reserved diagnostic log service and flusher policy headers in `src/runtime`
  - [x] 3D.3 Add reserved diagnostic buffer memory classification coverage in `src/memory`
  - [x] 3D.4 Add unit tests for diagnostic type stability and header-level compile coverage

- [ ] 4. Implement memory foundation interfaces
  - [x] 4.1 Add the `MemoryContext` base interface
  - [x] 4.2 Add a simple heap-backed memory-context implementation for bootstrap testing
  - [x] 4.3 Add unit tests for usage, peak tracking, and limit enforcement

- [ ] 5. Implement runtime ownership skeletons
  - [x] 5.1 Add `DBInstance` class skeleton and instance state handling
  - [x] 5.2 Add `DatabaseRuntime` class skeleton and database state handling
  - [x] 5.3 Add `Session` and `Agent` skeletons with attach and execute placeholders
  - [x] 5.4 Add unit tests for instance, database, and agent state flows

- [ ] 6. Add architecture-facing documentation and developer bootstrap notes
  - [x] 6.1 Add repository-level architecture notes that reference the Day 1 runtime model
  - [x] 6.2 Add developer bootstrap notes for building and testing the runtime foundation
  - [ ] 6.3 Verify the Day 1 spec stays aligned with the code skeleton names
