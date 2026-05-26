#pragma once

#include <cstddef>
#include <cstdint>
#include <new>
#include <unordered_map>

#include "memory/memory_context.h"

namespace northdb::memory {

class HeapMemoryContext final : public MemoryContext {
public:
    explicit HeapMemoryContext(std::size_t limit_bytes) : limit_bytes_(limit_bytes) {}

    ~HeapMemoryContext() override {
        Reset();
    }

    void* Allocate(std::size_t bytes,
                   std::size_t alignment = alignof(std::max_align_t)) override {
        if (!IsValidAlignment(alignment)) {
            return nullptr;
        }

        if (bytes > limit_bytes_ - used_bytes_) {
            return nullptr;
        }

        void* ptr = ::operator new(bytes, std::align_val_t(alignment), std::nothrow);
        if (ptr == nullptr) {
            return nullptr;
        }

        allocations_.emplace(ptr, AllocationRecord{bytes, alignment});
        used_bytes_ += bytes;
        if (used_bytes_ > peak_bytes_) {
            peak_bytes_ = used_bytes_;
        }

        return ptr;
    }

    void Deallocate(void* ptr, std::size_t bytes) override {
        if (ptr == nullptr) {
            return;
        }

        const auto it = allocations_.find(ptr);
        if (it == allocations_.end()) {
            return;
        }

        used_bytes_ -= bytes;
        ::operator delete(ptr, std::align_val_t(it->second.alignment));
        allocations_.erase(it);
    }

    void Reset() override {
        for (const auto& [ptr, record] : allocations_) {
            ::operator delete(ptr, std::align_val_t(record.alignment));
        }

        allocations_.clear();
        used_bytes_ = 0;
    }

    std::size_t UsedBytes() const noexcept override {
        return used_bytes_;
    }

    std::size_t PeakBytes() const noexcept override {
        return peak_bytes_;
    }

    std::size_t LimitBytes() const noexcept override {
        return limit_bytes_;
    }

private:
    struct AllocationRecord {
        std::size_t bytes;
        std::size_t alignment;
    };

    static bool IsValidAlignment(std::size_t alignment) noexcept {
        return alignment != 0 && (alignment & (alignment - 1)) == 0;
    }

    std::size_t limit_bytes_ = 0;
    std::size_t used_bytes_ = 0;
    std::size_t peak_bytes_ = 0;
    std::unordered_map<void*, AllocationRecord> allocations_;
};

}  // namespace northdb::memory
