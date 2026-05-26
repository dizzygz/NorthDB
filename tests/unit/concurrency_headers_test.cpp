#include <cassert>
#include <type_traits>

#include "common/wait_events.h"
#include "executor/parallel_execution_policy.h"
#include "executor/table_queue.h"
#include "lock/lock_escalation_policy.h"
#include "lock/lock_manager.h"
#include "lock/lock_mode.h"
#include "lock/lock_request.h"
#include "storage/latch.h"
#include "storage/latch_guard.h"

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
        return held_lock_count >= 16;
    }
};

class TestLatch final : public northdb::storage::Latch {
public:
    void Lock() override {
        locked_ = true;
    }

    bool TryLock() override {
        if (locked_) {
            return false;
        }

        Lock();
        return true;
    }

    void Unlock() override {
        locked_ = false;
    }

    bool locked() const noexcept {
        return locked_;
    }

private:
    bool locked_ = false;
};

class TestTableQueue final : public northdb::executor::TableQueue {
public:
    std::size_t Capacity() const noexcept override {
        return 32;
    }

    std::size_t Size() const noexcept override {
        return 0;
    }

    bool IsClosed() const noexcept override {
        return closed_;
    }

    void Close() override {
        closed_ = true;
    }

private:
    bool closed_ = false;
};

class TestParallelExecutionPolicy final : public northdb::executor::ParallelExecutionPolicy {
public:
    std::size_t WorkerCount() const noexcept override {
        return 2;
    }

    bool AllowParallelFanout() const noexcept override {
        return true;
    }
};

}  // namespace

int main() {
    static_assert(std::is_enum_v<northdb::common::WaitEventClass>);
    static_assert(static_cast<int>(northdb::common::WaitEventClass::kNone) == 0);
    static_assert(static_cast<int>(northdb::common::WaitEventClass::kLock) == 1);
    static_assert(static_cast<int>(northdb::common::WaitEventClass::kLatch) == 2);
    static_assert(static_cast<int>(northdb::common::WaitEventClass::kQueue) == 3);
    static_assert(static_cast<int>(northdb::common::WaitEventClass::kIo) == 4);
    static_assert(static_cast<int>(northdb::common::WaitEventClass::kLogFlush) == 5);
    static_assert(static_cast<int>(northdb::common::WaitEventClass::kClient) == 6);
    static_assert(static_cast<int>(northdb::common::WaitEventClass::kRemote) == 7);
    static_assert(static_cast<int>(northdb::common::WaitEventClass::kDiagnostic) == 8);

    static_assert(std::has_virtual_destructor_v<northdb::lock::LockManager>);
    static_assert(std::has_virtual_destructor_v<northdb::lock::LockEscalationPolicy>);
    static_assert(std::has_virtual_destructor_v<northdb::storage::Latch>);
    static_assert(std::has_virtual_destructor_v<northdb::executor::TableQueue>);
    static_assert(std::has_virtual_destructor_v<northdb::executor::ParallelExecutionPolicy>);

    northdb::lock::LockRequest request;
    request.transaction_id = 11;
    request.resource_id = 17;
    request.mode = northdb::lock::LockMode::kShared;
    request.wait = true;

    TestLockManager lock_manager;
    assert(lock_manager.Acquire(request).ok());
    assert(lock_manager.Release(request).ok());

    TestLockEscalationPolicy lock_policy;
    assert(!lock_policy.ShouldEscalate(15));
    assert(lock_policy.ShouldEscalate(16));

    TestLatch latch;
    assert(!latch.locked());
    {
        northdb::storage::LatchGuard guard(latch);
        assert(latch.locked());
    }
    assert(!latch.locked());

    TestTableQueue queue;
    assert(queue.Capacity() == 32);
    assert(queue.Size() == 0);
    assert(!queue.IsClosed());
    queue.Close();
    assert(queue.IsClosed());

    TestParallelExecutionPolicy parallel_policy;
    assert(parallel_policy.WorkerCount() == 2);
    assert(parallel_policy.AllowParallelFanout());

    return 0;
}
