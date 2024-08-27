#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// calculeaza f꜀(z) = z² + c
void mandelbrot_quadratic(double z_real, double z_im, double c_real, double c_im, double *rez_real, double *rez_im);

// calculeaza f꜀(z) = z³ + c
void mandelbrot_cubic(double z_real, double z_im, double c_real, double c_im, double *rez_real, double *rez_im);

// calculeaza f꜀(z) = z⁴ + c
void mandelbrot_quartic(double z_real, double z_im, double c_real, double c_im, double *rez_real, double *rez_im);

// calculeaza f꜀(z) = z⁵ + c
void mandelbrot_quintic(double z_real, double z_im, double c_real, double c_im, double *rez_real, double *rez_im);

// verifica daca diverge pentru c_real, c_im
int diverge(double c_real, double c_im, int num_iters, void (*mandelbrot_func)(double, double, double, double, double*, double*));

/*
    1. Determinam lungimea razei folosind Pitagora.
    2. Determinam unghiul in radiani.
    3. Aflam in ce cadran mutam punctul si calculam distanta lui fata de punctul (1, 0).
    4. Convertim radianii in grade si corectam valoarea daca este mai mare decat 360.
    5. Convertim gradele in radiani si calculam coordonatele reale si imaginare ale punctului rotit.
*/
void roteste(double *real, double *imaginar, double centru_real, double centru_im, double grade);

void progress_print(int* numar_pixeli, int* pixel_curent);

FILE* initialize_image(char* image_name, int height, int width);

void deseneaza_mandelbrot(char *nume_poza, int inaltime_poza, int latime_poza, double top_left_coord_real, double top_left_coord_imaginar, double pixel_width, int num_iters, double rotate_degrees);

void mandelbrot_around_center(char *nume_poza, int inaltime_poza, int latime_poza, double center_coord_real, double center_coord_imaginar, double radius, int num_iters, double rotate_degrees);

#endif