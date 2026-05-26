#include <cassert>
#include <type_traits>

#include "common/diagnostics.h"

int main() {
    static_assert(std::is_same_v<northdb::common::DiagnosticSequence, std::uint64_t>);
    static_assert(std::is_enum_v<northdb::common::DiagnosticSeverity>);
    static_assert(std::is_same_v<decltype(northdb::common::DiagnosticRecordHeader::global_sequence), northdb::common::DiagnosticSequence>);
    static_assert(std::is_same_v<decltype(northdb::common::DiagnosticRecordHeader::severity), northdb::common::DiagnosticSeverity>);
    static_assert(std::is_same_v<decltype(northdb::common::DiagnosticRecordHeader::wait_class), northdb::common::WaitEventClass>);

    northdb::common::DiagnosticRecordHeader header;
    header.global_sequence = 101;
    header.member_sequence = 55;
    header.event_time_unix_micros = 123456;
    header.event_time_monotonic_nanos = 654321;
    header.severity = northdb::common::DiagnosticSeverity::kError;
    header.wait_class = northdb::common::WaitEventClass::kDiagnostic;
    header.instance_id = 1;
    header.database_id = 2;
    header.session_id = 3;
    header.query_id = 4;
    header.edu_id = 5;
    header.member_id = 6;
    header.process_id = 7;
    header.thread_id = 8;
    header.component_id = 9;
    header.function_id = 10;
    header.probe_id = 11;
    header.flags = 12;

    assert(header.global_sequence == 101);
    assert(header.member_sequence == 55);
    assert(header.event_time_unix_micros == 123456);
    assert(header.event_time_monotonic_nanos == 654321);
    assert(header.severity == northdb::common::DiagnosticSeverity::kError);
    assert(header.wait_class == northdb::common::WaitEventClass::kDiagnostic);
    assert(header.instance_id == 1);
    assert(header.database_id == 2);
    assert(header.session_id == 3);
    assert(header.query_id == 4);
    assert(header.edu_id == 5);
    assert(header.member_id == 6);
    assert(header.process_id == 7);
    assert(header.thread_id == 8);
    assert(header.component_id == 9);
    assert(header.function_id == 10);
    assert(header.probe_id == 11);
    assert(header.flags == 12);

    return 0;
}
