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

    // parameter
    double p; 

    // boundary conditions
    double x_0;

    // acceptable errors
    double abs_err;
    double rel_err;

};

int function1(double t, const double r[], double drdx[], void* parameters){

    struct parameters* par = (struct parameters*)parameters;
    drdx[0] = (r[0] - 1)*(r[0]*r[0] + 2*r[0] - par->p);

    return GSL_SUCCESS;
}

int main(){

    std::vector<double> initial_conditions;
    for(int i=0; i<=30; i++){
        initial_conditions.push_back(-4.0 + i*0.2);
        //std::cout<<(-2.0 + i*0.2)<<std::endl;
    }

    std::vector<double> ps = {-1.0, 1.0, 3.0, 5.0};
    int i = 0;
    
    for(int j = 0; j < ps.size(); j++){
        i = 0;
        for(double element : initial_conditions){
            
            std::ofstream plik("lab_" + std::to_string(i) + "_" + std::to_string(j)+ ".dat");
            i++;

            //std::cout << element << std::endl;
            parameters par = {
                1, // dimension of a problem
                0.0, 2000, 1e-3, // domain
                ps[j], // G parameter
                element, // boundary conditions
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
            double r[par.dimension] = {par.x_0};

            for (int j = 1; j <= par.N_dt; j++){

                double ti = j*par.dt;

                int status = gsl_odeiv2_driver_apply(driver, &t, ti, r);

                plik << ti << " " << r[0] << std::endl;

            }

            plik.close();
            gsl_odeiv2_driver_free(driver);
        }

    }
    
    return 0;
}