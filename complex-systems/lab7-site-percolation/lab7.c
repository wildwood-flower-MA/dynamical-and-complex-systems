#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <stddef.h>
#include "lab7.h"

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
    if (etykiety == NULL) return;
    for(int i = 0; i < L; i++){
        free(etykiety[i]);
    }
    free(etykiety);
}

int random_0_1(double p){
    return ((double)rand()/RAND_MAX < p) ? 1 : 0;
}

void rozlej_sie(int i, int j, int** etykiety, size_t L){

    int e = etykiety[i][j];
    int di[] = {-1, 1, 0, 0};
    int dj[] = {0, 0, -1, 1};

    for (int d = 0; d < 4; d++) {
        int ii = i + di[d];
        int jj = j + dj[d];

        if (ii >= 0 && ii < L && jj >= 0 && jj < L) {
            int e_prim = etykiety[ii][jj];

            if (e_prim != INT_MAX && e != INT_MAX && e != e_prim) {
                etykiety[ii][jj] = (e < e_prim) ? e : e_prim;
                rozlej_sie(ii, jj, etykiety, L);
            }
        }
    }
}

void uporzadkuj_etykiety(int** etykiety, size_t L){
    int* mapa = malloc(sizeof(int)*L*L);
    int licznik = 1;

    for (int i = 0; i < L*L; i++) {
        mapa[i] = -1;
    }

    for (int i = 0; i < L; i++) {
        for (int j = 0; j < L; j++) {
            int e = etykiety[i][j];
            if (e != INT_MAX) {
                if (mapa[e] == -1) {
                    mapa[e] = licznik++;
                }
                etykiety[i][j] = mapa[e];
            }
        }
    }

    free(mapa);
}

struct Wezly_Etykiety W(int L, double probability, int druga_iteracja){

    struct Wezly_Etykiety to_return;

    // alokacja tablic wartości i etykiet układu
    int** wezly = malloc(L*sizeof(int*));    // wartości
    int** etykiety = malloc(L*sizeof(int*)); // etykiety
    for(int i = 0; i < L; i++){
        wezly[i] = calloc(L, sizeof(int));
        etykiety[i] = malloc(L*sizeof(int));
        for (int j = 0; j < L; j++) {
            etykiety[i][j] = INT_MAX;
        }
    }

    int etykieta = 0;
    int ei, ej, wi, wj;

    for(int i = 0; i < L; i++){
        for(int j = 0; j < L; j++){

            wezly[i][j] = random_0_1(probability);

            if(wezly[i][j]){
                if(i != 0 && j != 0){
                    ei = etykiety[i-1][j];
                    ej = etykiety[i][j-1];
                    wi = wezly[i-1][j];
                    wj = wezly[i][j-1];

                    if(wi == 0 && wj == 0){
                        etykieta++;
                        etykiety[i][j] = etykieta;
                    } else {
                        etykiety[i][j] = (ei <= ej) ?  ei : ej;
                    }

                } else if(i == 0 && j != 0) {
                    ej = etykiety[0][j-1];
                    wj = wezly[0][j-1];

                    if(wj == 0){
                        etykieta++;
                        etykiety[0][j] = etykieta;
                    } else {
                        etykiety[0][j] = ej;
                    }

                } else if(j == 0 && i != 0) {
                    ei = etykiety[i-1][0];
                    wi = wezly[i-1][0];

                    if(wi == 0){
                        etykieta++;
                        etykiety[i][0] = etykieta;
                    } else {
                        etykiety[i][0] = ei;
                    }
                } else {
                    if(wezly[0][0]){
                        etykieta++;
                        etykiety[0][0] = etykieta;
                    }
                }
            }
        }
    }

    if(druga_iteracja){
        for(int i = L-1; i >= 0; i--){
            for(int j = L-1; j >= 0; j--){
                if (wezly[i][j] != 0) {
                    rozlej_sie(i, j, etykiety, L);
                }
            }
        }
    }

    to_return.wezly = wezly;
    to_return.etykiety = etykiety;

    return to_return;
}

int czy_perkoluje(int** etykiety, size_t L){
    // weryfikacja perkolacji pionowej
    for(int i = 0; i < L; i++){
        if (etykiety[0][i] == INT_MAX) continue;
        for(int j = 0; j < L; j++){
            if(etykiety[0][i] == etykiety[L-1][j]){
                return 1;
            }
        }
    }
    return 0;
}

int main(){
    srand(time(NULL));

    // ZADANIE 1.
    FILE* zadanie1;
    struct Wezly_Etykiety zad1;
    zadanie1 = fopen("zadanie1.dat", "w");
    int L = 16; 
    int perkoluje; double pstwa[] = {0.4, 0.6, 0.8};
    for(int i = 0; i < 3; i++){
        zad1 = W(L, pstwa[i], 1);
        perkoluje = czy_perkoluje(zad1.etykiety, L);
        uporzadkuj_etykiety(zad1.etykiety, L);
        fprintf(zadanie1, "----------------------- etykiety\n");
        save_macierz(zadanie1, zad1.etykiety, L);
        fprintf(zadanie1, "----------------------- wezly\n");
        save_macierz(zadanie1, zad1.wezly, L);
        perkoluje ? fprintf(zadanie1, "czy perkoluje dla p-stwa %.1f? TAK \n\n", pstwa[i]) : fprintf(zadanie1, "czy perkoluje dla p-stwa %.1f? NIE \n\n", pstwa[i]);
        free_matrix(zad1.etykiety, L);
        free_matrix(zad1.wezly, L);
    }
    fclose(zadanie1);

    // ZADANIE 2.
    FILE* zadanie2;
    zadanie2 = fopen("zadanie2.dat", "w");
    struct Wezly_Etykiety zad2;
    int perkoluje2; int perkoluje3; double N;
    int liczba_losowan_dla_kazdego_pstwa = 1000;

    int L2 = 16; 
    for(double p = 0.4; p <= 0.801; p+=0.01){
        perkoluje2 = 0;
        for(int i = 0; i < liczba_losowan_dla_kazdego_pstwa; i++){
            zad2 = W(L2, p, 1);
            perkoluje2 += czy_perkoluje(zad2.etykiety, L2);
            free_matrix(zad2.etykiety, L2);
            free_matrix(zad2.wezly, L2);
        }
        N = (double)perkoluje2/(double)liczba_losowan_dla_kazdego_pstwa;
        fprintf(zadanie2, "%f ", N);
    }
    fprintf(zadanie2, "\n");

    struct Wezly_Etykiety zad3;
    int L3 = 32;
    for(double p = 0.4; p <= 0.801; p+=0.01){
        perkoluje3 = 0;
        for(int i = 0; i < liczba_losowan_dla_kazdego_pstwa; i++){
            zad3 = W(L3, p, 1);
            perkoluje3 += czy_perkoluje(zad3.etykiety, L3);
            free_matrix(zad3.etykiety, L3);
            free_matrix(zad3.wezly, L3);
        }
        N = (double)perkoluje3/(double)liczba_losowan_dla_kazdego_pstwa;
        fprintf(zadanie2, "%f ", N);
    }
    
    fclose(zadanie2);

    return 0;
}