#pragma once

#include <cstddef>

namespace northdb::executor {

class ParallelExecutionPolicy {
public:
    virtual ~ParallelExecutionPolicy() = default;

    virtual std::size_t WorkerCount() const noexcept = 0;
    virtual bool AllowParallelFanout() const noexcept = 0;
};

}  // namespace northdb::executor
