#ifndef THREAD_UTILS_H
#define THREAD_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include <stdint.h>

#include "mandelbrot.h"
#include "color_mapping.h"

void next_slice(image_slice* slice, const int* slice_height, const int* image_height, const double* pixel_width);

void start_worker_threads(const uint64_t* thread_count, color_palette* palette, image_info* image_info, int** buffer);

void wait_all_threads();

#ifdef __cplusplus
}
#endif

#endif