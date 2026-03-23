#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <cstdlib>
#include <random>
#include <ranges>
#include <algorithm>
#include <string>
#include <chrono>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>
#include <cmath>

struct parameters{

    // dimension of a problem
    size_t dimension;

    // domain
    double t_0;
    int N_dt;
    double dt;

    // parameter
    double m;
    double epsilon;

    // boundary conditions
    double x_0;
    double y_0;

    // acceptable errors
    double abs_err;
    double rel_err;

};

int function1(double t, const double r[], double drdx[], void* parameters){

    struct parameters* p = (struct parameters*)parameters;
    drdx[0] = r[1];
    drdx[1] = p->m - r[0] - (p->epsilon)*(r[0]*r[0] - 1)*r[1];

    return GSL_SUCCESS;
}

int main(){

    std::vector<std::vector<double>> initial_conditions = {
            {1.5, 4.2},
            {-1.0, -3.5},
            {-3.5, 2.8},
            {5.0, -0.1},
            {0.2, 0.1},
            {-1.5, -0.5},
            {6.5, -2.0},
            {-1.0, 3.0}
    };
    std::vector<double> epsilons = {
        0.0,
        0.5,
        3.0
    };

    double sqrt3_3 = std::sqrt(3)/3;
    double sqrt15_3 = std::sqrt(15)/3;

    std::vector<std::vector<double>> ms = {
        {1.0},
        {0.5, 1.0, (1.0 + std::sqrt(5))/2.0, std::sqrt(5), (std::sqrt(5) + 3.0)/2.0},
        {sqrt3_3/2.0, sqrt3_3, (sqrt3_3 + 1.0)/2.0, 1.0, (1.0 + sqrt15_3)/2.0, sqrt15_3, (sqrt15_3 + 3.0)/2.0}
    };

    for(int i = 0; i<3; i++){ // iteracja po epsilon
        for(auto [j, ms_i] : std::views::enumerate(ms[i])){ // iteracja po m
            for(auto [k, element] : std::views::enumerate(initial_conditions)){ // iteracja po warunkach poczatkowych

                std::ofstream plik("lab_" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k) + ".dat"); 

                parameters par = {
                    2, // dimension of a problem
                    0.0, 50000, 1e-3, // domain
                    ms_i, epsilons[i], // m, epsilon (0, 1/2, 3)
                    element[0], element[1], // boundary conditions
                    1e-3, 1e-6 // acceptable errors
                };

                gsl_odeiv2_system sys;
                sys.function = function1;
                sys.jacobian = NULL; //NULL;
                sys.dimension = par.dimension;
                sys.params = &par;

                gsl_odeiv2_driver *driver = gsl_odeiv2_driver_alloc_y_new(
                    &sys, gsl_odeiv2_step_rk4, par.dt, par.abs_err, par.rel_err);

                double t = par.t_0;
                double r[par.dimension] = {par.x_0, par.y_0};

                for (int j = 1; j <= par.N_dt; j++){

                    double ti = j*par.dt;

                    int status = gsl_odeiv2_driver_apply(driver, &t, ti, r);

                    plik << ti << " " << r[0] << " " << r[1] << std::endl;

                }

                plik.close();
                gsl_odeiv2_driver_free(driver);
            }
        }
    }

    std::vector<double> wartosci_m = {
        0.9, 0.92, 0.94, 0.96, 0.98, 1.0
    };
    for(auto [j, ms_i] : std::views::enumerate(wartosci_m)){ // iteracja po m

        std::ofstream plik2("amplituda_"+ std::to_string(j) + ".dat");
            
        parameters par2 = {
            2, // dimension of a problem
            0.0, 50000, 1e-3, // domain
            ms_i, 0.5, // m, epsilon (0, 1/2, 3)
            1, 1, // boundary conditions
            1e-3, 1e-6 // acceptable errors
        };

        gsl_odeiv2_system sys;
        sys.function = function1;
        sys.jacobian = NULL; //NULL;
        sys.dimension = par2.dimension;
        sys.params = &par2;

        gsl_odeiv2_driver *driver = gsl_odeiv2_driver_alloc_y_new(
            &sys, gsl_odeiv2_step_rk4, par2.dt, par2.abs_err, par2.rel_err);

        double t = par2.t_0;
        double r[par2.dimension] = {par2.x_0, par2.y_0};

        for (int j = 1; j <= par2.N_dt; j++){

            double ti = j*par2.dt;

            int status = gsl_odeiv2_driver_apply(driver, &t, ti, r);

            plik2 << ti << " " << r[0] << " " << r[1] << std::endl;

        }

        plik2.close();
        gsl_odeiv2_driver_free(driver);

    }


    }