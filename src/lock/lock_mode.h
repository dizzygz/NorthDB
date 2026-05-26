#pragma once

namespace northdb::lock {

enum class LockMode {
    kShared,
    kUpdate,
    kExclusive,
    kIntentShared,
    kIntentExclusive,
    kSharedIntentExclusive,
    kSchemaShared,
    kSchemaExclusive
};

}  // namespace northdb::lock
