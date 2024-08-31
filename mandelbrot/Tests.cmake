# Tests.cmake
cmake_minimum_required(VERSION 3.1.0)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Setup
enable_testing()
include(CTest)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/output)
set(LOGS_DIR ${CMAKE_SOURCE_DIR}/tests/output)
set(SCRIPTS_DIR ${CMAKE_SOURCE_DIR}/tests/scripts)

# Function to add custom flags to a build
# It builds the project, runs it and then deletes the executable
# Only for test purposes
function(add_custom_flags_test test_name flags)
    set(temp_target "${EXECUTABLE}_${test_name}")

    add_executable(${temp_target} ${C_SOURCES} ${MAIN_FILE_PATH})

    target_compile_options(${temp_target} PRIVATE ${flags})
    target_link_options(${temp_target} PRIVATE ${flags} -lm)

    add_custom_target(${test_name}
        COMMAND ${CMAKE_COMMAND} --build . --target ${temp_target}
        COMMAND ${CMAKE_COMMAND} -E env bash -c "${CMAKE_BINARY_DIR}/${temp_target}"
        COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/${temp_target}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Building and running ${test_name}..."
    )

    add_test(NAME ${test_name}
        COMMAND ${CMAKE_COMMAND} --build . --target ${test_name}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endfunction()

#
# Sanitizer release tests
#
add_custom_flags_test("sanitizer_address" ${SANITIZER_ADDRESS})
set_tests_properties(sanitizer_address PROPERTIES LABELS "sanitizers")

add_custom_flags_test("sanitizer_undefined" ${SANITIZER_UNDEFINED})
set_tests_properties(sanitizer_undefined PROPERTIES LABELS "sanitizers")

add_custom_flags_test("sanitizer_thread" ${SANITIZER_THREAD})
set_tests_properties(sanitizer_thread PROPERTIES LABELS "sanitizers")

#
# CPU profiling test
#
add_executable(${EXECUTABLE}_profiler ${C_SOURCES} ${MAIN_FILE_PATH})
target_link_options(${EXECUTABLE}_profiler PRIVATE -lm -lprofiler)

add_custom_target(cpu_profiler_gperftools_script
    COMMAND ${CMAKE_COMMAND} -E env bash ${SCRIPTS_DIR}/cpu_profiler_gperftools.sh
    "${CMAKE_BINARY_DIR}/${EXECUTABLE}_profiler"
    "${CMAKE_BINARY_DIR}/mandelbrot.prof"
    "${LOGS_DIR}/cpu_profile_analysis.txt"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running CPU profiler test..."
    DEPENDS ${EXECUTABLE}_profiler
)

add_test(NAME cpu_profiler_gperftools
    COMMAND ${CMAKE_COMMAND} --build . --target cpu_profiler_gperftools_script
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
set_tests_properties(cpu_profiler_gperftools PROPERTIES LABELS "profiling")

#
# Memcheck profiling test
#
add_custom_target(memcheck_valgrind_script
    COMMAND ${CMAKE_COMMAND} -E env bash ${SCRIPTS_DIR}/memcheck_valgrind.sh
    "${CMAKE_BINARY_DIR}/${EXECUTABLE}"
    "${LOGS_DIR}"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running Valgrind Memcheck..."
    DEPENDS ${EXECUTABLE}
)

add_test(NAME memcheck_valgrind
    COMMAND ${CMAKE_COMMAND} --build . --target memcheck_valgrind_script
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
set_tests_properties(memcheck_valgrind PROPERTIES LABELS "profiling")

#
# Gtest
#

# Get gtest lib
include(GoogleTest)
find_library(GTEST_LIB gtest HINTS /usr/lib64)
find_library(GTEST_MAIN_LIB gtest_main HINTS /usr/lib64)

# Find all test files
file(GLOB TEST_SOURCES ${CMAKE_SOURCE_DIR}/tests/performance/*.cpp)

# Add executable for tests
add_executable(test_mandelbrot ${TEST_SOURCES} ${C_SOURCES})

# Add compile flags
target_compile_options(test_mandelbrot PRIVATE ${COMPILE_FLAGS})

# Link libs for tests
target_link_libraries(test_mandelbrot ${GTEST_LIB} ${GTEST_MAIN_LIB} pthread)
target_link_options(test_mandelbrot PRIVATE -lm)

gtest_discover_tests(test_mandelbrot)
