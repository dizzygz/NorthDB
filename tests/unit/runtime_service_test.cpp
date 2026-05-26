#include <cassert>
#include <string>
#include <memory>
#include <string_view>
#include <type_traits>
#include <vector>

#include "runtime/service.h"

namespace {

class TestService final : public northdb::runtime::Service {
public:
    TestService(std::string_view name, std::vector<std::string>* events)
        : name_(name), events_(events) {}

    std::string_view Name() const noexcept override {
        return name_;
    }

    northdb::common::Status Start() override {
        events_->push_back(std::string(name_) + ":start");
        started_ = true;
        return northdb::common::Status::Ok();
    }

    northdb::common::Status Stop() override {
        events_->push_back(std::string(name_) + ":stop");
        stopped_ = true;
        return northdb::common::Status::Ok();
    }

    bool started() const noexcept {
        return started_;
    }

    bool stopped() const noexcept {
        return stopped_;
    }

private:
    std::string_view name_;
    std::vector<std::string>* events_;
    bool started_ = false;
    bool stopped_ = false;
};

class FailingService final : public northdb::runtime::Service {
public:
    FailingService(std::string_view name,
                   std::vector<std::string>* events,
                   bool fail_on_start,
                   bool fail_on_stop)
        : name_(name),
          events_(events),
          fail_on_start_(fail_on_start),
          fail_on_stop_(fail_on_stop) {}

    std::string_view Name() const noexcept override {
        return name_;
    }

    northdb::common::Status Start() override {
        events_->push_back(std::string(name_) + ":start");
        if (fail_on_start_) {
            return northdb::common::Status::Error("start failure");
        }

        return northdb::common::Status::Ok();
    }

    northdb::common::Status Stop() override {
        events_->push_back(std::string(name_) + ":stop");
        if (fail_on_stop_) {
            return northdb::common::Status::Error("stop failure");
        }

        return northdb::common::Status::Ok();
    }

private:
    std::string_view name_;
    std::vector<std::string>* events_;
    bool fail_on_start_;
    bool fail_on_stop_;
};

}  // namespace

int main() {
    static_assert(std::has_virtual_destructor_v<northdb::runtime::Service>);

    northdb::runtime::ServiceManager manager;
    assert(manager.RegisteredServiceCount() == 0);

    std::vector<std::string> lifecycle_events;
    auto service = std::make_shared<TestService>("DiagnosticLogService", &lifecycle_events);

    const auto register_status = manager.Register(service);
    assert(register_status.ok());
    assert(manager.RegisteredServiceCount() == 1);
    assert(service->Name() == "DiagnosticLogService");

    const auto start_status = manager.StartAll();
    assert(start_status.ok());
    assert(service->started());
    assert((lifecycle_events == std::vector<std::string>{"DiagnosticLogService:start"}));

    const auto stop_status = manager.StopAll();
    assert(stop_status.ok());
    assert(service->stopped());
    assert((lifecycle_events == std::vector<std::string>{
        "DiagnosticLogService:start",
        "DiagnosticLogService:stop"}));

    const auto null_status = manager.Register(nullptr);
    assert(!null_status.ok());
    assert(null_status.message() == "service handle must not be null");

    std::vector<std::string> ordering_events;
    northdb::runtime::ServiceManager ordering_manager;
    auto first = std::make_shared<TestService>("first", &ordering_events);
    auto second = std::make_shared<TestService>("second", &ordering_events);
    auto third = std::make_shared<TestService>("third", &ordering_events);

    assert(ordering_manager.Register(first).ok());
    assert(ordering_manager.Register(second).ok());
    assert(ordering_manager.Register(third).ok());
    assert(ordering_manager.StartAll().ok());
    assert(ordering_manager.StopAll().ok());

    assert((ordering_events == std::vector<std::string>{
        "first:start",
        "second:start",
        "third:start",
        "third:stop",
        "second:stop",
        "first:stop"}));

    std::vector<std::string> start_failure_events;
    northdb::runtime::ServiceManager start_failure_manager;
    auto start_ok = std::make_shared<TestService>("start-ok", &start_failure_events);
    auto start_fail = std::make_shared<FailingService>("start-fail", &start_failure_events, true, false);
    auto start_never = std::make_shared<TestService>("start-never", &start_failure_events);

    assert(start_failure_manager.Register(start_ok).ok());
    assert(start_failure_manager.Register(start_fail).ok());
    assert(start_failure_manager.Register(start_never).ok());

    const auto start_failure_status = start_failure_manager.StartAll();
    assert(!start_failure_status.ok());
    assert(start_failure_status.message() == "start failure");
    assert((start_failure_events == std::vector<std::string>{
        "start-ok:start",
        "start-fail:start"}));

    std::vector<std::string> stop_failure_events;
    northdb::runtime::ServiceManager stop_failure_manager;
    auto stop_first = std::make_shared<TestService>("stop-first", &stop_failure_events);
    auto stop_fail = std::make_shared<FailingService>("stop-fail", &stop_failure_events, false, true);
    auto stop_last = std::make_shared<TestService>("stop-last", &stop_failure_events);

    assert(stop_failure_manager.Register(stop_first).ok());
    assert(stop_failure_manager.Register(stop_fail).ok());
    assert(stop_failure_manager.Register(stop_last).ok());
    assert(stop_failure_manager.StartAll().ok());

    const auto stop_failure_status = stop_failure_manager.StopAll();
    assert(!stop_failure_status.ok());
    assert(stop_failure_status.message() == "stop failure");
    assert((stop_failure_events == std::vector<std::string>{
        "stop-first:start",
        "stop-fail:start",
        "stop-last:start",
        "stop-last:stop",
        "stop-fail:stop"}));

    return 0;
}
