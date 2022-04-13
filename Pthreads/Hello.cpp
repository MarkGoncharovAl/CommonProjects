// clang++-12 -std=c++2a -lpthread -lboost_program_options Hello.cpp 

#include <boost/program_options.hpp>
#include <iostream>
#include <pthread.h>

namespace po = boost::program_options;

namespace details {
void
check(int condition, size_t LINE, const char FILE[])
{
  if (condition) {
    std::cerr << "Failed! Error code: " << condition;
    std::abort();
  }
}
}
#define check(condition) details::check(condition, __LINE__, __FILE__)

void*
print(void* ptr);

// returns threads count
int
processStart(int argc, char* argv[]);

int
main(int argc, char* argv[])
{
  auto numThreads = processStart(argc, argv);
  std::vector<pthread_t> threads(numThreads);
  std::vector<std::string> messages(numThreads, "Hello world: ");
  for (int i = 0; i < threads.size(); ++i) {
    messages[i] += std::to_string(i);
    check(pthread_create(&threads[i], NULL, print, (void*)messages[i].c_str()));
  }
  for (auto& thread : threads)
    check(pthread_join(thread, NULL));

  return 0;
}

void*
print(void* ptr)
{
  std::cout << (char*)ptr << "\n";
  return nullptr;
}

int
processStart(int argc, char* argv[])
{
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