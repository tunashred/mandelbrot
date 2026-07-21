# Mandelbrot
In this repository I have implemented the Mandelbrot generator in 4 ways:
- singlecore
- multithreaded (using POSIX threads + a thread pool)
- multithreaded SIMD (using Intel intrinsics)
- CUDA

Each implementation resides on different branches.

### Author self note
There might be some unfinished _small_ things, such as multiple Mandelbrot divergence functions on the CUDA implementation, or the color palette creator... but I decided to stop here since I've been slowly milking this project for too long. Even though I learned a lot about profiling, benchmarking, and optimization with each implementation, the slow progress felt like I procrastinated. So I am moving on with other cool projects.
