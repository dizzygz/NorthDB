#pragma once

#include <chrono>
#include <cstdint>

#include "common/status.h"

namespace northdb::runtime {

class WaitPost {
public:
    enum class Mode {
        kStickyEvent,
        kAutoReset,
    };

    virtual ~WaitPost() = default;

    virtual northdb::common::Status Wait(std::chrono::milliseconds timeout) = 0;
    virtual northdb::common::Status Post() = 0;
    virtual northdb::common::Status Reset() = 0;
    virtual std::uint32_t WaiterCount() const noexcept = 0;
    virtual bool IsPosted() const noexcept = 0;
    virtual std::uint64_t Generation() const noexcept = 0;
    virtual Mode mode() const noexcept = 0;
};

}  // namespace northdb::runtime
