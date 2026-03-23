#ifndef LAB4_H
#define LAB4_H

#include <stdio.h>
#include <stddef.h>

int** malloc_tablica(int);
void free_tablica(int**, size_t);
int random_p(double);
double ilu_chorych_sasiadow(int**, size_t, int, int);
void losy_obywateli(int**, int**, size_t, double, double);
int* SIR(int**, size_t);
int dzien_w_dniach(int, int*, size_t);
void empty_tablica(int**, size_t);
void zapisz_zdjecie(FILE*, int**, size_t);
void zapisz_sir(FILE*, int*);

#endif