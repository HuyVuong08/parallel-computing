#include <boost/mpi.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/serialization/vector.hpp>
#include <mpi.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <numeric>
#include <fstream>
#include <cstdlib>

namespace mpi = boost::mpi;

template<typename T>
std::vector<T> flatten(const std::vector<std::vector<T>>& matrix2D) {
    std::vector<T> flattened;
    for (const auto& row : matrix2D) {
        for (const auto& val : row) {
            flattened.push_back(val);
        }
    }
    return flattened;
}

template<typename T>
void printMatrix(const std::string& name, const std::vector<T>& matrix, std::size_t rows, std::size_t cols) {
    std::cout << name << " = [\n";
    for (std::size_t i = 0; i < rows; ++i) {
        std::cout << "    [";
        for (std::size_t j = 0; j < cols; ++j) {
            std::cout << matrix[i * cols + j];
            if (j < cols - 1) std::cout << ", ";
        }
        std::cout << "]";
        if (i < rows - 1) std::cout << ",";
        std::cout << "\n";
    }
    std::cout << "]\n\n";
}

template<typename T>
static void matvec_local(const T* A_local, const T* x,
                         T* y_local, std::size_t m_local, std::size_t n) {
    for (std::size_t i = 0; i < m_local; ++i) {
        const std::size_t row_off = i * n;
        T sum = 0;
        for (std::size_t j = 0; j < n; ++j) {
            sum += A_local[row_off + j] * x[j];
        }
        y_local[i] = sum;
    }
}

template<typename T>
void matvec(const std::vector<T>& A, std::size_t m, std::size_t n,
            const std::vector<T>& x, std::vector<T>& y) {
    if (A.size() != m * n || x.size() != n) {
        throw std::runtime_error("Dimension mismatch in matvec");
    }
    y.assign(m, 0);
    matvec(A.data(), x.data(), y.data(), m, n);
}

int main(int argc, char** argv) {
    mpi::environment env(argc, argv);
    mpi::communicator world;

    int world_size = world.size();
    int world_rank = world.rank();

    // Parse command-line arguments
    std::size_t matrix_size = 1000;  // Default size
    const int NUM_TRIALS = 10;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        // Assume it's the matrix size
        try {
            matrix_size = std::stoull(arg);
        } catch (...) {
            if (world_rank == 0) {
                std::cerr << "Invalid argument: " << arg << "\n";
            }
        }
    }

    // Get SLURM environment variables
    int nodes = 1;
    int ppn = 1;
    if (world_rank == 0) {
        if (const char* env_nodes = std::getenv("SLURM_NNODES")) {
            nodes = std::atoi(env_nodes);
        }
        if (const char* env_ppn = std::getenv("SLURM_NTASKS_PER_NODE")) {
            ppn = std::atoi(env_ppn);
        }
    }

    // Define matrices using 2D vectors (only on root initially)
    std::vector<std::vector<int>> A_2D, x_2D;
    std::size_t a_rows = 0, a_cols = 0, x_rows = 0, x_cols = 0;
    
    if (world_rank == 0) {
        // Problem size: matrix_size x matrix_size matrix A and matrix_size x 1 vector x
        const std::size_t size = matrix_size;
        
        // Initialize matrix A as 2D vector
        A_2D.resize(size);
        for (std::size_t i = 0; i < size; ++i) {
            A_2D[i].resize(size);
            for (std::size_t j = 0; j < size; ++j) {
                A_2D[i][j] = static_cast<int>((i + j) % 10);  // Simple pattern
            }
        }
        
        // Initialize matrix x as 2D vector
        x_2D.resize(size);
        for (std::size_t i = 0; i < size; ++i) {
            x_2D[i].resize(1);
            x_2D[i][0] = 1;  // All ones
        }
        
        // Derive dimensions from the 2D vectors
        a_rows = A_2D.size();      // Number of rows in matrix A
        a_cols = A_2D[0].size();   // Number of columns in matrix A
        x_rows = x_2D.size();      // Number of rows in matrix x
        x_cols = x_2D[0].size();   // Number of columns in matrix x
        
        // Check dimension compatibility
        if (a_cols != x_rows) {
            std::cerr << "Error: Matrix dimensions incompatible for multiplication\n";
            std::cerr << "A is " << a_rows << "×" << a_cols << ", x is " << x_rows << "×" << x_cols << "\n";
            return 1;
        }
    }
    
    // Broadcast dimensions to all ranks
    boost::mpi::broadcast(world, a_rows, 0);
    boost::mpi::broadcast(world, a_cols, 0);
    boost::mpi::broadcast(world, x_rows, 0);
    boost::mpi::broadcast(world, x_cols, 0);
    
    std::size_t m = a_rows;  // rows of A
    std::size_t n = a_cols;  // cols of A (and rows of x)
    std::size_t y_rows = a_rows;
    std::size_t y_cols = x_cols;

    // Row partitioning
    const std::size_t base = m / static_cast<std::size_t>(world_size);
    const std::size_t rem  = m % static_cast<std::size_t>(world_size);

    auto rows_of = [&](int r) -> std::size_t {
        return base + (static_cast<std::size_t>(r) < rem ? 1 : 0);
    };

    std::size_t m_local = rows_of(world_rank);

    // Counts/displs for Scatterv (in elements, not bytes)
    std::vector<int> sendcounts_A, displs_A;
    std::vector<int> recvcounts_y, displs_y;
    if (world_rank == 0) {
        sendcounts_A.resize(world_size);
        displs_A.resize(world_size);
        recvcounts_y.resize(world_size);
        displs_y.resize(world_size);

        std::size_t row_disp = 0;
        for (int r = 0; r < world_size; ++r) {
            std::size_t rrows = rows_of(r);
            sendcounts_A[r] = static_cast<int>(rrows * n);
            displs_A[r]     = static_cast<int>(row_disp * n);
            recvcounts_y[r] = static_cast<int>(rrows);
            displs_y[r]     = static_cast<int>(row_disp);
            row_disp += rrows;
        }
    }

    // Root initializes A and x from 2D vectors
    std::vector<int> A_root, x, y_root;
    std::vector<int> y_serial; // serial baseline result (root only)
    std::vector<std::string> verifications; // per-trial yes/no (root only)
    if (world_rank == 0) {
        A_root = flatten(A_2D);
        x = flatten(x_2D);
        y_root.resize(m, 0);
        // Compute serial baseline once on root for verification
        y_serial.resize(m, 0);
        matvec(A_root, m, n, x, y_serial);
    } else {
        x.resize(x_rows, 0);
    }

    // Broadcast x to all ranks
    boost::mpi::broadcast(world, x, 0);

    // Allocate local buffers
    std::vector<int> A_local(m_local * n);
    std::vector<int> y_local(m_local);

    // Vector to store runtimes for multiple trials
    std::vector<double> runtimes;

    // Run multiple trials
    for (int trial = 0; trial < NUM_TRIALS; ++trial) {
        // Start timing
        world.barrier();
        double t_total0 = MPI_Wtime();

        // Scatter rows of A using scatterv
        boost::mpi::scatterv(world, 
                             A_root, 
                             sendcounts_A, 
                             displs_A, 
                             A_local.data(), 
                             static_cast<int>(m_local * n), 
                             0);

        // Compute local matvec
        matvec_local(A_local.data(), x.data(), y_local.data(), m_local, n);

        // Gather y using gatherv
        boost::mpi::gatherv(world, 
                            y_local.data(), 
                            static_cast<int>(m_local), 
                            y_root.data(), 
                            recvcounts_y, 
                            displs_y, 
                            0);

        world.barrier();
        double t_total1 = MPI_Wtime();
        double total_time = t_total1 - t_total0;

        if (world_rank == 0) {
            // Verify parallel vs serial result on root
            bool match = (y_root == y_serial);
            verifications.push_back(match ? "yes" : "no");
            runtimes.push_back(total_time);
        }
    }

    // Write results to CSV file (only root process)
    if (world_rank == 0) {
        double avg_runtime = std::accumulate(runtimes.begin(), runtimes.end(), 0.0) / NUM_TRIALS;

        system("mkdir -p ../data");
        std::ofstream outfile("../data/runs.csv", std::ios::app);

    // Write CSV header if file is empty
        outfile.seekp(0, std::ios::end);
        if (outfile.tellp() == 0) {
        // Note: Column name spelled as requested: 'verfication'
        outfile << "N,p,nodes,ppn,trial,seconds,verfication\n";
        }

        // Write data to CSV
        for (int i = 0; i < NUM_TRIALS; i++) {
            outfile << matrix_size << "," << world_size << "," << nodes << ","
            << ppn << "," << i + 1 << "," << std::fixed << std::setprecision(6) 
            << runtimes[i] << "," << verifications[i] << "\n";
        }
    // Aggregate verification for Average row: yes only if all trials matched
    bool all_match = true;
    for (const auto& v : verifications) {
        if (v != "yes") { all_match = false; break; }
    }
    outfile << matrix_size << "," << world_size << "," << nodes << ","
        << ppn << ",Average," << std::fixed << std::setprecision(6) 
        << avg_runtime << "," << (all_match ? "yes" : "no") << "\n";
        outfile.close();

        std::cout << "Completed " << NUM_TRIALS << " trials for p=" << world_size 
                  << ", Average time: " << avg_runtime << "s\n";
    }

    return 0;
}