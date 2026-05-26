#pragma once

#include <cstdint>

#include "common/ids.h"
#include "lock/lock_mode.h"

namespace northdb::lock {

using LockResourceId = std::uint64_t;

struct LockRequest {
    northdb::common::TransactionId transaction_id = 0;
    LockResourceId resource_id = 0;
    LockMode mode = LockMode::kShared;
    bool wait = true;
};

}  // namespace northdb::lock
