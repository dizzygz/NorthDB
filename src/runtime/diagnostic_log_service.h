#pragma once

#include "common/diagnostics.h"
#include "common/status.h"
#include "runtime/diagnostic_flush_policy.h"
#include "runtime/service.h"

namespace northdb::runtime {

class DiagnosticLogService : public Service {
public:
    ~DiagnosticLogService() override = default;

    virtual northdb::common::Status Publish(const northdb::common::DiagnosticRecordHeader& header) = 0;
    virtual northdb::common::Status Flush() = 0;
    virtual void SetFlushPolicy(DiagnosticFlushPolicy* policy) = 0;
};

}  // namespace northdb::runtime
