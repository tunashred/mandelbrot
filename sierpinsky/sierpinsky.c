#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

struct point2d {
	double x, y;
};

struct point2d introducere_punct2d();

void date_initiale_triunghi(int dimensiune, int latura, int *nr_puncte, struct point2d *puncte);
void date_initiale_triunghi_4k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte);
void date_initiale_triunghi_10k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte);
void date_initiale_patrat(int dimensiune, int latura, int *nr_puncte, struct point2d *puncte);
void date_initiale_patrat_4k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte);
void date_initiale_patrat_10k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte);
void date_initiale_romb(int dimensiune, int latura, int *nr_puncte, struct point2d *puncte);
void date_initiale_romb_4k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte);
void date_initiale_romb_10k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte);
void date_initiale_pentagon(int dimensiune, int latura, int *nr_puncte, struct point2d *puncte);
void date_initiale_pentagon_4k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte);
void date_initiale_pentagon_10k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte);
void date_initiale_pentagon_rotit(int dimensiune, int latura, int *nr_puncte, struct point2d *puncte);
void date_initiale_pentagon_rotit_4k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte);
void date_initiale_pentagon_rotit_10k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte);
void date_initiale_hexagon(int dimensiune, int latura, int *nr_puncte, struct point2d *puncte);
void date_initiale_hexagon_4k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte);
void date_initiale_hexagon_10k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte);
void date_initiale_hexagon_rotit(int dimensiune, int latura, int *nr_puncte, struct point2d *puncte);
void date_initiale_hexagon_rotit_4k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte);
void date_initiale_hexagon_rotit_10k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte);

struct point2d mijloc_latura(struct point2d p1, struct point2d p2);
void generare_puncte_random(struct point2d puncte[], int numar_puncte, int numar_puncte_initiale);
int **generare_poza(struct point2d puncte[], int numar_puncte, int dimensiune);

void swap(int *x, int *y);
void clockwise_rotation(int **data, int dimensiune);
void counterclockwise_rotation(int **data, int dimensiune);
void vertical_flip(int **data, int dimensiune);

void printare_poza(int **data, int dimensiune, char *nume_fisier);

int main () {
    static int nr_puncte = 5000000;
    static struct point2d puncte[5000000];
    int numar_puncte_initiale;
    time_t t;
    int dimensiune, latura;
    int **data;
    srand((unsigned) time(&t));

    printf("Se genereaza triunghi 4k\n");
    date_initiale_triunghi_4k(&dimensiune, &latura, &numar_puncte_initiale, puncte);
    generare_puncte_random(puncte, nr_puncte, numar_puncte_initiale);
    data = generare_poza(puncte, nr_puncte, dimensiune);
    vertical_flip(data, dimensiune);
    printare_poza(data, dimensiune, "sierpinsky_triunghi_4k.pgm");

    // printf("Se genereaza triunghi 10k\n");
    // date_initiale_triunghi_10k(&dimensiune, &latura, &numar_puncte_initiale, puncte);
    // generare_puncte_random(puncte, nr_puncte, numar_puncte_initiale);
    // data = generare_poza(puncte, nr_puncte, dimensiune);
    // printare_poza(data, dimensiune, "sierpinsky_triunghi_10k.pgm");

    // printf("Se genereaza patrat 4k\n");
    // date_initiale_patrat_4k(&dimensiune, &latura, &numar_puncte_initiale, puncte);
    // generare_puncte_random(puncte, nr_puncte, numar_puncte_initiale);
    // data = generare_poza(puncte, nr_puncte, dimensiune);
    // printare_poza(data, dimensiune, "sierpinsky_patrat_4k.pgm");

    // printf("Se genereaza patrat 10k\n");
    // date_initiale_patrat_10k(&dimensiune, &latura, &numar_puncte_initiale, puncte);
    // generare_puncte_random(puncte, nr_puncte, numar_puncte_initiale);
    // data = generare_poza(puncte, nr_puncte, dimensiune);
    // printare_poza(data, dimensiune, "sierpinsky_patrat_10k.pgm");

    // printf("Se genereaza romb 4k\n");
    // date_initiale_romb_4k(&dimensiune, &latura, &numar_puncte_initiale, puncte);
    // generare_puncte_random(puncte, nr_puncte, numar_puncte_initiale);
    // data = generare_poza(puncte, nr_puncte, dimensiune);
    // printare_poza(data, dimensiune, "sierpinsky_romb_4k.pgm");

    // printf("Se genereaza romb 10k\n");
    // date_initiale_romb_10k(&dimensiune, &latura, &numar_puncte_initiale, puncte);
    // generare_puncte_random(puncte, nr_puncte, numar_puncte_initiale);
    // data = generare_poza(puncte, nr_puncte, dimensiune);
    // counterclockwise_rotation(data, dimensiune);
    // printare_poza(data, dimensiune, "sierpinsky_romb_10k.pgm");

    // printf("Se genereaza pentagon 4k\n");
    // date_initiale_pentagon_4k(&dimensiune, &latura, &numar_puncte_initiale, puncte);
    // generare_puncte_random(puncte, nr_puncte, numar_puncte_initiale);
    // data = generare_poza(puncte, nr_puncte, dimensiune);
    // counterclockwise_rotation(data, dimensiune);
    // printare_poza(data, dimensiune, "sierpinsky_pentagon_4k.pgm");

    // printf("Se genereaza pentagon 10k\n");
    // date_initiale_pentagon_10k(&dimensiune, &latura, &numar_puncte_initiale, puncte);
    // generare_puncte_random(puncte, nr_puncte, numar_puncte_initiale);
    // data = generare_poza(puncte, nr_puncte, dimensiune);
    // counterclockwise_rotation(data, dimensiune);
    // printare_poza(data, dimensiune, "sierpinsky_pentagon_10k.pgm");

    // printf("Se genereaza pentagon rotit 4k\n");
    // date_initiale_pentagon_rotit_4k(&dimensiune, &latura, &numar_puncte_initiale, puncte);
    // generare_puncte_random(puncte, nr_puncte, numar_puncte_initiale);
    // data = generare_poza(puncte, nr_puncte, dimensiune);
    // counterclockwise_rotation(data, dimensiune);
    // printare_poza(data, dimensiune, "sierpinsky_pentagon_rotit_4k.pgm");

    // printf("Se genereaza pentagon rotit 10k\n");
    // date_initiale_pentagon_rotit_10k(&dimensiune, &latura, &numar_puncte_initiale, puncte);
    // generare_puncte_random(puncte, nr_puncte, numar_puncte_initiale);
    // data = generare_poza(puncte, nr_puncte, dimensiune);
    // counterclockwise_rotation(data, dimensiune);
    // printare_poza(data, dimensiune, "sierpinsky_pentagon_rotit_10k.pgm");

    // printf("Se genereaza hexagon 4k\n"); 
    // date_initiale_hexagon_4k(&dimensiune, &latura, &numar_puncte_initiale, puncte);
    // generare_puncte_random(puncte, nr_puncte, numar_puncte_initiale);
    // data = generare_poza(puncte, nr_puncte, dimensiune);
    // counterclockwise_rotation(data, dimensiune);
    // printare_poza(data, dimensiune, "sierpinsky_hexagon_4k.pgm");

    // printf("Se genereaza hexagon 10k\n");
    // date_initiale_hexagon_10k(&dimensiune, &latura, &numar_puncte_initiale, puncte);
    // generare_puncte_random(puncte, nr_puncte, numar_puncte_initiale);
    // data = generare_poza(puncte, nr_puncte, dimensiune);
    // counterclockwise_rotation(data, dimensiune);
    // printare_poza(data, dimensiune, "sierpinsky_hexagon_10k.pgm");

    // printf("Se genereaza hexagon rotit 4k\n");
    // date_initiale_hexagon_rotit_4k(&dimensiune, &latura, &numar_puncte_initiale, puncte);
    // generare_puncte_random(puncte, nr_puncte, numar_puncte_initiale);
    // data = generare_poza(puncte, nr_puncte, dimensiune);
    // counterclockwise_rotation(data, dimensiune);
    // printare_poza(data, dimensiune, "sierpinsky_hexagon_rotit_4k.pgm");

    // printf("Se genereaza hexagon rotit 10k\n");
    // date_initiale_hexagon_rotit_10k(&dimensiune, &latura, &numar_puncte_initiale, puncte);
    // generare_puncte_random(puncte, nr_puncte, numar_puncte_initiale);
    // data = generare_poza(puncte, nr_puncte, dimensiune);
    // counterclockwise_rotation(data, dimensiune);
    // printare_poza(data, dimensiune, "sierpinsky_hexagon_rotit_10k.pgm");

    printf("Spor la cafelutsa cu peste\n");

    return 0;
}

struct point2d introducere_punct2d() {
	struct point2d pct;
	printf("\nIntroduceti valoare coordonatei x: ");
	scanf("%lf", &pct.x);
	printf("Introduceti valoarea coordonatei y: ");
	scanf("%lf", &pct.y);
	return pct;
}

void date_initiale_triunghi(int dimensiune, int latura, int *nr_puncte, struct point2d *puncte) {
    int inaltime = round( sqrt(3) * latura / 2 );
    int offset_x = (dimensiune - latura) / 2;
    int offset_y = (dimensiune - inaltime) / 2;

    puncte[0] = (struct point2d) { offset_x + latura / 2, offset_y + 0        };
    puncte[1] = (struct point2d) { offset_x + 0,          offset_y + inaltime };
    puncte[2] = (struct point2d) { offset_x + latura,     offset_y + inaltime };
    *nr_puncte = 3;
}

void date_initiale_triunghi_4k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte) {
    *dimensiune = 4096;
    *latura = 4038;
    date_initiale_triunghi(*dimensiune, *latura, nr_puncte, puncte);
}

void date_initiale_triunghi_10k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte) {
    *dimensiune = 10000;
    *latura = 9666;
    date_initiale_triunghi(*dimensiune, *latura, nr_puncte, puncte);
}

void date_initiale_patrat(int dimensiune, int latura, int *nr_puncte, struct point2d *puncte) {
    int offset_y = (dimensiune - latura) / 2;
    int offset_x = (dimensiune - latura) / 2;

    puncte[0] = (struct point2d) { offset_x + 0,      offset_y + 0      };
    puncte[1] = (struct point2d) { offset_x + latura, offset_y + 0      };
    puncte[2] = (struct point2d) { offset_x + latura, offset_y + latura };
    puncte[3] = (struct point2d) { offset_x + 0,      offset_y + latura };
    *nr_puncte = 4;
}

void date_initiale_patrat_4k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte) {
    *dimensiune = 4096;
    *latura = 4000;
    date_initiale_patrat(*dimensiune, *latura, nr_puncte, puncte);
}

void date_initiale_patrat_10k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte) {
    *dimensiune = 10000;
    *latura = 9800;
    date_initiale_patrat(*dimensiune, *latura, nr_puncte, puncte);
}

void date_initiale_romb(int dimensiune, int latura, int *nr_puncte, struct point2d *puncte) {
    int offset_y = (dimensiune - latura) / 2;
    int offset_x = (dimensiune - latura) / 2;

    puncte[0] = (struct point2d) { offset_x + latura / 2, offset_y + 0          };
    puncte[1] = (struct point2d) { offset_x + latura,     offset_y + latura / 2 };
    puncte[2] = (struct point2d) { offset_x + latura / 2, offset_y + latura     };
    puncte[3] = (struct point2d) { offset_x + 0,          offset_y + latura / 2 };
    *nr_puncte = 4;
}

void date_initiale_romb_4k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte) {
    *dimensiune = 4096;
    *latura = 4000;
    date_initiale_romb(*dimensiune, *latura, nr_puncte, puncte);
}

void date_initiale_romb_10k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte) {
    *dimensiune = 10000;
    *latura = 9800;
    date_initiale_romb(*dimensiune, *latura, nr_puncte, puncte);
}
    
void date_initiale_pentagon(int dimensiune, int latura, int *nr_puncte, struct point2d *puncte) {
    int max_y_adjust = abs( round( cos(M_PI / 180 * 54) * latura ) );
    int half_x = abs( round( sin(M_PI / 180 * 54) * latura ) );
    int cateta_mare = abs( round( sin(M_PI / 180 * 72) * latura ) );
    int cateta_mica = abs( round( cos(M_PI / 180 * 72) * latura ) );
    int offset_y = (dimensiune - cateta_mare - max_y_adjust) / 2;
    int offset_x = dimensiune / 2 - half_x;

    puncte[0] = (struct point2d) { offset_x + half_x ,              offset_y + 0                          };
    puncte[1] = (struct point2d) { offset_x + half_x * 2,           offset_y + max_y_adjust               };
    puncte[2] = (struct point2d) { offset_x + latura + cateta_mica, offset_y + max_y_adjust + cateta_mare };
    puncte[3] = (struct point2d) { offset_x + cateta_mica,          offset_y + max_y_adjust + cateta_mare };
    puncte[4] = (struct point2d) { offset_x + 0,                    offset_y + max_y_adjust               };
    *nr_puncte = 5;
}

void date_initiale_pentagon_4k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte) {
    *dimensiune = 4096;
    *latura = 2472;
    date_initiale_pentagon(*dimensiune, *latura, nr_puncte, puncte);
}

void date_initiale_pentagon_10k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte) {
    *dimensiune = 10000;
    *latura = 6152;
    date_initiale_pentagon(*dimensiune, *latura, nr_puncte, puncte);
}

void date_initiale_pentagon_rotit(int dimensiune, int latura, int *nr_puncte, struct point2d *puncte) {
    int max_y_adjust = abs( round( cos(M_PI / 180 * 54) * latura ) );
    int half_x = abs( round( sin(M_PI / 180 * 54) * latura ) );
    int cateta_mare = abs( round( sin(M_PI / 180 * 72) * latura ) );
    int cateta_mica = abs( round( cos(M_PI / 180 * 72) * latura ) );
    int offset_y = (dimensiune - cateta_mare - max_y_adjust) / 2;
    int offset_x = dimensiune / 2 - half_x;

    puncte[0] = (struct point2d) { offset_x +  cateta_mica,         offset_y + 0                          };
    puncte[1] = (struct point2d) { offset_x + cateta_mica + latura, offset_y + 0                          };
    puncte[2] = (struct point2d) { offset_x + half_x * 2,           offset_y + cateta_mare                };
    puncte[3] = (struct point2d) { offset_x + half_x,               offset_y + cateta_mare + max_y_adjust };
    puncte[4] = (struct point2d) { offset_x + 0,                    offset_y + cateta_mare                };
    *nr_puncte = 5;
}

void date_initiale_pentagon_rotit_4k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte) {
    *dimensiune = 4096;
    *latura = 2472;
    date_initiale_pentagon_rotit(*dimensiune, *latura, nr_puncte, puncte);
}

void date_initiale_pentagon_rotit_10k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte) {
    *dimensiune = 10000;
    *latura = 6152;
    date_initiale_pentagon_rotit(*dimensiune, *latura, nr_puncte, puncte);
}

void date_initiale_hexagon(int dimensiune, int latura, int *nr_puncte, struct point2d *puncte) {
    int inaltime = round( sqrt(3) * latura / 2 );
    int offset_y = dimensiune / 2 - inaltime;
    int offset_x = dimensiune / 2 - latura;

    puncte[0] = (struct point2d) { offset_x + latura / 2,          offset_y + 0            };
    puncte[1] = (struct point2d) { offset_x + latura / 2 + latura, offset_y + 0            };
    puncte[2] = (struct point2d) { offset_x + latura * 2,          offset_y + inaltime     };
    puncte[3] = (struct point2d) { offset_x + latura + latura / 2, offset_y + inaltime * 2 };
    puncte[4] = (struct point2d) { offset_x + latura / 2,          offset_y + inaltime * 2 };
    puncte[5] = (struct point2d) { offset_x + 0 ,                  offset_y + inaltime     };
    *nr_puncte = 6;
}

void date_initiale_hexagon_4k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte) {
    *dimensiune = 4096;
    *latura = 1978;
    date_initiale_hexagon(*dimensiune, *latura, nr_puncte, puncte);
}

void date_initiale_hexagon_10k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte) {
    *dimensiune = 10000;
    *latura = 4874;
    date_initiale_hexagon(*dimensiune, *latura, nr_puncte, puncte);
}

void date_initiale_hexagon_rotit(int dimensiune, int latura, int *nr_puncte, struct point2d *puncte) {
    int inaltime = round( sqrt(3) * latura / 2 );
    int offset_x = dimensiune / 2 - inaltime;
    int offset_y = dimensiune / 2 - latura;

    puncte[0] = (struct point2d) { offset_x + inaltime,     offset_y + 0                   };
    puncte[1] = (struct point2d) { offset_x + inaltime * 2, offset_y + latura / 2          };
    puncte[2] = (struct point2d) { offset_x + inaltime * 2, offset_y + latura + latura / 2 };
    puncte[3] = (struct point2d) { offset_x + inaltime,     offset_y + latura * 2          };
    puncte[4] = (struct point2d) { offset_x + 0,            offset_y + latura + latura / 2 };
    puncte[5] = (struct point2d) { offset_x + 0 ,           offset_y + latura / 2          };
    *nr_puncte = 6;
}

void date_initiale_hexagon_rotit_4k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte) {
    *dimensiune = 4096;
    *latura = 1978;
    date_initiale_hexagon_rotit(*dimensiune, *latura, nr_puncte, puncte);
}

void date_initiale_hexagon_rotit_10k(int *dimensiune, int *latura, int *nr_puncte, struct point2d *puncte) {
    *dimensiune = 10000;
    *latura = 4874;
    date_initiale_hexagon_rotit(*dimensiune, *latura, nr_puncte, puncte);
}

struct point2d mijloc_latura(struct point2d p1, struct point2d p2) {
	struct point2d mijloc;
	mijloc.x = ((p2.x + p1.x) / 2);
	mijloc.y = ((p2.y + p1.y) / 2);
	return mijloc;
}

void generare_puncte_random(struct point2d puncte[], int numar_puncte, int numar_puncte_initiale) {
    int count = numar_puncte_initiale;

    while(++count < numar_puncte) {
        puncte[count] = mijloc_latura(puncte[count - 1], puncte[rand() % numar_puncte_initiale]);
    }
}

int **generare_poza(struct point2d puncte[], int numar_puncte, int dimensiune) {
    int **data;
    data = malloc(sizeof(int*) * dimensiune);
    for(int i = 0; i < dimensiune; i++) {
        data[i] = calloc(dimensiune, sizeof(int));
    }
    for(int i = 0; i < numar_puncte; i++) {
        data[(int) puncte[i].x][(int) puncte[i].y] = 1;
    }
    return data;
}

void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

/* Matrix rotated 90 degrees clockwise */
void clockwise_rotation(int **data, int dimensiune) {
    // transpose
    for(int i = 0; i < dimensiune; i++) {
        for(int j = i; j < dimensiune; j++) {
            swap(&data[i][j], &data[j][i]);
        }
    }

    // vertical mirroring
    for(int j = 0; j < dimensiune - j - 1; j++) {
        for(int i = 0; i < dimensiune; i++) {
            swap(&data[i][j], &data[i][dimensiune - j - 1]);
        }
    }
}

/* Matrix rotated 90 degrees counter-clockwise */
void counterclockwise_rotation(int **data, int dimensiune) {
    for(int i = 0; i < dimensiune; i++) {
        for(int j = i; j < dimensiune; j++) {
            swap(&data[i][j], &data[j][i]);
        }
    }

    // vertical mirroring
    for(int j = 0; j < dimensiune; j++) {
        for(int i = 0; i < dimensiune / 2; i++) {
            swap(&data[i][j], &data[dimensiune - i - 1][j]);
        }
    }
}

void vertical_flip(int **data, int dimensiune) {
    clockwise_rotation(data, dimensiune);
    clockwise_rotation(data, dimensiune);
}

void printare_poza(int **data, int dimensiune, char *nume_fisier) { 
    FILE* pgimg;
    pgimg = fopen(nume_fisier, "wb");
    fprintf(pgimg, "P2\n");
    fprintf(pgimg, "%d %d\n", dimensiune, dimensiune);
    fprintf(pgimg, "1\n");

    for(int y = 0; y < dimensiune; y++) {
        for(int x = 0; x < dimensiune; x++) {
            fprintf(pgimg, "%d ", data[x][y]);
        }
        fprintf(pgimg, "\n");
    }
    fclose(pgimg);

    for(int i = 0; i < dimensiune; i++) {
        free(data[i]);
        data[i] = NULL;
    }
    free(data);
    data = NULL;
    
    return;
}
