#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <numeric>
#include <chrono>
#include "lab10.h"

std::pair<lista, stopnie> graf_lista_sasiedztwa(int N, int m_0, int m){

    static std::random_device rd;
    static std::mt19937 gen(rd());

    int k_sum = N*(N - 1);
    stopnie stopnie_wierzcholkow(N, 0);
    lista graf(N);
    for (int n = 0; n < m_0; n++){
        for (int n_prim = 0; n_prim < m_0; n_prim++){
            if (n_prim != n){
                graf[n].push_back(n_prim);
            }
        }
        stopnie_wierzcholkow[n] = m_0 - 1;
    }
    
    for(int n = m_0; n < N; n++){
        for(int mth_added = 0; mth_added < m; mth_added++){

            std::discrete_distribution<> prob_dist(stopnie_wierzcholkow.begin(), stopnie_wierzcholkow.end());
            int idx = prob_dist(gen);

            graf[n].push_back(idx);
            stopnie_wierzcholkow[idx]++;

            graf[idx].push_back(n);
            stopnie_wierzcholkow[n]++;

        }
    }

    return {graf, stopnie_wierzcholkow};
}

// NIEOPTYMALNIE
stopnie graf_stopnie_wierzcholkow(int N, int m_0, int m){

    static std::random_device rd;
    static std::mt19937 gen(rd());

    stopnie stopnie_wierzcholkow(N, 0);
    for (int n = 0; n < m_0; n++){
        stopnie_wierzcholkow[n] = m_0 - 1;
    }
    
    for(int n = m_0; n < N; n++){
        for(int mth_added = 0; mth_added < m; mth_added++){

            std::discrete_distribution<> prob_dist(stopnie_wierzcholkow.begin(), stopnie_wierzcholkow.end());
            int idx = prob_dist(gen);

            stopnie_wierzcholkow[idx]++;
            stopnie_wierzcholkow[n]++;
        }
    }

    return stopnie_wierzcholkow;
}

// WERSJA 3 - DZIALA

stopnie graf_stopnie_wierzcholkow_lK(int N, int m_0, int m){

    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::vector<unsigned int> wezly_lK;
    wezly_lK.reserve(m_0 * (m_0 - 1) + 2 * m * (N - m_0));

    stopnie stopnie_wierzcholkow(N, 0);
    for (int n = 0; n < m_0; n++){
        stopnie_wierzcholkow[n] = m_0 - 1;

        for(int i = 0; i < m_0 - 1; i++){
            wezly_lK.push_back(n);
        }
    }
    
    for(int n = m_0; n < N; n++){
        std::vector<int> polaczeni_w_tym_kroku;
        polaczeni_w_tym_kroku.reserve(m);

        for(int mth_added = 0; mth_added < m; mth_added++){

            std::uniform_int_distribution<> dist(0, wezly_lK.size() - 1);
            
            int idx = n;
            bool autolink_or_multilink = true;

            while(autolink_or_multilink){
                idx = wezly_lK[dist(gen)];

                if(std::find(polaczeni_w_tym_kroku.begin(), polaczeni_w_tym_kroku.end(), idx) == polaczeni_w_tym_kroku.end()){
                    autolink_or_multilink = false;
                }
            }

            polaczeni_w_tym_kroku.push_back(idx);

            stopnie_wierzcholkow[idx]++;
            stopnie_wierzcholkow[n]++;

            wezly_lK.push_back(idx);
        }

        for(int i = 0; i < m; i++){
            wezly_lK.push_back(n);
        }
    }

    return stopnie_wierzcholkow;
}

std::vector<double> rozklad_stopni_wierzcholkow(stopnie& stopnie_wierzcholkow){

    unsigned int max_stopien = *std::max_element(stopnie_wierzcholkow.begin(), stopnie_wierzcholkow.end());
    std::vector<int> rozklad(max_stopien + 1, 0);

    for (auto s : stopnie_wierzcholkow) {
        rozklad[s]++;
    }
    
    int total = std::accumulate(rozklad.begin(), rozklad.end(), 0);
    std::vector<double> rozkl(rozklad.size());
    for (size_t i = 0; i < rozklad.size(); ++i) {
        rozkl[i] = static_cast<double>(rozklad[i])/total;
    }

    return rozkl;
}

int main(){ // g++ -O3 -march=native -flto lab10.cpp -o lab10.exe

    srand(time(NULL));

    auto start = std::chrono::high_resolution_clock::now();
    
    stopnie stopnie_wierzcholkow = graf_stopnie_wierzcholkow_lK(1000000, 5, 4);
    std::vector<double> rozkl = rozklad_stopni_wierzcholkow(stopnie_wierzcholkow);
    std::ofstream file("rozklad.dat");
    for (size_t i = 0; i < rozkl.size(); i++) {
        file << i << " " << rozkl[i] << "\n";
    }
    file.close();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Runtime: " << elapsed.count() << " seconds" << std::endl;

}