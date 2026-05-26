#include <cassert>
#include <type_traits>
#include <utility>

#include "storage/latch.h"
#include "storage/latch_guard.h"

namespace {

class TestLatch final : public northdb::storage::Latch {
public:
    void Lock() override {
        ++lock_count_;
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
        ++unlock_count_;
        locked_ = false;
    }

    bool locked() const noexcept {
        return locked_;
    }

    int lock_count() const noexcept {
        return lock_count_;
    }

    int unlock_count() const noexcept {
        return unlock_count_;
    }

private:
    bool locked_ = false;
    int lock_count_ = 0;
    int unlock_count_ = 0;
};

}  // namespace

int main() {
    static_assert(std::has_virtual_destructor_v<northdb::storage::Latch>);

    TestLatch latch;
    assert(!latch.locked());
    assert(latch.TryLock());
    assert(latch.locked());
    assert(!latch.TryLock());
    latch.Unlock();
    assert(!latch.locked());

    {
        northdb::storage::LatchGuard guard(latch);
        assert(latch.locked());
        assert(latch.lock_count() == 2);
        assert(latch.unlock_count() == 1);
    }

    assert(!latch.locked());
    assert(latch.unlock_count() == 2);

    {
        northdb::storage::LatchGuard first_guard(latch);
        assert(latch.locked());

        northdb::storage::LatchGuard second_guard(std::move(first_guard));
        assert(latch.locked());
        (void)second_guard;
    }

    assert(!latch.locked());
    assert(latch.lock_count() == 3);
    assert(latch.unlock_count() == 3);

    return 0;
}
