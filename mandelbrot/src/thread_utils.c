#include <pthread.h>

#include "thread_utils.h"
#include "mandelbrot.h"


job_info* start_worker_threads(const uint64_t* thread_count, color_palette* palette, image_info* image_info, int* buffer) {
    pthread_t* threads = (pthread_t*) malloc(*thread_count * sizeof(pthread_t));
    worker_task_info* workers_info = (worker_task_info*) malloc(*thread_count * sizeof(worker_task_info)); //TODO: investigate sizeof(workers_info) case

    const int slice_height = *(image_info->height) / *thread_count;

    for(unsigned int i = 0; i < *thread_count; i++) {
        image_slice current_slice = {
            .start_height = i * slice_height,
            .end_height   = (i == *thread_count - 1) ? *image_info->height : (i + 1) * slice_height,
            .slice_top_left_coor_im = *(image_info->top_left_coord_im) -
                                      (i * slice_height * (*image_info->pixel_width)),
            .start_width  = 0,
            .end_width    = *image_info->width
        };
        workers_info[i].palette = palette;
        workers_info[i].image_info = image_info;
        workers_info[i].image_slice = current_slice;
        workers_info[i].buffer = buffer;

        pthread_create(&threads[i], NULL, deseneaza_mandelbrot, &workers_info[i]);
    }
    job_info* job = (job_info*) malloc(sizeof *job);
    job->threads = threads;
    job->thread_count = *thread_count;
    job->workers_info = workers_info;

    return job;
}

void wait_all_threads(job_info* job) {
    for(uint64_t i = 0; i < job->thread_count; i++) {
        pthread_join(job->threads[i], NULL);
    }
    job->thread_count = 0;
    free(job->threads);
    job->threads = NULL;

    free(job->workers_info);
    job->workers_info = NULL;
}
