#include <stdio.h>
#include <cuda.h>
#include <cuda_runtime.h>

#include "kernel.cuh"
#include "mandelbrot.h"

__device__
void mandelbrot_quadratic(const double* z_real, const double* z_im,
                          const double* c_real, const double* c_im,
                          double* rez_real, double* rez_im) {
    *rez_real = (*z_real) * (*z_real) - (*z_im) * (*z_im) + *c_real;
    *rez_im   = 2.0 * (*z_real) * (*z_im) + *c_im;
}

__device__
int _diverge(double c_real, double c_im, int num_iters, mandelbrot_func_t mandelbrot_func) {
    int i = 0;
    double z_real = 0, z_im = 0;
    double z_real_returnat, z_im_returnat;
    while( z_real <= 2.f && z_real >= -2.f && i++ <= num_iters ) {
        mandelbrot_func(&z_real, &z_im, &c_real, &c_im, &z_real_returnat, &z_im_returnat);
        z_real = z_real_returnat;
        z_im = z_im_returnat;
    }
    if(i >= num_iters) {
        // nu diverge
        return 0;
    }
    return i;
}

__device__
double _linear_map(double from, double from_min, double from_max, double to_min, double to_max) {
    double interval_from = from_max - from_min;
    double interval_to = to_max - to_min;
    double normalised_from = from - from_min;
    double normalised_to = interval_to * normalised_from / interval_from;
    return (to_min + normalised_to);
}

__device__
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

    grade += _linear_map(radiani, 0, 2 * M_PI, 0, 360);
    if (grade > 360) {
        grade -= 360;
    }

    radiani = _linear_map(grade, 0, 360, 0, 2 * M_PI);

    *real =     cos(radiani) * raza + centru_real;
    *imaginar = sin(radiani) * raza + centru_im;
}

__global__
void diverge(double c_real, double c_im, int num_iters, mandelbrot_func_t mandelbrot_func, int* ret) {
    *ret = _diverge(c_real, c_im, num_iters, mandelbrot_func);
}

__global__
void _deseneaza_mandelbrot(image_info* d_image_info) {
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;

    if (col >= d_image_info->width || row >= d_image_info->height)
        return;

    double parte_reala     = d_image_info->top_left_coord_real + col * d_image_info->pixel_width;
    double parte_imaginara = d_image_info->top_left_coord_im   - row * d_image_info->pixel_width;

    int iter_count = _diverge(parte_reala, parte_imaginara,
                              d_image_info->num_iters,
                              mandelbrot_quadratic);

    d_image_info->buffer[row * d_image_info->width + col] = (uint32_t)iter_count;
}

extern "C" void cuda_generate_iter_array(image_info* h_image_info) {
    size_t size = (size_t)(h_image_info->height * h_image_info->width);

    uint32_t* d_buffer;
    CUDA_ASSERT(cudaMalloc((void**)&d_buffer, size * sizeof *d_buffer));

    uint32_t* original_host_buffer = h_image_info->buffer;
    h_image_info->buffer = d_buffer;

    image_info* d_image_info;
    CUDA_ASSERT(cudaMalloc((void**)&d_image_info, sizeof *h_image_info));
    CUDA_ASSERT(cudaMemcpy(d_image_info, h_image_info, sizeof *h_image_info, cudaMemcpyHostToDevice));

    h_image_info->buffer = original_host_buffer;

    dim3 block(16, 16);
    dim3 grid(
        (h_image_info->width  + block.x - 1) / block.x,
        (h_image_info->height + block.y - 1) / block.y
    );

    _deseneaza_mandelbrot<<<grid, block>>>(d_image_info);

    CUDA_ASSERT(cudaDeviceSynchronize());

    CUDA_ASSERT(cudaMemcpy(h_image_info->buffer, d_buffer,
                           size * sizeof *d_buffer, cudaMemcpyDeviceToHost));

    CUDA_ASSERT(cudaFree(d_buffer));
    CUDA_ASSERT(cudaFree(d_image_info));
}
