cmake_minimum_required(VERSION 3.15)

enable_testing()
include(CTest)
include(GoogleTest)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/output)

set(LOGS_DIR ${CMAKE_CURRENT_LIST_DIR}/output)
set(SCRIPTS_DIR ${CMAKE_CURRENT_LIST_DIR}/scripts)

set(SANITIZER_ADDRESS -fsanitize=address)
set(SANITIZER_THREAD -fsanitize=thread)
set(SANITIZER_UNDEFINED -fsanitize=undefined)

function(add_custom_flags_test test_name sanitizer_flag)
    set(temp_target "${EXECUTABLE}_${test_name}")

    add_executable(${temp_target} ${C_SOURCES} ${MAIN_FILE_PATH})

    target_link_libraries(${temp_target}
        PRIVATE
        global_flags
    )

    target_compile_options(${temp_target} PRIVATE ${sanitizer_flag})
    target_link_options(${temp_target} PRIVATE ${sanitizer_flag})

    add_test(NAME ${test_name}
        COMMAND ${temp_target}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endfunction()

#
# Sanitizer release tests
#
add_custom_flags_test("sanitizer_address" ${SANITIZER_ADDRESS})
set_tests_properties(sanitizer_address PROPERTIES LABELS "sanitizers;address")

add_custom_flags_test("sanitizer_undefined" ${SANITIZER_UNDEFINED})
set_tests_properties(sanitizer_undefined PROPERTIES LABELS "sanitizers;undefined")

add_custom_flags_test("sanitizer_thread" ${SANITIZER_THREAD})
set_tests_properties(sanitizer_thread PROPERTIES LABELS "sanitizers;thread")

find_library(GTEST_LIB gtest HINTS /usr/lib64)
find_library(GTEST_MAIN_LIB gtest_main HINTS /usr/lib64)

file(GLOB TEST_SOURCES ${CMAKE_CURRENT_LIST_DIR}/performance/*.cpp)

add_executable(test_mandelbrot ${TEST_SOURCES} ${C_SOURCES})

target_link_libraries(test_mandelbrot
    PRIVATE
    global_flags
    ${GTEST_LIB}
    ${GTEST_MAIN_LIB}
    pthread
)

gtest_discover_tests(test_mandelbrot DISCOVERY_TIMEOUT 10)

add_test(NAME performance_tests_mandatory
    COMMAND test_mandelbrot --gtest_color=yes --gtest_filter=-*extra*
)

add_executable(${EXECUTABLE}_profiler ${C_SOURCES} ${MAIN_FILE_PATH})

target_link_libraries(${EXECUTABLE}_profiler
    PRIVATE
    global_flags
    -lprofiler
)

add_custom_target(cpu_profiler_gperftools_script
    COMMAND ${CMAKE_COMMAND} -E env bash ${SCRIPTS_DIR}/cpu_profiler_gperftools.sh
        "${CMAKE_BINARY_DIR}/${EXECUTABLE}_profiler"
        "${CMAKE_BINARY_DIR}/mandelbrot.prof"
        "${LOGS_DIR}/cpu_profile_analysis.txt"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_test(NAME cpu_profiler_gperftools
    COMMAND ${CMAKE_COMMAND} --build . --target cpu_profiler_gperftools_script
)

#
# Memcheck profiling test
#
add_custom_target(memcheck_valgrind_script
    COMMAND ${CMAKE_COMMAND} -E env bash ${SCRIPTS_DIR}/memcheck_valgrind.sh
        "${CMAKE_BINARY_DIR}/${EXECUTABLE}"
        "${LOGS_DIR}"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_test(NAME memcheck_valgrind
    COMMAND ${CMAKE_COMMAND} --build . --target memcheck_valgrind_script
)

#
# Stack usage profiling test
#
file(GLOB STACK_USAGE_FILES "${CMAKE_BINARY_DIR}/CMakeFiles/mandelbrot.dir/src/*.su")

add_custom_target(stack_usage_script
    COMMAND ${CMAKE_COMMAND} -E env bash ${SCRIPTS_DIR}/stack_usage.sh ${STACK_USAGE_FILES}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_test(NAME stack_usage
    COMMAND ${CMAKE_COMMAND} --build . --target stack_usage_script
)
