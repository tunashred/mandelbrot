#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <sys/types.h>
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <string.h>

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

typedef struct {
    const int* total_pixels;
    const int pixel_per_procent;
    int current_pixel;
    int current_procent;
    int pixel_waypoint;
    const char* symbols;
    const int symbols_count;
} progress_state;

void progress_print(progress_state* progress);

FILE* initialize_image(const char* image_name, const int height, const int width);

typedef struct {
    const double* pixel_width;
    const double* top_left_coord_real;
    const double* rotate_degrees;
    const int* num_iters;
    const int* width;
    void (*mandelbrot_func)(double, double, double, double, double*, double*);
    const int* height;
    const double* top_left_coord_im;
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
    int* buffer;
    image_info* image_info;
    image_slice image_slice;
} worker_task_info;

void* deseneaza_mandelbrot(void* worker_task);

int* buffer_init(int rows, int columns);

void free_buffer(int* buffer);

void mandelbrot_around_center(
    const char* nume_poza, const int inaltime_poza, const int latime_poza,
    double center_coord_real, double center_coord_imaginar, double radius,
    int num_iters, double rotate_degrees, double brightness,
    int (*red_mapping_func)(int, int), int (*green_mapping_func)(int, int), int (*blue_mapping_func)(int, int),
    void (*mandelbrot_func)(double, double, double, double, double*, double*), const u_int64_t thread_count
);


#ifdef __cplusplus
}
#endif

#endif
