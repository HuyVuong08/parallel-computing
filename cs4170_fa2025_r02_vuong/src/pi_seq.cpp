#include <random>
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include "CStopWatch.h"

const unsigned int BASE_SEED = 12345;
std::random_device rd;
std::mt19937 rng(BASE_SEED);
std::uniform_real_distribution<double> myDist{-1.0, 1.0};

double calcPi(int nSamples) {
    int numInCircle = 0;
    for (int i = 0; i < nSamples; i++) {
        double x = myDist(rng);
        double y = myDist(rng);
        if (x*x + y*y <= 1.0) numInCircle++;
    }
    return 4.0 * static_cast<double>(numInCircle) / static_cast<double>(nSamples);
}

void main(int argc, char** argv) {
    const int NUM_TRIALS = 10;
    const int nSamples = 499999999;
    const int numProcs = 1;
    std::vector<double> runtimes, pi_values;
    
    int nodes = 1;

    int ppn = getenv("SLURM_NTASKS_PER_NODE") ? std::atoi(getenv("SLURM_NTASKS_PER_NODE")) : 1;

    std::cout << "Running " << NUM_TRIALS << " trials..." << std::endl;
    
    for (int trial = 0; trial < NUM_TRIALS; trial++) {
        CStopWatch sw;
        sw.startTimer();
        double pi = calcPi(nSamples);
        sw.stopTimer();
        double runtime = sw.getElapsedTime();
        
        runtimes.push_back(runtime);
        pi_values.push_back(pi);
        std::cout << "Trial " << trial + 1 << ": π = " << pi 
                  << ", time = " << runtime << "s" << std::endl;
    }
    
    // Calculate average runtime
    double avg_runtime = std::accumulate(runtimes.begin(), runtimes.end(), 0.0) / NUM_TRIALS;
    double avg_pi = std::accumulate(pi_values.begin(), pi_values.end(), 0.0) / NUM_TRIALS;

    std::cout << "\nAverage runtime: " << avg_runtime << "s" << std::endl;
    
    // Save results to CSV
    std::ofstream outfile("../data/runs.csv");
    outfile << "N,p,nodes,ppn,trial,seconds\n";
    for (int i = 0; i < NUM_TRIALS; i++) {
        outfile << nSamples << "," << numProcs << "," << nodes << ","
                << ppn << "," << i + 1 << "," << runtimes[i] << "\n";
    }
    outfile << nSamples << "," << numProcs << "," << nodes << ","
            << ppn << ",Average," << avg_runtime << "\n";
    outfile.close();
}