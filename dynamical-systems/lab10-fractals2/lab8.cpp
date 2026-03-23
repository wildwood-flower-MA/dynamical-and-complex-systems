#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <cmath>
#include <iomanip>
#include <limits>
#include "lab8.hpp"

using type = double;
using type2 = float;

int main(){

    int N= 10000;

    std::vector<double> as = {0.5, 1.1, 1.25, 1.4};
    int i = 0;
    for(auto a : as){
        parameters params(N,
                        0.0, 0.0,
                        static_cast<type>(a), 0.3);
        System<type> sys(params);
        solution2file(sys, "d"+std::to_string(i));
        i++;
    }

    int j = 0;
    for(auto a : as){
        parameters params2(N,
                        0.0, 0.0,
                        static_cast<type2>(a), 0.3);
        System<type2> sys2(params2);
        solution2file(sys2, "f" + std::to_string(j));
        j++;
    }

    N = 10000;
    for(int k = 0; k<301; k++){
        parameters params3(N,
                        0.0, 0.0,
                        1.5*(static_cast<double>(k)/300.0), 0.3);
        System<type> sys3(params3);
        solution2file(sys3, "bif" + std::to_string(k));
    }

    return 0;
}
