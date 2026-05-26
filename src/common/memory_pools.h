#pragma once

namespace northdb::common {

enum class MemoryPoolClass {
    kInstanceShared,
    kDatabaseShared,
    kSessionPrivate,
    kTransactionPrivate,
    kAgentRuntime,
    kQueryWork,
    kUtilityWork,
    kExchangeBuffer,
    kPlanCache,
    kCatalogCache,
    kLockMemory,
    kBufferPool,
    kDiagnosticBuffer,
    kDiagnosticEmergency
};

}  // namespace northdb::common
