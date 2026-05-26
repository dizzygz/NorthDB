#pragma once

#include <cstddef>

#include "common/memory_pools.h"

namespace northdb::memory {

struct MemoryGrantRequest {
    northdb::common::MemoryPoolClass pool_class = northdb::common::MemoryPoolClass::kQueryWork;
    std::size_t requested_bytes = 0;
    bool allow_spill = false;
};

struct MemoryGrant {
    northdb::common::MemoryPoolClass pool_class = northdb::common::MemoryPoolClass::kQueryWork;
    std::size_t granted_bytes = 0;
    bool spill_required = false;
};

}  // namespace northdb::memory
