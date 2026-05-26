#include <cassert>
#include <memory>
#include <string_view>
#include <type_traits>

#include "memory/heap_memory_context.h"
#include "runtime/database_runtime.h"

namespace {

class TestService final : public northdb::runtime::Service {
public:
    explicit TestService(bool fail_on_start = false, bool fail_on_stop = false)
        : fail_on_start_(fail_on_start), fail_on_stop_(fail_on_stop) {}

    std::string_view Name() const noexcept override {
        return "database-service";
    }

    northdb::common::Status Start() override {
        started_ = true;
        if (fail_on_start_) {
            return northdb::common::Status::Error("database service start failure");
        }

        return northdb::common::Status::Ok();
    }

    northdb::common::Status Stop() override {
        stopped_ = true;
        if (fail_on_stop_) {
            return northdb::common::Status::Error("database service stop failure");
        }

        return northdb::common::Status::Ok();
    }

    bool started() const noexcept {
        return started_;
    }

    bool stopped() const noexcept {
        return stopped_;
    }

private:
    bool fail_on_start_ = false;
    bool fail_on_stop_ = false;
    bool started_ = false;
    bool stopped_ = false;
};

}  // namespace

int main() {
    static_assert(std::is_same_v<decltype(std::declval<northdb::runtime::DatabaseRuntime>().State()), northdb::runtime::DatabaseState>);

    auto memory_root = std::make_unique<northdb::memory::HeapMemoryContext>(2048);
    auto* memory_root_ptr = memory_root.get();
    northdb::runtime::DatabaseRuntime runtime(std::move(memory_root));
    assert(runtime.State() == northdb::runtime::DatabaseState::kRegistered);
    assert(runtime.MemoryRoot() == memory_root_ptr);
    assert(runtime.RegisteredServiceCount() == 0);

    auto success_service = std::make_shared<TestService>();
    assert(runtime.RegisterService(success_service).ok());
    assert(runtime.RegisteredServiceCount() == 1);

    assert(runtime.Activate().ok());
    assert(runtime.State() == northdb::runtime::DatabaseState::kMounting);
    assert(runtime.Recover().ok());
    assert(runtime.State() == northdb::runtime::DatabaseState::kActive);
    assert(success_service->started());

    assert(runtime.Quiesce().ok());
    assert(runtime.State() == northdb::runtime::DatabaseState::kQuiesced);
    assert(runtime.Deactivate().ok());
    assert(runtime.State() == northdb::runtime::DatabaseState::kStopped);
    assert(success_service->stopped());

    northdb::runtime::DatabaseRuntime invalid_runtime(std::make_unique<northdb::memory::HeapMemoryContext>(1024));
    const auto invalid_deactivate_status = invalid_runtime.Deactivate();
    assert(!invalid_deactivate_status.ok());
    assert(invalid_deactivate_status.message() == "invalid database state transition");
    assert(invalid_runtime.State() == northdb::runtime::DatabaseState::kRegistered);

    northdb::runtime::DatabaseRuntime start_failure_runtime(std::make_unique<northdb::memory::HeapMemoryContext>(1024));
    auto failing_start_service = std::make_shared<TestService>(true, false);
    assert(start_failure_runtime.RegisterService(failing_start_service).ok());
    assert(start_failure_runtime.Activate().ok());
    const auto start_failure_status = start_failure_runtime.Recover();
    assert(!start_failure_status.ok());
    assert(start_failure_status.message() == "database service start failure");
    assert(start_failure_runtime.State() == northdb::runtime::DatabaseState::kFailed);

    northdb::runtime::DatabaseRuntime stop_failure_runtime(std::make_unique<northdb::memory::HeapMemoryContext>(1024));
    auto failing_stop_service = std::make_shared<TestService>(false, true);
    assert(stop_failure_runtime.RegisterService(failing_stop_service).ok());
    assert(stop_failure_runtime.Activate().ok());
    assert(stop_failure_runtime.Recover().ok());
    assert(stop_failure_runtime.Quiesce().ok());
    const auto stop_failure_status = stop_failure_runtime.Deactivate();
    assert(!stop_failure_status.ok());
    assert(stop_failure_status.message() == "database service stop failure");
    assert(stop_failure_runtime.State() == northdb::runtime::DatabaseState::kFailed);

    return 0;
}
