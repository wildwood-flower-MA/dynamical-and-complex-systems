#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <chrono>
#include <numeric>

using vector = std::vector<double>;
using matrix = std::vector<vector>;

// --- FUNKCJE POMOCNICZE ---

matrix probabs_mat(matrix graph){

    int N = graph.size();
    matrix probabs(N, vector(N, 0.0));
    for(int i = 0; i < N; i++){

        double suma = 0.0;

        for(int j = 0; j < N; j++) suma += graph[i][j];
        
        if(suma > 0) {
            for(int j = 0; j < N; j++) probabs[i][j] = graph[i][j]/suma;
        } else {
            for(int j = 0; j < N; j++) probabs[i][j] = 1.0/N;
        }
    }
    return probabs;
}

// --- ZADANIE 1 ---

vector zad1(matrix& graph_mat, double epsilon){

    static std::mt19937 rng(std::random_device{}());
    matrix graph = probabs_mat(graph_mat);

    int N_nodes = graph.size();
    vector countings(N_nodes, 0.0);
    vector probabs(N_nodes, 1.0/N_nodes);
    vector probabs_old(N_nodes, 0.0);

    int current_node = rng() % N_nodes;
    double probabs_diff = epsilon + 1.0;
    int iteration = 0;

    std::vector<std::discrete_distribution<int>> dists(N_nodes);
    for(int i = 0; i < N_nodes; i++){
        dists[i] = std::discrete_distribution<int>(graph[i].begin(), graph[i].end());
    }

    while(probabs_diff > epsilon || iteration < 10000){
        iteration++;
        probabs_old = probabs;

        current_node = dists[current_node](rng);
        countings[current_node]++;

        probabs_diff = 0.0;
        for(int node = 0; node < N_nodes; node++) {
            probabs[node] = countings[node]/iteration;
            probabs_diff += std::abs(probabs[node] - probabs_old[node]);
        }
    }
    return probabs;
}

// --- ZADANIE 2 ---

vector zad2(matrix& graph_mat, double epsilon){

    static std::mt19937 rng(std::random_device{}());
    matrix graph = probabs_mat(graph_mat);

    int N_nodes = graph.size();
    vector countings(N_nodes, 0.0);
    vector probabs(N_nodes, 1.0/N_nodes);
    vector probabs_old(N_nodes, 0.0);

    int current_node = rng() % N_nodes;
    double probabs_diff = epsilon + 1.0;
    int iteration = 0;

    std::vector<std::discrete_distribution<int>> dists(N_nodes);
    for(int i = 0; i < N_nodes; i++){
        dists[i] = std::discrete_distribution<int>(graph[i].begin(), graph[i].end());
    }
    
    std::uniform_real_distribution<double> unif(0.0, 1.0);
    std::uniform_int_distribution<int> teleport(0, N_nodes - 1);

    while(probabs_diff > epsilon || iteration < 10000){
        iteration++;
        probabs_old = probabs;

        double U = unif(rng);
        if(U < 0.85){
            current_node = dists[current_node](rng);
        } else {
            current_node = teleport(rng);
        }

        countings[current_node]++;

        probabs_diff = 0.0;
        for(int node = 0; node < N_nodes; node++) {
            probabs[node] = countings[node]/iteration;
            probabs_diff += std::abs(probabs[node] - probabs_old[node]);
        }
    }
    return probabs;
}

// --- ZADANIE 3 ---

vector PR_zad3(matrix A, double epsilon){

    int N_nodes = A.size();
    vector v(N_nodes, 1.0/N_nodes);
    double diff = 1.0;
    
    while(diff > epsilon){
        vector v_before = v;
        for(int i = 0; i < N_nodes; i++){
            v[i] = 0.0;
            for(int j = 0; j < N_nodes; j++){
                v[i] += A[j][i]*v_before[j];
            }
        }
        diff = 0;
        for(int i = 0; i < N_nodes; i++){
            diff += std::abs(v[i] - v_before[i]);
        }
    }
    return v;
}

// --- ZADANIE 4 ---

vector PR_zad4(matrix A, double epsilon){

    int N_nodes = A.size();
    vector v(N_nodes, 1.0/N_nodes);
    double diff = 1.0;
    double p = 0.15;
    matrix M(N_nodes, vector(N_nodes, 0.0));
    for(int i = 0; i < N_nodes; i++){
        for(int j = 0; j < N_nodes; j++){
            M[i][j] = (1 - p)*A[i][j] + p*(1.0/N_nodes);
        }
    }

    while(diff > epsilon){
        vector v_before = v;
        for(int i = 0; i < N_nodes; i++){
            v[i] = 0.0;
            for(int j = 0; j < N_nodes; j++){
                v[i] += M[j][i]*v_before[j];
            }
        }
        diff = 0;
        for(int i = 0; i < N_nodes; i++){
            diff += std::abs(v[i] - v_before[i]);
        }
    }
    return v;
}

matrix graph_1 = {
                {0.0, 1.0, 1.0, 1.0},
                {1.0, 0.0, 0.0, 0.0},
                {1.0, 1.0, 0.0, 0.0},
                {0.0, 1.0, 1.0, 0.0}
                };

matrix graph_2 = {
                {0.0, 1.0, 0.0, 0.0},
                {1.0, 0.0, 0.0, 0.0},
                {0.0, 1, 0.0, 1},
                {1, 0.0, 1, 0.0}
                };

matrix graph_3 = {
                {0.0, 1.0, 0.0, 0.0},
                {0.0, 1.0, 0.0, 0.0},
                {0.0, 1, 0.0, 1},
                {1, 0.0, 1, 0.0
                };

matrix A_1 = {
            {0.0, 1.0/3.0, 1.0/3.0, 1.0/3.0},
            {1.0, 0.0, 0.0, 0.0},
            {0.5, 0.5, 0.0, 0.0}, 
            {0.0, 0.5, 0.5, 0.0}
            };

matrix A_2 = {
            {0.0, 1.0, 0.0, 0.0},
            {1.0, 0.0, 0.0, 0.0},
            {0.0, 0.5, 0.0, 0.5},
            {0.5, 0.0, 0.5, 0.0}
            };

matrix A_3 = {
            {0.0, 1.0, 0.0, 0.0},
            {0.0, 1.0, 0.0, 0.0},
            {0.0, 0.5, 0.0, 0.5},
            {0.5, 0.0, 0.5, 0.0}
            };

int main(){

    double epsilon = 1e-6; 
    matrix graph = graph_2; 
    matrix A = A_2; 

    // ZADNIE 1
    auto start = std::chrono::high_resolution_clock::now();
    vector zad1_v = zad1(graph, epsilon);
    for(auto& el : zad1_v){ std::cout << el << " "; }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << " (runtime: " << elapsed.count() << " s)" << std::endl;

    // ZADNIE 2
    auto start2 = std::chrono::high_resolution_clock::now();
    vector zad2_v = zad2(graph, epsilon);
    for(auto& el : zad2_v){ std::cout << el << " "; }
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed2 = end2 - start2;
    std::cout << " (runtime: " << elapsed2.count() << " s)" << std::endl;

    // ZADNIE 3
    auto start3 = std::chrono::high_resolution_clock::now();
    vector zad3_v = PR_zad3(A, epsilon);
    for(auto& el : zad3_v){ std::cout << el << " "; }
    auto end3 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed3 = end3 - start3;
    std::cout << " (runtime: " << elapsed3.count() << " s)" << std::endl;

    // ZADNIE 4
    auto start4 = std::chrono::high_resolution_clock::now();
    vector zad4_v = PR_zad4(A, epsilon);
    for(auto& el : zad4_v){ std::cout << el << " "; }
    auto end4 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed4 = end4 - start4;
    std::cout << " (runtime: " << elapsed4.count() << " s)" << std::endl;
}