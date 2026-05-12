cmake_minimum_required(VERSION 3.16)
project(test_main_app_host LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EVSE_MAIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../main")

# Default OFF. Enable with: cmake -DCT_HOST_UNIT_TEST_VERBOSE=ON ...
# Pytest passes this from env CT_HOST_UNIT_TEST_VERBOSE when set.
option(CT_HOST_UNIT_TEST_VERBOSE "Log host runner + ../main helpers (host build only)" OFF)

# Instrument for gcov/lcov (host build only). Use scripts/run_host_coverage.sh — see that script
# for how this relates to ../main sources linked into host_unit_test_runner.
option(CT_HOST_COVERAGE "Instrument host_unit_test_runner for gcov/lcov (non-MSVC)" OFF)

add_executable(host_unit_test_runner
    "${EVSE_MAIN_DIR}/driver/ev_charger/get_readable_time.cpp"
    "${EVSE_MAIN_DIR}/driver/ev_charger/matter_schedule_allow.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/host_unit_test_runner.cpp"
)

target_include_directories(host_unit_test_runner PRIVATE
    "${EVSE_MAIN_DIR}/driver/ev_charger/include"
)

if(CT_HOST_UNIT_TEST_VERBOSE)
    target_compile_definitions(host_unit_test_runner PRIVATE CT_HOST_TEST_LOG_GET_READABLE_TIME=1)
endif()

if(MSVC)
    target_compile_options(host_unit_test_runner PRIVATE /W4)
else()
    target_compile_options(host_unit_test_runner PRIVATE -Wall -Wextra -Werror)
endif()

if(CT_HOST_COVERAGE AND NOT MSVC)
    target_compile_options(host_unit_test_runner PRIVATE --coverage -g -O0)
    target_link_options(host_unit_test_runner PRIVATE --coverage)
endif()
