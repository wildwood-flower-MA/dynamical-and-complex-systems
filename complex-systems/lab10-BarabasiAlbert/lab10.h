#ifndef LAB10_H
#define LAB10_H

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <chrono>

using lista = std::vector<std::vector<unsigned int>>;
using stopnie = std::vector<unsigned int>;
std::pair<lista, stopnie> graf_lista_sasiedztwa(int N, int m_0, int m);
stopnie graf_stopnie_wierzcholkow(int N, int m_0, int m);
stopnie graf_stopnie_wierzcholkow_lK(int N, int m_0, int m);
std::vector<double> rozklad_stopni_wierzcholkow(stopnie& stopnie_wierzcholkow);

#endif
