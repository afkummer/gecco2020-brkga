#pragma once

#include "Instance.h"
#include "Solution.h"


struct SortingDecoder {
   using TElem = std::tuple<double, Task>;

   const Instance &inst;

   // Cache of vehicles by skills.
   // [skill id] -> vehicles list
   std::vector <std::vector <int>> vehiSkills;

   // Caches the task vector used into the decoding process.
   std::vector <TElem> initialTasks;

   SortingDecoder(const Instance &inst_);

   int chromosomeLength() const;

   Solution decodeSolution(const std::vector <double> &chromosome) const;

   double decode(const std::vector <double> &chromosome) const;

};
