#include "color_mapping.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

double linear_map(double from, double from_min, double from_max, double to_min, double to_max) {
    double interval_from = from_max - from_min;
    double interval_to = to_max - to_min;
    double normalised_from = from - from_min;
    double normalised_to = interval_to * normalised_from / interval_from;
    return (to_min + normalised_to);
}

uint8_t sin_crescator(int iter_count, int num_iters) {
    double max_x = 100;
    double x = linear_map(iter_count, 0, num_iters, 0, max_x);
    double y = sin(x / 3) + x / 10;
    double max_y = sin(max_x / 3) + max_x / 10;
    return (uint8_t)linear_map(y, 0, max_y, 0, 255);
}

uint8_t log_pe_sin(int iter_count, int num_iters) {
    double x = linear_map(iter_count, 0, num_iters, 0.6, 2.5);
    double y = log(x) / sin(x) + 1;
    return (uint8_t)linear_map(y, 0, 2.532, 0, 255);
}

uint8_t unu_minus_unu_pe_x(int iter_count, int num_iters) {
    double x = linear_map(iter_count, 0, num_iters, 1, 2.5);
    double y = 1 - 1 / x;
    return (uint8_t)linear_map(y, 0, 0.6, 0, 255);
}

uint8_t x_patrat_0_5(int iter_count, int num_iters) {
    double x = linear_map(iter_count, 0, num_iters, 0, 0.5);
    double y = pow(x, 2);
    return (uint8_t)linear_map(y, 0, 0.25, 0, 255);
}

uint8_t x_patrat_0_1(int iter_count, int num_iters) {
    double x = linear_map(iter_count, 0, num_iters, 0, 0.1);
    double y = pow(x, 2);
    return (uint8_t)linear_map(y, 0, 0.01, 0, 255);
}

uint8_t x_patrat_0_1_to_0_2(int iter_count, int num_iters) {
    double min_x = 0.1;
    double max_x = 0.2;
    double x = linear_map(iter_count, 0, num_iters, min_x, max_x);
    double y = pow(x, 2);
    double min_y = pow(min_x, 2);
    double max_y = pow(max_x, 2);
    return (uint8_t)linear_map(y, min_y, max_y, 0, 255);
}

uint8_t sin_jos_sus_jos(int iter_count, int num_iters) {
    double min_x = 4.7;
    double max_x = 7.5;
    double x = linear_map(iter_count, 0, num_iters, min_x, max_x);
    double y = sin(x) + 1;
    double min_y = sin(min_x) + 1;
    double max_y = sin(max_x) + 1;
    return (uint8_t)linear_map(y, min_y, max_y, 0, 255);
}

uint8_t sin_x_la_4(int iter_count, int num_iters) {
    double min_x = 2.04;
    double max_x = 2.9;
    double x = linear_map(iter_count, 0, num_iters, min_x, max_x);
    double y = sin(pow(x, 4)) + 1;
    double min_y = sin(pow(min_x, 4)) + 1;
    double max_y = sin(pow(max_x, 4)) + 1;
    return (uint8_t)linear_map(y, min_y, max_y, 0, 255);
}

uint8_t mapare_simpla(int iter_count, int num_iters) {
    return (uint8_t) linear_map(iter_count, 0, num_iters, 0, 255);
}

uint8_t map_color(int iter_count, int num_iters, uint8_t (*color_mapping_func)(int, int)) {
    if(iter_count == 0) {
        return 0;
    }
    return color_mapping_func(iter_count, num_iters);
}

void generate_color_palette(
        color_palette* palette,
        char* palette_file,
        double brightness_rate,
        uint8_t (*red_func)(int, int),
        uint8_t (*green_func)(int, int),
        uint8_t (*blue_func)(int, int)
) {
    // Case 1: Loading from file
    if(palette_file) {
        FILE* file = fopen(palette_file, "r");
        if(!file) {
            fprintf(stderr, "Couldn't open the file: %s\n", palette_file);
            exit(EXIT_FAILURE);
        }
        
        for(int i = 0; i < NUM_COLORS; i++) {
            // what if am I going to support more than 1500 colors?
            if(fscanf(file, "%s %s %s", &palette->r[i], &palette->g[i], &palette->b[i]) != RGB_CHANNELS) {
                fprintf(stderr, "Incomplete palette file\n");
                exit(EXIT_FAILURE);
            }
            palette->rgb[i][0] = (uint8_t) fmin(1499, (double)palette->r[i] * brightness_rate);
            palette->rgb[i][1] = (uint8_t) fmin(1499, (double)palette->g[i] * brightness_rate);
            palette->rgb[i][2] = (uint8_t) fmin(1499, (double)palette->b[i] * brightness_rate);
        }
        fclose(file);
        return;
    }
    if (!red_func || !green_func || !blue_func) {
        fprintf(stderr, "Please provide all color mapping functions\n");
        exit(EXIT_FAILURE);
    }
    // Case 2: Generate using given color mapping functions
    for(int i = 0; i < NUM_COLORS; i++) {
        palette->r[i] = map_color(i, NUM_COLORS, red_func);
        palette->g[i] = map_color(i, NUM_COLORS, green_func);
        palette->b[i] = map_color(i, NUM_COLORS, blue_func);

        palette->rgb[i][0] = (uint8_t) fmin(1499, (double)palette->r[i] * brightness_rate);
        palette->rgb[i][1] = (uint8_t) fmin(1499, (double)palette->g[i] * brightness_rate);
        palette->rgb[i][2] = (uint8_t) fmin(1499, (double)palette->b[i] * brightness_rate);
    }
}
