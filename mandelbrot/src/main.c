#include <stdio.h>

#include <mandelbrot.h>
#include <color_mapping.h>

int main() {
    const float scale = 1;
    const int height_landscape = (int) (1080 * scale),
    width_landscape            = (int) (1920.f * scale),
    height_portrait            = (int) (1080 * scale),
    width_portrait             = (int) (1080 * 1.2 * scale);
    
    mandelbrot_around_center(
        "mandelbrot.ppm", height_landscape, width_landscape, -0.43, -0.1, 1, 1500, 0, 1, sin_x_la_4,
        unu_minus_unu_pe_x, x_patrat_0_1_to_0_2, mandelbrot_quadratic);
    
    // mandelbrot_around_center( // legacy name: pe_crestele_crapaturii_de_jos
    //     "shells.ppm", height_portrait, width_portrait, -0.75 + 0.00005, -0.02, 0.00025, 1500, 0, 1,
    //     sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2, mandelbrot_quadratic);
       
    // mandelbrot_around_center( // legacy name: spirale 
    //     "seastar.ppm", height_portrait, width_portrait, -0.72413, 0.28644, 0.0004, 1500, 0, 1, sin_x_la_4,
    //     unu_minus_unu_pe_x, x_patrat_0_1_to_0_2, mandelbrot_quadratic);
    
    // mandelbrot_around_center( // legacy name: subtioru_stang_7
    //     "stuff.ppm", height_portrait, width_portrait, -0.7, -0.26, 0.01, 1500, 0, 1, sin_x_la_4,
    //     unu_minus_unu_pe_x, x_patrat_0_1_to_0_2, mandelbrot_quadratic);
    
    // mandelbrot_around_center( // legacy name: pene_de_paun
    //     "galaxy.ppm", height_portrait, width_portrait, -0.700025 + 0.000000007, -0.26849991525, 0.0000000035,
    //     1500, 0, 0.3, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2, mandelbrot_quadratic);

    return 0;
}
