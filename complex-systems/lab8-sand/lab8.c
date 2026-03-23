#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>
#include "lab8.h"

void save_macierz(FILE* plik, int** macierz, size_t L){
    for(int i = 0; i < L; i++){
        for(int j = 0; j < L; j++){
            if(macierz[i][j] == INT_MAX){
                fprintf(plik , "%3d ", 0);
            } else {
                fprintf(plik ,"%3d ", macierz[i][j]);
            }
        }
        fprintf(plik ,"\n");
    }
}

void free_matrix(int** etykiety, size_t L){
    for(int i = 0; i < L; i++){
        free(etykiety[i]);
    }
    free(etykiety);
}

int** make_pryzma(size_t size){

    int** pryzma = malloc(size*sizeof(int*));
    for(int i = 0; i < size; i++){
        pryzma[i] = calloc(size, sizeof(int));
    }

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            pryzma[i][j] = rand()%4;
        }
    }

    return pryzma;
}

void evolution_every_grain_of_sand(int** pryzma, size_t size, int* avalanche_size){
    int unstable = 1;

    while(unstable) {
        unstable = 0;

        for(int idx_x = 0; idx_x < size; idx_x++) {
            for(int idx_y = 0; idx_y < size; idx_y++) {
                
                if(pryzma[idx_x][idx_y] >= 4){
                    unstable = 1;

                    // zmniejszamy stertę
                    pryzma[idx_x][idx_y] -= 4;
                    (*avalanche_size)++;

                    // rozsypujemy na boki
                    if(idx_x + 1 < size) pryzma[idx_x + 1][idx_y]++;
                    if(idx_x - 1 >= 0)   pryzma[idx_x - 1][idx_y]++;
                    if(idx_y + 1 < size) pryzma[idx_x][idx_y + 1]++;
                    if(idx_y - 1 >= 0)   pryzma[idx_x][idx_y - 1]++;
                }
            }
        }
    }
}

int add_grain_of_sand(int** pryzma, size_t size){

    int avalanche_size = 0;

    int idx_x = rand()%size;
    int idx_y = rand()%size;

    pryzma[idx_x][idx_y]++;
    
    evolution_every_grain_of_sand(pryzma, size, &avalanche_size);

    return avalanche_size;
}

int main(){
    srand(time(NULL));

    // ZADNIE 1
    int rozmiar_pryzmy1 = 10;

    FILE* p1 = fopen("pryzmy1.dat", "w");
    FILE* s1 = fopen("s1.dat", "w");

    int** pryzma1 = make_pryzma(rozmiar_pryzmy1);
    fprintf(p1, "pryzma 0\n");
    save_macierz(p1, pryzma1, rozmiar_pryzmy1);

    int rozmiar_lawiny = 0;
    int nr = 1;
    while(rozmiar_lawiny < 8){
        rozmiar_lawiny = add_grain_of_sand(pryzma1, rozmiar_pryzmy1);
        if(rozmiar_lawiny != 0){
            fprintf(s1, "%d %d\n", nr, rozmiar_lawiny);
        }
        fprintf(p1, "\n nr kroku: %d, rozmiar lawiny: %d \n", nr, rozmiar_lawiny);
        save_macierz(p1, pryzma1, rozmiar_pryzmy1);
        nr++;
    }

    free_matrix(pryzma1, rozmiar_pryzmy1);
    fclose(p1);
    fclose(s1);

    // ZADNIE 2
    int rozmiar_pryzmy2 = 20;
    int N_max = 10000;

    FILE* p2 = fopen("pryzmy2.dat", "w");
    FILE* s2 = fopen("s2.dat", "w");

    int** pryzma2 = make_pryzma(rozmiar_pryzmy2);
    fprintf(p2, "pryzma 0\n");
    save_macierz(p2, pryzma2, rozmiar_pryzmy2);

    nr = 1;
    for(int i = 0; i < N_max; i++){
        rozmiar_lawiny = add_grain_of_sand(pryzma2, rozmiar_pryzmy2);
        if(rozmiar_lawiny != 0){
            fprintf(s2, "%d %d\n", nr, rozmiar_lawiny);
        }
        fprintf(p2, "\n nr kroku: %d, rozmiar lawiny: %d \n", nr, rozmiar_lawiny);
        save_macierz(p2, pryzma2, rozmiar_pryzmy2);
        nr++;
    }

    free_matrix(pryzma2, rozmiar_pryzmy2);
    fclose(p2);
    fclose(s2);
    
    return 0;
}