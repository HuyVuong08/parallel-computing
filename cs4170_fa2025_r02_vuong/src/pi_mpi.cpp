#include <random>
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <mpi.h>
#include "CStopWatch.h"
#include <unistd.h>
#include <limits.h>

// Base seed for RNG
const unsigned int BASE_SEED = 12345;
std::random_device rd;
std::uniform_real_distribution<double> myDist{-1.0, 1.0};

int countInCircle(int nSamples, int myRank) {
    std::mt19937 rng(BASE_SEED + myRank);
    int countInCircle = 0;
    for (int i = 0; i < nSamples; i++) {
        double x = myDist(rng);
        double y = myDist(rng);
        if (x*x + y*y <= 1.0) countInCircle++;
    }
    return countInCircle;
}

double parallelPi(int nSamples) {

    int myRank, numProcs, mySize, myInCircle, totalInCircle;
    MPI_Status myStatus;
    const int NUM_TRIALS = 10, TOTAL_SAMPLES = nSamples;
    std::vector<double> runtimes;

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    mySize = TOTAL_SAMPLES / numProcs;

    if (myRank == 0) {
        mySize += TOTAL_SAMPLES % numProcs;
    }

    myInCircle = countInCircle(mySize, myRank);

    MPI_Reduce(&myInCircle, &totalInCircle, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (myRank == 0) {
        double pi = 4.0 * totalInCircle / TOTAL_SAMPLES;
        std::cout << "Estimated π = " << pi << std::endl;
        return pi;
    }
}

int main(int argc, char** argv) {

    const int NUM_TRIALS = 10;
    const int nSamples = 499999999;
    int numProcs, rank;
    std::vector<double> runtimes;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    int nodes = getenv("SLURM_NNODES") ? std::atoi(getenv("SLURM_NNODES")) : 1;

    int ppn = getenv("SLURM_NTASKS_PER_NODE") ? std::atoi(getenv("SLURM_NTASKS_PER_NODE")) : 1;

    for (int trial = 0; trial < NUM_TRIALS; trial++) {
        MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
        CStopWatch sw;
        sw.startTimer();
        double pi = parallelPi(nSamples);
        sw.stopTimer();
        double runtime = sw.getElapsedTime();
        runtimes.push_back(runtime);
    }

    // Write results to CSV file (only root process)
    if (rank == 0) {

        double avg_runtime = std::accumulate(runtimes.begin(), runtimes.end(), 0.0) / NUM_TRIALS;

        system("mkdir -p ../data");
        std::ofstream outfile("../data/runs.csv", std::ios::app);

        // Write data to CSV
        // outfile << "N,p,nodes,ppn,trial,seconds\n";
        for (int i = 0; i < NUM_TRIALS; i++) {
            outfile << nSamples << "," << numProcs << "," << nodes << ","
                    << ppn << "," << i + 1 << "," << runtimes[i] << "\n";
        }
        outfile << nSamples << "," << numProcs << "," << nodes << ","
                << ppn << ",Average," << avg_runtime << "\n";
        outfile.close();
    }

    MPI_Finalize();

    return 0;
}