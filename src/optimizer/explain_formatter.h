#pragma once

#include <string>
#include <string_view>

namespace northdb::optimizer {

class ExplainFormatter {
public:
    virtual ~ExplainFormatter() = default;

    virtual std::string Format(std::string_view plan_summary) const = 0;
};

}  // namespace northdb::optimizer
