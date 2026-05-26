#include <cassert>
#include <optional>
#include <type_traits>

#include "catalog/tablespace_catalog.h"

namespace {

class TestTablespaceCatalog final : public northdb::catalog::TablespaceCatalog {
public:
    std::optional<northdb::storage::Tablespace> FindById(northdb::common::TablespaceId tablespace_id) const override {
        if (tablespace_id != tablespace_.id) {
            return std::nullopt;
        }

        return tablespace_;
    }

    std::optional<northdb::storage::Tablespace> FindByName(std::string_view name) const override {
        if (name != tablespace_.name) {
            return std::nullopt;
        }

        return tablespace_;
    }

    northdb::common::Status Refresh() override {
        refreshed_ = true;
        return northdb::common::Status::Ok();
    }

    bool refreshed() const noexcept {
        return refreshed_;
    }

private:
    northdb::storage::Tablespace tablespace_{
        .id = 5,
        .storage_class_id = 7,
        .storage_cost_profile_id = 11,
        .name = "user_data",
    };
    bool refreshed_ = false;
};

}  // namespace

int main() {
    static_assert(std::has_virtual_destructor_v<northdb::catalog::TablespaceCatalog>);

    TestTablespaceCatalog catalog;

    const auto by_id = catalog.FindById(5);
    assert(by_id.has_value());
    assert(by_id->storage_class_id == 7);
    assert(by_id->storage_cost_profile_id == 11);

    const auto missing_id = catalog.FindById(99);
    assert(!missing_id.has_value());

    const auto by_name = catalog.FindByName("user_data");
    assert(by_name.has_value());
    assert(by_name->id == 5);

    const auto missing_name = catalog.FindByName("archive");
    assert(!missing_name.has_value());

    const auto refresh_status = catalog.Refresh();
    assert(refresh_status.ok());
    assert(catalog.refreshed());

    return 0;
}
