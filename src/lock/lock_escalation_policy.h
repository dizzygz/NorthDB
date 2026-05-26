#pragma once

#include <cstddef>

namespace northdb::lock {

class LockEscalationPolicy {
public:
    virtual ~LockEscalationPolicy() = default;

    virtual bool ShouldEscalate(std::size_t held_lock_count) const noexcept = 0;
};

}  // namespace northdb::lock
