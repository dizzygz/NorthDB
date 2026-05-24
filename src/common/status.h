#pragma once

#include <string>
#include <string_view>

namespace northdb::common {

class Status {
public:
    static Status Ok() {
        return Status(true, "");
    }

    static Status Error(std::string_view message) {
        return Status(false, std::string(message));
    }

    bool ok() const noexcept {
        return ok_;
    }

    const std::string& message() const noexcept {
        return message_;
    }

private:
    Status(bool ok, std::string message)
        : ok_(ok), message_(std::move(message)) {}

    bool ok_;
    std::string message_;
};

}  // namespace northdb::common
