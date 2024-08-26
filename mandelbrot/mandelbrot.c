#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// calculeaza f꜀(z)=z²+c
void mandelbrot(double z_real, double z_im, double c_real, double c_im, double *rez_real, double *rez_im) {
    *rez_real = pow(z_real, 2) - pow(z_im, 2) + c_real;
    *rez_im = 2 * z_real * z_im + c_im;
    // *rez_real = pow(z_real, 5) - 10 * pow(z_real, 3) * pow(z_im, 2) + 5 * z_real * pow(z_im, 4) + c_real;
    // *rez_im = 5 * pow(z_real, 4) * z_im - 10 * pow(z_real, 2) * pow(z_im, 3) + pow(z_im, 5) + c_im;
}

// verifica daca diverge pentru c_real, c_im
int diverge(double c_real, double c_im, int num_iters) {
    int i = 0;
    double z_real = 0, z_im = 0;
    double z_real_returnat, z_im_returnat;
    while( z_real <= 2.f && z_real >= -2.f && i++ <= num_iters ) {
        mandelbrot(z_real, z_im, c_real, c_im, &z_real_returnat, &z_im_returnat);
        z_real = z_real_returnat;
        z_im = z_im_returnat;
    }
    if(i >= num_iters) {
        // nu diverge
        return 0;
    }
    return i;
}

double linear_map(double from, double from_min, double from_max, double to_min, double to_max) {
    double interval_from = from_max - from_min;
    double interval_to = to_max - to_min;
    double normalised_from = from - from_min;
    double normalised_to = interval_to * normalised_from / interval_from;
    return to_min + normalised_to;
}

// y = sin(x/3), x <= 100
int sin_crescator(int iter_count, int num_iters) {
    double max_x = 100;
    double x = linear_map(iter_count, 0, num_iters, 0, max_x);
    double y = sin(x / 3) + x / 10;
    double max_y = sin(max_x / 3) + max_x / 10;
    return (int) linear_map(y, 0, max_y, 0, 255);
}

// y = log(x)/sin(x)+1, 0.6 < x < 2.5 ==> 0 < y < 2.532
int log_pe_sin(int iter_count, int num_iters) {
    double x = linear_map(iter_count, 0, num_iters, 0.6, 2.5);
    double y = log(x) / sin(x) + 1;
    return (int) linear_map(y, 0, 2.532, 0, 255);
}

// y = 1 - 1/x, 1 < x < 2.5 ==> 0 < y < 0.6
int unu_minus_unu_pe_x(int iter_count, int num_iters) {
    double x = linear_map(iter_count, 0, num_iters, 1, 2.5);
    double y = 1 - 1 / x;
    return (int) linear_map(y, 0, 0.6, 0, 255);
}

// y = x², 0 < x < 0.5 ==> 0 < y < 0.25
int x_patrat_0_5(int iter_count, int num_iters) {
    double x = linear_map(iter_count, 0, num_iters, 0, 0.5);
    double y = pow(x, 2);
    return (int) linear_map(y, 0, 0.25, 0, 255);
}

// y = x², 0 < x < 0.1 ==> 0 < y < 0.01
int x_patrat_0_1(int iter_count, int num_iters) {
    double x = linear_map(iter_count, 0, num_iters, 0, 0.1);
    double y = pow(x, 2);
    return (int) linear_map(y, 0, 0.01, 0, 255);
}

// y = x², 0.1 < x < 0.2
int x_patrat_0_1_to_0_2(int iter_count, int num_iters) {
    double min_x = 0.1;
    double max_x = 0.2;
    double x = linear_map(iter_count, 0, num_iters, min_x, max_x);
    double y = pow(x, 2);
    double min_y = pow(min_x, 2);
    double max_y = pow(max_x, 2);
    return (int) linear_map(y, min_y, max_y, 0, 255);
}

// y = sin(x)+1, 4.7 < x < 7.5
int sin_jos_sus_jos(int iter_count, int num_iters) {
    double min_x = 4.7;
    double max_x = 7.5;
    double x = linear_map(iter_count, 0, num_iters, min_x, max_x);
    double y = sin(x) + 1;
    double min_y = sin(min_x) + 1;
    double max_y = sin(max_x) + 1;
    return (int) linear_map(y, min_y, max_y, 0, 255);
}

// y = sin(x⁴)+1, 2.04 < x < 2.9
int sin_x_la_4(int iter_count, int num_iters) {
    double min_x = 2.04;
    double max_x = 2.9;
    double x = linear_map(iter_count, 0, num_iters, min_x, max_x);
    double y = sin(pow(x, 4)) + 1;
    double min_y = sin(pow(min_x, 4)) + 1;
    double max_y = sin(pow(max_x, 4)) + 1;
    return (int) linear_map(y, min_y, max_y, 0, 255);
}

int mapare_simpla(int iter_count, int num_iters) {
    return (int) linear_map(iter_count, 0, num_iters, 0, 255);
}

int red(int iter_count, int num_iters) {
    if(iter_count == 0) {
        return 0;
    }
    return mapare_simpla(iter_count, num_iters);
}

int green(int iter_count, int num_iters) {
    if(iter_count == 0) {
        return 0;
    }
    return mapare_simpla(iter_count, num_iters);
}

int blue(int iter_count, int num_iters) {
    if(iter_count == 0) {
        return 0;
    }
    return mapare_simpla(iter_count, num_iters);
}

/*
    1. Determinam lungimea razei folosind Pitagora.
    2. Determinam unghiul in radiani.
    3. Aflam in ce cadran mutam punctul si calculam distanta lui fata de punctul (1, 0).
    4. Convertim radianii in grade si corectam valoarea daca este mai mare decat 360.
    5. Convertim gradele in radiani si calculam coordonatele reale si imaginare ale punctului rotit.
*/
void roteste(double *real, double *imaginar, double centru_real, double centru_im, double grade) {
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

void deseneaza_mandelbrot(char *nume_poza, int inaltime_poza, int latime_poza, double top_left_coord_real, double top_left_coord_imaginar, double pixel_width, int num_iters) {
    // generam paleta de culori
    int r[1500];
    int g[1500];
    int b[1500];
    int rgb[1500][3];
    double brightness_rate = 1;
    for(int j = 0; j < 1500; j++) {
        r[j] = red(j, 1500);
        g[j] = green(j, 1500);
        b[j] = blue(j, 1500);
        rgb[j][0] = fmin(1499, (j * 7 % 1500) * brightness_rate);
        rgb[j][1] = fmin(1499, (j * 3 % 1500) * brightness_rate);
        rgb[j][2] = fmin(1499, (j * 11 % 1500) * brightness_rate);
    }

    FILE* pgimg;
    pgimg = fopen(nume_poza, "wb");
    fprintf(pgimg, "P3\n"); 
    fprintf(pgimg, "%d %d\n", latime_poza, inaltime_poza);
    fprintf(pgimg, "255\n");

    int numar_pixeli = inaltime_poza * latime_poza;
    int pixel_curent = 0;

    double parte_imaginara = top_left_coord_imaginar;
    for(int i = 0; i < inaltime_poza; i++) {
        double parte_reala = top_left_coord_real;
        for(int j = 0; j < latime_poza; j++) {
            double real_rotit = parte_reala,
                   im_rotit   = parte_imaginara;
            roteste(&real_rotit, &im_rotit, -0.75, 0, 15);
            int iter_count = diverge(real_rotit, im_rotit, num_iters);
            fprintf(pgimg, "%d %d %d\n", r[rgb[iter_count][0]], g[rgb[iter_count][1]], b[rgb[iter_count][2]]);
            parte_reala += pixel_width;

            progress_print(&numar_pixeli, &pixel_curent);
            pixel_curent++;
        }
        fprintf(pgimg, "\n");
        parte_imaginara -= pixel_width;
    }
    fclose(pgimg);
}

void mandelbrot_around_center(char *nume_poza, int inaltime_poza, int latime_poza, double center_coord_real, double center_coord_imaginar, double radius, int num_iters) {
    int latura_scurta =             (inaltime_poza < latime_poza) ? inaltime_poza : latime_poza;
    double pixel_width =             radius * 2 / latura_scurta;
    double top_left_coord_real =     center_coord_real - latime_poza / 2 * pixel_width;
    double top_left_coord_imaginar = center_coord_imaginar + inaltime_poza / 2 * pixel_width;

    deseneaza_mandelbrot(nume_poza, inaltime_poza, latime_poza, top_left_coord_real, top_left_coord_imaginar, pixel_width, num_iters);
}

int main() {
    float scale = 0.5;
    mandelbrot_around_center("tot_setul.ppm", 1080 * scale, 1920 * scale, -0.43, 0, 1, 1500);
    // mandelbrot_around_center("pe_crestele_crapaturii_de_jos.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.75 + 0.00005, -0.02, 0.00025, 1500);
    // mandelbrot_around_center("spirale.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.72413, 0.28644, 0.0004, 1500);
    // mandelbrot_around_center("subtioru_stang_7.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.7, -0.26, 0.01, 1500);
    // mandelbrot_around_center("pene_de_paun.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.700025 + 0.000000007, -0.26849991525, 0.0000000035, 1500);

    return 0;
}