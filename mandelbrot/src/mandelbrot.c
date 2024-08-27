#include "mandelbrot.h"
#include "color_mapping.h"

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


void progress_print(int* numar_pixeli, int* pixel_curent) {
    char* lines = "/-\\";
    static int procent = 1;
    static int anunta_la = 0;
    int pixel_per_procent = *numar_pixeli / 100; // need to find a way to make it work with static qualifier

    if(*pixel_curent == anunta_la) {
        fflush(stdout);
        printf("%d%% complete... %c\r", procent, lines[procent % 3]);
        anunta_la += pixel_per_procent;
        procent++;
    }
}

FILE* initialize_image(char* image_name, int height, int width) {
    FILE* pgimg;
    pgimg = fopen(image_name, "wb");
    fprintf(pgimg, "P3\n"); 
    fprintf(pgimg, "%d %d\n", width, height);
    fprintf(pgimg, "255\n");
    return pgimg;
}

void deseneaza_mandelbrot(char *nume_poza, int inaltime_poza, int latime_poza, double top_left_coord_real, double top_left_coord_imaginar, double pixel_width, int num_iters, double rotate_degrees) {
    // generam paleta de culori
    ColorPalette palette;
    double brightness_rate = 1;

    generate_color_palette(&palette, brightness_rate, NULL, x_patrat_0_5, log_pe_sin, sin_crescator);

    FILE* pgimg = initialize_image(nume_poza, inaltime_poza, latime_poza);

    int numar_pixeli = inaltime_poza * latime_poza;
    int pixel_curent = 0;

    double parte_imaginara = top_left_coord_imaginar;
    for(int i = 0; i < inaltime_poza; i++) {
        double parte_reala = top_left_coord_real;
        for(int j = 0; j < latime_poza; j++) {
            double real_rotit = parte_reala,
                   im_rotit   = parte_imaginara;
            roteste(&real_rotit, &im_rotit, -0.75, 0, rotate_degrees);
            
            int iter_count = diverge(real_rotit, im_rotit, num_iters, mandelbrot_quadratic);
            fprintf(pgimg, "%d %d %d\n",
                    palette.r[palette.rgb[iter_count][0]],
                    palette.g[palette.rgb[iter_count][1]],
                    palette.b[palette.rgb[iter_count][2]]
            );
            parte_reala += pixel_width;

            progress_print(&numar_pixeli, &pixel_curent);
            pixel_curent++;
        }
        fprintf(pgimg, "\n");
        parte_imaginara -= pixel_width;
    }
    fclose(pgimg);
}

void mandelbrot_around_center(char *nume_poza, int inaltime_poza, int latime_poza, double center_coord_real, double center_coord_imaginar, double radius, int num_iters, double rotate_degrees) {
    int latura_scurta =             (inaltime_poza < latime_poza) ? inaltime_poza : latime_poza;
    double pixel_width =             radius * 2 / latura_scurta;
    double top_left_coord_real =     center_coord_real - latime_poza / 2 * pixel_width;
    double top_left_coord_imaginar = center_coord_imaginar + inaltime_poza / 2 * pixel_width;

    deseneaza_mandelbrot(nume_poza, inaltime_poza, latime_poza, top_left_coord_real, top_left_coord_imaginar, pixel_width, num_iters, rotate_degrees);
}
