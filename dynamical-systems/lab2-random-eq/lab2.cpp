#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <string>
#include <chrono>
#include "lab2.h"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>
#include <cmath>

struct parameters{
    size_t dimension;
    double t_0;
    int N_dt;
    double dt;
    double x_0;
    double y_0;
    double y1;
    double x1;  
    double xy1;
    double t1;
    double y2;
    double x2;
    double xy2;
    double t2;
    double abs_err;
    double rel_err;
};

int function(double t, const double r[], double drdx[], void* parameters){
    struct parameters* p = (struct parameters*)parameters;
    drdx[0] = p->y1*r[0] + p->x1*r[1] + p->xy1*r[0]*r[1] + p->t1*t;
    drdx[1] = p->y2*r[0] + p->x2*r[1] + p->xy2*r[0]*r[1] + p->t2*t;
    return GSL_SUCCESS;
}

int jacobian(double t, const double r[], double *dfdr, double dfdt[], void *parameters){
    struct parameters* p = (struct parameters*)parameters;
    const double x = r[0];
    const double y = r[1];

    dfdr[0] = p->y1 + p->xy1*y;
    dfdr[1] = p->x1 + p->xy1*x;
    dfdr[2] = p->y2 + p->xy2*y;
    dfdr[3] = p->x2 + p->xy2*x;

    dfdt[0] = p->t1;
    dfdt[1] = p->t2;

    return GSL_SUCCESS;
}

int main(){
    std::ofstream plik("wyniki/lab1.dat");

    parameters par = {
        2, // dimension of a problem
        0.0, 10000, 1e-3, // domain
        1.0, 0.5, // boundary conditions
        1.0, -1.0, 0.0, 0.01, // constants for dx/dt
        1.0, -0.5, 0.0, 0.0, // constants for dy/dt
        1e-3, 1e-6 // acceptable errors
    };

    gsl_odeiv2_system sys;
    sys.function = function;
    sys.jacobian = jacobian;
    sys.dimension = par.dimension;
    sys.params = &par;

    gsl_odeiv2_driver *driver = gsl_odeiv2_driver_alloc_y_new(
        &sys, gsl_odeiv2_step_rk4, par.dt, par.abs_err, par.rel_err);

    double t = par.t_0;
    
    // POPRAWKA: Zmiana VLA na sztywny rozmiar 2 (kompatybilność C++)
    double r[2] = {par.x_0, par.y_0};

    for (int i = 1; i <= par.N_dt; i++){
        // POPRAWKA: Dokładniejsze ujęcie t_0
        double ti = par.t_0 + i*par.dt; 

        int status = gsl_odeiv2_driver_apply(driver, &t, ti, r);
        
        // Zignorowanie ostrzeżenia kompilatora, jeśli statusu nie obsługujemy
        (void)status; 

        plik << ti << " " << r[0] << " " << r[1] << std::endl;
    }

    plik.close();
    gsl_odeiv2_driver_free(driver);
    return 0;
}