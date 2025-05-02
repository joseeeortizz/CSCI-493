cmake_minimum_required(VERSION 3.16)
project(ParallelGaussianElimination)

# Find MPI
find_package(MPI REQUIRED)

# Include MPI headers
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

set(CMAKE_CXX_STANDARD 20)

# Main MPI executable for Gaussian Elimination
add_executable(mpi_gauss main.cpp generate_input.cpp read_output.cpp)
target_link_libraries(mpi_gauss ${MPI_LIBRARIES})

# Optional: Helper tools (do NOT submit these to your professor)
add_executable(generate_input Assignments/assignment_3/generate_input.cpp)
add_executable(read_output Assignments/assignment_3/read_output.cpp)

# Add each assignment as a separate executable
add_executable(assignment_1 Assignments/assignment_1/main.cpp)
add_executable(assignment_2 Assignments/assignment_2/Integral.cpp Assignments/assignment_2/Integral.h)