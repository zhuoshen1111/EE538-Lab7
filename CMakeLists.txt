cmake_minimum_required(VERSION 3.20)
project(EE538HeapHashLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(LAB_ENABLE_EXTRA_CREDIT "Build and test the optional log hash chain" OFF)
option(BUILD_TESTING "Build the separate local check programs" OFF)
include(CTest)

function(lab_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive-)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

add_library(heap_impl src/min_heap.cpp)
target_include_directories(heap_impl PUBLIC include)
lab_warnings(heap_impl)

add_executable(heap_demo heap_main.cpp)
target_link_libraries(heap_demo PRIVATE heap_impl)
lab_warnings(heap_demo)

add_library(hash_table_impl src/chained_hash_table.cpp)
target_include_directories(hash_table_impl PUBLIC include)
lab_warnings(hash_table_impl)

add_executable(hash_table_demo hash_table_main.cpp)
target_link_libraries(hash_table_demo PRIVATE hash_table_impl)
lab_warnings(hash_table_demo)

if(BUILD_TESTING OR LAB_ENABLE_EXTRA_CREDIT)
    add_library(sha256_helper src/sha256.cpp)
    target_include_directories(sha256_helper PUBLIC include)
    lab_warnings(sha256_helper)
endif()

if(BUILD_TESTING)
    add_executable(lab_tests tests/lab_tests.cpp)
    target_link_libraries(lab_tests PRIVATE heap_impl hash_table_impl)
    lab_warnings(lab_tests)

    add_executable(test_sha256 tests/test_sha256.cpp)
    target_link_libraries(test_sha256 PRIVATE sha256_helper)
    lab_warnings(test_sha256)

    add_test(NAME core_heap_and_hash COMMAND lab_tests)
    add_test(NAME provided_sha256_helper COMMAND test_sha256)
endif()

if(LAB_ENABLE_EXTRA_CREDIT)
    add_library(lab_log_chain src/log_chain.cpp)
    target_link_libraries(lab_log_chain PUBLIC sha256_helper)
    lab_warnings(lab_log_chain)

    add_executable(log_chain_demo log_chain_main.cpp)
    target_link_libraries(log_chain_demo PRIVATE lab_log_chain)
    lab_warnings(log_chain_demo)

    if(BUILD_TESTING)
        add_executable(extra_credit_tests tests/extra_credit_tests.cpp)
        target_link_libraries(extra_credit_tests PRIVATE lab_log_chain)
        lab_warnings(extra_credit_tests)
        add_test(NAME extra_credit_log_chain COMMAND extra_credit_tests)
    endif()
endif()
