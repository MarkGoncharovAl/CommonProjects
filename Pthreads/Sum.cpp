// clang++-12 -std=c++2a -lpthread -lboost_program_options Sum.cpp

#include <boost/program_options.hpp>
#include <iostream>
#include <pthread.h>

namespace po = boost::program_options;

namespace details {
void check(int condition, size_t LINE, const char FILE[]) {
  if (condition) {
    std::cerr << "Failed! Error code: " << condition;
    std::abort();
  }
}
} // namespace details
#define check(condition) details::check(condition, __LINE__, __FILE__)

void *calcSum(void *ptr);

// returns [threads, N]
std::pair<int, int> processStart(int argc, char *argv[]);

using pair = int[2];

int main(int argc, char *argv[]) {
  auto [threadSize, N] = processStart(argc, argv);
  int for_each = N / threadSize;
  double sum = 0;
  double offset = N % threadSize;

  std::vector<pthread_t> threads(threadSize);
  std::vector<std::unique_ptr<int[]>> sizes;
  for (int i = 0; i < threadSize; ++i) {
    sizes.push_back(std::unique_ptr<int[]>(new int[2]));
    if (i == 0) {
      sizes[i][0] = 0;
      sizes[i][1] = for_each + offset;
    } else {
      sizes[i][0] = for_each * i + offset;
      sizes[i][1] = sizes[i][0] + for_each;
    }
    check(
        pthread_create(&threads[i], nullptr, calcSum, (void *)sizes[i].get()));
  }

  for (auto &thread : threads) {
    double *new_sum = nullptr;
    check(pthread_join(thread, (void **)&new_sum));
    sum += *new_sum;
    delete new_sum;
  }

  std::cout << sum;
  return 0;
}

void *calcSum(void *ptr) {
  int *sizes = (int *)ptr;
  double *sum = new double;
  for (int i = sizes[0] + 1; i <= sizes[1]; ++i) {
    *sum += 1.0 / (double)i;
  }
  pthread_exit((void *)sum);
}

// returns [threads, N]
std::pair<int, int> processStart(int argc, char *argv[]) {
  po::options_description desc("Allowed options");
  desc.add_options()("help,h", "Information about options")(
      "threads,t", po::value<int>(), "Threads count")("N,n", po::value<int>(),
                                                      "N to count untill");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc;
    std::exit(0);
  }

  std::pair<int, int> out = {1, 10}; // default
  if (vm.count("threads"))
    out.first = vm["threads"].as<int>();
  if (vm.count("N"))
    out.second = vm["N"].as<int>();
  return out;
}