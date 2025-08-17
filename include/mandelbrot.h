#ifndef MANDELBROT_H
#define MANDELBROT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#include "thread_pool.h"

#include "color_mapping.h"

// calculeaza f꜀(z) = z² + c
void mandelbrot_quadratic(double z_real, double z_im, double c_real, double c_im, double *rez_real, double *rez_im);

// calculeaza f꜀(z) = z³ + c
void mandelbrot_cubic(double z_real, double z_im, double c_real, double c_im, double *rez_real, double *rez_im);

// calculeaza f꜀(z) = z⁴ + c
void mandelbrot_quartic(double z_real, double z_im, double c_real, double c_im, double *rez_real, double *rez_im);

// calculeaza f꜀(z) = z⁵ + c
void mandelbrot_quintic(double z_real, double z_im, double c_real, double c_im, double *rez_real, double *rez_im);

// verifica daca diverge pentru c_real, c_im
int diverge(double c_real, double c_im, int num_iters, void (*mandelbrot_func)(double, double, double, double, double*, double*));

/*
    1. Determinam lungimea razei folosind Pitagora.
    2. Determinam unghiul in radiani.
    3. Aflam in ce cadran mutam punctul si calculam distanta lui fata de punctul (1, 0).
    4. Convertim radianii in grade si corectam valoarea daca este mai mare decat 360.
    5. Convertim gradele in radiani si calculam coordonatele reale si imaginare ale punctului rotit.
*/
void roteste(double *real, double *imaginar, double centru_real, double centru_im, double grade);

FILE* initialize_image(const char* image_name, const int height, const int width);

// TODO: less pointers
typedef struct {
    double pixel_width;
    double top_left_coord_real;
    double top_left_coord_im;
    double rotate_degrees;
    int num_iters;
    int width;
    int height;
    uint32_t* buffer;
    void (*mandelbrot_func)(double, double, double, double, double*, double*);
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

uint32_t* buffer_init(int rows, int columns);

void free_buffer(int* buffer);

image_info* mandelbrot_around_center(
    const int inaltime_poza, const int latime_poza,
    double center_coord_real, double center_coord_imaginar, double radius,
    int num_iters, double rotate_degrees,
    void (*mandelbrot_func)(double, double, double, double, double*, double*));

worker_task_info* start_workers(tpool_t* pool, image_info* img_info, color_palette* palette);

void save_image_ppm(const char* image_name, const int height, const int width, uint32_t* data);

#ifdef __cplusplus
}
#endif

#endif
