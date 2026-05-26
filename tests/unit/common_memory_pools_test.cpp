#include <cassert>
#include <type_traits>

#include "common/memory_pools.h"

int main() {
    static_assert(std::is_enum_v<northdb::common::MemoryPoolClass>);

    const auto session_private = northdb::common::MemoryPoolClass::kSessionPrivate;
    const auto query_work = northdb::common::MemoryPoolClass::kQueryWork;
    const auto diagnostic_emergency = northdb::common::MemoryPoolClass::kDiagnosticEmergency;

    assert(session_private == northdb::common::MemoryPoolClass::kSessionPrivate);
    assert(query_work == northdb::common::MemoryPoolClass::kQueryWork);
    assert(diagnostic_emergency == northdb::common::MemoryPoolClass::kDiagnosticEmergency);

    return 0;
}
