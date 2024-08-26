# Tests.cmake

# Setup
enable_testing()
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/output)
set(LOGS_DIR ${CMAKE_SOURCE_DIR}/tests/output)
set(SCRIPTS_DIR ${CMAKE_SOURCE_DIR}/tests/scripts)

# Function to add custom flags to a build
# It builds the project, runs it and then deletes the executable
# Only for test purposes
function(add_custom_flags_test test_name flags)
    set(temp_target "${EXECUTABLE}_${test_name}")

    add_executable(${temp_target} ${SOURCES})

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
add_custom_flags_test("sanitizer_undefined" ${SANITIZER_UNDEFINED})
add_custom_flags_test("sanitizer_thread" ${SANITIZER_THREAD})

#
# CPU profiling test
#
add_custom_target(cpu_profiler_gperftools_script
    COMMAND ${CMAKE_COMMAND} -E env bash ${SCRIPTS_DIR}/cpu_profiler_gperftools.sh
    "${CMAKE_BINARY_DIR}/${EXECUTABLE}"
    "${CMAKE_BINARY_DIR}/mandelbrot.prof"
    "${LOGS_DIR}/cpu_profile_analysis.txt"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running CPU profiler test..."
    DEPENDS ${EXECUTABLE}
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

include(CTest)