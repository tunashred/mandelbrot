#include "gtest/gtest.h"
#include <pthread.h>
#include <cstdlib>
#include <cstring>
#include <mandelbrot.h>

#define RGB_VALUES 3

int thread_count = 6;
// these coordinates diverge at 1451 iterations
double test_real = -0.44;
double test_imaginary = -0.56;
int max_iterations = 1500;
int stress_iterations = 2000000;

struct ThreadData {
    int* buffer;
    int buffer_size;
    int start_index;
    int end_index;
};

void* stress_diverge(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    for (int i = 0; i < stress_iterations; ++i) {
        diverge(test_real, test_imaginary, max_iterations, mandelbrot_quadratic);
    }
    return NULL;
}

void* buffer_stress(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    int* buffer = data->buffer;
    int buffer_size = data->buffer_size;

    for (int i = 0; i < stress_iterations; ++i) {
        buffer[i] = diverge(test_real, test_imaginary, max_iterations, mandelbrot_quadratic);
    }
    return NULL;
}

void* shared_buffer_stress(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    int* buffer = data->buffer;
    int start_index = data->start_index;
    int end_index = data->end_index;

    for (int i = start_index; i < end_index; ++i) {
        buffer[i] = diverge(test_real, test_imaginary, max_iterations, mandelbrot_quadratic);
    }
    return NULL;
}

class MultithreadStressCompute : public ::testing::Test {
protected:
    pthread_t* threads;
    ThreadData* thread_data;
    
    void SetUp() override {
        threads = new pthread_t[thread_count];
        thread_data = new ThreadData[thread_count];
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

TEST_F(MultithreadStressCompute, MaxDiverge) {
    run_threads(stress_diverge);
    wait_all_threads();
}

TEST_F(MultithreadStressCompute, IndividualBuffersMaxDiverge) {
    for(int i = 0; i < thread_count; ++i) {
        thread_data[i].buffer_size = stress_iterations * RGB_VALUES;
        std::cout << thread_data[i].buffer_size << " <-- buffer size of " << i << " thread\n";
        thread_data[i].buffer = (int*) malloc(stress_iterations * sizeof(int));
    }

    run_threads(buffer_stress);
    wait_all_threads();

    for(int i = 0; i < thread_count; ++i) {
        free(thread_data[i].buffer);
    }
}

TEST_F(MultithreadStressCompute, SharedBufferMaxDiverge) {
    int buffer_size = stress_iterations * thread_count * RGB_VALUES;
    std::cout << "shared buffer size: " << buffer_size << '\n';
    int* shared_buffer = (int*) malloc(buffer_size * sizeof(int));

    for(int i = 0; i < thread_count; ++i) {
        thread_data[i].buffer = shared_buffer;
        thread_data[i].buffer_size = buffer_size;
        thread_data[i].start_index = i * (buffer_size / thread_count);
        int end_index = (i + 1) * (buffer_size / thread_count);
        if(end_index > buffer_size) {
            thread_data[i].end_index = buffer_size;
        } else {
            thread_data[i].end_index = end_index;
        }
    }

    run_threads(shared_buffer_stress);
    wait_all_threads();

    free(shared_buffer);
}
