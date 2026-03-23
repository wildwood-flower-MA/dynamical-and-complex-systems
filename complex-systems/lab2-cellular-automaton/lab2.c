#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "LAB2.h"
#define CZY_ZAPISAC(x) ((x)==1 || (x)==2 || (x)==5 || (x)==10 || (x)==50 || (x)==100)

int skok_w_bok[16] = {1,1, 1,-1, 1,0, 0,1, 0,-1, -1,1, -1,0, -1,-1};

int random_zero_jeden(double p_stwo){
    if((double)rand()/(double)(RAND_MAX) <= p_stwo){
        return 1;
    } else {
        return 0;
    }
}

int** losowa_plansza(int N, double p_stwo){

    int** plansza = (int**)malloc(N*sizeof(int*));
    for(int i = 0; i < N; i++){
        *(plansza + i) = (int*)malloc(N*sizeof(int)); 
    }

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            plansza[i][j] = random_zero_jeden(p_stwo);
        }
    }
    return plansza;
}

void fprintf_state(int** plansza, size_t N, FILE* plik){
    for(int j = 0; j < N; j++){
        for(int i = 0; i < N; i++){
            fprintf(plik, "%d ", plansza[i][j]);
        }
        fprintf(plik, "\n");
    }
}

int licznik_sasiadow(int** plansza, int N, int i, int j){
    int liczba_sasiadow = 0;
    for(int ii = 0; ii < 16; ii+=2){
        liczba_sasiadow += plansza[(i+skok_w_bok[ii]+N)%N][(j+skok_w_bok[ii+1]+N)%N];
    }
    return liczba_sasiadow;
}

int** nowa_plansza(int** plansza0, int N){

    int** plansza1 = (int**)malloc(N*sizeof(int*));
    for(int i = 0; i < N; i++){
        *(plansza1 + i) = (int*)malloc(N*sizeof(int)); 
    }
    
    int sasiadow = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            sasiadow = licznik_sasiadow(plansza0, N, i, j);
            switch (plansza0[i][j]){
                case 1:
                    if(sasiadow >= 3 && sasiadow != 4){
                        plansza1[i][j] = 1;
                    } else {
                        plansza1[i][j] = 0;
                    }
                break;
                case 0:
                    if(sasiadow >= 4 && sasiadow != 5){
                        plansza1[i][j] = 1;
                    } else {
                        plansza1[i][j] = 0;
                    }
                break;
            }
        }
    }
    return plansza1;
}

double procent_zapelnienia(int** plansza, int N){
    int suma = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            suma += plansza[i][j];
        }
    }
    return (double)suma/((double)N*(double)N);
}

int main(void){

    srand(time(NULL));
    FILE* plik1;
    plik1 = fopen("lab2_plansze.dat", "w");
    if (plik1 == NULL) {
        perror("Kaszana");
        return 1;
    }
    FILE* plik2;
    plik2 = fopen("lab2_zapelnienie.dat", "w");
    if (plik2 == NULL) {
        perror("Kaszanka");
        return 1;
    }

    int N = 60;
    double p_stwo = 0.5;
    int** plansza = losowa_plansza(N, p_stwo);
    fprintf_state(plansza, N, plik1);
    fprintf(plik2, "%d %f\n", 0, procent_zapelnienia(plansza, N));

    int** tymczasowa_plansza; // pamiec
    for(int i = 1; i <= 100; i++){
        tymczasowa_plansza = nowa_plansza(plansza, N);
        for (int j = 0; j < N; j++){
            free(plansza[j]);
        }
        free(plansza);
        plansza = tymczasowa_plansza;
        if(CZY_ZAPISAC(i)){
            fprintf_state(plansza, N, plik1);
            fprintf(plik2, "%d %f\n", i, procent_zapelnienia(plansza, N));
        }
    }

    fflush(plik1);
    fclose(plik1);
    fflush(plik2);
    fclose(plik2);
    for(int i = 0; i < N; i++){
        free(*(plansza + i)); 
    }
    free(plansza);

    return 0;
}