#include <cassert>
#include <string_view>
#include <type_traits>

#include "runtime/edu.h"

namespace {

class TestEdu final : public northdb::runtime::EDU {
public:
    explicit TestEdu(northdb::common::EduId id) : id_(id) {}

    northdb::common::EduId Id() const noexcept override {
        return id_;
    }

    northdb::runtime::EDUType Type() const noexcept override {
        return northdb::runtime::EDUType::kAgent;
    }

    std::string_view Name() const noexcept override {
        return "TestEdu";
    }

    void Run() override {
        ran_ = true;
    }

    void RequestStop() override {
        stop_requested_ = true;
    }

    bool ran() const noexcept {
        return ran_;
    }

    bool stop_requested() const noexcept {
        return stop_requested_;
    }

private:
    northdb::common::EduId id_;
    bool ran_ = false;
    bool stop_requested_ = false;
};

}  // namespace

int main() {
    static_assert(std::is_enum_v<northdb::runtime::EDUType>);
    static_assert(std::has_virtual_destructor_v<northdb::runtime::EDU>);

    TestEdu edu(42);

    assert(edu.Id() == 42);
    assert(edu.Type() == northdb::runtime::EDUType::kAgent);
    assert(edu.Name() == "TestEdu");
    assert(!edu.ran());
    assert(!edu.stop_requested());

    edu.Run();
    edu.RequestStop();

    assert(edu.ran());
    assert(edu.stop_requested());

    return 0;
}
