#pragma once

#include "mpi.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cassert>

// To print more data use define "debug"

namespace implement
{
    inline void act (int stat , size_t line , const char file [])
    {
        if (stat == MPI_SUCCESS)
        {
#ifdef debug
            printf ("OK! File: %s, line: %d\n" , file , line);
#endif
            return;
        }
        printf ("ERROR!\nStatus: %d in file: %s, line: %ld\n\n" , stat , file , line);
        std::exit (1);
    }

    inline void check (int stat , const char message [] , size_t line , const char file [])
    {
        if (stat)
        {
#ifdef debug
            printf ("OK! File: %s, line: %d\n" , file , line);
#endif
            return;
        }
        printf ("ERROR!\n%s.\nFails in file: %s, line: %ld\n\n" , message , file , line);
        std::exit (1);
    }

    inline void quit (size_t line , const char file [])
    {
        implement::act(MPI_Finalize () , line , file);
        std::exit (0);
    }
}


#define act(stat) implement::act(stat, __LINE__, __FILE__)
#define check(stat, message) implement::check(stat, message, __LINE__, __FILE__)
#define quit() implement::quit(__LINE__, __FILE__)