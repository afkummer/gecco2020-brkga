/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020
 * Alberto Francisco Kummer Neto (afkneto@inf.ufrgs.br),
 * Luciana Salete Buriol (buriol@inf.ufrgs.br) and
 * Olinto César Bassi de Araújo (olinto@ctism.ufsm.br)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

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
   if (argc != 10) {
      cout << "Usage: " << argv[0] << "  <1:inst path> <2:seed> <3:pop size> <4:num generations> <5:% elite> <6:% mutant> <7:% bias inherit elite> <8:iter w/o restart> <9:tol for improvement>" << endl;
      return EXIT_FAILURE;
   }

   const int itersNoImproving = stoi(argv[8]);
   const double improvingTol = stod(argv[9]);

   cout << "=== BRKGA-based solver for the HHCRSP ===\n" <<
      "Instance: " << argv[1] <<
      "\nSeed: " << argv[2] << endl;

   cout << "Iterations without improvement prior restarting: " << itersNoImproving << "\n";
   cout << "Improvement tolerance: " << improvingTol << "\n";

   Instance inst(argv[1]);

   MTRand rng(std::stoi(argv[2]));
   SortingDecoder dec(inst);

   BRKGA <SortingDecoder, MTRand> solver(
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

   int lastImprGen = 0;
   double lastImprValue = 1e6;
   double bestSolEver = 1e6;

   timer.start();
   string prefix = "";
   do {
      solver.evolve();
      ++generation;

      if (generation % 50 == 0 || generation == MAX_GENS) {
         timer.finish();
         cout << prefix << "Generation = " << generation << ", best solution =  " << solver.getBestFitness() <<
            ", elapsed time = " << timer.elapsed() << " secs." << endl;
         prefix = "";
      }

      sol = dec.decodeSolution(solver.getBestChromosome());
      timer.finish();
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
      bestSolEver = min(bestSolEver, sol.cachedCost);

      if (itersNoImproving > 0) {
         double impr = (lastImprValue - sol.cachedCost)/lastImprValue;
         if (impr > improvingTol) {
            lastImprGen = generation;
            lastImprValue = sol.cachedCost;
         } else {
            if (generation - lastImprGen >= itersNoImproving) {
               prefix = "R ";
               lastImprGen = generation;
               lastImprValue = 1e6;
               solver.reset();               
            }
         }
      }

   } while (generation < MAX_GENS);

   sol = dec.decodeSolution(solver.getBestChromosome());
   sol.writeTxt("solution.txt");
   cout << "\nBest solution written to solution.txt\n";

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

   cout << "Best solution found has objective value = " << bestSolEver <<
      "\nProcessing time = " << timer.elapsed() << " secs.\n";

   cout << endl << bestSolEver << endl;

   return EXIT_SUCCESS;
}

