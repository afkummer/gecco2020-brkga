#pragma once

#include "Instance.h"
#include "Solution.h"


struct SortingDecoder {

   const Instance &inst;

   // Cache of vehicles by skills.
   // [skill id] -> vehicles list
   std::vector <std::vector <int>> vehiSkills;

   SortingDecoder(const Instance &inst_);

   int chromosomeLength() const;

   Solution decodeSolution(const std::vector <double> &chromosome) const;

   double decode(const std::vector <double> &chromosome) const;

};
