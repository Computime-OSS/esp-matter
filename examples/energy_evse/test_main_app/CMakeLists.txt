cmake_minimum_required(VERSION 3.16)
project(test_main_app_host LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EVSE_MAIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../main")

# Default OFF. Enable with: cmake -DCT_HOST_UNIT_TEST_VERBOSE=ON ...
# Pytest passes this from env CT_HOST_UNIT_TEST_VERBOSE when set.
option(CT_HOST_UNIT_TEST_VERBOSE "Log host runner + ../main GetReadableTime (host build only)" OFF)

add_executable(host_unit_test_runner
    "${EVSE_MAIN_DIR}/driver/ev_charger/get_readable_time.cpp"
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
