#include <stdio.h>
#include <stdlib.h>
#include "lab3.h"

double x1(const double x, const double r){
    return r*x*(1.0-x);
}

double* population_in_time(size_t N, double r, double population_0, double (*logistic_equation)(double, double)){
    double* population = (double*)malloc(sizeof(double)*N);
    population[0] = population_0;
    
    for(int n = 1; n < N; n++){
        population[n] = logistic_equation(population[n-1], r);
    }

    return population;
}

double* population_in_time__save_last_Nv_values(size_t N, size_t Nv, double r, double population_0, double (*logistic_equation)(double, double)){
    
    double* population = (double*)malloc(sizeof(double)*Nv);
    double popl = population_0;
    
    for(int n = 0; n < (N-Nv); n++){
        popl = logistic_equation(popl, r);
    }
    population[0] = popl;
    for(int n = 1; n < Nv; n++){
        population[n] = logistic_equation(population[n-1], r);
    }

    return population;
}

void fprintf_state(double* populacja, size_t N, FILE* plik){
    for(int i = 0; i < N; i++){
        fprintf(plik, "%f ", populacja[i]);
    }
    fprintf(plik, "\n");
}

int main(void){

    // zadanie 1.
    FILE* plik1;
    plik1 = fopen("lab3_z1.dat", "w");
    double* population1;
    int N = 50;
    for(int i = 1; i<=9; i++){
        population1 = population_in_time(N, 2.0, 0.1*(double)i, x1);
        fprintf_state(population1, N, plik1);
        free(population1);
    }
    fflush(plik1);
    fclose(plik1);
    
    // zadanie 2.
    FILE* plik2;
    plik2 = fopen("lab3_z2.dat", "w");
    double* population2;
    N = 100;
    double rs[6] = {1.0, 2.0, 3.0, 3.5, 3.55, 3.6};
    for(int i = 0; i<6; i++){
        population2 = population_in_time(N, rs[i], 0.5, x1);
        fprintf_state(population2, N, plik2);
        free(population2);
    }
    fflush(plik2);
    fclose(plik2);

    // zadanie 3.
    FILE* plik3;
    plik3 = fopen("lab3_z3.dat", "w");
    double* population3;
    N = 10000;
    size_t Nv = 1000;
    for(int i = 100; i<=400; i++){
        population3 = population_in_time__save_last_Nv_values(N, Nv, (double)i/100, 0.5, x1);
        fprintf_state(population3, Nv, plik3);
        free(population3);
    }
    fflush(plik3);
    fclose(plik3);

    return 0;
}