#pragma once

#include "common/ids.h"

namespace northdb::storage {

struct StorageCostProfile {
    northdb::common::StorageCostProfileId id = 0;
    double fixed_io_cost = 0.0;
    double sequential_bytes_per_second = 0.0;
    double random_bytes_per_second = 0.0;
};

}  // namespace northdb::storage
