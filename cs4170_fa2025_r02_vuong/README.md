## What I learned:

- Decomposition: In the decomposition step, the effective way is to divide by data where each process generates an equal portion of the total points and count the number of points that fall within the unit circle.

- RNG seed: to prevent correlation between processes' randomly generated points, each process has its own RNG seed.

- Collectives: MPI_Reduce is used to aggregate the counts from all processes to the master process for the final estimation of Pi.

- Pitfalls: Not properly seeding the RNG for each process can lead to correlated results, which can skew the estimation of Pi.

## Performance

- Speedup: the algorithm shows good speedup with linear rate as the number of processes increases. The speedup plateau had not yet shown likely because the problem size is not large enough to observe diminishing returns from parallelization.

- Randomness variance: with different BASE_SEED, the estimation of Pi varies; however, when the number of points is large enough, the estimates converge closer to the actual value of Pi.

## Time accounting

- Coding: 3 hours
- Testing: 1 hour
- Analyzing: 2 hours
- Writing: 30 minutes

## Reproducibility notes

- N: 499999999 points
- Seed policy: random_seed = BASE_SEED + myRank
- BASE_SEED = 12345
- Compiler/MPI versions: 
    - slurm 24.05.5
    - g++ (GCC) 11.4.1 20231218 (Red Hat 11.4.1-4)
- Node type: Pitzer