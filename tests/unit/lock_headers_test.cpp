#include <cassert>
#include <type_traits>

#include "lock/lock_escalation_policy.h"
#include "lock/lock_manager.h"
#include "lock/lock_mode.h"
#include "lock/lock_request.h"

namespace {

class TestLockManager final : public northdb::lock::LockManager {
public:
    northdb::common::Status Acquire(const northdb::lock::LockRequest&) override {
        return northdb::common::Status::Ok();
    }

    northdb::common::Status Release(const northdb::lock::LockRequest&) override {
        return northdb::common::Status::Ok();
    }
};

class TestLockEscalationPolicy final : public northdb::lock::LockEscalationPolicy {
public:
    bool ShouldEscalate(std::size_t held_lock_count) const noexcept override {
        return held_lock_count >= 8;
    }
};

}  // namespace

int main() {
    static_assert(std::is_enum_v<northdb::lock::LockMode>);
    static_assert(std::has_virtual_destructor_v<northdb::lock::LockManager>);
    static_assert(std::has_virtual_destructor_v<northdb::lock::LockEscalationPolicy>);

    northdb::lock::LockRequest request;
    request.transaction_id = 7;
    request.resource_id = 99;
    request.mode = northdb::lock::LockMode::kExclusive;
    request.wait = false;

    assert(request.transaction_id == 7);
    assert(request.resource_id == 99);
    assert(request.mode == northdb::lock::LockMode::kExclusive);
    assert(!request.wait);

    TestLockManager manager;
    const auto acquire_status = manager.Acquire(request);
    const auto release_status = manager.Release(request);
    assert(acquire_status.ok());
    assert(release_status.ok());

    TestLockEscalationPolicy policy;
    assert(!policy.ShouldEscalate(7));
    assert(policy.ShouldEscalate(8));

    return 0;
}
