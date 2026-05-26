#pragma once

#include <optional>
#include <string_view>

#include "common/status.h"
#include "storage/tablespace.h"

namespace northdb::catalog {

class TablespaceCatalog {
public:
    virtual ~TablespaceCatalog() = default;

    virtual std::optional<northdb::storage::Tablespace> FindById(northdb::common::TablespaceId tablespace_id) const = 0;
    virtual std::optional<northdb::storage::Tablespace> FindByName(std::string_view name) const = 0;
    virtual northdb::common::Status Refresh() = 0;
};

}  // namespace northdb::catalog
