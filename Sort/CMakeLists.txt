cmake_minimum_required(VERSION 3.10)
project(Sort)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_COMPILER clang++-12)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3") 

find_package(Boost COMPONENTS program_options REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

option(MAIN "adding main" ON)
set(RUN run)

add_executable(${RUN} main.cpp)
target_link_libraries (
        ${RUN}    
        MPI::MPI_CXX 
        Boost::program_options
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
    )