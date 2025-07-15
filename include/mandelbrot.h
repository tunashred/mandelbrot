#ifndef MANDELBROT_H
#define MANDELBROT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

typedef uint8_t (*color_mapping_func)(int, int);

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

FILE* initialize_image(const char* image_name, int height, int width);

uint32_t* deseneaza_mandelbrot(
    const char* nume_poza, int inaltime_poza, int latime_poza,
    double top_left_coord_real, double top_left_coord_imaginar, double pixel_width,
    int num_iters, double rotate_degrees, double brightness,
    uint8_t (*red_mapping_func)(int, int), uint8_t (*green_mapping_func)(int, int), uint8_t (*blue_mapping_func)(int, int)
);

uint32_t* mandelbrot_around_center(
    const char* nume_poza, int inaltime_poza, int latime_poza,
    double center_coord_real, double center_coord_imaginar, double radius,
    int num_iters, double rotate_degrees, double brightness,
    uint8_t (*red_mapping_func)(int, int), uint8_t (*green_mapping_func)(int, int), uint8_t (*blue_mapping_func)(int, int)
);

#ifdef __cplusplus
}
#endif

#endif