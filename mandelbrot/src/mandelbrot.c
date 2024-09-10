#include "mandelbrot.h"
#include "color_mapping.h"
#include "thread_utils.h"

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

void progress_print(progress_state* progress) {
    if(progress->current_pixel++ == progress->pixel_waypoint) {
        int num_dots = progress->current_procent % 4;
        int num_spaces = 3 - num_dots;

        printf("\r\033[K%d%% complete %.*s%*s %c", 
            progress->current_procent, 
            num_dots, 
            "...",
            num_spaces,
            "",
            progress->symbols[progress->current_procent % progress->symbols_count]
        );
        fflush(stdout);
        progress->pixel_waypoint += progress->pixel_per_procent;
        progress->current_procent++;
    }
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

void* deseneaza_mandelbrot(void* worker_task) {
    worker_task_info* task = (worker_task_info*) worker_task;
    // TODO: add it as a image_info var
    const int numar_pixeli = *(task->image_info->height) * *(task->image_info->width);
    progress_state progress = (progress_state) {
        .total_pixels = &numar_pixeli,
        .pixel_per_procent = numar_pixeli / 100,
        .current_pixel = 0,
        .current_procent = 0,
        .pixel_waypoint = 0,
        .symbols = "|/-\\",
        .symbols_count = 4
    };

    double parte_imaginara = task->image_slice.slice_top_left_coor_im;
    for(int i = task->image_slice.start_height; i < task->image_slice.end_height; i++) {
        double parte_reala = *task->image_info->top_left_coord_real;
        for(int j = task->image_slice.start_width; j < task->image_slice.end_width; j++) {
            double real_rotit = parte_reala,
                   im_rotit   = parte_imaginara;
            roteste(&real_rotit, &im_rotit, -0.75, 0, *task->image_info->rotate_degrees);

            int index = (i * *task->image_info->width + j) * 3;
            int iter_count = diverge(real_rotit, im_rotit, *task->image_info->num_iters, task->image_info->mandelbrot_func);

            task->buffer[index]     = task->palette->r[task->palette->rgb[iter_count][0]];
            task->buffer[index + 1] = task->palette->g[task->palette->rgb[iter_count][1]];
            task->buffer[index + 2] = task->palette->b[task->palette->rgb[iter_count][2]];
            // printf("%d %d %d\n", task->buffer[index], task->buffer[index + 1], task->buffer[index + 2]);
            parte_reala += *task->image_info->pixel_width;

            // progress_print(&progress);
        }
        parte_imaginara -= *task->image_info->pixel_width;
    }
    return NULL;
}
//     |     |     |    |     |        |
// 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
// 

int* buffer_init(int rows, int columns) {
    int* buffer = (int*) malloc(rows * columns * sizeof(int));
    return buffer;
}

void free_buffer(int* buffer) {
    free(buffer);
    buffer = NULL;
}

void mandelbrot_around_center(
    const char *nume_poza, const int inaltime_poza, const int latime_poza,
    double center_coord_real, double center_coord_imaginar, double radius,
    int num_iters, double rotate_degrees, double brightness,
    int (*red_mapping_func)(int, int), int (*green_mapping_func)(int, int), int (*blue_mapping_func)(int, int),
    void (*mandelbrot_func)(double, double, double, double, double*, double*)
) {
    const int latura_scurta              = (inaltime_poza < latime_poza) ? inaltime_poza : latime_poza;
    const double pixel_width             = radius * 2 / latura_scurta;
    const double top_left_coord_real     = center_coord_real - (double)latime_poza / 2 * pixel_width;
    const double top_left_coord_imaginar = center_coord_imaginar + (double)inaltime_poza / 2 * pixel_width;

    FILE* pgimg = initialize_image(nume_poza, inaltime_poza, latime_poza);

    color_palette palette;
    generate_color_palette(
        &palette, NULL, brightness,
        red_mapping_func, green_mapping_func, blue_mapping_func
    );

    image_info image_info = {
        .mandelbrot_func     = mandelbrot_func,
        .height              = &inaltime_poza,
        .width               = &latime_poza,
        .top_left_coord_real = &top_left_coord_real,
        .top_left_coord_im   = &top_left_coord_imaginar,
        .pixel_width         = &pixel_width,
        .rotate_degrees      = &rotate_degrees,
        .num_iters           = &num_iters
    };

    // replace the magic number with a var
    int* buffer = buffer_init(latime_poza * inaltime_poza, 3);

    const uint64_t thread_count = 6;
    start_worker_threads(&thread_count, &palette, &image_info, buffer);

    wait_all_threads();

    // print the image
    // maybe memcpy would be a good idea?
    for(int i = 0; i < latime_poza * inaltime_poza * 3; i += 3) {
        fprintf(pgimg, "%d %d %d\n",
            buffer[i],
            buffer[i + 1],
            buffer[i + 2]
        );
    }

    free_buffer(buffer);

    fclose(pgimg);
}
