#!/bin/bash

#SBATCH --nodes=1
#SBATCH --ntasks-per-node=12
#SBATCH --time=1:00:00
#SBATCH --mail-type=END,FAIL
#SBATCH --account=PCS0294
#SBATCH --job-name=MPIish

module load mvapich/3.0
module load boost/1.83.0

# module load intel
cd $SLURM_SUBMIT_DIR
# rm -rf slurm-*.out
mkdir -p ../data
rm -f ../data/runs.csv
touch ../data/runs.csv

# make clean && make OSC
make clean && make OSC

echo "Running MPI matrix-vector multiplication benchmarks"

for M in 4000 6000 8000 10000 12000; do
    for P in 1 2 3 4 5 6 7 8 9 10; do
        echo "MPI Run with $P processes"
        mpiexec -n $P ./MPI $M
    done
done

echo "Results written to ../data/runs.csv"