#include "def.h"
#include <unistd.h>

namespace fs = std::filesystem;
constexpr std::string_view test_folder = "../tests";

int main() {
  auto [size, rank] = mpi::init();
  if (size != 4) {
    std::cerr << "Must be 4 threads to process!\n";
    quit();
  }

  std::cout << "\n";
  if (rank == mpi::rank_main)
    mainWork();
  else
    elseWork(rank);

  quit();
}

void mainWork() {
  auto streams = openFilesIn(std::string(test_folder));
  for (auto &stream : streams) {
    size_t size = 0;
    stream >> size;
    std::vector<int> data(size);
    for (auto &elem : data)
      stream >> elem;
    size_t extra_size = (4 - (data.size() % 4)) % 4;
    std::vector<int> data2(extra_size + data.size());
    for (auto &elem : data)
      std::cout << elem << "\t";
    std::cout << "\n";
    std::copy(data.begin(), data.end(), data2.begin());
    for (auto iter = data2.rbegin(); iter != data2.rbegin() + extra_size;
         ++iter)
      *iter = 100000;

    auto start_time = MPI_Wtime();
    std::sort(data.begin(), data.end());
    auto end_time = MPI_Wtime();
    std::cout << "Std::sort:\t" << end_time - start_time << "\n";

    start_time = MPI_Wtime();
    startWork(data2);
    end_time = MPI_Wtime();
    std::cout << "My sort:\t" << end_time - start_time << "\n";
  }
}

void startWork(std::vector<int> &data) {
  int small_size = data.size() / 4;
  for (int i = 1; i < 4; ++i) {
    mpi::send(small_size, i, 1);
    mpi::send(data.data() + small_size * i, i, small_size);
  }
  std::vector<int> small_data(small_size);
  std::copy(data.begin(), data.end(), small_data.begin());
  std::sort(small_data.begin(), small_data.end());
  mpi::send(small_data.data(), 2, small_size);
}

void elseWork(int rank) {
  auto size = mpi::recv<int>(0);
  auto data = mpi::recvVector<int>(0, size);
  std::sort(data.begin(), data.end());
  if (rank / 2) {
    auto new_data = mpi::recvVector<int>(rank % 2, size);
    auto out = sortUnity(data, new_data);
  } else {
    mpi::send(data, 2 + rank % 2);
  }
}

std::vector<std::ifstream> openFilesIn(const std::string &folder_name) {
  std::vector<std::ifstream> out;
  fs::path path{folder_name};
  fs::directory_entry dir(path);
  if (!dir.exists())
    mpi::quitAll();

  fs::directory_iterator iter(dir), end;
  for (; iter != end; ++iter) {
    if (fs::is_regular_file(*iter)) {
      std::ifstream file(fs::path(*iter).string(), std::ios::in);
      out.push_back(std::move(file));
    }
  }
  return out;
}
