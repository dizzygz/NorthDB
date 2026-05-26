#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "common/status.h"

namespace northdb::runtime {

class Service {
public:
    virtual ~Service() = default;

    virtual std::string_view Name() const noexcept = 0;
    virtual northdb::common::Status Start() = 0;
    virtual northdb::common::Status Stop() = 0;
};

class ServiceManager {
public:
    using ServiceHandle = std::shared_ptr<Service>;

    northdb::common::Status Register(ServiceHandle service) {
        if (service == nullptr) {
            return northdb::common::Status::Error("service handle must not be null");
        }

        services_.push_back(std::move(service));
        return northdb::common::Status::Ok();
    }

    northdb::common::Status StartAll() {
        for (const auto& service : services_) {
            const auto status = service->Start();
            if (!status.ok()) {
                return status;
            }
        }

        return northdb::common::Status::Ok();
    }

    northdb::common::Status StopAll() {
        for (auto it = services_.rbegin(); it != services_.rend(); ++it) {
            const auto status = (*it)->Stop();
            if (!status.ok()) {
                return status;
            }
        }

        return northdb::common::Status::Ok();
    }

    std::size_t RegisteredServiceCount() const noexcept {
        return services_.size();
    }

private:
    std::vector<ServiceHandle> services_;
};

}  // namespace northdb::runtime
