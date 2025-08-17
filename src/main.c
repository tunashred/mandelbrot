#include <mandelbrot.h>
#include <color_mapping.h>
#include <thread_pool.h>

int main() {
    const float scale = 1;
    const int height_landscape = (int) (1080 * scale),
    width_landscape            = (int) (1920.f * scale),
    height_portrait            = (int) (1080 * scale),
    width_portrait             = (int) (1080 * 1.2 * scale);

    tpool_t* pool = tpool_init(10);
    
    color_palette palette;
    worker_task_info* tasks;

    // mandelbrot
    image_info* img_info = mandelbrot_around_center(
        height_landscape, width_landscape, -0.43, -0.1, 1, 1500, 0, mandelbrot_quadratic);

    generate_color_palette(
        &palette, NULL, 1,
        sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2
    );
    tasks = start_workers(pool, img_info, &palette);
    tpool_wait(pool);

    save_image_ppm("mandelbrot.ppm", img_info->height, img_info->width, img_info->buffer);
    free(tasks);
    free(img_info->buffer);

    // shells
    img_info = mandelbrot_around_center(height_portrait, width_portrait, -0.75 + 0.00005, -0.02, 0.00025, 1500, 0, mandelbrot_quadratic);
    tasks = start_workers(pool, img_info, &palette);
    tpool_wait(pool);
    save_image_ppm("shells.ppm", img_info->height, img_info->width, img_info->buffer);
    free(tasks);
    free(img_info->buffer);
       
    // seastar
    img_info = mandelbrot_around_center(height_portrait, width_portrait, -0.72413, 0.28644, 0.0004, 1500, 0, mandelbrot_quadratic);
    tasks = start_workers(pool, img_info, &palette);
    tpool_wait(pool);
    save_image_ppm("shells.ppm", img_info->height, img_info->width, img_info->buffer);
    free(tasks);
    free(img_info->buffer);

    // stuff
    img_info = mandelbrot_around_center(height_portrait, width_portrait, -0.7, -0.26, 0.01, 1500, 0, mandelbrot_quadratic);
    tasks = start_workers(pool, img_info, &palette);
    tpool_wait(pool);
    save_image_ppm("stuff.ppm", img_info->height, img_info->width, img_info->buffer);
    free(tasks);
    free(img_info->buffer);
    
    // galaxy
    img_info = mandelbrot_around_center(height_portrait, width_portrait, -0.700025 + 0.000000007, -0.26849991525, 0.0000000035,1500, 0, mandelbrot_quadratic);
    tasks = start_workers(pool, img_info, &palette);
    tpool_wait(pool);
    save_image_ppm("galaxy.ppm", img_info->height, img_info->width, img_info->buffer);
    free(tasks);
    free(img_info->buffer);

    tpool_destroy(pool);
    free(img_info);

    return 0;
}
