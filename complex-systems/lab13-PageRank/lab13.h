#ifndef LAB13_H
#define LAB13_H

#include <vector>

using matrix = std::vector<std::vector<double>>;
using vector = std::vector<double>;
using vector_int = std::vector<int>;

vector zad1(matrix& graph_mat, double epsilon);
vector zad2(matrix& graph_mat, double epsilon);
vector PR_zad3(matrix A, double epsilon);
vector PR_zad4(matrix A, double epsilon);

matrix probabs_mat(matrix graph); 

#endif