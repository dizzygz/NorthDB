#pragma once

#include "common/memory_pools.h"
#include "memory/memory_pool_budget.h"

namespace northdb::memory {

struct DiagnosticBufferMemory {
    static constexpr northdb::common::MemoryPoolClass kPoolClass =
        northdb::common::MemoryPoolClass::kDiagnosticBuffer;

    MemoryPoolBudget budget{
        .pool_class = kPoolClass,
        .reserved_bytes = 0,
        .limit_bytes = 0,
    };
};

struct DiagnosticEmergencyMemory {
    static constexpr northdb::common::MemoryPoolClass kPoolClass =
        northdb::common::MemoryPoolClass::kDiagnosticEmergency;

    MemoryPoolBudget budget{
        .pool_class = kPoolClass,
        .reserved_bytes = 0,
        .limit_bytes = 0,
    };
};

}  // namespace northdb::memory
