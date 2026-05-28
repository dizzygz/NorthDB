#include <cassert>
#include <chrono>
#include <cstdint>
#include <thread>
#include <type_traits>

#include "runtime/process_local_wait_post.h"
#include "runtime/wait_post.h"

int main() {
    static_assert(std::has_virtual_destructor_v<northdb::runtime::WaitPost>);
    static_assert(std::is_enum_v<northdb::runtime::WaitPost::Mode>);
    static_assert(std::is_base_of_v<northdb::runtime::WaitPost, northdb::runtime::ProcessLocalWaitPost>);

    northdb::runtime::ProcessLocalWaitPost sticky_wait_post(
        northdb::runtime::WaitPost::Mode::kStickyEvent);
    assert(sticky_wait_post.mode() == northdb::runtime::WaitPost::Mode::kStickyEvent);
    assert(sticky_wait_post.WaiterCount() == 0);
    assert(!sticky_wait_post.IsPosted());
    assert(sticky_wait_post.Generation() == 0);

    northdb::common::Status wait_status = northdb::common::Status::Error("not started");
    std::thread waiting_thread([&]() {
        wait_status = sticky_wait_post.Wait(std::chrono::milliseconds(200));
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    assert(sticky_wait_post.WaiterCount() == 1);

    assert(sticky_wait_post.Post().ok());
    waiting_thread.join();
    assert(wait_status.ok());
    assert(sticky_wait_post.WaiterCount() == 0);
    assert(sticky_wait_post.IsPosted());
    assert(sticky_wait_post.Generation() == 0);

    assert(sticky_wait_post.Wait(std::chrono::milliseconds(1)).ok());

    assert(sticky_wait_post.Reset().ok());
    assert(!sticky_wait_post.IsPosted());
    assert(sticky_wait_post.Generation() == 0);

    const auto timeout_status = sticky_wait_post.Wait(std::chrono::milliseconds(5));
    assert(!timeout_status.ok());
    assert(timeout_status.message() == "waitpost timed out");
    assert(sticky_wait_post.WaiterCount() == 0);

    northdb::runtime::ProcessLocalWaitPost auto_reset_wait_post(
        northdb::runtime::WaitPost::Mode::kAutoReset);
    assert(auto_reset_wait_post.mode() == northdb::runtime::WaitPost::Mode::kAutoReset);
    assert(auto_reset_wait_post.Post().ok());
    assert(auto_reset_wait_post.IsPosted());

    return 0;
}
