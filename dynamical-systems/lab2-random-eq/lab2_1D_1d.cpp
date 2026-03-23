#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <string>
#include <chrono>
#include "lab1.h"
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

    // constants
    double k;

    // acceptable errors
    double abs_err;
    double rel_err;

};

int function(double t, const double r[], double drdx[], void* parameters){

    struct parameters* p = (struct parameters*)parameters;

    // r = x
    // dr/dt = f(r,t)
    // f = a*r^2 + b*r + c*t + d
    drdx[0] = p->k*r[0]*(1-r[0]);

    return GSL_SUCCESS;
}

int main(){
    int i = 0;
    std::vector<double> x0 = {-0.1, 0.1, 0.9, 1.1};
    for(double element : x0){
        std::cout << element << std::endl;
        std::ofstream plik("lab1_1d_" + std::to_string(i) + ".dat");
        i++;
        parameters par = {
                1, // dimension of a problem
                0.0, 3000, 1e-3, // domain
                element, // x_0
                1.0, // k
                1e-3, 1e-6 // acceptable errors
            };

        gsl_odeiv2_system sys;
        sys.function = function;
        sys.jacobian = NULL;
        sys.dimension = par.dimension;
        sys.params = &par;

        gsl_odeiv2_driver *driver = gsl_odeiv2_driver_alloc_y_new(
            &sys, gsl_odeiv2_step_rk4, par.dt, par.abs_err, par.rel_err);

        double t = par.t_0;
        double r[par.dimension] = {par.x_0};

        for (int i = 1; i <= par.N_dt; i++){

            double ti = i*par.dt;

            int status = gsl_odeiv2_driver_apply(driver, &t, ti, r);

            plik << ti << " " << r[0] << std::endl;

        }

        plik.close();
        gsl_odeiv2_driver_free(driver);


    }
    
    return 0;

}