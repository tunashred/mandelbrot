#ifndef CUDA_KERNEL_CUH
#define CUDA_KERNEL_CUH

#define CEIL_DIV(a, b) (a + b - 1) / b

#define CUDA_ASSERT(call)                                                      \
  {                                                                            \
    cudaError_t e = (call);                                                    \
    if (e != cudaSuccess) {                                                    \
      fprintf(stderr, "CUDA error %s:%d: %s\n", __FILE__, __LINE__,            \
              cudaGetErrorString(e));                                          \
      exit(1);                                                                 \
    }                                                                          \
  }

#endif
