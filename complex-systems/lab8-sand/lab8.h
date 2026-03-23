#ifndef LAB8_H
#define LAB8_H

#include <stdio.h>
#include <stddef.h>

void save_macierz(FILE* plik, int** macierz, size_t L);
void free_matrix(int** etykiety, size_t L);
int** make_pryzma(size_t size);
void evolution_every_grain_of_sand(int** pryzma, size_t size, int* avalanche_size);
int add_grain_of_sand(int** pryzma, size_t size);

#endif