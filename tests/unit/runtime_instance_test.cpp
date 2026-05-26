#include <cassert>
#include <memory>
#include <string_view>
#include <type_traits>

#include "memory/heap_memory_context.h"
#include "runtime/db_instance.h"

namespace {

class TestService final : public northdb::runtime::Service {
public:
    explicit TestService(bool fail_on_start = false, bool fail_on_stop = false)
        : fail_on_start_(fail_on_start), fail_on_stop_(fail_on_stop) {}

    std::string_view Name() const noexcept override {
        return "test-service";
    }

    northdb::common::Status Start() override {
        started_ = true;
        if (fail_on_start_) {
            return northdb::common::Status::Error("service start failure");
        }

        return northdb::common::Status::Ok();
    }

    northdb::common::Status Stop() override {
        stopped_ = true;
        if (fail_on_stop_) {
            return northdb::common::Status::Error("service stop failure");
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
    static_assert(std::is_same_v<decltype(std::declval<northdb::runtime::DBInstance>().State()), northdb::runtime::InstanceState>);

    auto memory_root = std::make_unique<northdb::memory::HeapMemoryContext>(4096);
    auto* memory_root_ptr = memory_root.get();
    northdb::runtime::DBInstance instance(std::move(memory_root));
    assert(instance.State() == northdb::runtime::InstanceState::kCreated);
    assert(instance.MemoryRoot() == memory_root_ptr);
    assert(instance.RegisteredServiceCount() == 0);

    auto success_service = std::make_shared<TestService>();
    assert(instance.RegisterService(success_service).ok());
    assert(instance.RegisteredServiceCount() == 1);

    assert(instance.Initialize().ok());
    assert(instance.State() == northdb::runtime::InstanceState::kInitializing);
    assert(instance.Start().ok());
    assert(instance.State() == northdb::runtime::InstanceState::kRunning);
    assert(success_service->started());

    assert(instance.Quiesce().ok());
    assert(instance.State() == northdb::runtime::InstanceState::kQuiescing);
    assert(instance.Stop().ok());
    assert(instance.State() == northdb::runtime::InstanceState::kStopped);
    assert(success_service->stopped());

    northdb::runtime::DBInstance invalid_instance(std::make_unique<northdb::memory::HeapMemoryContext>(1024));
    const auto invalid_stop_status = invalid_instance.Stop();
    assert(!invalid_stop_status.ok());
    assert(invalid_stop_status.message() == "invalid instance state transition");
    assert(invalid_instance.State() == northdb::runtime::InstanceState::kCreated);

    northdb::runtime::DBInstance start_failure_instance(std::make_unique<northdb::memory::HeapMemoryContext>(1024));
    auto failing_start_service = std::make_shared<TestService>(true, false);
    assert(start_failure_instance.RegisterService(failing_start_service).ok());
    assert(start_failure_instance.Initialize().ok());
    const auto start_failure_status = start_failure_instance.Start();
    assert(!start_failure_status.ok());
    assert(start_failure_status.message() == "service start failure");
    assert(start_failure_instance.State() == northdb::runtime::InstanceState::kFailed);

    northdb::runtime::DBInstance stop_failure_instance(std::make_unique<northdb::memory::HeapMemoryContext>(1024));
    auto failing_stop_service = std::make_shared<TestService>(false, true);
    assert(stop_failure_instance.RegisterService(failing_stop_service).ok());
    assert(stop_failure_instance.Initialize().ok());
    assert(stop_failure_instance.Start().ok());
    assert(stop_failure_instance.Quiesce().ok());
    const auto stop_failure_status = stop_failure_instance.Stop();
    assert(!stop_failure_status.ok());
    assert(stop_failure_status.message() == "service stop failure");
    assert(stop_failure_instance.State() == northdb::runtime::InstanceState::kFailed);

    return 0;
}
