#include <stdio.h>

#include <mandelbrot.h>
#include <color_mapping.h>

int main() {
    float scale = 4;
    // mandelbrot_around_center("mandelbrot.ppm", 1080 * scale, 1920 * scale, -0.43, -0.1, 1, 1500, 0, 1, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);
    // mandelbrot_around_center("shells.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.75 + 0.00005, -0.02, 0.00025, 1500, 0, 1, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2); // legacy name: pe_crestele_crapaturii_de_jos
    // mandelbrot_around_center("seastar.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.72413, 0.28644, 0.0004, 1500, 0, 1, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2); // legacy name: spirale
    // mandelbrot_around_center("stuff.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.7, -0.26, 0.01, 1500, 0, 1, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2); // legacy name: subtioru_stang_7
    mandelbrot_around_center("galaxy.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.700025 + 0.000000007, -0.26849991525, 0.0000000035, 1500, 0, 0.3, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2); // legacy name: "pene_de_paun"

    return 0;
}
