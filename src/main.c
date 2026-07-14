#include <mandelbrot.h>
#include <stdlib.h>
#include <color_mapping.h>
#include <time.h>

int main() {
    const float scale = 1;
    const int height_landscape = (int) (1080 * scale),
    width_landscape            = (int) (1920.f * scale),
    height_portrait            = (int) (1080 * scale),
    width_portrait             = (int) (1080 * 1.2 * scale);

    color_palette palette;
    generate_color_palette(
        &palette, NULL, 1,
        sin_crescator, sin_x_la_4, x_patrat_0_1_to_0_2
    );

    image_info* img_info;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // mandelbrot
    // img_info = mandelbrot_around_center(height_landscape, width_landscape, -0.43, -0.1, 1, 1500, 0, dummy_mandelbrot_func);
    // deseneaza_mandelbrot(img_info);
    // save_image_ppm("mandelbrot.ppm", img_info, &palette);
    // free(img_info->buffer);
    // free(img_info);

    // // shells
    // img_info = mandelbrot_around_center(height_portrait, width_portrait, -0.75 + 0.00005, -0.02, 0.00025, 1500, 0, dummy_mandelbrot_func);
    // save_image_ppm("shells.ppm", img_info, &palette);
    // free(img_info->buffer);
    // free(img_info);

    // // seastar
    // img_info = mandelbrot_around_center(height_portrait, width_portrait, -0.72413, 0.28644, 0.0004, 1500, 0, dummy_mandelbrot_func);
    // save_image_ppm("shells.ppm", img_info, &palette);
    // free(img_info->buffer);
    // free(img_info);

    // // stuff
    // img_info = mandelbrot_around_center(height_portrait, width_portrait, -0.7, -0.26, 0.01, 1500, 0, dummy_mandelbrot_func);
    // save_image_ppm("stuff.ppm", img_info, &palette);
    // free(img_info->buffer);
    // free(img_info);
    
    // galaxy
    img_info = mandelbrot_around_center(height_portrait, width_portrait, -0.700025 + 0.000000007, -0.26849991525, 0.0000000035, 1500, 0, dummy_mandelbrot_func);
    deseneaza_mandelbrot(img_info);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed =
        (double)(end.tv_sec - start.tv_sec) +
        (double)(end.tv_nsec - start.tv_nsec) / 1e9;
    printf("main(): time taken for gpu: %.2f\n", elapsed);

    // clock_gettime(CLOCK_MONOTONIC, &start);
    save_image_ppm("galaxy.ppm", img_info, &palette);
    // clock_gettime(CLOCK_MONOTONIC, &end);
    // elapsed =
    //     (double)(end.tv_sec - start.tv_sec) +
    //     (double)(end.tv_nsec - start.tv_nsec) / 1e9;
    // printf("time taken for write: %.2f\n", elapsed);
    free(img_info->buffer);
    free(img_info);

    return 0;
}
