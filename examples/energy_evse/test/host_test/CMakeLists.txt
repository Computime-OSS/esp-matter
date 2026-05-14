cmake_minimum_required(VERSION 3.10)
project(host_tests)

# 1. Enable Coverage Flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

add_compile_options(--coverage -O0 -g -fprofile-arcs -ftest-coverage)
add_link_options(--coverage)

# 2. Include directories (current folder for unity.h and main for charger_main.h)
include_directories(.) 
include_directories(../../main/driver/unit)

# 3. Define the executable - ADD unity.c HERE
add_executable(run_tests 
    test_charger.cpp 
    unity.c
    ../../main/driver/unit/calculate_energy.cpp
    ../../main/driver/unit/get_readable_time.cpp
)

# 4. Links
target_link_libraries(run_tests)