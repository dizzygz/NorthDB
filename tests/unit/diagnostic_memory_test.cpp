#include <cassert>
#include <type_traits>

#include "memory/diagnostic_buffer_memory.h"

int main() {
    static_assert(std::is_same_v<decltype(northdb::memory::DiagnosticBufferMemory::kPoolClass), const northdb::common::MemoryPoolClass>);
    static_assert(std::is_same_v<decltype(northdb::memory::DiagnosticEmergencyMemory::kPoolClass), const northdb::common::MemoryPoolClass>);

    northdb::memory::DiagnosticBufferMemory diagnostic_buffer_memory;
    diagnostic_buffer_memory.budget.reserved_bytes = 4096;
    diagnostic_buffer_memory.budget.limit_bytes = 16384;

    northdb::memory::DiagnosticEmergencyMemory diagnostic_emergency_memory;
    diagnostic_emergency_memory.budget.reserved_bytes = 1024;
    diagnostic_emergency_memory.budget.limit_bytes = 2048;

    assert(northdb::memory::DiagnosticBufferMemory::kPoolClass == northdb::common::MemoryPoolClass::kDiagnosticBuffer);
    assert(northdb::memory::DiagnosticEmergencyMemory::kPoolClass == northdb::common::MemoryPoolClass::kDiagnosticEmergency);
    assert(diagnostic_buffer_memory.budget.pool_class == northdb::common::MemoryPoolClass::kDiagnosticBuffer);
    assert(diagnostic_buffer_memory.budget.reserved_bytes == 4096);
    assert(diagnostic_buffer_memory.budget.limit_bytes == 16384);
    assert(diagnostic_emergency_memory.budget.pool_class == northdb::common::MemoryPoolClass::kDiagnosticEmergency);
    assert(diagnostic_emergency_memory.budget.reserved_bytes == 1024);
    assert(diagnostic_emergency_memory.budget.limit_bytes == 2048);

    return 0;
}
