#include <cassert>
#include <optional>
#include <type_traits>

#include "catalog/tablespace_catalog.h"
#include "common/ids.h"
#include "storage/storage_class.h"
#include "storage/storage_cost_profile.h"
#include "storage/tablespace.h"

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
        .id = 21,
        .storage_class_id = 34,
        .storage_cost_profile_id = 55,
        .name = "warehouse",
    };
    bool refreshed_ = false;
};

}  // namespace

int main() {
    static_assert(std::is_same_v<northdb::common::TablespaceId, std::uint64_t>);
    static_assert(std::is_same_v<northdb::common::StorageClassId, std::uint64_t>);
    static_assert(std::is_same_v<northdb::common::StorageCostProfileId, std::uint64_t>);
    static_assert(std::is_same_v<northdb::common::TupleId, std::uint64_t>);
    static_assert(std::is_same_v<northdb::common::PageId, std::uint64_t>);
    static_assert(std::has_virtual_destructor_v<northdb::catalog::TablespaceCatalog>);

    northdb::storage::StorageClass storage_class;
    storage_class.id = 34;
    storage_class.name = "nvme";

    northdb::storage::StorageCostProfile storage_cost_profile;
    storage_cost_profile.id = 55;
    storage_cost_profile.fixed_io_cost = 0.1;
    storage_cost_profile.sequential_bytes_per_second = 2048.0;
    storage_cost_profile.random_bytes_per_second = 768.0;

    northdb::storage::Tablespace tablespace;
    tablespace.id = 21;
    tablespace.storage_class_id = storage_class.id;
    tablespace.storage_cost_profile_id = storage_cost_profile.id;
    tablespace.name = "warehouse";

    assert(tablespace.id == 21);
    assert(tablespace.storage_class_id == 34);
    assert(tablespace.storage_cost_profile_id == 55);
    assert(storage_cost_profile.fixed_io_cost == 0.1);
    assert(storage_class.name == "nvme");

    TestTablespaceCatalog catalog;
    const auto by_id = catalog.FindById(tablespace.id);
    assert(by_id.has_value());
    assert(by_id->name == "warehouse");

    const auto by_name = catalog.FindByName("warehouse");
    assert(by_name.has_value());
    assert(by_name->storage_class_id == 34);

    const auto missing = catalog.FindByName("archive");
    assert(!missing.has_value());

    assert(catalog.Refresh().ok());
    assert(catalog.refreshed());

    return 0;
}
