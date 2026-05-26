#pragma once

#include <cstddef>

namespace northdb::executor {

class TableQueue {
public:
    virtual ~TableQueue() = default;

    virtual std::size_t Capacity() const noexcept = 0;
    virtual std::size_t Size() const noexcept = 0;
    virtual bool IsClosed() const noexcept = 0;
    virtual void Close() = 0;
};

}  // namespace northdb::executor
