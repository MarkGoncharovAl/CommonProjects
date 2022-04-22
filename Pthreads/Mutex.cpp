// clang++-12 -std=c++2a -lpthread -lboost_program_options Mutex.cpp

#include <boost/program_options.hpp>
#include <iostream>
#include <pthread.h>
#include <vector>

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

void *Mutex(void *dummy);

// returns threads
int processStart(int argc, char *argv[]);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

int main(int argc, char *argv[]) {
  int threads = processStart(argc, argv);
  std::vector<pthread_t> thread_ids(threads);

  for (auto &thread : thread_ids)
    check(pthread_create(&thread, NULL, Mutex, NULL));
  for (auto &thread : thread_ids)
    check(pthread_join(thread, NULL));
  return 0;
}

void *Mutex(void *dummy) {
  pthread_mutex_lock(&mutex);
  std::cout << "Count: " << counter << "\n";
  counter++;
  pthread_mutex_unlock(&mutex);
  return NULL;
}

int processStart(int argc, char *argv[]) {
  po::options_description desc("Allowed options");
  desc.add_options()("help,h", "Information about options")(
      "threads,t", po::value<int>(), "Threads count");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc;
    std::exit(0);
  }

  if (vm.count("threads"))
    return vm["threads"].as<int>();
  return 3; // default size
}