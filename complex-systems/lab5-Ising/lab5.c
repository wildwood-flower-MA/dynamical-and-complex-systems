#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "lab5.h"

int random_p(double p){
    return ((double)rand()/RAND_MAX < p) ? 1 : 0;
}

double* create_state_m0(size_t size){
    double* spins = malloc(size*sizeof(double));
    for(int i = 0; i < size; i++){
        spins[i] = random_p(0.5) == 1 ? 1.0 : -1.0;
    }
    return spins;
}

double* create_state_m1(size_t size){
    double* spins = malloc(sizeof(double)*size);
    for(int i = 0; i < size; i++){
        spins[i] = 1.0;
    }
    return spins;
}

double energy_density(double* spins, size_t size){
    double e = 0.0;
    for(int i = 0; i < size; i++){
        e -= spins[i]*spins[(i+1)%size];
    }
    return e/(double)size;
}

double DeltaE(double s_left, double s_mid, double s_right, double J, double B){
    return 2.0*(J*(s_mid*(s_right + s_left)) + B*s_mid);
}

void calculate_possible_probs(double* probs, double J, double B, double T){
    double mp[] = {-1.0, 1.0};
    int idx_local;
    double s_left, s_mid, s_right, deltaE;

    for(int idx = 0; idx < 8; idx++){
        s_left = mp[idx%2];
        idx_local = idx/2;
        s_mid = mp[idx_local%2];
        idx_local /= 2; // POPRAWA BŁĘDU (było /= idx_local)
        s_right = mp[idx_local%2];
        
        deltaE = DeltaE(s_left, s_mid, s_right, J, B);
        probs[idx] = (deltaE < 0.0) ? 1.0 : ((T>0) ? exp(-deltaE/T) : 0.0);
    }
}

void print_possible_probs(FILE* plik, double J, double B, double T){
    double probs[8];
    calculate_possible_probs(probs, J, B, T);
    for(int idx = 0; idx < 8; idx++){
        fprintf(plik, "%.10f ", probs[idx]);
    }
}

void single_MC_relaxation(double* spins, size_t size, double* precalc_probs){
    int situation_idx;
    int pc_idx;
    double s_left, s_mid, s_right;
    double probability;

    for(int mc_step = 0; mc_step < size; mc_step++){
        pc_idx = rand()%size;

        s_left = spins[(pc_idx-1+size)%size];
        s_mid  = spins[pc_idx];
        s_right = spins[(pc_idx+1)%size];

        // konwersja (-1, 1) na indeksy (0, 1) do szukania w tablicy
        int i0 = (s_left == 1.0) ? 1 : 0;
        int i1 = (s_mid == 1.0) ? 1 : 0;
        int i2 = (s_right == 1.0) ? 1 : 0;

        situation_idx = 4*i2 + 2*i1 + i0;
        probability = precalc_probs[situation_idx];

        if(random_p(probability) == 1){
            spins[pc_idx] = -spins[pc_idx];
        }
    }
}

double* N_MC_relaxations(double* spins, size_t size, double J, double B, double T, long long unsigned int N){
    double* energies = malloc(sizeof(double)*N);
    double precalc_probs[8];
    calculate_possible_probs(precalc_probs, J, B, T);

    for(int turn = 0; turn < N; turn++){
        single_MC_relaxation(spins, size, precalc_probs);
        energies[turn] = energy_density(spins, size);
    }
    return energies;
}

void save_array(FILE* file, double* array, size_t size){
    for(int idx = 0; idx < size; idx++){
        fprintf(file, "%.17f ", array[idx]);
    }
    fprintf(file, "\n");
    fflush(file);
}

void update_E_E2(double* E, double* E2, long long unsigned int time_steps,
                double* array, size_t size, double J, double B){
    double energy = 0.0;
    double energy_local;

    for(int idx = 0; idx < size; idx++){
        energy_local = -J*array[idx]*array[(idx+1)%size] - B*array[idx];
        energy += energy_local;
    }

    *E += energy/(double)size/(double)time_steps;
    *E2 += (energy*energy)/(double)size/(double)time_steps;
}

void N_relaxations_save_1T_E_E2(FILE* file, double* spins, size_t size, double J, double B, double T_inv,
                                long long unsigned int N, long long unsigned int save_N_last){
    double E = 0.0;
    double E2 = 0.0;
    double T = 1.0/T_inv;
    
    double precalc_probs[8];
    calculate_possible_probs(precalc_probs, J, B, T);

    for(int turn = 0; turn < N-save_N_last; turn++){
        single_MC_relaxation(spins, size, precalc_probs);
    }
    for(int turn = 0; turn < save_N_last; turn++){
        single_MC_relaxation(spins, size, precalc_probs);
        update_E_E2(&E, &E2, save_N_last, spins, size, J, B);
    }

    fprintf(file, "%.17f %.17f %.17f\n", T_inv, E, E2);
    fflush(file);
}

int main(){
    srand(time(NULL));
    double J = 1.0, B = 0.0;

    // ZADANIE 1.
    FILE* pstwa = fopen("pstwa.dat", "w");
    fprintf(pstwa, "Możliwe prawdopodobieństwa zmiany spinu przy T = 5:\n"); print_possible_probs(pstwa, J, B, 5.0); fprintf(pstwa, "\n\n");
    fprintf(pstwa, "Możliwe prawdopodobieństwa zmiany spinu przy T = 1:\n"); print_possible_probs(pstwa, J, B, 1.0); fprintf(pstwa, "\n\n");
    fprintf(pstwa, "Możliwe prawdopodobieństwa zmiany spinu przy T = 2/3:\n"); print_possible_probs(pstwa, J, B, 2.0/3.0); fprintf(pstwa, "\n\n");
    fprintf(pstwa, "Możliwe prawdopodobieństwa zmiany spinu przy T = 0.25:\n"); print_possible_probs(pstwa, J, B, 0.25); fprintf(pstwa, "\n\n");
    fclose(pstwa);

    // ZADANIE 2.
    FILE* energie = fopen("energie.dat", "w");
    int size = pow(10,3);
    long long unsigned int MC_steps = pow(10,4);

    double temperatures[] = {5.0, 1.0, 2.0/3.0, 0.25};
    for(int i = 0; i < 4; i++){
        double* spins_m0 = create_state_m0(size);
        double* energie_m0 = N_MC_relaxations(spins_m0, size, J, B, temperatures[i], MC_steps);
        save_array(energie, energie_m0, MC_steps);
        free(spins_m0); free(energie_m0);
    }
    for(int i = 0; i < 4; i++){
        double* spins_m1 = create_state_m1(size);
        double* energie_m1 = N_MC_relaxations(spins_m1, size, J, B, temperatures[i], MC_steps);
        save_array(energie, energie_m1, MC_steps);
        free(spins_m1); free(energie_m1);
    }
    fclose(energie);

    // ZADANIE 3.
    FILE* T_inv__E__E2 = fopen("vals.dat", "w");
    for(double T_inv = 0.5; T_inv <= 4.0; T_inv += 0.25){
        double* spins = create_state_m0(size);
        N_relaxations_save_1T_E_E2(T_inv__E__E2, spins, size, J, B, T_inv, MC_steps, pow(10,3));
        free(spins);
    }
    fclose(T_inv__E__E2);

    return 0;
}