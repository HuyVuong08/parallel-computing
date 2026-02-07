# Matrix Multiplication Benchmark (Group 08)

## Summary (What we did?)

- Implemented a sparse matrix-matrix multiplication in C++ using the CSR format (`src/matrix-multiplication.cpp`).
- Uses OpenMP to parallelize across rows while keeping a serial (1-thread) baseline for verification.
- Automatically sweeps matrix sizes `N ∈ {100, 200, …, 1000}` and thread counts `1..10`.
- Every configuration runs 10 trials, collects runtimes, and logs both per-trial and average statistics (mean ± std. dev.) to `data/runs.csv`.

### Building and running

```bash
cd Default
make          # builds ./matrix-multiplication with g++-15 + -fopenmp when available
./matrix-multiplication
```

The executable prints a short summary to stdout and emits a full CSV to `../data/runs.csv`. Delete the CSV if you want a fresh log.

## Explanation of design (How we did it?)

- **Data layout:** All matrices are stored as CSR (compressed sparse row) structures (row_ptr, col_idx, vals). `build_csr` cleans duplicate entries and keeps rows sorted.
- **Random generation:** `random_csr` builds reproducible sparse matrices using `std::mt19937_64`. Seeds are a deterministic function of `(size, trial)` so serial and parallel runs see identical inputs.
- **Parallel multiply:** `multiply_csr` executes two OpenMP-parallel passes (row nnz counting + row fill) to avoid race conditions. Each row uses private scratch vectors for markers and accumulation.
- **Validation:** For every `(N, trial)` the code first computes the result with one thread and stores it. When threads > 1, the result is compared against the cached baseline row-by-row to ensure correctness.

## Benchmarking details (How it performs?)

- **Configurations:** Matrix sizes `N = 100, 200, …, 1000`; OpenMP thread counts `1..10`.
- **Trials:** 10 trials per configuration. Each trial multiplies two freshly generated sparse matrices at 20% density.
- **Metrics:** Wall-clock time (seconds) per trial plus aggregated mean and standard deviation. All values are printed with fixed decimal precision.
- **Output:** `data/runs.csv` contains
  - Header: `N,threads,trial,seconds,std_seconds,validate`
  - Trial rows: per-trial timings (std column empty by design)
  - Average rows: mean & std-dev per `(N, threads)` plus validation status

Example snippet:

```
N,threads,trial,seconds,std_seconds,validate
200,4,1,0.012345,,OK
…
200,4,Average,0.011872,0.000154,OK
```

## Verification (Do serial/parallel match?)

- All baselines are computed with `omp_set_num_threads(1)` using the exact same randomly generated matrices and seeds.
- Validation compares CSR rows entry-by-entry with a tolerance of `1e-9`. Any mismatch prints a diagnostic (`stderr`) and marks the CSV row as `FAIL`.
  - If a mismatch occurs, we treat the run as invalid. None were observed in our latest runs.

## Reflection

- **What went well**
  - CSR + OpenMP provided a straightforward way to exploit sparsity while keeping the code deterministic.
  - Deterministic seeds make regression testing easy and keep serial/parallel comparisons fair.
  - Writing measurements directly from C++ avoided external scripting (bash or Python) for experimentation.
- **What was hard / lessons learned**
  - Managing per-thread scratch buffers without reallocations required careful structuring of the inner loops.
  - Keeping validation cheap yet comprehensive meant storing multiple CSR baselines (one per trial per size) which increases memory usage slightly.
- **Advice to future teams**
  - Start with tiny matrix sizes and one thread to make sure CSR assembly and multiply are correct before scaling up.
  - Persist raw timing data (like we do with CSV) so you can re-plot or recompute statistics without rerunning everything.
- **Future ideas**
  1. Introduce multi-node MPI to distribute rows as well as threads for larger matrices.
  2. Experiment with blocked CSR or formats like CSF/ELL for better cache behavior in extremely sparse regimes.
