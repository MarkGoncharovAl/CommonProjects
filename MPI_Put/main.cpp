#include "helper.h"
 
/**
 * @brief Illustrate how to put data into a target window.
 * @details This application consists of two MPI processes. MPI process 1
 * exposes a window containing an integer. MPI process 0 puts the value 12345
 * in it via MPI_Put. After the MPI_Put is issued, synchronisation takes place
 * via MPI_Win_fence and the MPI process 1 prints the value in its window.
 **/

int change_num (int num)
{
    return num + 1;
}

int main(int argc, char* argv[])
{
    auto [size, rank] = mpi::init();
    std::cout << "\n";
    
    // Create the window
    int window_buffer = 0;
    MPI_Win window;
    act(MPI_Win_create(&window_buffer, sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &window));
    MPI_Win_fence(0, window);

    if(rank == 0)
    {
        // Push my value into the first integer in MPI process 1 window
        int my_value = 1;
        MPI_Put(&my_value, 1, MPI_INT, 1, 0, 1, MPI_INT, window);
    }
 
    // Wait for the MPI_Put issued to complete before going any further
    MPI_Win_fence(0, window);

    for (int i = 1; i < size; ++i) {
        int rank_to = i + 1;
        if (i == size - 1)
            rank_to = 0;

        if (rank == i) {
            std::cout << "Rank: " << rank << " has " << window_buffer << "\n";
            int out = change_num(window_buffer);
            MPI_Put(&out, 1, MPI_INT, rank_to, 0, 1, MPI_INT, window);
        }
        MPI_Win_fence(0, window);
    }
 
    if(rank == 0)
        std::cout << "Rank: " << rank << " has " << window_buffer << "\n";
 
    // Destroy the window
    MPI_Win_free(&window);
    quit();
    return 0;
}