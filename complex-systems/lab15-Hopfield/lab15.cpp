#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>
#include "lab15.h"

vector make_flat(const matrix& a){

    vector flat_prompt;
    for(auto& row : a){
        for(auto element : row){
            flat_prompt.push_back(element);
        }
    }

    return flat_prompt;
};

matrix make_2D(const vector& flat){

    int size_squared = static_cast<int>(std::sqrt(flat.size()));
    matrix two_dim(size_squared, vector(size_squared, 0.0));
    for(int i = 0; i < size_squared; i++){
        for(int j = 0; j < size_squared; j++){
            two_dim[i][j] = flat[i * size_squared + j];
        }
    }
    return two_dim;
}

inline double signum(double element){
    
    if(element < 0) return -1.0;
    else return 1.0;
}

matrix T = {
    {1, 1, 1, 1, 1},
    {-1, -1, 1, -1, -1},
    {-1, -1, 1, -1, -1},
    {-1, -1, 1, -1, -1},
    {-1, -1, 1, -1, -1}
};

matrix H = {
    {1, -1, -1, -1, 1},
    {1, -1, -1, -1, 1},
    {1, 1, 1, 1, 1},
    {1, -1, -1, -1, 1},
    {1, -1, -1, -1, 1}
};

matrix A = {
    {-1, -1, 1, -1, -1},
    {-1, 1, -1, 1, -1},
    {-1, 1, -1, 1, -1},
    {1, 1, 1, 1, 1},
    {1, -1, -1, -1, 1}
};

matrix E = {
    {1, 1, 1, 1, 1},
    {1, -1, -1, -1, -1},
    {1, 1, 1, 1, -1},
    {1, -1, -1, -1, -1},
    {1, 1, 1, 1, 1}
};

matrix Tb = {
    {1, 1, 1, -1, 1},
    {-1, -1, 1, -1, -1},
    {-1, -1, -1, -1, -1},
    {1, -1, -1, -1, -1},
    {-1, -1, 1, -1, -1}
};

matrix Hb = {
    {1, -1, -1, -1, 1},
    {1, -1, -1, -1, 1},
    {1, -1, 1, 1, -1},
    {1, -1, -1, -1, -1},
    {1, -1, -1, -1, 1}
};

matrix Ab = {
    {-1, -1, -1, -1, -1},
    {-1, 1, -1, 1, -1},
    {-1, 1, -1, 1, -1},
    {-1, -1, -1, -1, -1},
    {1, -1, -1, -1, 1}
};

matrix make_w(std::vector<matrix> matrices){

    matrix matrices_1D;
    int size = matrices[0].size();
    matrix w(size*size, vector(size*size, 0.0));

    for(auto& m : matrices){
        matrices_1D.push_back(make_flat(m));
    }

    for(auto& lesson : matrices_1D){
        for(int i = 0; i<size*size; i++){
            for(int j = 0; j<size*size; j++){
                if (i != j){
                    w[i][j] += lesson[i]*lesson[j];
                }
            }
        }
    }

    return w;
}

matrix think(const matrix& w, const matrix& prompt){
    
    vector flat_prompt = make_flat(prompt);
    int size = flat_prompt.size();

    bool whether_matrices_are_equal = false;
    while(whether_matrices_are_equal == false){

        vector flat_prompt_2 = flat_prompt;

        for(int i = 0; i < size; i++){
            flat_prompt[i] = 0.0;
        }

        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                flat_prompt[i] += w[i][j]*flat_prompt_2[j];
            }
        }

        for(int i = 0; i < size; i++){
            flat_prompt[i] = signum(flat_prompt[i]);
        }

        whether_matrices_are_equal = true;
        for(int i = 0; i < size; i++){
            if(flat_prompt_2[i] != flat_prompt[i]){
                whether_matrices_are_equal = false;
                break;
            }
        }
    }

    return make_2D(flat_prompt);
}

int main(){

    matrix w;
    matrix answer;

    // ZADANIE 1.
    w = make_w({H, A, T}); // <- kapelusz

    // ZADANIE 2.
    answer = think(w, Ab);
    for (auto& row : answer){
        for (auto element : row) {
            std::cout << std::setw(3) << (element > 0 ? "X" : "O") << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl; 
    answer = think(w, Hb);
    for (auto& row : answer){
        for (auto element : row) {
            std::cout << std::setw(3) << (element > 0 ? "X" : "O") << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl; 
    answer = think(w, Tb);
    for (auto& row : answer){
        for (auto element : row) {
            std::cout << std::setw(3) << (element > 0 ? "X" : "O") << " ";
        }
        std::cout << std::endl;
    }

    // ZADNIE 3.
    w = make_w({H, A, T, E}); // <- bardzo brzydkie słowo
    std::cout << std::endl; 
    answer = think(w, Ab);
    for (auto& row : answer){
        for (auto element : row) {
            std::cout << std::setw(3) << (element > 0 ? "X" : "O") << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl; 
    answer = think(w, Hb);
    for (auto& row : answer){
        for (auto element : row) {
            std::cout << std::setw(3) << (element > 0 ? "X" : "O") << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl; 
    answer = think(w, Tb);
    for (auto& row : answer){
        for (auto element : row) {
            std::cout << std::setw(3) << (element > 0 ? "X" : "O") << " ";
        }
        std::cout << std::endl;
    }

    // Hopfield wykazał, że maksymalna liczba wzorców (w granicy termodynamicznej)
    // to 0.138*N, gdzie N to liczba zapamiętanych wzorców - w sensie granicy termodynamicznej
    // mamy na myśli, że wzorzec jest przechowywany na nieskończenie długim wektorze 
}

