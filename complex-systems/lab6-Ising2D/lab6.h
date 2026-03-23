#ifndef LAB6_H
#define LAB6_H

int** plansza_jedynek(int N);
int check_neighborhood(int** spins, int size, int i, int j);
void possible_probabilities(double J, double T, double* probabs);
double gestosc_namagnesowania(int** plansza, int size);
void mcs(double* probabilities, int** plansza, int size);

#endif