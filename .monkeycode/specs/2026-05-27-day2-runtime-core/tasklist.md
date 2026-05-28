# Task List

## Day 2 Runtime Core

- [ ] 1. Establish Day 2 runtime synchronization and ownership direction
  - [x] 1.1 Add `SessionState` and `ExecutionRole` vocabulary in `src/runtime`
  - [x] 1.2 Extend runtime registry design from counters toward handle-aware ownership tracking
  - [x] 1.3 Define the `WaitPost` runtime interface in `src/runtime/wait_post.h`

- [ ] 2. Implement bootstrap `WaitPost`
  - [x] 2.1 Add a process-local bootstrap implementation using standard C++ blocking primitives
  - [ ] 2.2 Support `kStickyEvent` and `kAutoReset` semantics
  - [ ] 2.3 Add generation tracking and explicit `Reset()` behavior for stale-wakeup prevention
  - [ ] 2.4 Expose waiter-count and posted-state inspection for diagnostics and runtime tests

- [ ] 3. Integrate `WaitPost` into runtime components
  - [ ] 3.1 Add wait-channel ownership to `Session` for request completion and cancel acknowledgement
  - [ ] 3.2 Add wait-channel ownership to `Agent` for execution handoff and completion signaling
  - [ ] 3.3 Add lifecycle wait channels to `DBInstance` or `DatabaseRuntime` for startup and shutdown coordination
  - [ ] 3.4 Add flusher wakeup and shutdown-drain wait paths to `DiagnosticLogService`

- [ ] 4. Add Day 2 runtime tests
  - [ ] 4.1 Add unit tests for sticky-event behavior and reset semantics
  - [ ] 4.2 Add unit tests for auto-reset single-waiter wakeup behavior
  - [ ] 4.3 Add unit tests for timeout, waiter-count accounting, and generation rollover safety
  - [ ] 4.4 Add runtime integration tests for session, agent, service, and diagnostic wakeup flows

- [ ] 5. Refresh runtime-facing documentation after implementation
  - [ ] 5.1 Update `.monkeycode/docs/INTERFACES.md` for the new `WaitPost` boundary
  - [ ] 5.2 Update `.monkeycode/docs/ARCHITECTURE.md` to add waitpost channels to the runtime synchronization model
  - [ ] 5.3 Update developer guidance with Day 2 synchronization and testing expectations
