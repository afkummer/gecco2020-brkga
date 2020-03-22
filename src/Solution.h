#pragma once

#include <vector>

#include "Instance.h"
#include "Task.h"

struct Solution {
   constexpr static double ONE_THIRD = 1.0/3.0;
   constexpr static double COEFS[] = {ONE_THIRD, ONE_THIRD, ONE_THIRD}; // 0:dist, 1:tard, 2:tmax

   const Instance &inst;

   // [vehicle] -> route
   // get<0> -> node
   // get<1> -> skill
   std::vector <std::vector<std::tuple<int,int>>> routes;

   std::vector <int> vehiPos;
   std::vector <double> vehiLeaveTime;

   double dist;
   double tard;
   double tmax;
   double cachedCost;

   Solution(const Instance &inst_);

   Solution &operator=(const Solution &other);

   double findInsertionCost(Task &task) const;

   void updateRoutes(const Task &task);

   void finishRoutes();

   void writeTxt(const char *fname) const;

};

