// serial.cpp
// Simple serial sparse matrix multiplication using CSR format.
// Builds C = A * B where A and B are in CSR. Result is returned in CSR.
//
// Usage: compile and run. The main() demonstrates multiplication on small random matrices.

#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <cmath>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <omp.h>
using namespace std;

struct CSR {
    int nrows = 0;
    int ncols = 0;
    vector<int> row_ptr;   // size nrows+1
    vector<int> col_idx;   // size nnz
    vector<double> vals;   // size nnz

    CSR() = default;
    CSR(int r, int c): nrows(r), ncols(c), row_ptr(r+1, 0) {}
};

// Build CSR from triplet (i,j,val) arrays. Assumes 0-based indices.
CSR build_csr(int nrows, int ncols, const vector<int>& I, const vector<int>& J, const vector<double>& V) {
    CSR M(nrows, ncols);
    int nnz = (int)I.size();
    M.row_ptr.assign(nrows+1, 0);
    for (int k = 0; k < nnz; ++k) {
        if (I[k] < 0 || I[k] >= nrows) continue;
        ++M.row_ptr[I[k] + 1];
    }
    for (int i = 1; i <= nrows; ++i) M.row_ptr[i] += M.row_ptr[i-1];
    M.col_idx.assign(nnz, 0);
    M.vals.assign(nnz, 0.0);
    vector<int> next = M.row_ptr; // insertion pointers
    for (int k = 0; k < nnz; ++k) {
        int r = I[k];
        if (r < 0 || r >= nrows) continue;
        int dest = next[r]++;
        M.col_idx[dest] = J[k];
        M.vals[dest] = V[k];
    }
    // Optionally compress duplicates within each row (sum duplicates)
    for (int r = 0; r < nrows; ++r) {
        int start = M.row_ptr[r], end = M.row_ptr[r+1];
        if (end - start <= 1) continue;
        // sort by column
        vector<pair<int,double>> tmp;
        tmp.reserve(end-start);
        for (int p = start; p < end; ++p) tmp.emplace_back(M.col_idx[p], M.vals[p]);
        sort(tmp.begin(), tmp.end(), [](auto &a, auto &b){ return a.first < b.first; });
        int w = start;
        for (size_t t = 0; t < tmp.size(); ++t) {
            if (w>start && tmp[t].first == M.col_idx[w-1]) {
                M.vals[w-1] += tmp[t].second;
            } else {
                M.col_idx[w] = tmp[t].first;
                M.vals[w] = tmp[t].second;
                ++w;
            }
        }
        // shift remaining entries left if duplicates removed
        // (w may be <= end)
        // nothing else needed since we leave trailing entries unused; adjust row_ptrs
        int removed = end - w;
        if (removed > 0) {
            // compact arrays by shifting subsequent rows left
            int shift_from = end;
            int shift_to = w;
            int tail = (int)M.col_idx.size();
            for (int i = r+1; i <= nrows; ++i) M.row_ptr[i] -= removed;
            // perform shifting
            for (int p = shift_from; p < tail; ++p, ++shift_to) {
                M.col_idx[shift_to] = M.col_idx[p];
                M.vals[shift_to] = M.vals[p];
            }
            // resize
            M.col_idx.resize((int)M.col_idx.size() - removed);
            M.vals.resize((int)M.vals.size() - removed);
        }
    }
    return M;
}

// Multiply CSR A * CSR B -> CSR C (serial).
// Algorithm: for each row i of A, accumulate contributions using marker and list of touched columns.
// Parallel CSR multiply using a two-pass method (OpenMP).
CSR multiply_csr(const CSR& A, const CSR& B) {
    if (A.ncols != B.nrows) throw runtime_error("Dimension mismatch for multiplication.");
    int m = A.nrows;
    int n = B.ncols;

    // Phase 1: compute nnz per row in parallel
    vector<int> row_nnz(m, 0);

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < m; ++i) {
        // Per-row temporaries (private to each iteration/thread)
        vector<int> marker(n, -1);
        vector<int> cols;
        vector<double> temp(n, 0.0);

        int a_start = A.row_ptr[i], a_end = A.row_ptr[i+1];
        for (int ap = a_start; ap < a_end; ++ap) {
            int k = A.col_idx[ap];
            double a_val = A.vals[ap];
            int b_start = B.row_ptr[k], b_end = B.row_ptr[k+1];
            for (int bp = b_start; bp < b_end; ++bp) {
                int j = B.col_idx[bp];
                double prod = a_val * B.vals[bp];
                if (marker[j] == -1) {
                    marker[j] = 1;     // mark touched
                    cols.push_back(j);
                    temp[j] = prod;
                } else {
                    temp[j] += prod;
                }
            }
        }
        // count nonzeros (skip tiny zeros if you like)
        int cnt = 0;
        for (int col : cols) {
            if (temp[col] != 0.0) ++cnt;
        }
        row_nnz[i] = cnt;
    }

    // Prefix sum to get row_ptr
    vector<int> row_ptr(m+1, 0);
    for (int i = 0; i < m; ++i) row_ptr[i+1] = row_ptr[i] + row_nnz[i];
    int total_nnz = row_ptr[m];

    // Allocate output arrays exactly once (no push_back in parallel fill)
    vector<int> col_idx(total_nnz);
    vector<double> vals(total_nnz);

    // Phase 2: fill in parallel — each row writes to its own range [row_ptr[i], row_ptr[i+1])
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < m; ++i) {
        // Per-row temporaries
        vector<int> marker(n, -1);
        vector<int> cols;
        vector<double> temp(n, 0.0);

        int a_start = A.row_ptr[i], a_end = A.row_ptr[i+1];
        for (int ap = a_start; ap < a_end; ++ap) {
            int k = A.col_idx[ap];
            double a_val = A.vals[ap];
            int b_start = B.row_ptr[k], b_end = B.row_ptr[k+1];
            for (int bp = b_start; bp < b_end; ++bp) {
                int j = B.col_idx[bp];
                double prod = a_val * B.vals[bp];
                if (marker[j] == -1) {
                    marker[j] = (int)cols.size();
                    cols.push_back(j);
                    temp[j] = prod;
                } else {
                    temp[j] += prod;
                }
            }
        }

        // sort column indices for the row (optional)
        sort(cols.begin(), cols.end());

        // write into the precomputed slot
        int write_pos = row_ptr[i];
        for (int col : cols) {
            double v = temp[col];
            if (v != 0.0) {
                col_idx[write_pos] = col;
                vals[write_pos] = v;
                ++write_pos;
            }
        }
        // sanity: write_pos should equal row_ptr[i+1]
        // (If numerical zero pruning changed the count, it's fine as long as counts matched.)
    }

    CSR C;
    C.nrows = m;
    C.ncols = n;
    C.row_ptr = move(row_ptr);
    C.col_idx = move(col_idx);
    C.vals = move(vals);
    return C;
}

// Generate a random sparse matrix in CSR form with the requested density.
CSR random_csr(int nrows, int ncols, double density, uint64_t seed) {
    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<double> val_dist(-5.0, 5.0);
    std::bernoulli_distribution nz_dist(density);

    vector<int> I;
    vector<int> J;
    vector<double> V;
    size_t reserve = static_cast<size_t>(density * nrows * ncols);
    I.reserve(reserve);
    J.reserve(reserve);
    V.reserve(reserve);

    for (int i = 0; i < nrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            if (nz_dist(rng)) {
                I.push_back(i);
                J.push_back(j);
                V.push_back(val_dist(rng));
            }
        }
    }
    return build_csr(nrows, ncols, I, J, V);
}

static string format_seconds(double value) {
    ostringstream oss;
    oss << fixed << setprecision(6) << value;
    return oss.str();
}

bool compare_csr(const CSR& X, const CSR& Y, double tol = 1e-9) {
    if (X.nrows != Y.nrows || X.ncols != Y.ncols) return false;
    if (X.row_ptr != Y.row_ptr) return false;
    if (X.col_idx.size() != Y.col_idx.size()) return false;
    for (size_t idx = 0; idx + 1 < X.row_ptr.size(); ++idx) {
        int lenX = X.row_ptr[idx + 1] - X.row_ptr[idx];
        int lenY = Y.row_ptr[idx + 1] - Y.row_ptr[idx];
        if (lenX != lenY) return false;
        for (int p = 0; p < lenX; ++p) {
            int posX = X.row_ptr[idx] + p;
            int posY = Y.row_ptr[idx] + p;
            if (X.col_idx[posX] != Y.col_idx[posY]) return false;
            if (fabs(X.vals[posX] - Y.vals[posY]) > tol) return false;
        }
    }
    return true;
}

// Utility: print CSR as dense (for small tests)
void print_dense(const CSR& M) {
    for (int i = 0; i < M.nrows; ++i) {
        int p = M.row_ptr[i], end = M.row_ptr[i+1];
        int idx = p;
        for (int j = 0; j < M.ncols; ++j) {
            double v = 0.0;
            if (idx < end && M.col_idx[idx] == j) {
                v = M.vals[idx];
                ++idx;
            }
            cout << v << (j+1==M.ncols ? "" : " ");
        }
        cout << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    double density = 0.2; // approximate fraction of nonzeros
    const vector<int> problem_sizes = {500, 1000, 1500, 2000, 2500, 3000};
    const int trials = 10;

    unordered_map<int, vector<CSR>> serial_baselines;
    const string output_path = "../data/runs.csv";
    ofstream csv(output_path);
    if (!csv) {
        cerr << "Failed to open " << output_path << " for writing.\n";
        return 1;
    }
    csv << "N,threads,trial,seconds,std_seconds,validate\n";

    auto seed_for = [](int size, int trial, uint64_t offset) -> uint64_t {
        return static_cast<uint64_t>(size) * 100000ull +
               static_cast<uint64_t>(trial) * 7919ull + offset;
    };

    for (int size : problem_sizes) {
        for (int threads = 1; threads <= 10; ++threads) {
            omp_set_num_threads(threads);
            double total_seconds = 0.0;
            bool all_valid = true;
            vector<double> trial_times(trials, 0.0);
            vector<bool> trial_valid(trials, true);
            for (int trial = 0; trial < trials; ++trial) {
                CSR A = random_csr(size, size, density, seed_for(size, trial, 1));
                CSR B = random_csr(size, size, density, seed_for(size, trial, 50001));

                auto start = chrono::high_resolution_clock::now();
                CSR C = multiply_csr(A, B);
                auto end = chrono::high_resolution_clock::now();
                double elapsed_seconds = chrono::duration<double>(end - start).count();
                total_seconds += elapsed_seconds;
                trial_times[trial] = elapsed_seconds;

                if (threads == 1) {
                    auto &vec = serial_baselines[size];
                    if ((int)vec.size() < trials) vec.resize(trials);
                    vec[trial] = std::move(C);
                } else {
                    auto it = serial_baselines.find(size);
                    bool this_valid = false;
                    if (it != serial_baselines.end() &&
                        trial < (int)it->second.size()) {
                        this_valid = compare_csr(it->second[trial], C);
                    }
                    trial_valid[trial] = this_valid;
                    all_valid &= this_valid;
                    if (!this_valid) {
                        cerr << "Mismatch detected for threads=" << threads
                             << " size=" << size << " trial=" << trial << "\n";
                    }
                }
            }

            double avg_seconds = total_seconds / static_cast<double>(trials);
            double variance = 0.0;
            for (double t : trial_times) {
                double diff = t - avg_seconds;
                variance += diff * diff;
            }
            variance /= static_cast<double>(trials);
            double std_seconds = sqrt(max(variance, 0.0));
            cout << "N=" << size << " Threads=" << threads
                 << " mean_s=" << format_seconds(avg_seconds)
                 << " std_s=" << format_seconds(std_seconds)
                 << " validate=" << ((threads == 1 || all_valid) ? "OK" : "FAIL")
                 << "\n";

            for (int trial = 0; trial < trials; ++trial) {
                csv << size << ',' << threads << ',' << (trial + 1) << ','
                    << format_seconds(trial_times[trial]) << ",,"
                    << ((threads == 1 || trial_valid[trial]) ? "OK" : "FAIL")
                    << "\n";
            }
            csv << size << ',' << threads << ",Average," << format_seconds(avg_seconds) << ','
                << format_seconds(std_seconds) << ','
                << ((threads == 1 || all_valid) ? "OK" : "FAIL") << "\n";
        }
    }

    return 0;
}
