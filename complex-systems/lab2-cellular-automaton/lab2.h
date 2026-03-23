#ifndef LAB2_H
#define LAB2_H

extern int skok_w_bok[16];

int random_zero_jeden(double);
void fprintf_state(int**, size_t, FILE*);
int licznik_sasiadow(int**, int, int, int);
int** losowa_plansza(int, double);
int** nowa_plansza(int**, int);


#endif