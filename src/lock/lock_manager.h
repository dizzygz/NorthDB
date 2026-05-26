#pragma once

#include "common/status.h"
#include "lock/lock_request.h"

namespace northdb::lock {

class LockManager {
public:
    virtual ~LockManager() = default;

    virtual northdb::common::Status Acquire(const LockRequest& request) = 0;
    virtual northdb::common::Status Release(const LockRequest& request) = 0;
};

}  // namespace northdb::lock
