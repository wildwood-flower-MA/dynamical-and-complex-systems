#ifndef LAB7_H
#define LAB7_H

#include <stdio.h>
#include <stddef.h>

struct Wezly_Etykiety{
    int** wezly;
    int** etykiety;
};

void free_matrix(int** etykiety, size_t L);
void uporzadkuj_etykiety(int** etykiety, size_t L);
void save_macierz(FILE* plik, int** macierz, size_t L);
int random_0_1(double p);
struct Wezly_Etykiety W(int L, double probability, int druga_iteracja);
void rozlej_sie(int i, int j, int** etykiety, size_t L);
int czy_perkoluje(int** etykiety, size_t L);

#endif