#include "color_mapping.h"
#include "gtest/gtest.h"
#include <pthread.h>
#include <cstdlib>
#include <cstring>
#include <mandelbrot.h>

int thread_count = 6;
// these coordinates diverge at 1451 iterations
double test_real = -0.44;
double test_imaginary = -0.56;
int max_iterations = 1500;

struct ThreadData {
    int* buffer;
    long buffer_size;
    long start_index;
    long end_index;
    int stress_iterations;
};

void* stress_diverge(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    for (int i = 0; i < data->stress_iterations; ++i) {
        diverge(test_real, test_imaginary, max_iterations, mandelbrot_quadratic);
    }
    return NULL;
}

void* buffer_stress(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    int* buffer = data->buffer;
    long buffer_size = data->buffer_size;

    for (long i = 0; i < data->buffer_size; ++i) {
        buffer[i] = diverge(test_real, test_imaginary, max_iterations, mandelbrot_quadratic);
    }
    return NULL;
}

void* shared_buffer_stress(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    int* buffer = data->buffer;
    long start_index = data->start_index;
    long end_index = data->end_index;

    for (long i = start_index; i < end_index; ++i) {
        buffer[i] = diverge(test_real, test_imaginary, max_iterations, mandelbrot_quadratic);
    }
    return NULL;
}

class MultithreadStressCompute : public ::testing::TestWithParam<int> {
protected:
    pthread_t* threads;
    ThreadData* thread_data;
    int stress_iterations;

    void SetUp() override {
        stress_iterations = GetParam();
        threads = new pthread_t[thread_count];
        thread_data = new ThreadData[thread_count];
        for (int i = 0; i < thread_count; ++i) {
            thread_data[i].stress_iterations = stress_iterations;
        }
    }

    void TearDown() override {
        delete[] threads;
        delete[] thread_data;
    }

    void run_threads(void* (*thread_function)(void*)) {
        for (int i = 0; i < thread_count; ++i) {
            pthread_create(&threads[i], NULL, thread_function, &thread_data[i]);
        }
    }

    void wait_all_threads() {
        for (int i = 0; i < thread_count; ++i) {
            pthread_join(threads[i], NULL);
        }
    }
};

TEST_P(MultithreadStressCompute, MaxDiverge) {
    run_threads(stress_diverge);
    wait_all_threads();
}

TEST_P(MultithreadStressCompute, IndividualBuffersMaxDiverge) {
    for(int i = 0; i < thread_count; ++i) {
        thread_data[i].buffer_size = (stress_iterations / thread_count) * RGB_CHANNELS;
        thread_data[i].buffer = (int*) malloc(thread_data[i].buffer_size * sizeof(int));
    }

    run_threads(buffer_stress);
    wait_all_threads();

    for(int i = 0; i < thread_count; ++i) {
        free(thread_data[i].buffer);
    }
}

TEST_P(MultithreadStressCompute, SharedBufferMaxDiverge) {
    long total_buffer_size = stress_iterations * RGB_CHANNELS;
    int* shared_buffer = (int*) malloc(total_buffer_size * sizeof(int));

    long slice_size = total_buffer_size / thread_count;

    for (int i = 0; i < thread_count; ++i) {
        thread_data[i].buffer = shared_buffer;
        thread_data[i].buffer_size = total_buffer_size;

        thread_data[i].start_index = i * slice_size;
        thread_data[i].end_index = (i == thread_count - 1) ? total_buffer_size : (i + 1) * slice_size;
    }

    run_threads(shared_buffer_stress);
    wait_all_threads();

    free(shared_buffer);
}


INSTANTIATE_TEST_SUITE_P(
    mandatory_compute,
    MultithreadStressCompute,
    ::testing::Values(2000000)
);