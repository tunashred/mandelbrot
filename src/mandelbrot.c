#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <immintrin.h>

#include "mandelbrot.h"
#include "color_mapping.h"
#include "thread_pool.h"

void mandelbrot_quadratic(const __m256d* z_real, const __m256d* z_im,
                          const __m256d* c_real, const __m256d* c_im,
                          __m256d* rez_real, __m256d* rez_im) {
    // rez_real = z_real * z_real - z_im * z_im + c_real;
    *rez_real = _mm256_fmadd_pd(*z_real,
                                *z_real,
                                _mm256_fnmadd_pd(*z_im, *z_im, *c_real));
    // rez_im = 2 * z_real * z_im + c_im;
    *rez_im = _mm256_fmadd_pd(_mm256_add_pd(*z_real, *z_real),
                              *z_im,
                              *c_im);
}

__m128i simd_diverge(__m256d c_real, __m256d c_im, const __m128i num_iters, mandelbrot_func_t mandelbrot_func) {
    static const __m256d SIMD_DOUBLES_TRUE = {-1, -1, -1, -1};
    static const __m256d SIMD_DOUBLES_2 = {2, 2, 2, 2};
    static const __m256d SIMD_DOUBLES_MINUS_2 = {-2, -2, -2, -2};

    __m128i i = _mm_set1_epi32(0);
    __m256d z_real = _mm256_set1_pd(0), z_im = _mm256_set1_pd(0);
    __m256d z_real_ret, z_im_ret, mask = SIMD_DOUBLES_TRUE;

    while(_mm256_movemask_pd(mask) != 0) {
        mandelbrot_func(&z_real, &z_im, &c_real, &c_im, &z_real_ret, &z_im_ret);

        i = _mm_add_epi32(i, _mm_set1_epi32(1));

        __m256d not_diverged = _mm256_cmp_pd(z_real, SIMD_DOUBLES_2, _CMP_LE_OQ);
        not_diverged = _mm256_and_pd(not_diverged, _mm256_cmp_pd(z_real, SIMD_DOUBLES_MINUS_2, _CMP_GE_OQ));

        __m256d not_max_iter = _mm256_cvtepi32_pd(_mm_cmpgt_epi32(num_iters, i));

        mask = _mm256_and_pd(not_diverged, not_max_iter);

        z_real = _mm256_blendv_pd(z_real, z_real_ret, mask);
        z_im = _mm256_blendv_pd(z_im, z_im_ret, mask);
    }
    return i;
}

void roteste(double *real, double *imaginar, double centru_real, double centru_im, double grade) {
    if (grade == 0) {
        return;
    }

    double cateta_reala = *real - centru_real,
           cateta_im    = *imaginar - centru_im;
    double raza = sqrt(pow(cateta_reala, 2) + pow(cateta_im, 2));
    double radiani = asin(cateta_im / raza);

    if (cateta_reala < 0) {
        if (cateta_im > 0) {  // cadranul 2
            radiani = M_PI - radiani;
        }  else {  // cadranul 3
            radiani = -radiani + M_PI;
        }
    } else if (cateta_im < 0) {  // cadranul 4
        radiani += 2 * M_PI;
    }

    grade += linear_map(radiani, 0, 2 * M_PI, 0, 360);
    if (grade > 360) {
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
    int* buffer = task->image_info->buffer;
    image_slice* slice = &task->image_slice;
    image_info* image = task->image_info;

    double im_scalar = slice->slice_top_left_coor_im;
    const __m256d step = _mm256_set1_pd(SIMD_DOUBLE_WIDTH * image->pixel_width);
    const __m256d lane_offsets = _mm256_set_pd(3 * image->pixel_width,
                                               2 * image->pixel_width,
                                               image->pixel_width,
                                               0 );

    for (int i = slice->start_height; i < slice->end_height; i++) {
        __m256d im_part = _mm256_set1_pd(im_scalar);

        __m256d real_base = _mm256_add_pd(_mm256_set1_pd(image->top_left_coord_real),
                                          _mm256_set1_pd(slice->start_width * image->pixel_width));

        int width_offset = i * image->width * RGB_CHANNELS;
        __m256d real_part = _mm256_add_pd(real_base, lane_offsets);

        for (int j = slice->start_width; j + SIMD_DOUBLE_WIDTH <= slice->end_width; j += SIMD_DOUBLE_WIDTH) {
            double real_rot[4] __attribute__((aligned(ALIGNMENT)));
            double im_rot[4]   __attribute__((aligned(ALIGNMENT)));
            _mm256_store_pd(real_rot, real_part);
            _mm256_store_pd(im_rot, im_part);

            for (int k = 0; k < 4; k++) {
                roteste(&real_rot[k], &im_rot[k], -0.75, 0, image->rotate_degrees);
            }
            __m256d real_rotit = _mm256_load_pd(real_rot);
            __m256d im_rotit = _mm256_load_pd(im_rot);
            __m128i iter_count = simd_diverge(real_rotit, im_rotit, _mm_set1_epi32(image->num_iters), image->mandelbrot_func);

            int iters[4] __attribute__((aligned(ALIGNMENT)));
            _mm_store_si128((__m128i*)iters, iter_count);
            for (int k = 0; k < 4; k++) {
                int iter = iters[k] >= image->num_iters ? 0 : iters[k];
                int r = palette.r[palette.rgb[iter][0]];
                int g = palette.g[palette.rgb[iter][1]];
                int b = palette.b[palette.rgb[iter][2]];

                int index = width_offset + (j + k) * RGB_CHANNELS;
                buffer[index]     = r;
                buffer[index + 1] = g;
                buffer[index + 2] = b;
            }
            real_part = _mm256_add_pd(real_part, step);
        }
        im_scalar -= image->pixel_width;
    }
}

int* buffer_init(int rows, int columns) {
    size_t total_size = (size_t)rows * (size_t)columns * sizeof(int);

    if (total_size % ALIGNMENT != 0) {
        total_size += ALIGNMENT - (total_size % ALIGNMENT);
    }

    int* buffer = aligned_alloc(sizeof(int), total_size);
    if (!buffer) {
        fprintf(stderr, "Unable to allocate buffer for image");
        exit(EXIT_FAILURE);
    }
    // "touching" the buffer to make sure the memory was *really* allocated
    memset(buffer, 0, total_size);
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

    int* buffer = buffer_init(latime_poza * inaltime_poza, RGB_CHANNELS);
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

void save_image_ppm(const char* image_name, const int height, const int width, int* data) {
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
