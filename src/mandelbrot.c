#include "mandelbrot.h"
#include "color_mapping.h"
#include "thread_pool.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

void mandelbrot_quadratic(double z_real, double z_im, double c_real, double c_im, double *rez_real, double *rez_im) {
    *rez_real = pow(z_real, 2) - pow(z_im, 2) + c_real;
    *rez_im = 2 * z_real * z_im + c_im;
}

void mandelbrot_cubic(double z_real, double z_im, double c_real, double c_im, double *rez_real, double *rez_im) {
    *rez_real = pow(z_real, 3) - 3 * z_real * pow(z_im, 2) + c_real;
    *rez_im = 3 * pow(z_real, 2) * z_im - pow(z_im, 3) + c_im;
}

void mandelbrot_quartic(double z_real, double z_im, double c_real, double c_im, double *rez_real, double *rez_im) {
    *rez_real = pow(z_real, 4) - 6 * pow(z_real, 2) * pow(z_im, 2) + pow(z_im, 4) + c_real;
    *rez_im = 4 * pow(z_real, 3) * z_im - 4 * z_real * pow(z_im, 3) + c_im;
}

void mandelbrot_quintic(double z_real, double z_im, double c_real, double c_im, double *rez_real, double *rez_im) {
    *rez_real = pow(z_real, 5) - 10 * pow(z_real, 3) * pow(z_im, 2) + 5 * z_real * pow(z_im, 4) + c_real;
    *rez_im = 5 * pow(z_real, 4) * z_im - 10 * pow(z_real, 2) * pow(z_im, 3) + pow(z_im, 5) + c_im;
}

int diverge(double c_real, double c_im, int num_iters, void (*mandelbrot_func)(double, double, double, double, double*, double*)) {
    int i = 0;
    double z_real = 0, z_im = 0;
    double z_real_returnat, z_im_returnat;
    while( z_real <= 2.f && z_real >= -2.f && i++ <= num_iters ) {
        mandelbrot_func(z_real, z_im, c_real, c_im, &z_real_returnat, &z_im_returnat);
        z_real = z_real_returnat;
        z_im = z_im_returnat;
    }
    if(i >= num_iters) {
        // nu diverge
        return 0;
    }
    return i;
}

void roteste(double *real, double *imaginar, double centru_real, double centru_im, double grade) {
    if(grade == 0) {
        return;
    }

    double cateta_reala = *real - centru_real,
           cateta_im =    *imaginar - centru_im;
    double raza = sqrt( pow(cateta_reala, 2) + pow(cateta_im, 2) );
    double radiani = asin(cateta_im / raza);

    if(cateta_reala < 0) {
        if(cateta_im > 0) {  // cadranul 2
            radiani = M_PI - radiani;
        }
        else {  // cadranul 3
            radiani = -radiani + M_PI;
        }
    }
    else if(cateta_im < 0) {  // cadranul 4
        radiani += 2 * M_PI;
    }

    grade += linear_map(radiani, 0, 2 * M_PI, 0, 360);
    if(grade > 360) {
        grade -= 360;
    }

    radiani = linear_map(grade, 0, 360, 0, 2 * M_PI);

    *real =     cos(radiani) * raza + centru_real;
    *imaginar = sin(radiani) * raza + centru_im;
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

void deseneaza_mandelbrot(void* worker_task) {
    worker_task_info* task = (worker_task_info*) worker_task;
    color_palette palette = *task->palette;
    uint32_t* buffer = task->image_info->buffer;

    double parte_imaginara = task->image_slice.slice_top_left_coor_im;
    for(int i = task->image_slice.start_height; i < task->image_slice.end_height; i++) {
        double parte_reala = task->image_info->top_left_coord_real;
        int width_offset = (i * task->image_info->width) * RGB_CHANNELS,
        end_width        = task->image_slice.end_width * RGB_CHANNELS;
        for(int j = task->image_slice.start_width; j < end_width; j += RGB_CHANNELS) {
            double real_rotit = parte_reala,
                   im_rotit   = parte_imaginara;
            roteste(&real_rotit, &im_rotit, -0.75, 0, task->image_info->rotate_degrees);

            int iter_count = diverge(real_rotit, im_rotit, task->image_info->num_iters, task->image_info->mandelbrot_func);

            uint32_t r = palette.r[palette.rgb[iter_count][0]];
            uint32_t g = palette.g[palette.rgb[iter_count][1]];
            uint32_t b = palette.b[palette.rgb[iter_count][2]];

            int index = width_offset + j;
            buffer[index]     = r;
            buffer[index + 1] = g;
            buffer[index + 2] = b;

            parte_reala += task->image_info->pixel_width;
        }
        parte_imaginara -= task->image_info->pixel_width;
    }
}

uint32_t* buffer_init(int rows, int columns) {
    size_t total_size = (size_t)rows * (size_t)columns;
    uint32_t* buffer = malloc(total_size * sizeof(uint32_t));
    // "touching" the buffer to make sure the memory was *really* allocated
    memset(buffer, 0, (size_t)(total_size * sizeof(uint32_t)));
    return buffer;
}

void free_buffer(int* buffer) {
    free(buffer);
    buffer = NULL;
}

image_info* mandelbrot_around_center(
    const int inaltime_poza, const int latime_poza,
    double center_coord_real, double center_coord_imaginar, double radius,
    int num_iters, double rotate_degrees,
    void (*mandelbrot_func)(double, double, double, double, double*, double*)
) {
    const int latura_scurta              = (inaltime_poza < latime_poza) ? inaltime_poza : latime_poza;
    const double pixel_width             = radius * 2 / latura_scurta;
    const double top_left_coord_real     = center_coord_real - (double)latime_poza / 2 * pixel_width;
    const double top_left_coord_imaginar = center_coord_imaginar + (double)inaltime_poza / 2 * pixel_width;

    image_info* img_info = malloc(sizeof *img_info);
    img_info->mandelbrot_func     = mandelbrot_func;
    img_info->height              = inaltime_poza;
    img_info->width               = latime_poza;
    img_info->top_left_coord_real = top_left_coord_real;
    img_info->top_left_coord_im   = top_left_coord_imaginar;
    img_info->pixel_width         = pixel_width;
    img_info->rotate_degrees      = rotate_degrees;
    img_info->num_iters           = num_iters;

    uint32_t* buffer = buffer_init(latime_poza * inaltime_poza, RGB_CHANNELS);
    img_info->buffer = buffer;

    return img_info;
}

worker_task_info* start_workers(tpool_t* pool, image_info* img_info, color_palette* palette) {
    worker_task_info* workers_info = malloc(pool->thread_count * sizeof(worker_task_info));

    const int slice_height = img_info->height / (int)pool->thread_count;

    for(int i = 0; i < (int)pool->thread_count; i++) {
        image_slice current_slice = {
            .start_height = i * slice_height,
            .end_height   = ((size_t)i == pool->thread_count - 1) ? img_info->height : (i + 1) * slice_height,
            .slice_top_left_coor_im = img_info->top_left_coord_im -
                                      (i * slice_height * img_info->pixel_width),
            .start_width  = 0,
            .end_width    = img_info->width
        };
        workers_info[i].palette = palette;
        workers_info[i].image_info = img_info;
        workers_info[i].image_slice = current_slice;
        tpool_work_add(pool, deseneaza_mandelbrot, (void*)&workers_info[i]);
    }
    return workers_info;
}

void save_image_ppm(const char* image_name, const int height, const int width, uint32_t* data) {
    FILE* pgimg = initialize_image(image_name, height, width);

    for(int i = 0; i < height * width * RGB_CHANNELS; i += RGB_CHANNELS) {
        fprintf(pgimg, "%u %u %u\n",
            data[i],
            data[i + 1],
            data[i + 2]
        );
    }

    fclose(pgimg);
}
