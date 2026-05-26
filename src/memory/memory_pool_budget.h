#pragma once

#include <cstddef>

#include "common/memory_pools.h"

namespace northdb::memory {

struct MemoryPoolBudget {
    northdb::common::MemoryPoolClass pool_class = northdb::common::MemoryPoolClass::kInstanceShared;
    std::size_t reserved_bytes = 0;
    std::size_t limit_bytes = 0;
};

}  // namespace northdb::memory
