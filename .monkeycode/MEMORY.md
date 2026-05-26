# 用户指令记忆

本文件记录了用户的指令、偏好和教导，用于在未来的交互中提供参考。

## 格式

### 用户指令条目
用户指令条目应遵循以下格式：

[用户指令摘要]
- Date: [YYYY-MM-DD]
- Context: [提及的场景或时间]
- Instructions:
  - [用户教导或指示的内容，逐行描述]

### 项目知识条目
Agent 在任务执行过程中发现的条目应遵循以下格式：

[项目知识摘要]
- Date: [YYYY-MM-DD]
- Context: Agent 在执行 [具体任务描述] 时发现
- Category: [运维部署|构建方法|测试方法|排错调试|工作流协作|环境配置]
- Instructions:
  - [具体的知识点，逐行描述]

## 去重策略
- 添加新条目前，检查是否存在相似或相同的指令
- 若发现重复，跳过新条目或与已有条目合并
- 合并时，更新上下文或日期信息
- 这有助于避免冗余条目，保持记忆文件整洁

## 条目

[Response language and implementation preferences]
- Date: 2026-05-24
- Context: User specified project-wide communication and implementation constraints for the database design work
- Instructions:
  - Use English for all user-facing contents in this project thread.
  - Use C++ for development decisions and implementation planning.
  - Consider memory management explicitly for instances, databases, and related subsystems.

[Feature implementation workflow]
- Date: 2026-05-24
- Context: User defined the required execution workflow for implementing tasks from project specifications
- Instructions:
  - Before implementing any task, read `.monkeycode/specs/{FEATURE_NAME}/design.md`, `tasklist.md`, and optionally `requirements.md`.
  - Before implementing any task, also read `.monkeycode/docs/INDEX.md`, `ARCHITECTURE.md`, `INTERFACES.md`, and `DEVELOPER_GUIDE.md`.
  - Implement one task at a time and stop after each completed task for user review.
  - If the user does not specify a task, recommend the first uncompleted task from `tasklist.md` and wait for confirmation.
  - Write appropriate tests for each implemented task and ensure tests pass before marking the task complete.
  - After completing a task, update `tasklist.md` from `[ ]` to `[x]` for that task.
  - After completing a task, sync project documentation via the `project-wiki` workflow.

[Feature design workflow]
- Date: 2026-05-24
- Context: User defined the required workflow for turning rough feature ideas into formal requirement and design documents
- Instructions:
  - Use a spec-driven development workflow for feature design work.
  - Generate feature requirements with EARS patterns and validate them against INCOSE semantic quality rules.
  - Create design documents with sections for description, architecture, components and interfaces, data models, correctness properties, error handling, test strategy, and references.
  - During feature-design work, read project docs from `.monkeycode/docs/`, scan related historical specs in `.monkeycode/specs/`, and understand existing code structure before finalizing requirements or design.
  - Keep user interaction iterative and ask at most 3 clarifying questions per round when clarification is required.
  - For parser or serializer features, explicitly include parser, serializer, pretty-printer, and round-trip validation requirements.

[DB2 concurrency design guidance]
- Date: 2026-05-24
- Context: Derived from DB2 lock-management and latch-contention references during database architecture design
- Category: 排错调试
- Instructions:
  - Keep logical locks, internal latches, and parallel exchange queues as separate concurrency domains in both APIs and observability.
  - Treat logical locks as transaction-visible isolation mechanisms with escalation and deadlock-aware waiting.
  - Treat latches as short critical-section guards for internal shared-memory structures and keep blocking I/O and logical-lock waits out of latch critical sections.
  - Treat parallel exchange queue contention as an executor and optimizer concern, especially for highly selective row-goal queries where parallel fan-out may hurt performance.
  - Preserve wait-event diagnostics that distinguish lock waits, latch waits, queue waits, I/O waits, and log-flush waits.

[DB2 optimizer and memory-governance guidance]
- Date: 2026-05-24
- Context: Derived from DB2 query-optimization and memory-allocation references during database architecture design
- Category: 环境配置
- Instructions:
  - Treat the optimizer as a statistics-driven cost-based subsystem with explicit access-path enumeration, cardinality estimation, costing, and plan-cache boundaries.
  - Treat query shape, index selectivity, and catalog statistics as core optimizer inputs rather than optional tuning extras.
  - Keep shared engine memory separate from transient query and utility memory in both APIs and accounting.
  - Use named memory classes for instance-shared, database-shared, session-private, query-work, and utility-work memory.
  - Reserve a memory-broker or governor boundary that can arbitrate budgets, grants, and spill behavior across shared pools and workload-driven consumers.

[DB2 storage placement guidance]
- Date: 2026-05-24
- Context: Derived from DB2 disk-storage and table-space performance references during database architecture design
- Category: 构建方法
- Instructions:
  - Treat storage placement as an optimizer-visible concern through logical tablespaces, storage classes, and summarized storage cost profiles.
  - Keep raw physical device topology inside the storage engine and expose only stable cost parameters such as overhead and transfer-rate style metrics to the optimizer.
  - Make scans, index probes, and spill destinations sensitive to storage-class costing rather than assuming uniform I/O behavior.
  - Treat storage metadata changes as plan-affecting events that may require explain review, recompilation, or plan invalidation.

[DB2 heap and index maintenance guidance]
- Date: 2026-05-24
- Context: Derived from DB2 table organization, index structure, asynchronous cleanup, and clustering references during database architecture design
- Category: 排错调试
- Instructions:
  - Treat stable tuple identity as a prerequisite for deferred secondary-index maintenance and row relocation tolerance.
  - Use page-oriented B+ tree structures as the baseline future secondary-index design.
  - Preserve correctness on index probes through tuple revalidation when stale entries, forwarding, or row indirection are possible.
  - Treat clustering quality, fragmentation, and cleanup backlog as optimizer-visible and maintenance-visible signals.
  - Prioritize local partitioned indexes before partition-spanning global indexes in the partitioned-table roadmap.
  - Defer specialized MDC-style, ITC-style, and block-index designs until the baseline heap and B+ tree path is stable.

[DB2 agent and connection concentration guidance]
- Date: 2026-05-24
- Context: Derived from DB2 client-server agent, connection concentrator, and partitioned-database agent references during runtime architecture design
- Category: 工作流协作
- Instructions:
  - Treat `Session` as durable logical client state and `Agent` as pooled execution capacity attached only while work is active.
  - Keep coordinator responsibility request-scoped rather than permanently bound to a client thread or connection.
  - Preserve attach and detach boundaries around active work, wait states, and post-response idle periods.
  - Keep durable session memory separate from transient agent runtime memory and future exchange-buffer memory.
  - Reserve partition-local agent pools and subagent-style worker roles for the partitioned execution roadmap.
  - Keep runtime observability separate for connected sessions, waiting sessions, active agents, and idle pooled agents.

[English-only project-facing content]
- Date: 2026-05-24
- Context: User specified the required language for ongoing project context, interactions, and design artifacts
- Instructions:
  - Use English for all project context notes.
  - Use English for all user-facing interaction content in this project thread when producing project artifacts or structured task communication.
  - Use English for all design documents and related specification artifacts.

[DB2 diagnostic logging guidance]
- Date: 2026-05-24
- Context: Derived from DB2 diagnostic log documentation and synchronous logging failure analysis during runtime observability design
- Category: 排错调试
- Instructions:
  - Treat diagnostic logging as a dedicated asynchronous runtime service rather than a foreground open-lock-write-close helper path.
  - Preserve structured diagnostic metadata including sequence, timestamps, severity, component, worker identity, and probe site for later investigation.
  - Keep diagnostic publication on bounded preallocated buffers and move file I/O to a dedicated flusher worker.
  - Use suppression, rate limiting, and explicit drop accounting to survive repeated client, SSL, and network failure storms.
  - Keep diagnostic logs member-local in the distributed roadmap and rely on structured correlation metadata rather than file write order alone.
  - Reserve crash-path emergency flush behavior that avoids dynamic allocation and non-essential lock acquisition.
