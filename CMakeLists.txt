cmake_minimum_required(VERSION 3.15)

project(mandelbrot VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMPILE_FLAGS
    -Wall
    -Wextra
    -Wshadow
    -Wcast-align
    -Wwrite-strings
    -Wcast-qual
    -Wconversion
    -Wunreachable-code
    -fstack-usage
    -pthread
    -mavx
    -mavx2
    -march=native
    -mtune=native
)

set(DEBUG_FLAGS
    -g
    -O0
    -fverbose-asm
    -finstrument-functions
    -fno-inline
)

set(RELEASE_FLAGS
    -O3
    -D_FORTIFY_SOURCE=2
)

add_library(global_flags INTERFACE)

target_compile_options(global_flags INTERFACE
    ${COMPILE_FLAGS}
    $<$<CONFIG:Debug>:${DEBUG_FLAGS}>
    $<$<CONFIG:Release>:${RELEASE_FLAGS}>
)

target_link_options(global_flags INTERFACE
    -lm
    $<$<CONFIG:Release>:${RELEASE_FLAGS}>
)

target_include_directories(global_flags INTERFACE
    ${CMAKE_SOURCE_DIR}/include
)

file(GLOB C_SOURCES "src/*.c")

set(MAIN_FILE_PATH "${CMAKE_SOURCE_DIR}/src/main.c")
list(REMOVE_ITEM C_SOURCES "${MAIN_FILE_PATH}")

set(EXECUTABLE "mandelbrot")

add_executable(${EXECUTABLE} ${C_SOURCES} ${MAIN_FILE_PATH})

target_link_libraries(${EXECUTABLE}
    PRIVATE global_flags
)

function(generate_assembly_for_file SRC_FILE)
    get_filename_component(FILE_NAME ${SRC_FILE} NAME_WE)

    add_custom_target(
        ${FILE_NAME}_assembly
        COMMAND ${CMAKE_C_COMPILER}
            ${DEBUG_FLAGS}
            -I${CMAKE_SOURCE_DIR}/include
            -S ${SRC_FILE}
            -o ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.s
        COMMENT "Generating assembly for ${FILE_NAME}.c"
    )
endfunction()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    foreach(SRC_FILE IN LISTS C_SOURCES)
        generate_assembly_for_file(${SRC_FILE})
    endforeach()
endif()

include(${CMAKE_SOURCE_DIR}/tests/Tests.cmake)
