#include <cassert>
#include <cstddef>
#include <type_traits>

#include "memory/memory_context.h"

namespace {

class TestMemoryContext final : public northdb::memory::MemoryContext {
public:
    explicit TestMemoryContext(std::size_t limit_bytes) : limit_bytes_(limit_bytes) {}

    void* Allocate(std::size_t bytes,
                   std::size_t alignment = alignof(std::max_align_t)) override {
        last_allocation_size_ = bytes;
        last_alignment_ = alignment;
        used_bytes_ += bytes;
        if (used_bytes_ > peak_bytes_) {
            peak_bytes_ = used_bytes_;
        }

        allocated_ = true;
        return this;
    }

    void Deallocate(void* ptr, std::size_t bytes) override {
        last_deallocation_ptr_ = ptr;
        last_deallocation_size_ = bytes;
        used_bytes_ -= bytes;
        allocated_ = false;
    }

    void Reset() override {
        used_bytes_ = 0;
        allocated_ = false;
        reset_called_ = true;
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

    std::size_t last_allocation_size() const noexcept {
        return last_allocation_size_;
    }

    std::size_t last_alignment() const noexcept {
        return last_alignment_;
    }

    void* last_deallocation_ptr() const noexcept {
        return last_deallocation_ptr_;
    }

    std::size_t last_deallocation_size() const noexcept {
        return last_deallocation_size_;
    }

    bool allocated() const noexcept {
        return allocated_;
    }

    bool reset_called() const noexcept {
        return reset_called_;
    }

private:
    std::size_t limit_bytes_ = 0;
    std::size_t used_bytes_ = 0;
    std::size_t peak_bytes_ = 0;
    std::size_t last_allocation_size_ = 0;
    std::size_t last_alignment_ = 0;
    void* last_deallocation_ptr_ = nullptr;
    std::size_t last_deallocation_size_ = 0;
    bool allocated_ = false;
    bool reset_called_ = false;
};

}  // namespace

int main() {
    static_assert(std::has_virtual_destructor_v<northdb::memory::MemoryContext>);

    TestMemoryContext context(8192);
    assert(context.LimitBytes() == 8192);
    assert(context.UsedBytes() == 0);
    assert(context.PeakBytes() == 0);

    void* first_ptr = context.Allocate(256);
    assert(first_ptr == &context);
    assert(context.last_allocation_size() == 256);
    assert(context.last_alignment() == alignof(std::max_align_t));
    assert(context.UsedBytes() == 256);
    assert(context.PeakBytes() == 256);
    assert(context.allocated());

    void* second_ptr = context.Allocate(128, 64);
    assert(second_ptr == &context);
    assert(context.last_allocation_size() == 128);
    assert(context.last_alignment() == 64);
    assert(context.UsedBytes() == 384);
    assert(context.PeakBytes() == 384);

    context.Deallocate(first_ptr, 256);
    assert(context.last_deallocation_ptr() == &context);
    assert(context.last_deallocation_size() == 256);
    assert(context.UsedBytes() == 128);
    assert(!context.allocated());

    context.Reset();
    assert(context.reset_called());
    assert(context.UsedBytes() == 0);
    assert(context.PeakBytes() == 384);

    return 0;
}
