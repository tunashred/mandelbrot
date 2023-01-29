#include <stdio.h>
#include <stdlib.h>

void afisare_element(int **matrix, int i, int j);
int** input(char* nume_fisier, int* linii, int* coloane);
void curatare_matrice(int*** matrix, int linii, int coloane);

int main() {
    int linii, coloane;
    int** matrix = input("matrice_serpuita_5x5.in", &linii, &coloane);
    // int** matrix = input("matrice_serpuita_6x6.in", &linii, &coloane);
    // int** matrix = input("matrice_serpuita_6x9.in", &linii, &coloane);
    int i = 0, j = 0;
    linii--;
    coloane--;
    printf("%d ", matrix[i][j]);
    while( i != linii || j != coloane ) {
        if(i != linii) {
            i++;
        }
        else j++;
        for(; (i != 0) && (j != coloane); i--, j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("%d ", matrix[i][j]);
        if(j != coloane) {
            j++;
        }
        else i++;
        for(; (i != linii) && (j != 0); i++, j--) {
            printf("%d ", matrix[i][j]);
        }
        printf("%d ", matrix[i][j]);
    }
    curatare_matrice(&matrix, ++linii, ++coloane);

    printf("\nSpor la cafelutsa cu peste");
    return 0;
}

void afisare_element(int **matrix, int i, int j) {
    printf("%d ", matrix[i][j]);
}

int** input(char* nume_fisier, int* linii, int* coloane) {
    FILE* file = fopen (nume_fisier, "r");

    fscanf (file, "%d %d", linii, coloane);

    int** matrice = (int**) malloc(*linii * sizeof(int*));
    

    for(int i = 0; i < *linii; i++) {
        matrice[i] = (int*) malloc(*coloane * sizeof(int)); 
        for(int j = 0; j < *coloane; j++) {
            fscanf(file, "%d", &matrice[i][j]);
        }
    }
    fclose (file);

    return matrice;
}

void curatare_matrice(int*** matrix, int linii, int coloane) {
    for(int i = 0; i < linii; i++) {
        free( (*matrix)[i] );
        (*matrix)[i] = NULL;  
    }
    free(*matrix);
    *matrix = NULL;
}