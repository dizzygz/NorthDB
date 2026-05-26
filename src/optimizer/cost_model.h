#pragma once

#include <cstddef>

namespace northdb::optimizer {

class CostModel {
public:
    virtual ~CostModel() = default;

    virtual double EstimateCost(std::size_t row_count) const = 0;
};

}  // namespace northdb::optimizer
