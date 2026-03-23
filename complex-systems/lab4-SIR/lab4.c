#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "lab4.h"

// do wygodnego alokowania i zwalniania pamięci
int** malloc_tablica(int N){
    // mallokuj tablice kwadratowa N na N

    int** tablica = malloc(N*sizeof(int*));
    for(int i = 0; i < N; i++){
        tablica[i] = calloc(N, sizeof(int));
    }

    return tablica;
}
void free_tablica(int** tablica, size_t N){
    for(int i = 0; i < N; i++){
        free(tablica[i]);
    }
    free(tablica);
}

// losowanie 1 lub 0 z p-stwem jedynki rownym p
int random_p(double p){
    return ((double)rand()/RAND_MAX < p) ? 1 : 0;
}

double ilu_chorych_sasiadow(int** tablica, size_t size, int x, int y){

    double ilu_chorych = 0.0;
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <=1; j++){
            if (i == 0 && j == 0) continue;
            if(tablica[(x + i + size)%size][(y + j + size)%size] == 1){
                ilu_chorych += 1.0;
            }
        }
    }
    return ilu_chorych;
}

void losy_obywateli(int** tablica, int** osiedle_tymczasowe, size_t size,
                    double pstwo_zarazenia, double pstwo_ozdrowienia){

    // kopiowanie do lokalnej kopii
    for(int x = 0; x < size; x++){
        for(int y = 0; y < size; y++){
            osiedle_tymczasowe[x][y] = tablica[x][y];
        }
    }
    double tylu_ma_chorych_sasiadow;

    if(pstwo_zarazenia > 1 || pstwo_zarazenia < 0){ 
        printf("Bardzo dziwna choroba.\n");
    }
    if(pstwo_ozdrowienia > 1 || pstwo_ozdrowienia < 0){ 
        printf("Nie pytają cię o imię, walczą z ostrym cieniem mgły.\n");
    }

    // iteracja po wszystkich obywatelach
    for(int x = 0; x < size; x++){
        for(int y = 0; y < size; y++){

            tylu_ma_chorych_sasiadow = ilu_chorych_sasiadow(osiedle_tymczasowe, size, x, y);
            switch(osiedle_tymczasowe[x][y]){
                case 0:  // gdy zdrowy
                    if(random_p(1.0 - pow(1.0-pstwo_zarazenia, tylu_ma_chorych_sasiadow))){
                        // 1 minus prawdopodobienstwo, ze n razy nie zarazi
                        tablica[x][y] = 1;
                    }
                    break;
                case 1: // gdy chory
                    if(random_p(pstwo_ozdrowienia)){
                        tablica[x][y] = 2;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

int* SIR(int** tablica, size_t size){

    int* sir = calloc(3, sizeof(int));
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            switch(tablica[i][j]){
                case 0:
                    sir[0]++;
                    break;
                case 1:
                    sir[1]++;
                    break;
                case 2:
                    sir[2]++;
                    break;
                default:
                    break;
            }
        }
    }

    return sir;
}

int dzien_w_dniach(int dzien, int* dni, size_t dni_size){
    for(int i = 0; i<dni_size; i++){
        if(dzien == dni[i]){
            return 1;
        }
    }
    return 0;
}

void zapisz_zdjecie(FILE* plik, int** tablica, size_t size){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            fprintf(plik, "%d ", tablica[i][j]);
        }
        fprintf(plik, "\n");
    }
    fflush(plik);
}

void zapisz_sir(FILE* plik, int* sir){
    fprintf(plik, "%d %d %d \n", sir[0], sir[1], sir[2]);
}

void empty_tablica(int** tablica, size_t size){
    for(int i = 0; i<size; i++){
        for(int j = 0; j<size; j++){
            tablica[i][j] = 0;
        }
    }
}


int main(void){

    FILE* osiedla;
    FILE* sir;
    FILE* sir_3;
    osiedla = fopen("osiedla.dat", "w");
    sir = fopen("sir.dat", "w");
    sir_3 = fopen("sir_3.dat", "w");

    srand(time(NULL));
    int N = 60;
    int dni_pandemii = 100;

    double pstwo_zarazenia;
    double pstwo_ozdrowienia;

    int** bufor_tymczasowy = malloc_tablica(N);
    int* aktualny_sir;

    // zadanie 1
    pstwo_ozdrowienia = 0.1;
    int** krowodrza = malloc_tablica(N);

    double pstwa_zarazenia[] = {0.1, 0.2, 0.3};
    int dni[] = {0, 1, 2, 5, 10, 50, 100};

    for(int i = 0; i < 3; i++){

        krowodrza[N/2-1][N/2-1] = 1; // chory w srodku miasta
        for(int dzien = 0; dzien <= dni_pandemii; dzien++){

            // aktualny_sir = SIR(krowodrza, N);
            // zapisz_sir(sir, aktualny_sir);
            // free(aktualny_sir);

            if(dzien_w_dniach(dzien, dni, sizeof(dni)/sizeof(int))){
                zapisz_zdjecie(osiedla, krowodrza, N);
            }

            losy_obywateli(krowodrza, bufor_tymczasowy, N, pstwa_zarazenia[i], pstwo_ozdrowienia);
        }
        empty_tablica(krowodrza, N);
    }

    // zadanie 2
    pstwo_ozdrowienia = 0.1;
    int** kazimierz = malloc_tablica(N);

    double pstwa_zarazenia2[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};

    for(int i = 0; i < 9; i++){

        kazimierz[N/2-1][N/2-1] = 1; // chory w srodku miasta
        for(int dzien = 0; dzien <= dni_pandemii; dzien++){

            aktualny_sir = SIR(kazimierz, N);
            zapisz_sir(sir, aktualny_sir);
            free(aktualny_sir); // zapobiega wyciekowi

            losy_obywateli(kazimierz, bufor_tymczasowy, N, pstwa_zarazenia2[i], pstwo_ozdrowienia);
        }
        empty_tablica(kazimierz, N);
    }

    // zadanie 3
    pstwo_ozdrowienia = 0.2;
    int** czyzyny = malloc_tablica(N);

    double pstwa_zarazenia3[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};

    for(int i = 0; i < 9; i++){

        czyzyny[N/2-1][N/2-1] = 1; // chory w srodku miasta
        for(int dzien = 0; dzien <= dni_pandemii; dzien++){

            aktualny_sir = SIR(czyzyny, N);
            zapisz_sir(sir_3, aktualny_sir);
            free(aktualny_sir); // zapobiega wyciekowi

            losy_obywateli(czyzyny, bufor_tymczasowy, N, pstwa_zarazenia3[i], pstwo_ozdrowienia);
        }
        empty_tablica(czyzyny, N);
    }

    fclose(osiedla); fclose(sir); fclose(sir_3);
    free_tablica(krowodrza, N); free_tablica(kazimierz, N); free_tablica(czyzyny, N);
    free_tablica(bufor_tymczasowy, N);

    return 0;
}