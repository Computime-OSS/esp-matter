#!/usr/bin/env bash
# Host coverage for test_main_app: instruments and runs the same native binary that pytest uses,
# including production sources under ../main/ that are linked into host_unit_test_runner only.
#
# Relationship to main/:
#   - Only .cpp files listed in test_main_app/CMakeLists.txt (paths under ../main/...) get .gcno/.gcda.
#   - Full firmware (idf.py build) is NOT measured here; Matter/ESP-only code in main/ is excluded
#     unless you add it to the host CMake target.
#
# Prerequisites: cmake, C++ compiler, optional lcov+genhtml for HTML.
set -euo pipefail

ROOT="$(cd "$(dirname "${0}")/.." && pwd)"
BUILD="${ROOT}/build_host_coverage"
BIN="${BUILD}/host_unit_test_runner"

command -v cmake >/dev/null 2>&1 || {
    echo "cmake is required" >&2
    exit 1
}

rm -rf "${BUILD}"
cmake -S "${ROOT}" -B "${BUILD}" \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCT_HOST_COVERAGE=ON \
    -DCT_HOST_UNIT_TEST_VERBOSE=OFF

cmake --build "${BUILD}" --target host_unit_test_runner

export TZ=UTC
"${BIN}" all

echo "Gcov data written under ${BUILD} (see CMakeFiles/**.gcda)."

if command -v lcov >/dev/null 2>&1 && command -v genhtml >/dev/null 2>&1; then
    lcov --capture --directory "${BUILD}" \
        --ignore-errors unsupported,mismatch \
        --output-file "${BUILD}/coverage.info" --quiet
    genhtml "${BUILD}/coverage.info" --output-directory "${BUILD}/html" --quiet
    echo "HTML coverage report: file://${BUILD}/html/index.html"
else
    echo "Install lcov (lcov + genhtml) for HTML; or inspect .gcno/.gcda under ${BUILD}." >&2
fi
