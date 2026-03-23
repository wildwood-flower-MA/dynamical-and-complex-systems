#include <stdio.h>
#include <stdlib.h>
#include "lab1.h"

int main(void){

    int N = 10;

    // ZADANIE 1 -------------------------
    
    int** matrix1 = malloc(N*sizeof(int*));
    for(int i = 0; i<N; i++){
        matrix1[i] = malloc(N*sizeof(int));
    }

    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            if(x == y){
                matrix1[y][x] = 0;
            } else {
                matrix1[y][x] = 1;
            }
        }
    }

    // znajdowanie rozkladu
    int* sums = calloc(N, sizeof(int));
    for(int y = 0; y < N; y++){
        for(int x = y; x < N; x++){
            sums[y] += matrix1[y][x];            
        }
    }
    int* degree_distribution = calloc(N-1, sizeof(int));
    for(int n = 0; n < N-1; n++){
        if(sums[n] == (n+1)){
            degree_distribution[n]++;
        }
    }

    free(sums);
    free(degree_distribution);
    for(int i = 0; i<N; i++){
        free(matrix1[i]);
    }
    free(matrix1);

    // ZADANIE 2 -------------------------



    /* srand(time(NULL));
    int neighbors_list = malloc(N*sizeof(int*));
    for(int x = 0; x < N; i++){

        neighbors_list
    } */


    /*
    int** matrix = malloc(N*sizeof(int*));
    for(int i = 0; i<N; i++){
        matrix[i] = malloc(N*sizeof(int));
    }
    
    for(int y = 0; y < N; y++){
        for(int x = 0; x <= y; x++){
            if (rand()%3 == 0){
                matrix[y][x] = 1;
                matrix[x][y] = 1;
            }
        }
    }
    free(matrix); */

    // ZADANIE 3 -------------------------

    int** matrix = malloc(N*sizeof(int*));
    for(int i = 0; i<N; i++){
        matrix[i] = malloc(N*sizeof(int));
    }

    // kwadrat
    for (int y = 1; y < N-1; y++) {
        for (int x = 1; x < N-1; x++) {
            if(x == y){
                matrix[y][x] = 0;
            } else {
                matrix[y][x] = 1;
            }
        }
    }

    // przekatna 1
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            if(x == y){
                matrix[y][x] = 1;
            } else {
                matrix[y][x] = 0;
            }
        }
    }

    // przekatna 2
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            if(x == N-1-y){
                matrix[y][x] = 1;
            } else {
                matrix[y][x] = 0;
            }
        }
    }

    // 2 przekatne
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            matrix[y][x] = ((x == y) || (x == N-1-y)) ? 0 : 1;
        }
    }

    // szachownica
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            matrix[x][y] = ((x%2) == (y%2)) ? 1 : 0;
        }
    }

    for(int i = 0; i<N; i++){
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}