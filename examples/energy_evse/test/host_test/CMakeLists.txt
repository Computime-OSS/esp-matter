cmake_minimum_required(VERSION 3.10)
project(host_tests)

# 1. Enable Coverage Flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

add_compile_options(-fmacro-prefix-map=${CMAKE_SOURCE_DIR}/=)
add_compile_options(--coverage -O0 -g -fprofile-arcs -ftest-coverage)
add_link_options(--coverage)

# 2. Include directories
include_directories(.) 
include_directories(./mock) 
include_directories(../../main)
include_directories(../../main/driver/unit)
include_directories(../../main/driver/ev_charger/include)

# 3. Automatically find all source files
file(GLOB LOCAL_SOURCES *.cpp *.c)
file(GLOB UNIT_DRIVER_SOURCES ../../main/driver/unit/*.cpp)
# FIX: Renamed variable to avoid overwriting the driver source lists
file(GLOB EV_DRIVER_SOURCES ../../main/driver/ev_charger/app_cmd.cpp)

# 4. Define the executable using the discovered source lists
add_executable(run_tests 
    ${LOCAL_SOURCES}
    ${UNIT_DRIVER_SOURCES}
    ${EV_DRIVER_SOURCES}
)

# 4.5 Add the UNIT_TEST compiler macro definition
# This explicitly replaces '-DUNIT_TEST' in your command-line build string
target_compile_definitions(run_tests PRIVATE UNIT_TEST)

# 5. Links
target_link_libraries(run_tests)