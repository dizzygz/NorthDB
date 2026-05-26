#pragma once

#include <string_view>

#include "common/status.h"

namespace northdb::optimizer {

class StatisticsCatalog {
public:
    virtual ~StatisticsCatalog() = default;

    virtual northdb::common::Status Refresh(std::string_view object_name) = 0;
};

}  // namespace northdb::optimizer
