#include <cassert>

#include "memory/heap_memory_context.h"

int main() {
    northdb::memory::HeapMemoryContext context(512);

    void* first_ptr = context.Allocate(200);
    assert(first_ptr != nullptr);
    assert(context.UsedBytes() == 200);
    assert(context.PeakBytes() == 200);

    void* second_ptr = context.Allocate(312);
    assert(second_ptr != nullptr);
    assert(context.UsedBytes() == 512);
    assert(context.PeakBytes() == 512);

    void* limit_failure_ptr = context.Allocate(1);
    assert(limit_failure_ptr == nullptr);
    assert(context.UsedBytes() == 512);
    assert(context.PeakBytes() == 512);

    context.Deallocate(first_ptr, 200);
    assert(context.UsedBytes() == 312);
    assert(context.PeakBytes() == 512);

    void* third_ptr = context.Allocate(128);
    assert(third_ptr != nullptr);
    assert(context.UsedBytes() == 440);
    assert(context.PeakBytes() == 512);

    context.Reset();
    assert(context.UsedBytes() == 0);
    assert(context.PeakBytes() == 512);

    void* post_reset_ptr = context.Allocate(512);
    assert(post_reset_ptr != nullptr);
    assert(context.UsedBytes() == 512);
    assert(context.PeakBytes() == 512);

    context.Deallocate(post_reset_ptr, 512);
    assert(context.UsedBytes() == 0);
    assert(context.PeakBytes() == 512);

    return 0;
}
