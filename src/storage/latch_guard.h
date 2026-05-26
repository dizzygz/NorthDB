#pragma once

#include "storage/latch.h"

namespace northdb::storage {

class LatchGuard {
public:
    explicit LatchGuard(Latch& latch) : latch_(&latch) {
        latch_->Lock();
    }

    LatchGuard(const LatchGuard&) = delete;
    LatchGuard& operator=(const LatchGuard&) = delete;

    LatchGuard(LatchGuard&& other) noexcept : latch_(other.latch_) {
        other.latch_ = nullptr;
    }

    LatchGuard& operator=(LatchGuard&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Release();
        latch_ = other.latch_;
        other.latch_ = nullptr;
        return *this;
    }

    ~LatchGuard() {
        Release();
    }

private:
    void Release() noexcept {
        if (latch_ != nullptr) {
            latch_->Unlock();
            latch_ = nullptr;
        }
    }

    Latch* latch_ = nullptr;
};

}  // namespace northdb::storage
