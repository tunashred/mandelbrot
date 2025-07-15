#ifndef COLOR_MAPPING_H
#define COLOR_MAPPING_H

#include "mandelbrot.h"
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define RGB_CHANNELS 3
#define NUM_COLORS 1500

typedef struct {
    int r[NUM_COLORS];
    int g[NUM_COLORS];
    int b[NUM_COLORS];
    int rgb[NUM_COLORS][RGB_CHANNELS];
} color_palette;

double linear_map(double from, double from_min, double from_max, double to_min, double to_max);

// y = sin(x/3), x <= 100
uint8_t sin_crescator(int iter_count, int num_iters);

// y = log(x)/sin(x)+1, 0.6 < x < 2.5 ==> 0 < y < 2.532
uint8_t log_pe_sin(int iter_count, int num_iters);

// y = 1 - 1/x, 1 < x < 2.5 ==> 0 < y < 0.6
uint8_t unu_minus_unu_pe_x(int iter_count, int num_iters);

// y = x², 0 < x < 0.5 ==> 0 < y < 0.25
uint8_t x_patrat_0_5(int iter_count, int num_iters);

// y = x², 0 < x < 0.1 ==> 0 < y < 0.01
uint8_t x_patrat_0_1(int iter_count, int num_iters);

// y = x², 0.1 < x < 0.2
uint8_t x_patrat_0_1_to_0_2(int iter_count, int num_iters);

// y = sin(x)+1, 4.7 < x < 7.5
uint8_t sin_jos_sus_jos(int iter_count, int num_iters);

// y = sin(x⁴)+1, 2.04 < x < 2.9
uint8_t sin_x_la_4(int iter_count, int num_iters);

uint8_t mapare_simpla(int iter_count, int num_iters);

uint8_t red(int iter_count, int num_iters, color_mapping_func mapping_func);

uint8_t green(int iter_count, int num_iters, color_mapping_func mapping_func);

uint8_t blue(int iter_count, int num_iters, color_mapping_func mapping_func);

int generate_color_palette(
    color_palette* palette,
    double brightness_rate,
    const char* palette_file,
    uint8_t (*red_func)(int, int),
    uint8_t (*green_func)(int, int),
    uint8_t (*blue_func)(int, int)
);

#ifdef __cplusplus
}
#endif

#endif