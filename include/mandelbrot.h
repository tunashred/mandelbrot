#ifndef MANDELBROT_H
#define MANDELBROT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>

#include "color_mapping.h"

typedef void(*mandelbrot_func_t)(const double*, const double*, const double*, const double*, double*, double*);

void dummy_mandelbrot_func(const double* z_real, const double* z_im,
                           const double* c_real, const double* c_im,
                           double* rez_real, double* rez_im);

extern int cuda_diverge(double c_real, double c_im, int num_iters, mandelbrot_func_t mandelbrot_func);

FILE* initialize_image(const char* image_name, const int height, const int width);

// TODO: switch back buffer to uin32_t?
typedef struct {
    double pixel_width;
    double top_left_coord_real;
    double top_left_coord_im;
    double rotate_degrees;
    int num_iters;
    int width;
    int height;
    uint32_t* buffer;
    mandelbrot_func_t mandelbrot_func;
} image_info;

void deseneaza_mandelbrot(image_info* image_info);

uint32_t* buffer_init(int rows, int columns);

image_info* mandelbrot_around_center(const int inaltime_poza, const int latime_poza,
                                     double center_coord_real, double center_coord_imaginar, double radius,
                                     int num_iters, double rotate_degrees, mandelbrot_func_t mandelbrot_func);

void save_image_ppm(const char* image_name, image_info* image_info, color_palette* palette);

extern void cuda_generate_iter_array(image_info* image_info);

#ifdef __cplusplus
}
#endif

#endif
