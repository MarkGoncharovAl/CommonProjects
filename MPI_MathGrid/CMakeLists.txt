cmake_minimum_required(VERSION 3.9)

project(MPI_prog)
set(CMAKE_CXX_STANDARD 20)
find_package(Boost COMPONENTS program_options REQUIRED)

set(RUN run)
add_executable(${RUN} main.cpp)

find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})
target_link_libraries(${RUN} MPI::MPI_CXX Boost::program_options)