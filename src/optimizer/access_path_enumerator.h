#pragma once

#include <cstddef>
#include <string_view>

namespace northdb::optimizer {

class AccessPathEnumerator {
public:
    virtual ~AccessPathEnumerator() = default;

    virtual std::size_t Enumerate(std::string_view relation_name) const = 0;
};

}  // namespace northdb::optimizer
