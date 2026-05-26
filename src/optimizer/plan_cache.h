#pragma once

#include <string_view>

namespace northdb::optimizer {

class PlanCache {
public:
    virtual ~PlanCache() = default;

    virtual bool Contains(std::string_view statement_text) const = 0;
};

}  // namespace northdb::optimizer
