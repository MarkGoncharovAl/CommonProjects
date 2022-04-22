# Math solver #
##### 3rd grade MPI #####

## Target ##
To more detail about project look at [**task documentation**](task.pdf)
## Build ##

> - *mkdir build*
> - *cd build*
> - *cmake ../*
> - *cmake --build .*
> - *mpirun -np* **threads** *./run*

It launch default settings, use **--help** to find more about application.

## Example ##

* Use *--oversubscribe* option to use as many threads as you want
* To redefine boundary conditions or function in equation just correct **phi** or **psi** or **f** in main function.
* Concrete example: ```mpirun -np 4 ./run --size=15 --x=0.5 --tau=0.5```

## Task to access ##

> - *cd build*
> - *mpic++ ../accessToLab.cpp -o ./access*
> - *mpirun -np* **threads** *access*

According to data that is got on my computer, it's about **2 micro-seconds**
and not depend on count of used threads.

## MPI speed analysis ##

To check not for answer, just for time use **--time** option

| Threads | Size | Acceleration |
| --- | --- | --- |
| 2 | 15 | 0.26 |
| 2 | 30 | 0.76 |
| 2 | 60 | 0.98 |
| 2 | 100 | 1.38 |
| 2 | 200 | 1.84 |
| 2 | 400 | 1.96 |
| 2 | 800 | 1.6 |
| 2 | 1600 | 1.76 |
| Threads | Size | Acceleration |
| 3 | 15 | 0.16 |
| 3 | 30 | 0.44 |
| 3 | 60 | 0.69 |
| 3 | 100 | 1.29 |
| 3 | 200 | 1.99 |
| 3 | 400 | 1.79 |
| 3 | 800 | 1.82 |
| 3 | 1600 | 2.13 |
| 3 | 2400 | 2.43 |
| 3 | 4000 | 2.58 |
| Threads | Size | Acceleration |
| 4 | 15 | 0.075 |
| 4 | 30 | 0.37 |
| 4 | 60 | 0.67 |
| 4 | 100 | 1.36 |
| 4 | 200 | 1.5 |
| 4 | 400 | 1.62 |
| 4 | 800 | 2.09 |
| 4 | 1600 | 2.17 |
| 4 | 2400 | 2.51 |
| 4 | 4000 | 2.95 |
| 4 | 10000 | 3.07 |