#pragma once

#include <cstddef>

namespace northdb::memory {

class MemoryContext {
public:
    virtual ~MemoryContext() = default;

    virtual void* Allocate(std::size_t bytes,
                           std::size_t alignment = alignof(std::max_align_t)) = 0;
    virtual void Deallocate(void* ptr, std::size_t bytes) = 0;
    virtual void Reset() = 0;
    virtual std::size_t UsedBytes() const noexcept = 0;
    virtual std::size_t PeakBytes() const noexcept = 0;
    virtual std::size_t LimitBytes() const noexcept = 0;
};

}  // namespace northdb::memory
