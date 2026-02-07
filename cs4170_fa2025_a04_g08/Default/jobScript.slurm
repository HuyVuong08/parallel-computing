#!/bin/bash

#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=12
#SBATCH --time=1:00:00
#SBATCH --mail-type=END,FAIL
#SBATCH --account=PCS0294
#SBATCH --job-name=csr-omp

cd "$SLURM_SUBMIT_DIR"

mkdir -p ../data
rm -f ../data/runs.csv
touch ../data/runs.csv

# Build the OpenMP serial binary (only target available in the makefile).
# Intel compiler defaults to no OpenMP flag, so force it here and pin CXX to icpc.
make clean
make all CXX=icpc OPENMP_FLAGS="-qopenmp"

echo "Running OpenMP matrix-multiplication benchmark"
./matrix-multiplication

echo "Results written to ../data/runs.csv by matrix-multiplication.cpp"
