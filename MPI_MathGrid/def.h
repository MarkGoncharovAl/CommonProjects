#pragma once
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include "helper.h"

namespace po = boost::program_options;

struct scale_t
{
    size_t size = 10;
    double tau = 1.0;
    double h = 1.0;
};

inline scale_t processStart(int argc, char * argv[]) 
{
    scale_t result;
    po::options_description desc ("Allowed options");
    desc.add_options ()
            ("help,h" , "Information about options")
            ("size,s" , po::value<size_t> () , "Size of result matrix")
            ("tau,t", po::value<double>(), "Time step")
            ("x,x", po::value<double>(), "Coordinate step");
    
    po::variables_map vm;
    po::store (po::parse_command_line (argc , argv , desc) , vm);
    po::notify (vm);

    if (vm.count ("help")) {
        std::cout << desc;
        std::exit(0);
    }
    if (vm.count ("size"))
        result.size = vm["size"].as<size_t> ();
    if (vm.count ("tau"))
        result.tau = vm["tau"].as<double> ();
    if (vm.count ("tau"))
        result.h = vm["x"].as<double> ();

    std::cout << "Size = " << result.size << 
                 "\nTau = " << result.tau << 
                 "\nX = " << result.h << "\n";
    return result;
}