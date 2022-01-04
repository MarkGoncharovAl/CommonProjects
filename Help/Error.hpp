#pragma once

#include <iostream>

namespace Err {
    inline void assert(bool State, std::string Mes) {
        if (!State) {
            std::cout << Mes << std::endl;
            exit(0);
        }   
    }
}