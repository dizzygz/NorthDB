#include <cassert>
#include <type_traits>

#include "common/memory_pools.h"
#include "common/status.h"
#include "memory/memory_broker.h"
#include "memory/memory_grant.h"
#include "memory/memory_pool_budget.h"

namespace {

class TestMemoryBroker final : public northdb::memory::MemoryBroker {
public:
    northdb::common::Status ConfigurePool(const northdb::memory::MemoryPoolBudget& budget) override {
        last_budget_ = budget;
        return northdb::common::Status::Ok();
    }

    northdb::memory::MemoryGrant Grant(const northdb::memory::MemoryGrantRequest& request) override {
        last_request_ = request;
        return northdb::memory::MemoryGrant{
            .pool_class = request.pool_class,
            .granted_bytes = request.allow_spill ? request.requested_bytes / 2 : request.requested_bytes,
            .spill_required = request.allow_spill,
        };
    }

    northdb::common::Status Release(const northdb::memory::MemoryGrant& grant) override {
        last_released_grant_ = grant;
        return northdb::common::Status::Ok();
    }

    const northdb::memory::MemoryPoolBudget& last_budget() const noexcept {
        return last_budget_;
    }

    const northdb::memory::MemoryGrantRequest& last_request() const noexcept {
        return last_request_;
    }

    const northdb::memory::MemoryGrant& last_released_grant() const noexcept {
        return last_released_grant_;
    }

private:
    northdb::memory::MemoryPoolBudget last_budget_;
    northdb::memory::MemoryGrantRequest last_request_;
    northdb::memory::MemoryGrant last_released_grant_;
};

}  // namespace

int main() {
    static_assert(std::has_virtual_destructor_v<northdb::memory::MemoryBroker>);

    northdb::memory::MemoryPoolBudget budget;
    budget.pool_class = northdb::common::MemoryPoolClass::kPlanCache;
    budget.reserved_bytes = 4096;
    budget.limit_bytes = 16384;

    assert(budget.pool_class == northdb::common::MemoryPoolClass::kPlanCache);
    assert(budget.reserved_bytes == 4096);
    assert(budget.limit_bytes == 16384);

    northdb::memory::MemoryGrantRequest request;
    request.pool_class = northdb::common::MemoryPoolClass::kQueryWork;
    request.requested_bytes = 8192;
    request.allow_spill = true;

    assert(request.pool_class == northdb::common::MemoryPoolClass::kQueryWork);
    assert(request.requested_bytes == 8192);
    assert(request.allow_spill);

    TestMemoryBroker broker;
    const auto configure_status = broker.ConfigurePool(budget);
    assert(configure_status.ok());
    assert(broker.last_budget().pool_class == northdb::common::MemoryPoolClass::kPlanCache);

    const auto grant = broker.Grant(request);
    assert(broker.last_request().requested_bytes == 8192);
    assert(grant.pool_class == northdb::common::MemoryPoolClass::kQueryWork);
    assert(grant.granted_bytes == 4096);
    assert(grant.spill_required);

    const auto release_status = broker.Release(grant);
    assert(release_status.ok());
    assert(broker.last_released_grant().granted_bytes == 4096);

    return 0;
}
