cmake_minimum_required(VERSION 3.15)

project(mandelbrot VERSION 0.1.0 LANGUAGES C CXX CUDA)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

find_package(CUDAToolkit REQUIRED)

set(COMPILE_FLAGS
    -Wall
    -Wextra
    -Wshadow
    -Wcast-align
    -Wwrite-strings
    -Wcast-qual
    -Wconversion
    -Wunreachable-code
)

set(DEBUG_FLAGS
    -g
    -O0
    -fverbose-asm
    -finstrument-functions
    -fno-inline
)

set(CUDA_DEBUG_FLAGS
    -g
    -G
    -O0
)

set(RELEASE_FLAGS
    -O3
    -D_FORTIFY_SOURCE=2
)

add_library(global_flags INTERFACE)

target_compile_options(global_flags INTERFACE
    $<$<COMPILE_LANGUAGE:C,CXX>:${COMPILE_FLAGS}>
    $<$<COMPILE_LANGUAGE:C,CXX>:
        $<$<CONFIG:Debug>:${DEBUG_FLAGS}>
    >
    $<$<COMPILE_LANGUAGE:C,CXX>:
        $<$<CONFIG:Release>:${RELEASE_FLAGS}>
    >

    $<$<COMPILE_LANGUAGE:CUDA>:
        $<$<CONFIG:Debug>:${CUDA_DEBUG_FLAGS}>
    >
    $<$<COMPILE_LANGUAGE:CUDA>:
        $<$<CONFIG:Release>:-O3 -arch=sm_86>
    >
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

add_executable(${EXECUTABLE} ${C_SOURCES} ${MAIN_FILE_PATH} "${CMAKE_SOURCE_DIR}/src/cuda/kernel.cu")

set_target_properties(${EXECUTABLE} PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
)

set_target_properties(${EXECUTABLE} PROPERTIES
    CUDA_ARCHITECTURES native
)

target_link_libraries(${EXECUTABLE}
    PRIVATE global_flags
)

include(${CMAKE_SOURCE_DIR}/tests/Tests.cmake)
