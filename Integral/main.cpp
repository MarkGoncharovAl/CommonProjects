#include "experimental.hpp"
#include <atomic>
#include <boost/program_options.hpp>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>

// Compile:
//    clang++-12 main.cpp -lboost_program_options -lpthread -std=c++2a

namespace po = boost::program_options;
std::tuple<int, long double, std::pair<long double, long double>>
processStart(int argc, char *argv[]);
long double integral(long double left, long double right, long double error);

auto f = [](long double x) -> long double { return sin(1 / (x * x)); };

int main(int argc, char *argv[]) {
  auto [threadSize, error, bounds] = processStart(argc, argv);
  if (bounds.second <= bounds.first) {
    std::cerr << "Right boundary must be righter!\n";
    std::exit(0);
  }
  threadSize *= 8;
  long double step = (bounds.second - bounds.first) / threadSize;
  long double left = bounds.first;
  long double errorPT = error / (threadSize * 4.0);

  std::vector<std::future<long double>> data(threadSize);
  auto startTime = std::chrono::steady_clock::now();
  for (auto &elem : data) {
    elem = std::async(std::launch::async, integral, left, left + step, errorPT);
    left += step;
  }

  long double result = 0;
  for (auto &elem : data)
    result += elem.get();
  auto endTime = std::chrono::steady_clock::now();

  std::cout << "Parallel result:  " << result << "\tTIME:\t"
            << std::chrono::duration_cast<std::chrono::microseconds>(endTime -
                                                                     startTime)
                   .count()
            << "\n";

  startTime = std::chrono::steady_clock::now();
  result = integral(bounds.first, bounds.second, error);
  endTime = std::chrono::steady_clock::now();
  std::cout << "Linear result:    " << result << "\tTIME:\t"
            << std::chrono::duration_cast<std::chrono::microseconds>(endTime -
                                                                     startTime)
                   .count()
            << "\n";
}

long double calcDerivative(long double point) {
  constexpr long double small_step = 0.0005;
  return (f(point + small_step) - f(point - small_step)) / small_step;
}

std::tuple<long double, long double, int>
calcStep(long double left, long double microStep, long double error) {
  long double relation = 1.0;
  long double firstDerivative = calcDerivative(left);
  microStep *= 10.0;
  error *= 10.0;
  do {
    microStep *= 0.1;
    error *= 0.1;
    long double secDerivative = calcDerivative(left + microStep / 2);
    relation = std::abs(secDerivative / firstDerivative);
  } while (relation > 10.0 || relation < 0.1);

  long double curStep = error / std::abs(firstDerivative);
  int steps = static_cast<int>(std::ceil(microStep / curStep));
  curStep = microStep / steps;
  return std::tuple(microStep, curStep, steps);
}

long double integralPart(long double left, long double microStep,
                         long double error) {
  auto [newMicroStep, curStep, steps] = calcStep(left, microStep, error);
  long double curResult = 0;

  for (int i = 1; i < steps - 1; ++i) {
    curResult += f(left + i * curStep);
  }
  curResult *= 2;
  curResult += f(left) + f(left + newMicroStep - curStep);
  curResult *= curStep / 2.0;

  if (newMicroStep < microStep) {
    int times = int(microStep / newMicroStep);
    for (int i = 1; i < times; ++i)
      curResult +=
          integralPart(left + i * newMicroStep, newMicroStep, error / times);
  }
  return curResult;
}

/* -----------------------------------------------------------------
 * integral = h / 2 * [ f(x0) + f[xN] + 2 * {f(x1) + f(x2) + ...}
 * error = max(f``) * (xN - x0) * h^2 / 12
 *
 * We aleady used 12 earlier for each thread
 * curError = max(f``) * (xN - x0) * h^2 ,
 * thus
 * h = np.sqrt(curError / max(f``) / (xN - x0)) .
 * -------------------------------------------------------------- */
long double integral(long double left, long double right, long double error) {
  constexpr double microStep = 0.1;
  long double result = 0;
  error = error * microStep / (right - left);

  while (left + microStep < right) {
    result += integralPart(left, microStep, error);
    left += microStep;
  }
  return result + integralPart(left, right - left, error);
}

std::tuple<int, long double, std::pair<long double, long double>>
processStart(int argc, char *argv[]) {
  std::vector<long double> boundaries;
  po::options_description desc("Allowed options");
  desc.add_options()("help,h", "Information about options")(
      "thread,t", po::value<int>(), "Num of threads")(
      "error,e", po::value<long double>(), "Error of calculation")(
      "boundaries,b",
      po::value<std::vector<long double>>(&boundaries)->multitoken(),
      "Boundaries where integral will be calculated");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc;
    std::exit(0);
  }

  int threads = 1; // default value
  long double error = 0;
  if (vm.count("thread"))
    threads = vm["thread"].as<int>();
  if (vm.count("error"))
    error = vm["error"].as<long double>();
  else {
    std::cerr << "Must be specified error for task!\n";
    std::cerr << desc;
    std::exit(0);
  }
  if (boundaries.size() != 2) {
    std::cerr << "Must be specified boundaries for task using 2 integers!\n";
    std::cerr << desc;
    std::exit(0);
  }

  return std::make_tuple(threads, error,
                         std::make_pair(boundaries[0], boundaries[1]));
}