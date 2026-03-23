#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <string>
#include <chrono>
#include <memory>
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
    double gamma;
    double F;
    double omega; 

    // boundary conditions
    double x_0;
    double v_0;

    // acceptable errors
    double abs_err;
    double rel_err;

};

struct System {

    parameters& pm;
    System(parameters& pm) : pm(pm) {};

    static int function(double t, const double r[], double drdx[], void* params) {

        struct parameters* p = (struct parameters*)params;
        drdx[0] = r[1];
        drdx[1] = -(p->gamma)*r[1] - std::sin(r[0]) + (p->F)*std::cos((p->omega)*t);

        return GSL_SUCCESS;
    }

    std::vector<std::vector<double>> solve() {

        double t = this->pm.t_0;
        std::vector<double> r = {this->pm.x_0, this->pm.v_0};
        std::vector<std::vector<double>> results;

        gsl_odeiv2_system sys;
        sys.function = function;
        sys.jacobian = NULL;
        sys.dimension = this->pm.dimension;
        sys.params = &(this->pm);

        std::unique_ptr<gsl_odeiv2_driver, decltype(&gsl_odeiv2_driver_free)> driver(
            gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rk4,
                this->pm.dt, this->pm.abs_err, this->pm.rel_err),
            gsl_odeiv2_driver_free
        );

        for (int i = 1; i <= this->pm.N_dt; i++) {

            double ti = i*(this->pm.dt);
            int status = gsl_odeiv2_driver_apply(driver.get(), &t, ti, r.data());

            results.push_back({ti, r[0], r[1]});
        }

        return results;
    }
};

int main(){

    parameters pm = {
        2, // dimension of a problem
        0.0, 10000, 1e-3, // domain
        0.2, 1.2, 2.0/3.0, // gamma, F, omega
        0, 0, // boundary conditions
        1e-3, 1e-6 // acceptable errors
    };

    std::ofstream plik("lab.dat");
    System sys(pm);

    std::vector<std::vector<double>> results = sys.solve();

    for (auto result : results) {

        plik << result[0] << " " << result[1] << " " << result[2] << std::endl;

    }

    plik.close();
}