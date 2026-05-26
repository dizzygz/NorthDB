#pragma once

#include <string_view>

#include "common/ids.h"

namespace northdb::runtime {

enum class EDUType {
    kAgent,
    kUtilityJob,
    kParallelQueryWorker,
    kPartitionWorker,
    kLogWriter,
    kPageCleaner,
    kCheckpoint,
    kPrefetch,
    kDeadlockDetector,
    kRecoveryMaster,
    kRecoveryWorker,
    kDiagnosticFlusher
};

class EDU {
public:
    virtual ~EDU() = default;

    virtual northdb::common::EduId Id() const noexcept = 0;
    virtual EDUType Type() const noexcept = 0;
    virtual std::string_view Name() const noexcept = 0;
    virtual void Run() = 0;
    virtual void RequestStop() = 0;
};

}  // namespace northdb::runtime
