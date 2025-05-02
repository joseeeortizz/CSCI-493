/*******************************************
 * Name: Jose Miguel Ortiz
 * MPI Gaussian Elimination - main.cpp
 *******************************************/

#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <mpi.h>
#include <vector>

void read_matrix(const char* filename, std::vector<std::vector<double>>& matrix, unsigned int& N) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening input file.\n";
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    file.read(reinterpret_cast<char*>(&N), sizeof(unsigned int));
    matrix.resize(N, std::vector<double>(N + 1));

    for (unsigned int i = 0; i < N; ++i) {
        file.read(reinterpret_cast<char*>(matrix[i].data()), (N + 1) * sizeof(double));
    }

    file.close();
}

void write_solution(const char* filename, const std::vector<double>& solution, unsigned int N) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening output file.\n";
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    file.write(reinterpret_cast<const char*>(&N), sizeof(unsigned int));
    file.write(reinterpret_cast<const char*>(solution.data()), N * sizeof(double));
    file.close();
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 3) {
        if (rank == 0) std::cerr << "Usage: " << argv[0] << " input.bin output.bin\n";
        MPI_Finalize();
        return 1;
    }

    unsigned int N;
    std::vector<std::vector<double>> matrix;

    if (rank == 0) {
        read_matrix(argv[1], matrix, N);
    }

    MPI_Bcast(&N, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        matrix.resize(N, std::vector<double>(N + 1));
    }

    for (unsigned int i = 0; i < N; ++i) {
        MPI_Bcast(matrix[i].data(), N + 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    for (unsigned int k = 0; k < N; ++k) {
        if (rank == k % size) {
            double pivot = matrix[k][k];
            for (unsigned int j = k; j <= N; ++j) {
                matrix[k][j] /= pivot;
            }
        }

        MPI_Bcast(matrix[k].data(), N + 1, MPI_DOUBLE, k % size, MPI_COMM_WORLD);

        for (unsigned int i = k + 1; i < N; ++i) {
            if (i % size == rank) {
                double factor = matrix[i][k];
                for (unsigned int j = k; j <= N; ++j) {
                    matrix[i][j] -= factor * matrix[k][j];
                }
            }
        }
    }

    std::vector<double> solution(N);
    for (int i = N - 1; i >= 0; --i) {
        if (rank == i % size) {
            solution[i] = matrix[i][N];
            for (unsigned int j = i + 1; j < N; ++j) {
                solution[i] -= matrix[i][j] * solution[j];
            }
        }
        MPI_Bcast(&solution[i], 1, MPI_DOUBLE, i % size, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        write_solution(argv[2], solution, N);
        std::cout << "Solution written to output.bin\n";
    }

    MPI_Finalize();
    return 0;
}