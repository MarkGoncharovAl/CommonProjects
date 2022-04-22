#include "experimental.hpp"
#include <atomic>
#include <boost/program_options.hpp>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

namespace po = boost::program_options;
std::tuple<int, long double, std::pair<long double, long double>> processStart(int argc,
                                                                char *argv[]);
void integral(long double left, long double right, long double error);

constexpr double microStep = 0.2;
std::atomic<double> RESULT;
std::mutex m;

auto f = [](long double x) -> long double { return x * x; };

int main(int argc, char *argv[]) {
  auto [threadSize, error, bounds] = processStart(argc, argv);
  if (bounds.second <= bounds.first) {
    std::cerr << "Right boundary must be righter!\n";
    std::exit(0);
  }

  long double step = (bounds.second - bounds.first) / threadSize;
  long double left = bounds.first;
  long double errorPT = 12.0 * error / threadSize;

  std::vector<std::thread> threads(threadSize);
  for (auto &thread : threads) {
    thread = std::thread(integral, left, left + step, errorPT);
    left += step;
  }
  for (auto &thread : threads) {
    thread.join();
  }
  std::cout << "RESULT:\t" << RESULT << "\n";
}

long double calcSecDerivative(long double point) {
  constexpr long double small_step = 0.001;
  return f(point - small_step) - 2 * f(point) +
         f(point + small_step) / (small_step * small_step);
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
void integral(long double left, long double right, long double error) {
  long double result = 0;
  long double curError = error / (right - left);
  while (left + microStep < right) {
    long double secDerivative = calcSecDerivative(left + microStep / 2);
    long double curStep = std::sqrt(curError / (secDerivative * microStep));
    int steps = static_cast<int>(std::ceil(microStep / curStep));
    curStep = microStep / steps;
    long double curResult = 0;
    for (int i = 1; i < steps - 1; ++i) {
        result += f(left + i * curStep);
    }
    curResult *= 2;
    curResult += f(left) + f(left + microStep - curStep);
    curResult *= curStep / 2.0;
    result += curResult;
    left += microStep;
  }

    long double curMicroStep = right - left;
  long double secDerivative = calcSecDerivative((left + right) / 2);
    long double curStep = std::sqrt(curError / (secDerivative * curMicroStep));
    int steps = static_cast<int>(std::ceil(curMicroStep / curStep));
    curStep = curMicroStep / steps;
    long double curResult = 0;
    for (int i = 1; i < steps - 1; ++i) {
        result += f(left + i * curStep);
    }
    curResult *= 2;
    curResult += f(left) + f(right - curStep);
    curResult *= curStep / 2.0;
    result += curResult;

  // ----- mutex part ------
  RESULT += result;
  // -----------------------
}

std::tuple<int, long double, std::pair<long double, long double>> processStart(int argc,
                                                                char *argv[]) {
  std::vector<long double> boundaries;
  po::options_description desc("Allowed options");
  desc.add_options()("help,h", "Information about options")(
      "thread,t", po::value<int>(),
      "Num of threads")("error,e", po::value<long double>(), "Error of calculation")(
      "boundaries,b", po::value<std::vector<long double>>(&boundaries)->multitoken(),
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