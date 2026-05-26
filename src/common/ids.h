#pragma once

#include <cstdint>

namespace northdb::common {

using InstanceId = std::uint64_t;
using DatabaseId = std::uint64_t;
using SessionId = std::uint64_t;
using TransactionId = std::uint64_t;
using QueryId = std::uint64_t;
using EduId = std::uint64_t;
using TablespaceId = std::uint64_t;
using StorageClassId = std::uint64_t;
using StorageCostProfileId = std::uint64_t;
using TupleId = std::uint64_t;
using PageId = std::uint64_t;

}  // namespace northdb::common
