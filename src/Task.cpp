#include "Task.h"

#include <iostream>
#include <limits>

using namespace std;


Task::Task(): Task(-1, -1, -1) {
   // Empty.
}

Task::Task(int node_, int skill1, int skill2) {
   this->node = node_;
   skills[0] = skill1;
   skills[1] = skill2;

#ifndef NDEBUG
   vehi[0] = -1;
   vehi[1] = -1;
   leaveTime[0] = numeric_limits<double>::infinity();
   leaveTime[1] = leaveTime[0];
#endif

}

std::ostream &operator<<(std::ostream &out, const Task &t) {
   out << "Task for patient = " << t.node << " with skills = {" << t.skills[0] <<
      "," << t.skills[1] << "} and cost = " << t.cachedCost << "\n";

   out << "   Vehicle #0 = " << t.vehi[0] << ", leaving at = " << t.leaveTime[0] << "\n";
   out << "   Vehicle #1 = " << t.vehi[1] << ", leaving at = " << t.leaveTime[1] << "\n";

   out << "   Increment on distance indicator: " << t.incDist << "\n";
   out << "   Increment on tard indicator:     " << t.incTard << "\n";
   out << "   Value of tmax indicator:         " << t.incDist << endl;

   return out;
}

std::vector<Task> createTaskList(const Instance &inst) {
   vector <Task> pending(inst.numNodes()-2);

   for (size_t i = 0; i < pending.size(); ++i) {
      Task &t = pending[i];
      t.node = i+1;
      int ss = 0;

      for (int s = 0; s < inst.numSkills(); ++s) {
         if (inst.nodeReqSkill(i+1, s))
            t.skills[ss++] = s;
      }
   }

   return pending;
}
