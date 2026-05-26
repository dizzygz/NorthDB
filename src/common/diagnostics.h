#pragma once

#include <cstdint>

#include "common/ids.h"
#include "common/wait_events.h"

namespace northdb::common {

using DiagnosticSequence = std::uint64_t;

enum class DiagnosticSeverity {
    kDebug,
    kInfo,
    kEvent,
    kWarn,
    kError,
    kFatal
};

struct DiagnosticRecordHeader {
    DiagnosticSequence global_sequence = 0;
    DiagnosticSequence member_sequence = 0;
    std::uint64_t event_time_unix_micros = 0;
    std::uint64_t event_time_monotonic_nanos = 0;
    DiagnosticSeverity severity = DiagnosticSeverity::kInfo;
    WaitEventClass wait_class = WaitEventClass::kNone;
    InstanceId instance_id = 0;
    DatabaseId database_id = 0;
    SessionId session_id = 0;
    QueryId query_id = 0;
    EduId edu_id = 0;
    std::uint32_t member_id = 0;
    std::uint32_t process_id = 0;
    std::uint32_t thread_id = 0;
    std::uint32_t component_id = 0;
    std::uint32_t function_id = 0;
    std::uint32_t probe_id = 0;
    std::uint32_t flags = 0;
};

}  // namespace northdb::common
