#include <cassert>
#include <string>
#include <type_traits>

#include "common/ids.h"
#include "common/status.h"

int main() {
    static_assert(std::is_same_v<northdb::common::InstanceId, std::uint64_t>);
    static_assert(std::is_same_v<northdb::common::DatabaseId, std::uint64_t>);
    static_assert(std::is_same_v<northdb::common::SessionId, std::uint64_t>);
    static_assert(std::is_same_v<northdb::common::TransactionId, std::uint64_t>);
    static_assert(std::is_same_v<northdb::common::QueryId, std::uint64_t>);
    static_assert(std::is_same_v<northdb::common::EduId, std::uint64_t>);

    const auto ok_status = northdb::common::Status::Ok();
    assert(ok_status.ok());
    assert(ok_status.message().empty());

    const auto error_status = northdb::common::Status::Error("bootstrap failure");
    assert(!error_status.ok());
    assert(error_status.message() == "bootstrap failure");

    return 0;
}
