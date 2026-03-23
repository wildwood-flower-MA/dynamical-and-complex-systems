#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <cstdlib>
#include <random>
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
    drdx[1] = -r[0];

    return GSL_SUCCESS;
}

int function2(double t, const double r[], double drdx[], void* parameters){

    struct parameters* p = (struct parameters*)parameters;
    drdx[0] = r[1];
    drdx[1] = -r[0] + (1.0/6.0)*r[0]*r[0]*r[0];

    return GSL_SUCCESS;
}

int function3(double t, const double r[], double drdx[], void* parameters){

    struct parameters* p = (struct parameters*)parameters;
    drdx[0] = r[1];
    drdx[1] = -sin(r[0]);

    return GSL_SUCCESS;
}

int main(){

    std::vector<std::vector<double>> initial_conditions = {
        {0.0, 0.5},
        {0.0, 1.0},
        {0.0, 1.5},
        {0.0, 2.0},
        {0.0, 2.5},
        {0.0, 3.0}
    };

    int i = 0;
    for(auto element : initial_conditions){
        
        std::ofstream plik("lab_" + std::to_string(i) + ".dat");
        i++;

        std::cout << element[0] << " " << element[1] << std::endl;
        parameters par = {
            2, // dimension of a problem
            0.0, 10000, 1e-3, // domain
            element[0], element[1], // boundary conditions
            1e-3, 1e-6 // acceptable errors
        };

        gsl_odeiv2_system sys;
        sys.function = function3;
        sys.jacobian = NULL; //NULL;
        sys.dimension = par.dimension;
        sys.params = &par;

        gsl_odeiv2_driver *driver = gsl_odeiv2_driver_alloc_y_new(
            &sys, gsl_odeiv2_step_rk4, par.dt, par.abs_err, par.rel_err);

        double t = par.t_0;
        double r[par.dimension] = {par.x_0, par.y_0};

        for (int i = 1; i <= par.N_dt; i++){

            double ti = i*par.dt;

            int status = gsl_odeiv2_driver_apply(driver, &t, ti, r);

            plik << ti << " " << r[0] << " " << r[1] << std::endl;

        }

        plik.close();
        gsl_odeiv2_driver_free(driver);
    }
    
    return 0;
}