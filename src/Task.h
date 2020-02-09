#pragma once

#include "Instance.h"


struct Task {
   Task();
   Task(int node_, int skill1, int skill2 = -1);

   // Patient and service type information.
   // In case of double services with precedences, skills[0]
   // holds the skill with greater priority.
   int node;
   int skills[2];

   // Vehicle assignment information.
   int vehi[2];
   double leaveTime[2];

   // Cost related information.
   double incDist;
   double incTard;
   double currTmax;
   double cachedCost;
};

std::ostream &operator<<(std::ostream &out, const Task &t);

std::vector <Task> createTaskList(const Instance &inst);
