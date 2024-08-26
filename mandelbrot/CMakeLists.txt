cmake_minimum_required(VERSION 3.0.0)

# Set C standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Project name and version
project(mandelbrot VERSION 0.1.0 LANGUAGES C)

# Set source file
set(SOURCE_FILE ./mandelbrot.c)

# Set executable name
set(EXECUTABLE "mandelbrot")

# Add executable target
add_executable(${EXECUTABLE} ${SOURCE_FILE})

# Set sanitizer types
set(SANITIZER_ADDRESS "-fsanitize=address")
set(SANITIZER_THREAD "-fsanitize=thread")
set(SANITIZER_UNDEFINED "-fsanitize=undefined")

#
# Compile flags for any build type
#
add_compile_options(
    -Wall
    -Wextra
    -pthread
    -D_FORTIFY_SOURCE=2
    -Wshadow
    -Wcast-align
    -Wwrite-strings
    -Wcast-qual
    -Wconversion
    -pg
    -Wunreachable-code
)

# Sanitizer option needed at linking stage
target_link_options(${EXECUTABLE} PRIVATE -lm -lprofiler)

# Release specific flags
set(RELEASE_FLAGS
    -O3
)

# Debug specific flags
set(DEBUG_FLAGS
    -g
    -O0
    -fverbose-asm
    -finstrument-functions
    -fanalyzer
)

# Extra compile flags for release build
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(${RELEASE_FLAGS})

# Extra compile flags for debug build
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(${DEBUG_FLAGS})

    # Generate assembly file
    add_custom_target(generate_assembly
        COMMAND ${CMAKE_C_COMPILER} ${DEBUG_FLAGS} -S ${CMAKE_SOURCE_DIR}/${SOURCE_FILE} -o ${CMAKE_CURRENT_BINARY_DIR}/assembly.s
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS ${EXECUTABLE}
        COMMENT "Generating assembly file assembly.s"
    )
endif()

# # # # #
# Tests #
# # # # #

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

    add_executable(${temp_target} ${SOURCE_FILE})

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
##################################################################
# Commentary on compile flags
#
# Common flags
# -Wall: Enable all warnings
# -pthread: Enable pthread support
# -D_FORTIFY_SOURCE=2: Runtime buffer overflow detection
# -Wshadow: Warns if a local variable shadows a function parameter or global variable, or when a lib function is shadowed
# -Wpointer-arith: Warns about questionable pointer operations or pointer arithmetic with void* type or function pointers
# -Wcast-align: Warns for potentially unsafe pointer casts related to strict pointer alignment
# -Wwrite-strings: Warns if a string literal is assigned to a char*
# -Wcast-qual: Warns when casting removes type qualifiers such as const or volatile
# -Wconversion: Warns about implicit type conversions that can lead to data loss
# -Wunreachable-code: Detects code that will never be executed
#
# Link flags
# -lm: needed for math.h
# -pg: For gprof
# -lprofiler: For gperftools
#
# Release flags
# -O3: Max optimization level
#
# Debug flags
# -g: Include debug info
# -O0: No optimizations
# -fverbose-asm: Puts extra comments to generated assembly code to make it more readable
# -finstrument-functions: Inserts hooks for each function in the program
# -fanalyzer: Static analyser at compile time
#
# Sanitizers:
# ${SANITIZER}: Applied sanitizer (address, thread, undefined). Needs to be manually changed because only one sanitizer can be used in a build.
