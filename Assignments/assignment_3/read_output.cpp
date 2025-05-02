/*

Name: Jose Miguel Ortiz
Date: 05/01/2025
Email: jose.ortiz60@myhunter.cuny.edu
*/

#include <iostream>
#include <fstream>
#include <vector>

int main() {
    std::ifstream in("output.bin", std::ios::binary);
    if (!in) {
        std::cerr << "Error opening output.bin\n";
        return 1;
    }

    unsigned int N;
    in.read(reinterpret_cast<char*>(&N), sizeof(unsigned int));
    std::vector<double> solution(N);
    in.read(reinterpret_cast<char*>(solution.data()), N * sizeof(double));

    std::cout << "Solution:\n";
    for (unsigned int i = 0; i < N; ++i) {
        std::cout << "x[" << i << "] = " << solution[i] << '\n';
    }

    in.close();
    return 0;
}