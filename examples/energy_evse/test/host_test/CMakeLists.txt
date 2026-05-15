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
include_directories(../../main/driver/unit)

# 3. Automatically find all source files
file(GLOB LOCAL_SOURCES *.cpp *.c)
file(GLOB DRIVER_SOURCES ../../main/driver/unit/*.cpp)

# 4. Define the executable using the discovered source lists
add_executable(run_tests 
    ${LOCAL_SOURCES}
    ${DRIVER_SOURCES}
)

# 5. Links
target_link_libraries(run_tests)