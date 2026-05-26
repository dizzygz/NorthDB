#pragma once

#include <cstddef>
#include <string_view>

namespace northdb::optimizer {

class CardinalityEstimator {
public:
    virtual ~CardinalityEstimator() = default;

    virtual std::size_t EstimateRows(std::string_view relation_name) const = 0;
};

}  // namespace northdb::optimizer
