#pragma once

#include <condition_variable>
#include <mutex>

#include "runtime/wait_post.h"

namespace northdb::runtime {

class ProcessLocalWaitPost final : public WaitPost {
public:
    explicit ProcessLocalWaitPost(Mode mode) noexcept : mode_(mode) {}

    northdb::common::Status Wait(std::chrono::milliseconds timeout) override {
        std::unique_lock<std::mutex> lock(mutex_);

        if (posted_) {
            return northdb::common::Status::Ok();
        }

        ++waiter_count_;
        const auto waiting_scope = [this]() noexcept {
            if (waiter_count_ > 0) {
                --waiter_count_;
            }
        };

        const bool awakened = condition_.wait_for(lock, timeout, [this]() noexcept { return posted_; });
        waiting_scope();
        if (!awakened) {
            return northdb::common::Status::Error("waitpost timed out");
        }

        return northdb::common::Status::Ok();
    }

    northdb::common::Status Post() override {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            posted_ = true;
        }

        condition_.notify_all();
        return northdb::common::Status::Ok();
    }

    northdb::common::Status Reset() override {
        std::lock_guard<std::mutex> lock(mutex_);
        posted_ = false;
        return northdb::common::Status::Ok();
    }

    std::uint32_t WaiterCount() const noexcept override {
        std::lock_guard<std::mutex> lock(mutex_);
        return waiter_count_;
    }

    bool IsPosted() const noexcept override {
        std::lock_guard<std::mutex> lock(mutex_);
        return posted_;
    }

    std::uint64_t Generation() const noexcept override {
        std::lock_guard<std::mutex> lock(mutex_);
        return generation_;
    }

    Mode mode() const noexcept override {
        return mode_;
    }

private:
    Mode mode_;
    mutable std::mutex mutex_;
    std::condition_variable condition_;
    std::uint64_t generation_ = 0;
    std::uint32_t waiter_count_ = 0;
    bool posted_ = false;
};

}  // namespace northdb::runtime
