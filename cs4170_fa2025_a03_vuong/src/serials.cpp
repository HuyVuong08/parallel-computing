#include <iostream>
#include <vector>
#include <cstdlib>
#include <iomanip>

// Template function to flatten a 2D vector to 1D (row-major)
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

// Template function to print a matrix in row-major format
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

// Compute y = A(m x n) * x(n) using row-major A.
// Template version for generic types
template<typename T>
void matvec(const T* A, const T* x, T* y, std::size_t m, std::size_t n) {
    for (std::size_t i = 0; i < m; ++i) {
        const std::size_t row = i * n;
        T sum = 0;
        for (std::size_t j = 0; j < n; ++j) {
            sum += A[row + j] * x[j];
        }
        y[i] = sum;
    }
}

// std::vector convenience wrapper. A is flattened row-major (size m*n).
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
    // Problem size: 1000x1000 matrix A and 1000x1 vector x
    const std::size_t matrix_size = 1000;
    
    // Initialize matrix A as 2D vector (1000x1000)
    std::vector<std::vector<int>> A_2D(matrix_size);
    for (std::size_t i = 0; i < matrix_size; ++i) {
        A_2D[i].resize(matrix_size);
        for (std::size_t j = 0; j < matrix_size; ++j) {
            A_2D[i][j] = static_cast<int>((i + j) % 10);  // Simple pattern
        }
    }
    
    // Initialize matrix x as 2D vector (1000x1)
    std::vector<std::vector<int>> x_2D(matrix_size);
    for (std::size_t i = 0; i < matrix_size; ++i) {
        x_2D[i].resize(1);
        x_2D[i][0] = 1;  // All ones
    }
    
    // Flatten the 2D vectors to 1D for matvec computation
    std::vector<int> A = flatten(A_2D);
    std::vector<int> x = flatten(x_2D);
    
    // Derive dimensions from the 2D vectors
    std::size_t a_rows = A_2D.size();      // Number of rows in matrix A (3)
    std::size_t a_cols = A_2D[0].size();   // Number of columns in matrix A (3)
    std::size_t x_rows = x_2D.size();      // Number of rows in matrix x (3)
    std::size_t x_cols = x_2D[0].size();   // Number of columns in matrix x (1)
    
    // For matrix multiplication A * x:
    // A is (a_rows × a_cols), x is (x_rows × x_cols)
    // Result y will be (a_rows × x_cols)
    // Requirement: a_cols must equal x_rows
    
    if (a_cols != x_rows) {
        std::cerr << "Error: Matrix dimensions incompatible for multiplication\n";
        std::cerr << "A is " << a_rows << "×" << a_cols << ", x is " << x_rows << "×" << x_cols << "\n";
        return 1;
    }
    
    std::size_t y_rows = a_rows;   // y has same number of rows as A
    std::size_t y_cols = x_cols;   // y has same number of columns as x

    std::vector<int> y;
    
    // matvec expects: A (m×n), x (n), produces y (m)
    // Pass: m = a_rows, n = a_cols (which equals x_rows)
    matvec(A, a_rows, a_cols, x, y);

    // Print result
    std::cout << std::fixed << std::setprecision(3);
    
    std::cout << "Matrix dimensions:\n";
    std::cout << "A: " << a_rows << " × " << a_cols << "\n";
    std::cout << "x: " << x_rows << " × " << x_cols << "\n";
    std::cout << "y: " << y_rows << " × " << y_cols << "\n\n";
    
    // Only print matrices if they are small (less than 20x20)
    if (a_rows <= 20 && a_cols <= 20) {
        printMatrix("A", A, a_rows, a_cols);
        printMatrix("x", x, x_rows, x_cols);
        printMatrix("y", y, y_rows, y_cols);
    } else {
        // For large matrices, just print first few and last few elements of y
        std::cout << "Result y (showing first 10 and last 10 elements):\n";
        for (std::size_t i = 0; i < std::min(static_cast<std::size_t>(10), y_rows); ++i) {
            std::cout << "y[" << i << "] = " << y[i] << "\n";
        }
        if (y_rows > 20) {
            std::cout << "...\n";
            for (std::size_t i = y_rows - 10; i < y_rows; ++i) {
                std::cout << "y[" << i << "] = " << y[i] << "\n";
            }
        }
    }
    
    return 0;
}