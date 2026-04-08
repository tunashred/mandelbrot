#ifndef MANDELBROT_H
#define MANDELBROT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#include <immintrin.h>

#include "thread_pool.h"
#include "color_mapping.h"

#define SIMD_DOUBLE_WIDTH 4
#define ALIGNMENT 32

typedef void(*mandelbrot_func_t)(const __m256d*, const __m256d*, const __m256d*, const __m256d*, __m256d*, __m256d*);

void mandelbrot_quadratic(const __m256d* z_real, const __m256d* z_im, const __m256d* c_real, const __m256d* c_im,
                          __m256d* rez_real, __m256d* rez_im);

__m128i diverge(__m256d c_real, __m256d c_im, const __m128i* num_iters, mandelbrot_func_t mandelbrot_func);

/*
    1. Determinam lungimea razei folosind Pitagora.
    2. Determinam unghiul in radiani.
    3. Aflam in ce cadran mutam punctul si calculam distanta lui fata de punctul (1, 0).
    4. Convertim radianii in grade si corectam valoarea daca este mai mare decat 360.
    5. Convertim gradele in radiani si calculam coordonatele reale si imaginare ale punctului rotit.
*/
void roteste(double *real, double *imaginar, double centru_real, double centru_im, double grade);

FILE* initialize_image(const char* image_name, const int height, const int width);

typedef struct {
    double pixel_width;
    double top_left_coord_real;
    double top_left_coord_im;
    double rotate_degrees;
    int num_iters;
    int width;
    int height;
    int* buffer;
    mandelbrot_func_t mandelbrot_func;
} image_info;

typedef struct {
    int start_height;
    int end_height;
    double slice_top_left_coor_im;
    int start_width;
    int end_width;
} image_slice;

typedef struct {
    color_palette* palette;
    image_info* image_info;
    image_slice image_slice;
} worker_task_info;

void deseneaza_mandelbrot(void* worker_task);

int* buffer_init(int rows, int columns);

image_info* mandelbrot_around_center(const int inaltime_poza, const int latime_poza,
                                     double center_coord_real, double center_coord_imaginar, double radius,
                                     int num_iters, double rotate_degrees,mandelbrot_func_t mandelbrot_func);

worker_task_info* start_workers(tpool_t* pool, image_info* img_info, color_palette* palette);

void save_image_ppm(const char* image_name, const int height, const int width, int* data);

#ifdef __cplusplus
}
#endif

#endif
