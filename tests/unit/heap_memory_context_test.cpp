#include <cassert>
#include <cstdint>

#include "memory/heap_memory_context.h"

int main() {
    northdb::memory::HeapMemoryContext context(1024);
    assert(context.LimitBytes() == 1024);
    assert(context.UsedBytes() == 0);
    assert(context.PeakBytes() == 0);

    void* first_ptr = context.Allocate(128);
    assert(first_ptr != nullptr);
    assert(context.UsedBytes() == 128);
    assert(context.PeakBytes() == 128);

    void* second_ptr = context.Allocate(256, 64);
    assert(second_ptr != nullptr);
    assert((reinterpret_cast<std::uintptr_t>(second_ptr) % 64) == 0);
    assert(context.UsedBytes() == 384);
    assert(context.PeakBytes() == 384);

    context.Deallocate(first_ptr, 128);
    assert(context.UsedBytes() == 256);
    assert(context.PeakBytes() == 384);

    void* invalid_alignment_ptr = context.Allocate(64, 3);
    assert(invalid_alignment_ptr == nullptr);
    assert(context.UsedBytes() == 256);

    context.Reset();
    assert(context.UsedBytes() == 0);
    assert(context.PeakBytes() == 384);

    return 0;
}
