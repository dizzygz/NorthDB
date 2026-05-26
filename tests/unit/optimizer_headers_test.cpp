#include <cassert>
#include <type_traits>

#include "common/status.h"
#include "optimizer/access_path_enumerator.h"
#include "optimizer/cardinality_estimator.h"
#include "optimizer/cost_model.h"
#include "optimizer/explain_formatter.h"
#include "optimizer/plan_cache.h"
#include "optimizer/statistics_catalog.h"

namespace {

class TestStatisticsCatalog final : public northdb::optimizer::StatisticsCatalog {
public:
    northdb::common::Status Refresh(std::string_view object_name) override {
        last_object_name_ = object_name;
        return northdb::common::Status::Ok();
    }

    std::string_view last_object_name() const noexcept {
        return last_object_name_;
    }

private:
    std::string_view last_object_name_;
};

class TestCardinalityEstimator final : public northdb::optimizer::CardinalityEstimator {
public:
    std::size_t EstimateRows(std::string_view relation_name) const override {
        return relation_name.size() * 10;
    }
};

class TestAccessPathEnumerator final : public northdb::optimizer::AccessPathEnumerator {
public:
    std::size_t Enumerate(std::string_view relation_name) const override {
        return relation_name.empty() ? 0 : 3;
    }
};

class TestCostModel final : public northdb::optimizer::CostModel {
public:
    double EstimateCost(std::size_t row_count) const override {
        return static_cast<double>(row_count) * 1.5;
    }
};

class TestPlanCache final : public northdb::optimizer::PlanCache {
public:
    bool Contains(std::string_view statement_text) const override {
        return statement_text == "select 1";
    }
};

class TestExplainFormatter final : public northdb::optimizer::ExplainFormatter {
public:
    std::string Format(std::string_view plan_summary) const override {
        return std::string("EXPLAIN: ") + std::string(plan_summary);
    }
};

}  // namespace

int main() {
    static_assert(std::has_virtual_destructor_v<northdb::optimizer::StatisticsCatalog>);
    static_assert(std::has_virtual_destructor_v<northdb::optimizer::CardinalityEstimator>);
    static_assert(std::has_virtual_destructor_v<northdb::optimizer::AccessPathEnumerator>);
    static_assert(std::has_virtual_destructor_v<northdb::optimizer::CostModel>);
    static_assert(std::has_virtual_destructor_v<northdb::optimizer::PlanCache>);
    static_assert(std::has_virtual_destructor_v<northdb::optimizer::ExplainFormatter>);

    TestStatisticsCatalog statistics_catalog;
    const auto refresh_status = statistics_catalog.Refresh("orders");
    assert(refresh_status.ok());
    assert(statistics_catalog.last_object_name() == "orders");

    TestCardinalityEstimator cardinality_estimator;
    assert(cardinality_estimator.EstimateRows("orders") == 60);

    TestAccessPathEnumerator access_path_enumerator;
    assert(access_path_enumerator.Enumerate("") == 0);
    assert(access_path_enumerator.Enumerate("orders") == 3);

    TestCostModel cost_model;
    assert(cost_model.EstimateCost(4) == 6.0);

    TestPlanCache plan_cache;
    assert(plan_cache.Contains("select 1"));
    assert(!plan_cache.Contains("select 2"));

    TestExplainFormatter explain_formatter;
    assert(explain_formatter.Format("table scan") == "EXPLAIN: table scan");

    return 0;
}
