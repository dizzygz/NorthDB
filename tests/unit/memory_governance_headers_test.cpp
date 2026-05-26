#include <cassert>
#include <type_traits>

#include "common/memory_pools.h"
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
            .granted_bytes = request.requested_bytes,
            .spill_required = false,
        };
    }

    northdb::common::Status Release(const northdb::memory::MemoryGrant& grant) override {
        last_grant_ = grant;
        return northdb::common::Status::Ok();
    }

    const northdb::memory::MemoryPoolBudget& last_budget() const noexcept {
        return last_budget_;
    }

    const northdb::memory::MemoryGrantRequest& last_request() const noexcept {
        return last_request_;
    }

    const northdb::memory::MemoryGrant& last_grant() const noexcept {
        return last_grant_;
    }

private:
    northdb::memory::MemoryPoolBudget last_budget_;
    northdb::memory::MemoryGrantRequest last_request_;
    northdb::memory::MemoryGrant last_grant_;
};

}  // namespace

int main() {
    static_assert(std::is_enum_v<northdb::common::MemoryPoolClass>);
    static_assert(static_cast<int>(northdb::common::MemoryPoolClass::kInstanceShared) == 0);
    static_assert(static_cast<int>(northdb::common::MemoryPoolClass::kDatabaseShared) == 1);
    static_assert(static_cast<int>(northdb::common::MemoryPoolClass::kSessionPrivate) == 2);
    static_assert(static_cast<int>(northdb::common::MemoryPoolClass::kTransactionPrivate) == 3);
    static_assert(static_cast<int>(northdb::common::MemoryPoolClass::kAgentRuntime) == 4);
    static_assert(static_cast<int>(northdb::common::MemoryPoolClass::kQueryWork) == 5);
    static_assert(static_cast<int>(northdb::common::MemoryPoolClass::kUtilityWork) == 6);
    static_assert(static_cast<int>(northdb::common::MemoryPoolClass::kExchangeBuffer) == 7);
    static_assert(static_cast<int>(northdb::common::MemoryPoolClass::kPlanCache) == 8);
    static_assert(static_cast<int>(northdb::common::MemoryPoolClass::kCatalogCache) == 9);
    static_assert(static_cast<int>(northdb::common::MemoryPoolClass::kLockMemory) == 10);
    static_assert(static_cast<int>(northdb::common::MemoryPoolClass::kBufferPool) == 11);
    static_assert(static_cast<int>(northdb::common::MemoryPoolClass::kDiagnosticBuffer) == 12);
    static_assert(static_cast<int>(northdb::common::MemoryPoolClass::kDiagnosticEmergency) == 13);

    static_assert(std::has_virtual_destructor_v<northdb::memory::MemoryBroker>);

    northdb::memory::MemoryPoolBudget budget;
    budget.pool_class = northdb::common::MemoryPoolClass::kBufferPool;
    budget.reserved_bytes = 32768;
    budget.limit_bytes = 131072;

    northdb::memory::MemoryGrantRequest request;
    request.pool_class = northdb::common::MemoryPoolClass::kUtilityWork;
    request.requested_bytes = 2048;
    request.allow_spill = false;

    TestMemoryBroker broker;
    assert(broker.ConfigurePool(budget).ok());
    assert(broker.last_budget().pool_class == northdb::common::MemoryPoolClass::kBufferPool);
    assert(broker.last_budget().limit_bytes == 131072);

    const auto grant = broker.Grant(request);
    assert(broker.last_request().pool_class == northdb::common::MemoryPoolClass::kUtilityWork);
    assert(grant.granted_bytes == 2048);
    assert(!grant.spill_required);

    assert(broker.Release(grant).ok());
    assert(broker.last_grant().pool_class == northdb::common::MemoryPoolClass::kUtilityWork);
    assert(broker.last_grant().granted_bytes == 2048);

    return 0;
}
