#pragma once

#include <cmath>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <concepts>

struct parameters{

    int N_points;
    double x_0;
    double y_0;
    double a;
    double b;

};

template<std::floating_point T>
struct System{

    parameters& pm;
    std::vector<std::vector<T>> solution;
    std::vector<std::vector<T>> autocorr;

    void solve(){

        solution[0][0] = static_cast<T>(this->pm.x_0);
        solution[1][0] = static_cast<T>(this->pm.y_0);

        for(int i=1; i < this->pm.N_points; i++){
            solution[0][i] = static_cast<T>(1.0) - static_cast<T>(pm.a)*solution[0][i-1]*solution[0][i-1] + solution[1][i-1];
            solution[1][i] = static_cast<T>(pm.b)*solution[0][i-1];
        }
    };

    System(parameters& params) : pm(params) {   
        solution.assign(2, std::vector<T>(pm.N_points));
        autocorr.assign(2, std::vector<T>(pm.N_points));
        solve();
    }

};

template<std::floating_point T>
void solution2file(System<T>& system, std::string filename){
    
    std::filesystem::create_directory("./wyniki/");
    std::ofstream outfile("./wyniki/" + filename + ".txt");
    outfile << std::setprecision(std::numeric_limits<T>::max_digits10);
    for(int i=0; i < system.pm.N_points; i++){
        outfile << system.solution[0][i] << " " << system.solution[1][i] << "\n";
    }
    outfile.close();
}



