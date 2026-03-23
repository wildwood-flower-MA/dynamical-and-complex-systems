#ifndef LAB5_H
#define LAB5_H

#include <stdio.h>
#include <stddef.h>

int random_p(double p);
double* create_state_m0(size_t size);
double* create_state_m1(size_t size);
double energy_density(double* spins, size_t size);
double DeltaE(double s_left, double s_mid, double s_right, double J, double B);
void calculate_possible_probs(double* probs, double J, double B, double T);
void print_possible_probs(FILE* plik, double J, double B, double T);
void single_MC_relaxation(double* spins, size_t size, double* precalc_probs);
double* N_MC_relaxations(double* spins, size_t size, double J, double B, double T, long long unsigned int N);
void save_array(FILE* file, double* array, size_t size);
void update_E_E2(double* E, double* E2, long long unsigned int time_steps,
    double* array, size_t size, double J, double B);
void N_relaxations_save_1T_E_E2(FILE* file, double* spins, size_t size, double J, double B, double T_inv,
                                long long unsigned int N, long long unsigned int save_N_last);

#endif