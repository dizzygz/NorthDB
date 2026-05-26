#pragma once

#include <string>

#include "common/ids.h"

namespace northdb::storage {

struct Tablespace {
    northdb::common::TablespaceId id = 0;
    northdb::common::StorageClassId storage_class_id = 0;
    northdb::common::StorageCostProfileId storage_cost_profile_id = 0;
    std::string name;
};

}  // namespace northdb::storage
