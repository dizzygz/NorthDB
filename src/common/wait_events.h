#pragma once

namespace northdb::common {

enum class WaitEventClass {
    kNone,
    kLock,
    kLatch,
    kQueue,
    kIo,
    kLogFlush,
    kClient,
    kRemote,
    kDiagnostic
};

}  // namespace northdb::common
