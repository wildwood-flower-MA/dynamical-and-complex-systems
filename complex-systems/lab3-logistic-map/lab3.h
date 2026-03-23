#ifndef LAB1_H
#define LAB1_H

double x1(const double, const double);
double* population_in_time(size_t, double, double, double (*)(double, double));
double* population_in_time__save_last_Nv_values(size_t, size_t, double, double, double (*)(double, double));
void fprintf_state(double*, size_t, FILE*);

#endif