#!/bin/bash

#SBATCH --nodes=10
#SBATCH --ntasks-per-node=1
#SBATCH --time=1:00:00
#SBATCH --mail-type=END,FAIL
#SBATCH --account=PCS0294
#SBATCH --job-name=MPIish

module load mvapich/3.0
module load boost/1.83.0

# module load intel
cd $SLURM_SUBMIT_DIR
# rm -rf slurm-*.out
cd $SLURM_SUBMIT_DIR/Default
mkdir -p ../data
rm -f ../data/runs.csv
touch ../data/runs.csv

# make clean && make OSC
make clean && make pi_seq pi_mpi

echo "Sequential Run with 1 processe"
./pi_seq

for i in {1..10}
do
    echo "MPI Runs with $i processes"
    mpiexec -n $i ./pi_mpi
done

python3 ../src/plots.py