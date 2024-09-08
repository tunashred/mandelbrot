#include "thread_utils.h"
#include "mandelbrot.h"
#include <pthread.h>

static pthread_t* threads = NULL;
static uint64_t g_thread_count = 0;

void next_slice(image_slice* slice, const int* slice_height, const int* slice_width, const int* inaltime_poza, const int* latime_poza) {
    slice->start_height += *slice_height;
    int next_slice_limit = slice->end_height + *slice_height;
    if(next_slice_limit > *inaltime_poza) {
        slice->end_height = *inaltime_poza;
    } else {
        slice->end_height = next_slice_limit;
    }

    slice->start_width += *slice_width;
    next_slice_limit = slice->end_width + *slice_width;
    if(next_slice_limit > *latime_poza) {
        slice->end_width = *latime_poza;
    } else {
        slice->end_width += *slice_width;
    }
}

void start_worker_threads(const uint64_t* thread_count, color_palette* palette, image_info* image_info) {
    threads = (pthread_t*) malloc(*thread_count * sizeof(*threads));
    g_thread_count = *thread_count;
    worker_task_info* workers_info = (worker_task_info*) malloc(*thread_count * sizeof(*workers_info));

    const int slice_height = *(image_info->height) / *thread_count,
        slice_width        = *(image_info->width) / *thread_count;

    image_slice current_slice = {
        .start_height = 0,
        .end_height   = slice_height,
        .start_width  = 0,
        .end_width    = slice_width
    };
    for(int i = 0; i < thread_count; i++) {
        workers_info[i].palette = &palette;
        workers_info[i].image_info = &image_info;
        workers_info[i].image_slice = current_slice;

        // pthread_create(&threads[i], NULL, deseneaza_mandelbrot, &workers_info[i]);

        next_slice(&current_slice, &slice_height, &slice_width, &image_info->height, &image_info->width);
    }
}

void wait_all_threads() {
    for(uint64_t i = 0; i < g_thread_count; i++) {
        pthread_join(threads[i], NULL);
    }
    free(threads);
    threads = NULL;
    g_thread_count = 0;
}