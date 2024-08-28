#include <stdio.h>

#include <mandelbrot.h>
#include <color_mapping.h>

int main() {
    float scale = 1;
    // mandelbrot_around_center("tot_setul.ppm", 1080 * scale, 1920 * scale, -0.43, -0.1, 1, 1500, 0, 0, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);
    // mandelbrot_around_center("pe_crestele_crapaturii_de_jos.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.75 + 0.00005, -0.02, 0.00025, 1500, 0, 1, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);
    // mandelbrot_around_center("spirale.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.72413, 0.28644, 0.0004, 1500, 0, 1, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);
    // mandelbrot_around_center("subtioru_stang_7.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.7, -0.26, 0.01, 1500, 0, 1, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);
    mandelbrot_around_center("pene_de_paun.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.700025 + 0.000000007, -0.26849991525, 0.0000000035, 1500, 0, 0, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);

    return 0;
}
