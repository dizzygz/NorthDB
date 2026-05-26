#include <cassert>
#include <type_traits>

#include "storage/storage_class.h"
#include "storage/storage_cost_profile.h"
#include "storage/tablespace.h"

int main() {
    static_assert(std::is_same_v<decltype(northdb::storage::Tablespace::id), northdb::common::TablespaceId>);
    static_assert(std::is_same_v<decltype(northdb::storage::Tablespace::storage_class_id), northdb::common::StorageClassId>);
    static_assert(std::is_same_v<decltype(northdb::storage::Tablespace::storage_cost_profile_id), northdb::common::StorageCostProfileId>);
    static_assert(std::is_same_v<decltype(northdb::storage::StorageClass::id), northdb::common::StorageClassId>);
    static_assert(std::is_same_v<decltype(northdb::storage::StorageCostProfile::id), northdb::common::StorageCostProfileId>);

    northdb::storage::StorageClass storage_class;
    storage_class.id = 7;
    storage_class.name = "ssd";

    northdb::storage::StorageCostProfile storage_cost_profile;
    storage_cost_profile.id = 11;
    storage_cost_profile.fixed_io_cost = 0.25;
    storage_cost_profile.sequential_bytes_per_second = 1024.0;
    storage_cost_profile.random_bytes_per_second = 256.0;

    northdb::storage::Tablespace tablespace;
    tablespace.id = 3;
    tablespace.storage_class_id = storage_class.id;
    tablespace.storage_cost_profile_id = storage_cost_profile.id;
    tablespace.name = "user_data";

    assert(storage_class.id == 7);
    assert(storage_class.name == "ssd");
    assert(storage_cost_profile.id == 11);
    assert(storage_cost_profile.fixed_io_cost == 0.25);
    assert(storage_cost_profile.sequential_bytes_per_second == 1024.0);
    assert(storage_cost_profile.random_bytes_per_second == 256.0);
    assert(tablespace.id == 3);
    assert(tablespace.storage_class_id == 7);
    assert(tablespace.storage_cost_profile_id == 11);
    assert(tablespace.name == "user_data");

    return 0;
}
