cmake_minimum_required(VERSION 3.9)

project(MPI_prog)
set(CMAKE_CXX_STANDARD 17)
find_package(Boost COMPONENTS program_options REQUIRED)

set(RUN run)
add_executable(${RUN} main.cpp)

find_package(MPI REQUIRED)
message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")
target_link_libraries(${RUN} MPI::MPI_CXX Boost::program_options)