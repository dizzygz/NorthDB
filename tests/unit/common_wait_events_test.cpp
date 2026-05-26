#include <cassert>
#include <type_traits>

#include "common/wait_events.h"

int main() {
    static_assert(std::is_enum_v<northdb::common::WaitEventClass>);

    const auto lock_wait = northdb::common::WaitEventClass::kLock;
    const auto queue_wait = northdb::common::WaitEventClass::kQueue;
    const auto diagnostic_wait = northdb::common::WaitEventClass::kDiagnostic;

    assert(lock_wait == northdb::common::WaitEventClass::kLock);
    assert(queue_wait == northdb::common::WaitEventClass::kQueue);
    assert(diagnostic_wait == northdb::common::WaitEventClass::kDiagnostic);

    return 0;
}
