#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "lab6.h"

int** plansza_jedynek(int N){
    int** plansza = (int**)malloc(N*sizeof(int*));
    int* data = (int*)malloc(N*N*sizeof(int)); 
    for(int i = 0; i < N; i++){
        plansza[i] = data + i*N;
    }

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            plansza[i][j] = 1;
        }
    }
    return plansza;
}

int check_neighborhood(int** spins, int size, int i, int j){

    int up = (i == 0) ? size - 1 : i - 1;
    int down = (i == size - 1) ? 0 : i + 1;
    int left = (j == 0) ? size - 1 : j - 1;
    int right = (j == size - 1) ? 0 : j + 1;

    int value = spins[up][j] + spins[down][j] + spins[i][left] + spins[i][right];
    return value;
}

void possible_probabilities(double J, double T, double* probabs){
    double values[] = {-4.0, -2.0, 0.0, 2.0, 4.0};
    double deltaE, exponent;

    for (int i = 0; i < 5; i++) {
        deltaE = 2.0*values[i]*J;
        exponent = (T > 0) ? exp(-deltaE/T) : 0.0;
        probabs[i] = (exponent < 1.0) ? exponent : 1.0;
    }
}

void mcs(double* probabilities, int** plansza, int size){
    int value, i, j;
    double prob;

    for (int step = 0; step < size*size; step++){

        i = rand()%size;
        j = rand()%size;

        value = check_neighborhood(plansza, size, i, j);
        
        // Prawidłowa logika Metropolisa
        if(plansza[i][j] == 1){
            prob = probabilities[2 + value/2];
        } else {
            prob = probabilities[2 - value/2];
        }

        if((double)rand()/RAND_MAX < prob){ 
            plansza[i][j] *= -1; // Odwrócenie spinu
        }
    }
}

double gestosc_namagnesowania(int** plansza, int size){
    int gestosc = 0;
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            gestosc += plansza[i][j];
        }
    }

    return (double)gestosc/(double)size/(double)size;
}

int main(){
    srand(time(NULL));

    double J = 1.0;
    int N = 32;
    int N_MCS = 100000;

    double Ts[] = {1.0, 2.0, 2.5, 4.0};
    double probabilities[5];
    int** space = plansza_jedynek(N);

    FILE* pstwa_zad1 = fopen("pstwa_zad1.dat", "w");
    FILE* g_nam_zad1 = fopen("g_nam_zad1.dat", "w");
    FILE* mapa_zad1 = fopen("mapa_zad1.dat", "w");

    for (int t_idx = 0; t_idx < 4; t_idx++){

        // ZADANIE 1.
        fprintf(pstwa_zad1, "P-stwa dla T = %.1f, to ", Ts[t_idx]);
        possible_probabilities(J, Ts[t_idx], probabilities);

        for (int p_idx = 0; p_idx < 5; p_idx++) {
            fprintf(pstwa_zad1, "%.10f ", probabilities[p_idx]);
        }
        fprintf(pstwa_zad1,"\n");

        // ZADANIE 2.
        for (int x = 0; x < N; x++){
            for (int y = 0; y < N; y++){
                space[x][y] = 1;
            }
        }

        for (int t = 0; t < N_MCS; t++) {
            mcs(probabilities, space, N);
            fprintf(g_nam_zad1,"%f ", gestosc_namagnesowania(space, N));
        }
        fprintf(g_nam_zad1,"\n");

        for(int x = 0; x < N; x++){
            for(int y = 0; y < N; y++){
                fprintf(mapa_zad1,"%d %d %d\n", x, y, space[x][y]);
            }
        }
    }
    fclose(g_nam_zad1);
    fclose(mapa_zad1);
    fclose(pstwa_zad1);

    
    FILE* srednie_w_czasie = fopen("srednie_w_czasie.dat", "w");

    double dens_m, dens2_m, dens_m_in_time, dens2_m_in_time;
    for(double t = 0.25; t <= 4.0; t += 0.25){

        possible_probabilities(J, t, probabilities);
        for (int x = 0; x < N; x++) {
            for (int y = 0; y < N; y++) {
                space[x][y] = 1;
            }
        }

        dens_m_in_time = 0.0;
        dens2_m_in_time = 0.0;
        for (int i = 0; i < N_MCS; i++){

            mcs(probabilities, space, N);

            if(i >= 9*N_MCS/10){
                dens_m = gestosc_namagnesowania(space, N);
                dens_m_in_time += fabs(dens_m); 
                dens2_m_in_time += dens_m*dens_m;
            }
        }

        dens_m_in_time /= N_MCS/10;
        dens2_m_in_time /= N_MCS/10;
        fprintf(srednie_w_czasie, "%.15f %.15f %.15f\n", t, dens_m_in_time, dens2_m_in_time);
        fflush(srednie_w_czasie);
    }

    free(space[0]);
    free(space);
    fclose(srednie_w_czasie);

    return 0;
}