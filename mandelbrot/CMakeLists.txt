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

# Set sanitizer type
set(SANITIZER "-fsanitize=address")

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
    ${SANITIZER}
)

# Sanitizer option needed at linking stage
target_link_options(${EXECUTABLE} PRIVATE ${SANITIZER} -lm -lprofiler)

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
    add_compile_options(
        ${RELEASE_FLAGS}
    )
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

#
# Tests
#

# Setup
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/output)
set(SCRIPTS_DIR ${CMAKE_SOURCE_DIR}/tests/scripts)

# Profiling test
add_custom_target(cputest
    COMMAND ${CMAKE_COMMAND} -E env bash ${SCRIPTS_DIR}/cpu_profiler_gperftools.sh
        "${CMAKE_BINARY_DIR}/${EXECUTABLE}"  # Path to the executable
        "${CMAKE_BINARY_DIR}/mandelbrot.prof" # Path to the profile file
        "${CMAKE_SOURCE_DIR}/tests/output/cpu_profile_analysis.txt" # Path to the output file
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running CPU profiler test..."
    DEPENDS ${EXECUTABLE}
)

# Register the custom target as a test
add_test(NAME CPU_profiler
    COMMAND ${CMAKE_COMMAND} --build . --target cputest
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

include(CTest)
enable_testing()
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
