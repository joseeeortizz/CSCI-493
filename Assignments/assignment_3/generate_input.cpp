/**
Name: Jose Miguel Ortiz
Date: 05/01/2025
Email: jose.ortiz60@myhunter.cuny.edu

*/

#include <iostream>
#include <fstream>
#include <vector>

int main() {
    // Adjust N and matrix as needed for your project/MPI format.
    unsigned int N = 3;
    std::vector<std::vector<double>> matrix = {
        {2, 1, -1, 8},
        {-3, -1, 2, -11},
        {-2, 1, 2, -3}
    };

    std::ofstream out("input.bin", std::ios::binary);
    if (!out) {
        std::cerr << "Error opening file for writing.\n";
        return 1;
    }

    // Write matrix size (N)
    out.write(reinterpret_cast<const char*>(&N), sizeof(unsigned int));
    // Write matrix data (N rows, N+1 columns)
    for (const auto& row : matrix) {
        out.write(reinterpret_cast<const char*>(row.data()), (N + 1) * sizeof(double));
    }

    out.close();
    std::cout << "input.bin written.\n";
    return 0;
}