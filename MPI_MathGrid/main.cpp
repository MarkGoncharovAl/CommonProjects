#include "helper.h"
#include "def.h"
#include <vector>
#include <array>
#include <algorithm>
#include <iterator>
#include <functional>

constexpr double tau = 0.2;
constexpr double h = 1.0;

std::vector<std::vector<double>>
    solveLinear (scale_t scale ,
                 std::function<double (double)> phi ,
                 std::function<double (double)> psi ,
                 std::function<double (double, double)> f);

int main (int argc , char* argv [])
{
    auto scale = processStart (argc , argv);
    auto phi = [](double x) -> double { return x; };
    auto psi = [](double t) -> double { return t; };
    auto f = [](double x , double t) -> double { return x + t; };

    auto check = solveLinear (scale , phi , psi , f);
    std::cout << "Final Matrix";
    std::for_each (check.begin () , check.end () , [](auto& arr) {
        std::copy (arr.begin () , arr.end () , std::ostream_iterator<double> (std::cout , "\t| "));
        std::cout << "\n";
    });
}

std::vector<std::vector<double>>
    solveLinear (scale_t scale ,
                 std::function<double (double)> phi ,
                 std::function<double (double)> psi ,
                 std::function<double (double, double)> f)
{
    double tauh = (scale.h + scale.tau) / (2 * scale.h * scale.tau);

    // first arg = time, second arg = coord
    std::vector<std::vector<double>> data (scale.size);
    std::for_each (data.begin () , data.end () , [scale](std::vector<double>& arr) {arr.resize (scale.size); });

    // beg ------- boundary conditions ------------
    std::generate (data[0].begin () , data[0].end () , [n = 0, &phi, scale] () mutable 
                    { return phi (scale.h * n++); });
    std::for_each (data.begin () + 1 , data.end () , [n = 1, &psi, scale] (std::vector<double>& arr) mutable 
                    { arr[0] = psi (scale.tau * n++); });
    // end ------- boundary conditions ------------

    for (int t = 0; t < scale.size - 1; ++t)
    {
        for (int x = 0; x < scale.size - 1; ++x)
        {
            data[t + 1][x + 1] = (
                f (scale.tau * (0.5 + t) , scale.h * (0.5 + x)) +
                (data[t][x] + data[t][x + 1] - data[t + 1][x]) / 2.0 / scale.tau +
                (data[t][x] - data[t][x + 1] + data[t + 1][x]) / 2.0 / scale.h
                ) / tauh;
        }
    }

    return data;
}