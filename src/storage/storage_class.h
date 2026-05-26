#pragma once

#include <string>

#include "common/ids.h"

namespace northdb::storage {

struct StorageClass {
    northdb::common::StorageClassId id = 0;
    std::string name;
};

}  // namespace northdb::storage
