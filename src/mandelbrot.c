#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "mandelbrot.h"
#include "color_mapping.h"

void dummy_mandelbrot_func(const double* z_real, const double* z_im,
                           const double* c_real, const double* c_im,
                           double* rez_real, double* rez_im) {
    printf("deadbeef\n");
    return (void) "moo";
}

FILE* initialize_image(const char* image_name, const int height, const int width) {
    FILE* pgimg;
    pgimg = fopen(image_name, "wb");
    if(pgimg == NULL) {
        fprintf(stderr, "Error initializing image\n");
        exit(EXIT_FAILURE);
    }
    fprintf(pgimg, "P3\n"); 
    fprintf(pgimg, "%d %d\n", width, height);
    fprintf(pgimg, "255\n");
    return pgimg;
}

void deseneaza_mandelbrot(image_info* image_info) {
    // maybe other checks too
    if (!image_info) {
        fprintf(stderr, "Image information is null\n");
        return;
    }
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    cuda_generate_iter_array(image_info);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed =
        (double)(end.tv_sec - start.tv_sec) +
        (double)(end.tv_nsec - start.tv_nsec) / 1e9;
    printf("deseneaza_mandelbrot(): time taken for gpu: %.2f\n", elapsed);
}

uint32_t* buffer_init(int rows, int columns) {
    size_t total_size = (size_t)rows * (size_t)columns;
    uint32_t* buffer = malloc(total_size * sizeof(uint32_t));
    // "touching" the buffer to make sure the memory was *really* allocated
    memset(buffer, 0, (size_t)(total_size * sizeof(uint32_t)));
    return buffer;
}

image_info* mandelbrot_around_center(const int inaltime_poza, const int latime_poza,
                                     double center_coord_real, double center_coord_imaginar, double radius,
                                     int num_iters, double rotate_degrees, mandelbrot_func_t mandelbrot_func) {
    const int latura_scurta              = (inaltime_poza < latime_poza) ? inaltime_poza : latime_poza;
    const double pixel_width             = radius * 2 / latura_scurta;
    const double top_left_coord_real     = center_coord_real - (double)latime_poza / 2 * pixel_width;
    const double top_left_coord_imaginar = center_coord_imaginar + (double)inaltime_poza / 2 * pixel_width;

    image_info* img_info          = malloc(sizeof *img_info);
    img_info->mandelbrot_func     = mandelbrot_func;
    img_info->height              = inaltime_poza;
    img_info->width               = latime_poza;
    img_info->top_left_coord_real = top_left_coord_real;
    img_info->top_left_coord_im   = top_left_coord_imaginar;
    img_info->pixel_width         = pixel_width;
    img_info->rotate_degrees      = rotate_degrees;
    img_info->num_iters           = num_iters;

    uint32_t* buffer = buffer_init(latime_poza, inaltime_poza);
    img_info->buffer = buffer;

    return img_info;
}

void save_image_ppm(const char* image_name, image_info* image_info, color_palette* palette) {
    FILE* pgimg = initialize_image(image_name, image_info->height, image_info->width);

    int pixel = 0;
    for(int i = 0; i < image_info->height * image_info->width * RGB_CHANNELS; i += RGB_CHANNELS) {
        uint32_t r = palette->r[palette->rgb[image_info->buffer[pixel]][0]];
        uint32_t g = palette->g[palette->rgb[image_info->buffer[pixel]][1]];
        uint32_t b = palette->b[palette->rgb[image_info->buffer[pixel]][2]];

        // int index = (i * image_info->width) + j;

        fprintf(pgimg, "%u %u %u\n", r, g, b);
        pixel++;
    }

    fclose(pgimg);
}
