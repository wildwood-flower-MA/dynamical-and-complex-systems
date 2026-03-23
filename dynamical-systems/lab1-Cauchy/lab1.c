#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>
#include "lab1.h"

struct parameters{

    // dimension of a problem
    size_t dimension;

    // domain
    double x_0;
    int N_dx;
    double dx;

    // boundary conditions
    double y_0;

    // constants (dy/dx = a2*y^2 + a1*y + b2*x^2 + b1*x + ab*xy + c)
    double a1;
    double a2; 
    double b1;
    double b2;
    double ab;
    double c;

    // acceptable errors
    double abs_err;
    double rel_err;

};

int function(double x, const double y[], double dydx[], void* parameters){

    struct parameters* par = (struct parameters*)parameters;
    dydx[0] = par->a2*y[0]*y[0] + par->a1*y[0] + par->b2*x*x + par->b1*x + par->ab*x*y[0] + par->c;

    return GSL_SUCCESS;
}

double solution_analytical(double x, double y0){
    return 1.0 / ((1.0/y0) - x);
}

int main(void){

    struct parameters par = {
                            1, // dimension of a problem
                            0.0, 1000, 1e-3, // domain
                            0.5, // boundary conditions
                            0.0, 1.0, 0.0, 0.0, 0.0, 0.0, // constants
                            1e-3, 1e-6 // acceptable errors
                            };
    
    gsl_odeiv2_system sys = {function, NULL, par.dimension, &par};

    gsl_odeiv2_driver *driver = gsl_odeiv2_driver_alloc_y_new(
        &sys, gsl_odeiv2_step_rk4, par.dx, par.abs_err, par.rel_err);

    //gsl_odeiv2_driver *driver = gsl_odeiv2_driver_alloc_y_new(
    //    &sys, gsl_odeiv2_step_rk8pd, par.dx, par.abs_err, par.rel_err);
    //gsl_odeiv2_driver_reset(driver);
    
    double x = par.x_0;
    
    double y[par.dimension];
    y[0] = par.y_0;
    // --------------------------------

    char filename[64];
    snprintf(filename, sizeof(filename), "wyn/wyniki.txt");
    FILE *wyniki = fopen(filename, "w");
    if (!wyniki) {
        perror("kaszana");
        exit(1);
    }
    fprintf(wyniki, "%.6g %.17g %.17g\n", x, solution_analytical(x, par.y_0), y[0]);

    int status;
    for(int i = 0; i < par.N_dx; i++){
        status = gsl_odeiv2_driver_apply(driver, &x, x + par.dx, y);
        //status = gsl_odeiv2_driver_apply_fixed_step(driver, &x, par.dx, 1, y);
        fprintf(wyniki, "%.17g %.17g %.17g\n", x, solution_analytical(x, par.y_0), y[0]);
    }

    fclose(wyniki);
    gsl_odeiv2_driver_free(driver);

    return status;
}