cmake_minimum_required(VERSION 3.20)

project(northdb LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(NORTHDB_BUILD_TESTS "Build NorthDB tests" ON)

add_library(northdb_common INTERFACE)
target_include_directories(northdb_common INTERFACE "${PROJECT_SOURCE_DIR}/src")
target_compile_features(northdb_common INTERFACE cxx_std_20)

add_library(northdb_runtime INTERFACE)
target_link_libraries(northdb_runtime INTERFACE northdb_common)
target_include_directories(northdb_runtime INTERFACE "${PROJECT_SOURCE_DIR}/src")
target_compile_features(northdb_runtime INTERFACE cxx_std_20)

add_library(northdb_memory INTERFACE)
target_link_libraries(northdb_memory INTERFACE northdb_common)
target_include_directories(northdb_memory INTERFACE "${PROJECT_SOURCE_DIR}/src")
target_compile_features(northdb_memory INTERFACE cxx_std_20)

if(NORTHDB_BUILD_TESTS)
    enable_testing()
    add_executable(common_types_test tests/unit/common_types_test.cpp)
    target_link_libraries(common_types_test PRIVATE northdb_common)
    add_test(NAME common_types_test COMMAND common_types_test)

    add_executable(common_wait_events_test tests/unit/common_wait_events_test.cpp)
    target_link_libraries(common_wait_events_test PRIVATE northdb_common)
    add_test(NAME common_wait_events_test COMMAND common_wait_events_test)

    add_executable(common_diagnostics_test tests/unit/common_diagnostics_test.cpp)
    target_link_libraries(common_diagnostics_test PRIVATE northdb_common)
    add_test(NAME common_diagnostics_test COMMAND common_diagnostics_test)

    add_executable(diagnostic_headers_test tests/unit/diagnostic_headers_test.cpp)
    target_link_libraries(diagnostic_headers_test PRIVATE northdb_runtime)
    add_test(NAME diagnostic_headers_test COMMAND diagnostic_headers_test)

    add_executable(common_memory_pools_test tests/unit/common_memory_pools_test.cpp)
    target_link_libraries(common_memory_pools_test PRIVATE northdb_common)
    add_test(NAME common_memory_pools_test COMMAND common_memory_pools_test)

    add_executable(lock_headers_test tests/unit/lock_headers_test.cpp)
    target_link_libraries(lock_headers_test PRIVATE northdb_common)
    add_test(NAME lock_headers_test COMMAND lock_headers_test)

    add_executable(storage_latch_headers_test tests/unit/storage_latch_headers_test.cpp)
    target_link_libraries(storage_latch_headers_test PRIVATE northdb_common)
    add_test(NAME storage_latch_headers_test COMMAND storage_latch_headers_test)

    add_executable(storage_headers_test tests/unit/storage_headers_test.cpp)
    target_link_libraries(storage_headers_test PRIVATE northdb_common)
    add_test(NAME storage_headers_test COMMAND storage_headers_test)

    add_executable(catalog_headers_test tests/unit/catalog_headers_test.cpp)
    target_link_libraries(catalog_headers_test PRIVATE northdb_common)
    add_test(NAME catalog_headers_test COMMAND catalog_headers_test)

    add_executable(storage_identity_headers_test tests/unit/storage_identity_headers_test.cpp)
    target_link_libraries(storage_identity_headers_test PRIVATE northdb_common)
    add_test(NAME storage_identity_headers_test COMMAND storage_identity_headers_test)

    add_executable(executor_headers_test tests/unit/executor_headers_test.cpp)
    target_link_libraries(executor_headers_test PRIVATE northdb_common)
    add_test(NAME executor_headers_test COMMAND executor_headers_test)

    add_executable(concurrency_headers_test tests/unit/concurrency_headers_test.cpp)
    target_link_libraries(concurrency_headers_test PRIVATE northdb_common)
    add_test(NAME concurrency_headers_test COMMAND concurrency_headers_test)

    add_executable(optimizer_headers_test tests/unit/optimizer_headers_test.cpp)
    target_link_libraries(optimizer_headers_test PRIVATE northdb_common)
    add_test(NAME optimizer_headers_test COMMAND optimizer_headers_test)

    add_executable(memory_headers_test tests/unit/memory_headers_test.cpp)
    target_link_libraries(memory_headers_test PRIVATE northdb_memory)
    add_test(NAME memory_headers_test COMMAND memory_headers_test)

    add_executable(memory_context_test tests/unit/memory_context_test.cpp)
    target_link_libraries(memory_context_test PRIVATE northdb_memory)
    add_test(NAME memory_context_test COMMAND memory_context_test)

    add_executable(heap_memory_context_test tests/unit/heap_memory_context_test.cpp)
    target_link_libraries(heap_memory_context_test PRIVATE northdb_memory)
    add_test(NAME heap_memory_context_test COMMAND heap_memory_context_test)

    add_executable(heap_memory_context_limits_test tests/unit/heap_memory_context_limits_test.cpp)
    target_link_libraries(heap_memory_context_limits_test PRIVATE northdb_memory)
    add_test(NAME heap_memory_context_limits_test COMMAND heap_memory_context_limits_test)

    add_executable(diagnostic_memory_test tests/unit/diagnostic_memory_test.cpp)
    target_link_libraries(diagnostic_memory_test PRIVATE northdb_memory)
    add_test(NAME diagnostic_memory_test COMMAND diagnostic_memory_test)

    add_executable(memory_governance_headers_test tests/unit/memory_governance_headers_test.cpp)
    target_link_libraries(memory_governance_headers_test PRIVATE northdb_memory)
    add_test(NAME memory_governance_headers_test COMMAND memory_governance_headers_test)

    add_executable(runtime_states_test tests/unit/runtime_states_test.cpp)
    target_link_libraries(runtime_states_test PRIVATE northdb_runtime)
    add_test(NAME runtime_states_test COMMAND runtime_states_test)

    add_executable(runtime_edu_test tests/unit/runtime_edu_test.cpp)
    target_link_libraries(runtime_edu_test PRIVATE northdb_runtime)
    add_test(NAME runtime_edu_test COMMAND runtime_edu_test)

    add_executable(runtime_instance_test tests/unit/runtime_instance_test.cpp)
    target_link_libraries(runtime_instance_test PRIVATE northdb_runtime northdb_memory)
    add_test(NAME runtime_instance_test COMMAND runtime_instance_test)

    add_executable(runtime_database_test tests/unit/runtime_database_test.cpp)
    target_link_libraries(runtime_database_test PRIVATE northdb_runtime northdb_memory)
    add_test(NAME runtime_database_test COMMAND runtime_database_test)

    add_executable(runtime_session_agent_test tests/unit/runtime_session_agent_test.cpp)
    target_link_libraries(runtime_session_agent_test PRIVATE northdb_runtime northdb_memory)
    add_test(NAME runtime_session_agent_test COMMAND runtime_session_agent_test)

    add_executable(runtime_ownership_flows_test tests/unit/runtime_ownership_flows_test.cpp)
    target_link_libraries(runtime_ownership_flows_test PRIVATE northdb_runtime northdb_memory)
    add_test(NAME runtime_ownership_flows_test COMMAND runtime_ownership_flows_test)

    add_executable(runtime_service_test tests/unit/runtime_service_test.cpp)
    target_link_libraries(runtime_service_test PRIVATE northdb_runtime)
    add_test(NAME runtime_service_test COMMAND runtime_service_test)

    add_executable(runtime_diagnostics_test tests/unit/runtime_diagnostics_test.cpp)
    target_link_libraries(runtime_diagnostics_test PRIVATE northdb_runtime)
    add_test(NAME runtime_diagnostics_test COMMAND runtime_diagnostics_test)

    add_executable(runtime_control_test tests/unit/runtime_control_test.cpp)
    target_link_libraries(runtime_control_test PRIVATE northdb_runtime)
    add_test(NAME runtime_control_test COMMAND runtime_control_test)
endif()
