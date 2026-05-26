#pragma once

#include <cstddef>

#include "common/diagnostics.h"

namespace northdb::runtime {

class DiagnosticFlushPolicy {
public:
    virtual ~DiagnosticFlushPolicy() = default;

    virtual bool ShouldFlush(const northdb::common::DiagnosticRecordHeader& header,
                             std::size_t buffered_record_count) const = 0;
};

}  // namespace northdb::runtime
