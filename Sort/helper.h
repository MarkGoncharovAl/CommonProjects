#pragma once

#include "mpi.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>

// To print more data use define "debug"

namespace implement {
inline void act(int stat, size_t line, const char file[]) {
  if (stat == MPI_SUCCESS) {
#ifdef debug
    printf("OK! File: %s, line: %d\n", file, line);
#endif
    return;
  }
  printf("ERROR!\nStatus: %d in file: %s, line: %ld\n\n", stat, file, line);
  std::exit(1);
}

inline void check(int stat, const char message[], size_t line,
                  const char file[]) {
  if (stat) {
#ifdef debug
    printf("OK! File: %s, line: %d\n", file, line);
#endif
    return;
  }
  printf("ERROR!\n%s.\nFails in file: %s, line: %ld\n\n", message, file, line);
  std::exit(1);
}

inline void quit(size_t line, const char file[]) {
  implement::act(MPI_Finalize(), line, file);
  std::exit(0);
}
} // namespace implement

#define act(stat) implement::act(stat, __LINE__, __FILE__)
#define check(stat, message) implement::check(stat, message, __LINE__, __FILE__)
#define quit() implement::quit(__LINE__, __FILE__)

namespace mpi {
constexpr int rank_main = 0;
std::pair</*size*/ int, /*rank*/ int> init() {
  int size = 0, rank = 0;
  act(MPI_Init(NULL, NULL));
  act(MPI_Comm_size(MPI_COMM_WORLD, &size));
  act(MPI_Comm_rank(MPI_COMM_WORLD, &rank));
  return std::make_pair(size, rank);
}

void quitAll() {
  std::cout << "QUITTING ALL!\n";
  MPI_Abort(MPI_COMM_WORLD, 1);
}

namespace details {
template <typename T> MPI_Datatype getType() {
  std::cerr << "Type isn't recognized!\n";
  return MPI_DATATYPE_NULL;
}
template <> MPI_Datatype getType<char>() { return MPI_CHAR; }
template <> MPI_Datatype getType<int>() { return MPI_INT; }
template <> MPI_Datatype getType<double>() { return MPI_DOUBLE; }
template <> MPI_Datatype getType<size_t>() { return MPI_UINT64_T; }

} // namespace details

template <typename T> void send(T data, int rank, size_t size, int tag = 0) {
  if constexpr (std::is_pointer_v<T>)
    act(MPI_Send(data, size, details::getType<std::remove_pointer_t<T>>(), rank,
                 tag, MPI_COMM_WORLD));
  else
    act(MPI_Send(&data, size, details::getType<T>(), rank, tag,
                 MPI_COMM_WORLD));
}
template <typename T>
void send(std::vector<T> &data, int rank, size_t size = 0, int tag = 0) {
  if (size == 0)
    size = data.size();
  std::cout << size;
  act(MPI_Send(data.data(), size, details::getType<T>(), rank, tag,
               MPI_COMM_WORLD));
}
template <typename T>
void recv(T data, int rank, size_t size = 1,
          int tag = 0) requires(std::is_pointer_v<T>) {
  act(MPI_Recv(data, size, details::getType<std::remove_pointer_t<T>>(), rank,
               tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE));
}
template <typename T> T recv(int rank, size_t size = 1, int tag = 0) {
  T data;
  act(MPI_Recv(&data, size, details::getType<T>(), rank, tag, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE));
  return data;
}
template <typename T>
std::vector<T> recvVector(int rank, size_t size, int tag = 0) {
  std::vector<T> data(size);
  act(MPI_Recv(data.data(), data.size(), details::getType<T>(), rank, tag,
               MPI_COMM_WORLD, MPI_STATUS_IGNORE));
  return data;
}
} // namespace mpi