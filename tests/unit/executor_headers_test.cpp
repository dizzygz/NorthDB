#include <cassert>
#include <type_traits>

#include "executor/parallel_execution_policy.h"
#include "executor/table_queue.h"

namespace {

class TestTableQueue final : public northdb::executor::TableQueue {
public:
    explicit TestTableQueue(std::size_t capacity) : capacity_(capacity) {}

    std::size_t Capacity() const noexcept override {
        return capacity_;
    }

    std::size_t Size() const noexcept override {
        return size_;
    }

    bool IsClosed() const noexcept override {
        return closed_;
    }

    void Close() override {
        closed_ = true;
    }

private:
    std::size_t capacity_ = 0;
    std::size_t size_ = 0;
    bool closed_ = false;
};

class TestParallelExecutionPolicy final : public northdb::executor::ParallelExecutionPolicy {
public:
    TestParallelExecutionPolicy(std::size_t worker_count, bool allow_parallel_fanout)
        : worker_count_(worker_count), allow_parallel_fanout_(allow_parallel_fanout) {}

    std::size_t WorkerCount() const noexcept override {
        return worker_count_;
    }

    bool AllowParallelFanout() const noexcept override {
        return allow_parallel_fanout_;
    }

private:
    std::size_t worker_count_ = 0;
    bool allow_parallel_fanout_ = false;
};

}  // namespace

int main() {
    static_assert(std::has_virtual_destructor_v<northdb::executor::TableQueue>);
    static_assert(std::has_virtual_destructor_v<northdb::executor::ParallelExecutionPolicy>);

    TestTableQueue queue(128);
    assert(queue.Capacity() == 128);
    assert(queue.Size() == 0);
    assert(!queue.IsClosed());
    queue.Close();
    assert(queue.IsClosed());

    TestParallelExecutionPolicy parallel_policy(4, true);
    assert(parallel_policy.WorkerCount() == 4);
    assert(parallel_policy.AllowParallelFanout());

    TestParallelExecutionPolicy serial_policy(1, false);
    assert(serial_policy.WorkerCount() == 1);
    assert(!serial_policy.AllowParallelFanout());

    return 0;
}
