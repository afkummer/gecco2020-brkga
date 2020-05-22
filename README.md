## A Biased Random Key Genetic Algorithm applied to the VRPTW with Skill Requirements and Synchronization Constraints

This repository contains the code of the genetic algorithm presented in [GECCO 2020 conference](https://gecco-2020.sigevo.org/index.html/HomePage).

## Building the project

To be able to compile the code, you need:
- A C++11 compiler (clang >= 6 or gcc >= 7 should be fine)
- CMake build system
- Make utility
- OpenMP support, if you plan to use multithreading

To build the project, you first need to use the `cmake` utility to generate the makefile.

1. Browse to the `build` directory and issue the command `cmake ..` to generate the makefile.

2. Then, run `make` to build the project. By default, `cmake` sets compiler flags to generate a debug-friendly binary. To change this behavior, run `cmake` of step (1) with the additional argument `-DCMAKE_BUILD_TYPE=Release`.

3. Run `make` to build the binaries. The main executable is called `brkga`.

The example below shows the output of following the build steps.

```bash
~/gecco2020-brkga $ cd build

~/gecco2020-brkga/build $ cmake .. -DCMAKE_BUILD_TYPE=Release
-- The C compiler identification is Clang 10.0.0
-- The CXX compiler identification is Clang 10.0.0
-- Check for working C compiler: /usr/bin/clang
-- Check for working C compiler: /usr/bin/clang -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/clang++
-- Check for working CXX compiler: /usr/bin/clang++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Using compilation flags of mode "Debug"
-- Configuring done
-- Generating done
-- Build files have been written to: /home/alberto/work/gecco2020_brkga/build

~/gecco2020-brkga/build $ make
Scanning dependencies of target commons
[ 25%] Building CXX object CMakeFiles/commons.dir/src/SortingDecoder.cpp.o
[ 25%] Building CXX object CMakeFiles/commons.dir/src/Task.cpp.o
[ 37%] Building CXX object CMakeFiles/commons.dir/src/brkga/Population.cpp.o
[ 62%] Building CXX object CMakeFiles/commons.dir/src/Instance.cpp.o
[ 62%] Building CXX object CMakeFiles/commons.dir/src/Solution.cpp.o
[ 75%] Linking CXX static library libcommons.a
[ 75%] Built target commons
Scanning dependencies of target brkga
[ 87%] Building CXX object CMakeFiles/brkga.dir/src/mainBrkga.cpp.o
[100%] Linking CXX executable brkga
[100%] Built target brkga
```

## Running the genetic algorithm

Once compiled, you should be ready to use this implementation of the BRKGA to solve any instance from Mankowska et al. (2014) dataset. If you execute the binary `brkga` without any arguments, it will present you the command line usage.

```bash
$ ./brkga
Usage: ./brkga  <1:inst path> <2:seed> <3:pop size> <4:num generations> <5:% elite> <6:% mutant> <7:% bias inherit elite>
```

Following the parameters list, you need to specify:

- `<1: inst path>` The path to the instance file to be solved
- `<2: seed>` The seed to be set into the Pseudo-RNG
- `<3: pop size>` Number of individuals of each generation
- `<4:num generations>` Number of generations to evolve
- `<5:% elite>` Population percentage of elite individuals
- `<6:% mutant>` Population percentage of new mutants
- `<7:% bias inherit elite>` Probability to inherit an allele from the elite parent during the mating

__Note__: `<5:% elite> +  <6:% mutant> < 1.0`

__Note 2__: `<7:% bias inherit elite> < 1.0`

You can check an example of usage below. This example uses 8 threads to decode the individuals. To disable multithreading, set the environment variable `OMP_NUM_THREADS=1` or remove the OpenMP support on `CMakeLists.txt`.

```bash
$ ./brkga ../instances-HHCRSP/InstanzVNS_HCSRP_200_8.txt 2 885 1823 0.20655 0.05408 0.32728
=== BRKGA-based solver for the HHCRSP ===
Instance: ../instances-HHCRSP/InstanzVNS_HCSRP_200_8.txt
Seed: 2
Generation = 50, best solution =  2903.09, elapsed time = 3.481 secs.
Generation = 100, best solution =  1765.38, elapsed time = 7.168 secs.
Generation = 150, best solution =  1571.42, elapsed time = 10.492 secs.
Generation = 200, best solution =  1461.08, elapsed time = 13.887 secs.
Generation = 250, best solution =  1392.1, elapsed time = 17.318 secs.
Generation = 300, best solution =  1320.64, elapsed time = 20.737 secs.
Generation = 350, best solution =  1274.02, elapsed time = 24.355 secs.
Generation = 400, best solution =  1232.03, elapsed time = 27.664 secs.
Generation = 450, best solution =  1182.36, elapsed time = 31.117 secs.
Generation = 500, best solution =  1166.19, elapsed time = 34.72 secs.
Generation = 550, best solution =  1161.21, elapsed time = 38.215 secs.
Generation = 600, best solution =  1158.65, elapsed time = 41.631 secs.
Generation = 650, best solution =  1154.9, elapsed time = 45.002 secs.
Generation = 700, best solution =  1154.05, elapsed time = 48.51 secs.
Generation = 750, best solution =  1152.38, elapsed time = 51.901 secs.
Generation = 800, best solution =  1147.3, elapsed time = 55.352 secs.
Generation = 850, best solution =  1147.3, elapsed time = 58.882 secs.
Generation = 900, best solution =  1147.3, elapsed time = 62.523 secs.
Generation = 950, best solution =  1140.87, elapsed time = 66.19 secs.
Generation = 1000, best solution =  1138.8, elapsed time = 69.812 secs.
Generation = 1050, best solution =  1137.1, elapsed time = 73.386 secs.
Generation = 1100, best solution =  1135.76, elapsed time = 76.845 secs.
Generation = 1150, best solution =  1135.76, elapsed time = 80.285 secs.
Generation = 1200, best solution =  1135.3, elapsed time = 83.768 secs.
Generation = 1250, best solution =  1135.17, elapsed time = 87.235 secs.
Generation = 1300, best solution =  1135.17, elapsed time = 90.75 secs.
Generation = 1350, best solution =  1135.17, elapsed time = 94.337 secs.
Generation = 1400, best solution =  1135.17, elapsed time = 98.075 secs.
Generation = 1450, best solution =  1135.17, elapsed time = 101.765 secs.
Generation = 1500, best solution =  1135.14, elapsed time = 105.311 secs.
Generation = 1550, best solution =  1135.14, elapsed time = 109.195 secs.
Generation = 1600, best solution =  1135.14, elapsed time = 112.752 secs.
Generation = 1650, best solution =  1134.21, elapsed time = 116.168 secs.
Generation = 1700, best solution =  1132.77, elapsed time = 119.571 secs.
Generation = 1750, best solution =  1132.77, elapsed time = 122.933 secs.
Generation = 1800, best solution =  1132.77, elapsed time = 126.297 secs.
Generation = 1823, best solution =  1132.77, elapsed time = 127.88 secs.

Best solution written to solution.txt
Best solution found has objective value = 1132.77
Processing time = 127.88 secs.

1132.77
```

After finishing the running, you may be interest into some of those files:

- `brkga-solutions.csv`: The solver appends to this file. It contais the output for each execution of the `brkga` binary, with details about the best individual of each generation.
- `solution.txt`: Contains the best solution found during the search. This file is overwritten on each run of `brkga`.
