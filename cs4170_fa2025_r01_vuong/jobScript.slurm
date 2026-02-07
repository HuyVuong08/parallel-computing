#!/bin/bash

#SBATCH --nodes=1
#SBATCH --ntasks-per-node=12
#SBATCH --time=1:00:00
#SBATCH --mail-type=END,FAIL
#SBATCH --account=PCS0294
#SBATCH --job-name=MPIish

module load openmpi 
module load boost/1.83.0

#module load intel
cd $SLURM_SUBMIT_DIR/Default
rm -f results.csv
make clean && make OSC

echo "=== 1 process ===" >> results.csv
mpiexec -n 1 MPI >> results.csv
echo "=== 2 processes ===" >> results.csv
mpiexec -n 2 MPI >> results.csv
echo "=== 3 processes ===" >> results.csv
mpiexec -n 3 MPI >> results.csv
echo "=== 4 processes ===" >> results.csv
mpiexec -n 4 MPI >> results.csv
echo "=== 5 processes ===" >> results.csv
mpiexec -n 5 MPI >> results.csv
echo "=== 6 processes ===" >> results.csv
mpiexec -n 6 MPI >> results.csv
echo "=== 7 processes ===" >> results.csv
mpiexec -n 7 MPI >> results.csv
echo "=== 8 processes ===" >> results.csv
mpiexec -n 8 MPI >> results.csv
echo "=== 9 processes ===" >> results.csv
mpiexec -n 9 MPI >> results.csv
echo "=== 10 processes ===" >> results.csv
mpiexec -n 10 MPI >> results.csv
