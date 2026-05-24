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
