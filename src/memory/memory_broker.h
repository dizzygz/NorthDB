#pragma once

#include "common/status.h"
#include "memory/memory_grant.h"
#include "memory/memory_pool_budget.h"

namespace northdb::memory {

class MemoryBroker {
public:
    virtual ~MemoryBroker() = default;

    virtual northdb::common::Status ConfigurePool(const MemoryPoolBudget& budget) = 0;
    virtual MemoryGrant Grant(const MemoryGrantRequest& request) = 0;
    virtual northdb::common::Status Release(const MemoryGrant& grant) = 0;
};

}  // namespace northdb::memory
