#include <cassert>
#include <type_traits>

#include "common/diagnostics.h"
#include "memory/diagnostic_buffer_memory.h"
#include "runtime/diagnostic_log_service.h"

namespace {

class TestDiagnosticFlushPolicy final : public northdb::runtime::DiagnosticFlushPolicy {
public:
    bool ShouldFlush(const northdb::common::DiagnosticRecordHeader& header,
                     std::size_t buffered_record_count) const override {
        return header.severity == northdb::common::DiagnosticSeverity::kFatal || buffered_record_count >= 2;
    }
};

class TestDiagnosticLogService final : public northdb::runtime::DiagnosticLogService {
public:
    std::string_view Name() const noexcept override {
        return "DiagnosticLogService";
    }

    northdb::common::Status Start() override {
        started_ = true;
        return northdb::common::Status::Ok();
    }

    northdb::common::Status Stop() override {
        stopped_ = true;
        return northdb::common::Status::Ok();
    }

    northdb::common::Status Publish(const northdb::common::DiagnosticRecordHeader& header) override {
        last_header_ = header;
        ++published_count_;
        return northdb::common::Status::Ok();
    }

    northdb::common::Status Flush() override {
        flushed_ = true;
        return northdb::common::Status::Ok();
    }

    void SetFlushPolicy(northdb::runtime::DiagnosticFlushPolicy* policy) override {
        policy_ = policy;
    }

    std::size_t published_count() const noexcept {
        return published_count_;
    }

    const northdb::common::DiagnosticRecordHeader& last_header() const noexcept {
        return last_header_;
    }

    northdb::runtime::DiagnosticFlushPolicy* policy() const noexcept {
        return policy_;
    }

    bool started() const noexcept {
        return started_;
    }

    bool stopped() const noexcept {
        return stopped_;
    }

    bool flushed() const noexcept {
        return flushed_;
    }

private:
    northdb::common::DiagnosticRecordHeader last_header_;
    northdb::runtime::DiagnosticFlushPolicy* policy_ = nullptr;
    std::size_t published_count_ = 0;
    bool started_ = false;
    bool stopped_ = false;
    bool flushed_ = false;
};

}  // namespace

int main() {
    static_assert(std::is_same_v<northdb::common::DiagnosticSequence, std::uint64_t>);
    static_assert(std::is_enum_v<northdb::common::DiagnosticSeverity>);
    static_assert(static_cast<int>(northdb::common::DiagnosticSeverity::kDebug) == 0);
    static_assert(static_cast<int>(northdb::common::DiagnosticSeverity::kInfo) == 1);
    static_assert(static_cast<int>(northdb::common::DiagnosticSeverity::kEvent) == 2);
    static_assert(static_cast<int>(northdb::common::DiagnosticSeverity::kWarn) == 3);
    static_assert(static_cast<int>(northdb::common::DiagnosticSeverity::kError) == 4);
    static_assert(static_cast<int>(northdb::common::DiagnosticSeverity::kFatal) == 5);
    static_assert(std::is_same_v<decltype(northdb::common::DiagnosticRecordHeader::global_sequence), northdb::common::DiagnosticSequence>);
    static_assert(std::is_same_v<decltype(northdb::common::DiagnosticRecordHeader::member_sequence), northdb::common::DiagnosticSequence>);
    static_assert(std::is_same_v<decltype(northdb::common::DiagnosticRecordHeader::severity), northdb::common::DiagnosticSeverity>);
    static_assert(std::is_same_v<decltype(northdb::common::DiagnosticRecordHeader::wait_class), northdb::common::WaitEventClass>);
    static_assert(std::has_virtual_destructor_v<northdb::runtime::DiagnosticFlushPolicy>);
    static_assert(std::has_virtual_destructor_v<northdb::runtime::DiagnosticLogService>);

    northdb::memory::DiagnosticBufferMemory buffer_memory;
    northdb::memory::DiagnosticEmergencyMemory emergency_memory;
    assert(buffer_memory.budget.pool_class == northdb::common::MemoryPoolClass::kDiagnosticBuffer);
    assert(emergency_memory.budget.pool_class == northdb::common::MemoryPoolClass::kDiagnosticEmergency);

    TestDiagnosticFlushPolicy flush_policy;
    TestDiagnosticLogService service;
    assert(service.Start().ok());
    assert(service.started());

    service.SetFlushPolicy(&flush_policy);
    assert(service.policy() == &flush_policy);

    northdb::common::DiagnosticRecordHeader header;
    header.global_sequence = 123;
    header.member_sequence = 45;
    header.severity = northdb::common::DiagnosticSeverity::kWarn;
    header.wait_class = northdb::common::WaitEventClass::kDiagnostic;
    header.flags = 99;

    assert(service.Publish(header).ok());
    assert(service.published_count() == 1);
    assert(service.last_header().global_sequence == 123);
    assert(service.last_header().member_sequence == 45);
    assert(service.last_header().severity == northdb::common::DiagnosticSeverity::kWarn);
    assert(service.last_header().flags == 99);

    assert(!flush_policy.ShouldFlush(service.last_header(), 1));
    assert(flush_policy.ShouldFlush(service.last_header(), 2));
    assert(service.Flush().ok());
    assert(service.flushed());

    assert(service.Stop().ok());
    assert(service.stopped());

    return 0;
}
