#include <mandelbrot.h>
#include <color_mapping.h>

#include <gtest/gtest.h>

TEST(image_generation, image_4k) {
    float scale = 4;
    mandelbrot_around_center("tot_setul.ppm", 1080 * scale, 1920 * scale, -0.43, -0.1, 1, 1500, 0, 0, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);
}
