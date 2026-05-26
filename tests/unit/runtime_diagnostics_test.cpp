#include <cassert>
#include <string_view>
#include <type_traits>

#include "runtime/diagnostic_log_service.h"

namespace {

class TestDiagnosticFlushPolicy final : public northdb::runtime::DiagnosticFlushPolicy {
public:
    bool ShouldFlush(const northdb::common::DiagnosticRecordHeader& header,
                     std::size_t buffered_record_count) const override {
        return header.severity == northdb::common::DiagnosticSeverity::kFatal || buffered_record_count >= 4;
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

    bool started() const noexcept {
        return started_;
    }

    bool stopped() const noexcept {
        return stopped_;
    }

    bool flushed() const noexcept {
        return flushed_;
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
    static_assert(std::has_virtual_destructor_v<northdb::runtime::DiagnosticFlushPolicy>);
    static_assert(std::has_virtual_destructor_v<northdb::runtime::DiagnosticLogService>);
    static_assert(std::is_base_of_v<northdb::runtime::Service, northdb::runtime::DiagnosticLogService>);

    TestDiagnosticFlushPolicy flush_policy;
    TestDiagnosticLogService service;

    assert(service.Name() == "DiagnosticLogService");
    assert(service.Start().ok());
    assert(service.started());

    service.SetFlushPolicy(&flush_policy);
    assert(service.policy() == &flush_policy);

    northdb::common::DiagnosticRecordHeader header;
    header.global_sequence = 88;
    header.severity = northdb::common::DiagnosticSeverity::kFatal;
    header.wait_class = northdb::common::WaitEventClass::kDiagnostic;

    assert(service.Publish(header).ok());
    assert(service.published_count() == 1);
    assert(service.last_header().global_sequence == 88);
    assert(service.last_header().severity == northdb::common::DiagnosticSeverity::kFatal);

    assert(flush_policy.ShouldFlush(service.last_header(), 1));
    assert(service.Flush().ok());
    assert(service.flushed());

    assert(service.Stop().ok());
    assert(service.stopped());

    return 0;
}
