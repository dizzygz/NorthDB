#pragma once

namespace northdb::storage {

class Latch {
public:
    virtual ~Latch() = default;

    virtual void Lock() = 0;
    virtual bool TryLock() = 0;
    virtual void Unlock() = 0;
};

}  // namespace northdb::storage
