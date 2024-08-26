#ifndef COLOR_MAPPING_H
#define COLOR_MAPPING_H

#include <math.h>

double linear_map(double from, double from_min, double from_max, double to_min, double to_max);

// y = sin(x/3), x <= 100
int sin_crescator(int iter_count, int num_iters);

// y = log(x)/sin(x)+1, 0.6 < x < 2.5 ==> 0 < y < 2.532
int log_pe_sin(int iter_count, int num_iters);

// y = 1 - 1/x, 1 < x < 2.5 ==> 0 < y < 0.6
int unu_minus_unu_pe_x(int iter_count, int num_iters);

// y = x², 0 < x < 0.5 ==> 0 < y < 0.25
int x_patrat_0_5(int iter_count, int num_iters);

// y = x², 0 < x < 0.1 ==> 0 < y < 0.01
int x_patrat_0_1(int iter_count, int num_iters);

// y = x², 0.1 < x < 0.2
int x_patrat_0_1_to_0_2(int iter_count, int num_iters);

// y = sin(x)+1, 4.7 < x < 7.5
int sin_jos_sus_jos(int iter_count, int num_iters);

// y = sin(x⁴)+1, 2.04 < x < 2.9
int sin_x_la_4(int iter_count, int num_iters);

int mapare_simpla(int iter_count, int num_iters);

int red(int iter_count, int num_iters);

int green(int iter_count, int num_iters);

int blue(int iter_count, int num_iters);

#endif