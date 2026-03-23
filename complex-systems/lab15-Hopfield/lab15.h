#ifndef LAB15_H
#define LAB15_H

#include <vector>
#include <cmath>

using vector = std::vector<double>;
using matrix = std::vector<std::vector<double>>;
vector make_flat(const matrix& a);
matrix make_2D(const vector& flat);
matrix make_w(std::vector<matrix> matrices);
matrix think(const matrix& w, const matrix& prompt);

#endif