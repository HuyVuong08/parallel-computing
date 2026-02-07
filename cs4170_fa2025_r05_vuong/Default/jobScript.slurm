#!/bin/bash

#SBATCH --nodes=1 --ntasks-per-node=12
#SBATCH --time=1:00:00
#SBATCH --mail-type=END,FAIL
#SBATCH --account=PCS0294
#SBATCH --job-name=OpenMPish

cd $SLURM_SUBMIT_DIR
mkdir -p ../data
rm -f ../data/runs.csv
touch ../data/runs.csv
# Force rebuild in case prior build artifacts are up-to-date
make clean && make all
./OpenMP > ../data/runs.csv
