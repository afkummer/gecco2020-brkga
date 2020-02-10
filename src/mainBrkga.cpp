#include "Instance.h"
#include "Solution.h"
#include "SortingDecoder.h"
#include "Timer.h"

#include "brkga/BRKGA.h"
#include "brkga/MTRand.h"

#include <iostream>
#include <fstream>

using namespace std;


int main(int argc, char **argv) {
   if (argc != 8) {
      cout << "Usage: " << argv[0] << "  <1:inst path> <2:seed> <3:pop size> <4:num generations> <5:% elite> <6:% mutant> <7:% bias inherit elite>" << endl;
      return EXIT_FAILURE;
   }

   cout << "=== BRKGA-based solver for the HHCRSP ===\n" <<
      "Instance: " << argv[1] <<
      "\nSeed: " << argv[2] << endl;

   Instance inst(argv[1]);

   MTRand rng(std::stoi(argv[2]));
   SortingDecoder dec(inst);

   BRKGA solver(
      dec.chromosomeLength(), // chromo length
      std::stoi(argv[3]),     // pop size
      std::stod(argv[5]),     // % elite
      std::stod(argv[6]),     // % mutant
      std::stod(argv[7]),     // bias to inherit from elite parent
      dec,                    // Decoder reference
      rng,                    // rng
      1,                      // number of populations
      omp_get_max_threads()   // number of threads
   );

   // Opens the CSV file to register the output of the solver.
   std::ofstream objProgressFid("brkga-solutions.csv", ios::out | ios::app);
   if (!objProgressFid)
      abort();

   if (objProgressFid.tellp() == 0) {
      objProgressFid <<
         "instance," <<
         "seed," <<
         "generation," <<
         "time," <<
         "cost," <<
         "dist," <<
         "tard," <<
         "tmax," <<
         endl
      ;
   }

   Timer timer;
   Solution sol(inst);

   int generation = 0;
   const int MAX_GENS = std::stoi(argv[4]);

   timer.start();
   do {
      solver.evolve();
      ++generation;

      if (generation % 50 == 0 || generation == MAX_GENS) {
         timer.finish();
         cout << "Generation = " << generation << ", best solution =  " << solver.getBestFitness() <<
            ", elapsed time = " << timer.elapsed() << " secs." << endl;
      }

   } while (generation < MAX_GENS);

   sol = dec.decodeSolution(solver.getBestChromosome());

   objProgressFid <<
      argv[1] << "," <<
      argv[2] << "," <<
      generation << "," <<
      timer.elapsed() << "," <<
      sol.cachedCost << "," <<
      sol.dist << "," <<
      sol.tard << "," <<
      sol.tmax << "," <<
      endl
   ;

   cout << "Best solution found has objective value = " << sol.cachedCost <<
      " Processing time = " << timer.elapsed() << " secs.\n";

   cout << endl << solver.getBestFitness() << endl;

   return EXIT_SUCCESS;
}

