#include "Solution.h"

#include <cassert>
#include <fstream>
#include <iostream>

using namespace std;

Solution::Solution(const Instance& inst_): inst(inst_) {
   // Resizing of data structures
   routes.resize(inst.numVehicles());

   // All vehicles start at depot node...
   vehiPos.resize(inst.numVehicles(), 0);
   for (int v = 0; v < inst.numVehicles(); ++v) {
      routes[v].reserve(inst.numNodes());
      routes[v].emplace_back(make_tuple(0, 0));
   }

   // And are ready to leave at time 0.
   vehiLeaveTime.resize(inst.numVehicles(), 0.0);

   // Initialize the solution cost indicators.
   dist = 0.0;
   tard = 0.0;
   tmax = 0.0;
   cachedCost = 0.0;
}

Solution &Solution::operator=(const Solution &other) {
   routes = move(other.routes);
   vehiPos = move(other.vehiPos);
   vehiLeaveTime = move(other.vehiLeaveTime);
   dist = other.dist;
   tard = other.tard;
   tmax = other.tmax;
   cachedCost = other.cachedCost;
   return *this;
}

double Solution::findInsertionCost(Task &task) const {
   assert(task.vehi[0] != -1 && "First vehicle is unset.");

   // Compute the arrival time of the first vehicle.
   double arrivalV0 = max(inst.nodeTwMin(task.node), vehiLeaveTime[task.vehi[0]] + inst.distance(vehiPos[task.vehi[0]], task.node));

   if (inst.nodeSvcType(task.node) == Instance::SvcType::SINGLE) {

      assert(task.skills[0] != -1 && "First skill for single service patient unset.");
      assert(task.skills[1] == -1 && "Second skill for single service patient set.");

      double tardinessV0 = max(0.0, arrivalV0 - inst.nodeTwMax(task.node));

      task.leaveTime[0] = arrivalV0 + inst.nodeProcTime(task.node, task.skills[0]);

      task.incDist = inst.distance(vehiPos[task.vehi[0]], task.node);
      task.incTard = tardinessV0;
      task.currTmax = tardinessV0;

   } else {
      assert(task.vehi[1] != -1 && "Second vehicle is unset.");

      // Computes the arrival time of the second vehicle.
      double arrivalV1 = max(inst.nodeTwMin(task.node), vehiLeaveTime[task.vehi[1]] + inst.distance(vehiPos[task.vehi[1]], task.node));

      if (inst.nodeSvcType(task.node) == Instance::SvcType::SIM) {

         assert(task.skills[0] != -1 && "First skill for simultaneous double service patient unset.");
         assert(task.skills[1] != -1 && "Second skill for simultaneous double service patient unset.");

         double startTime = max(arrivalV0, arrivalV1);

         double svcTime = max(inst.nodeProcTime(task.node, task.skills[0]), inst.nodeProcTime(task.node, task.skills[1]));

         task.leaveTime[0] = startTime + svcTime;
         task.leaveTime[1] = startTime + svcTime;

         task.incDist = inst.distance(vehiPos[task.vehi[0]], task.node) + inst.distance(vehiPos[task.vehi[1]], task.node);

         double tardinessAll = max(0.0, startTime - inst.nodeTwMax(task.node));
         task.incTard = 2.0 * tardinessAll;
         task.currTmax = tardinessAll;

      } else {

         assert(task.skills[0] != -1 && "First skill for double service precedence patient unset.");
         assert(task.skills[1] != -1 && "Second skill for double service precedence patient unset.");

         double startTimeV0 = arrivalV0;
         double startTimeV1 = max(arrivalV1, startTimeV0 + inst.nodeDeltaMin(task.node));

         // Fix any violation of maximum separation time.
         double violDeltaMax = max(0.0, (startTimeV1 - startTimeV0) - inst.nodeDeltaMax(task.node));
         startTimeV0 += violDeltaMax;

         assert((startTimeV1-startTimeV0)+0.5 >= inst.nodeDeltaMin(task.node) && "Violation delta_min on double service with precedence.");
         assert((startTimeV1-startTimeV0)-0.5 <= inst.nodeDeltaMax(task.node) && "Violation delta_max on double service with precedence.");

         double tardinessV0 = max(0.0, startTimeV0 - inst.nodeTwMax(task.node));
         double tardinessV1 = max(0.0, startTimeV1 - inst.nodeTwMax(task.node));

         task.leaveTime[0] = startTimeV0 + inst.nodeProcTime(task.node, task.skills[0]);
         task.leaveTime[1] = startTimeV1 + inst.nodeProcTime(task.node, task.skills[1]);

         task.incDist = inst.distance(vehiPos[task.vehi[0]], task.node) + inst.distance(vehiPos[task.vehi[1]], task.node);
         task.incTard = tardinessV0 + tardinessV1;
         task.currTmax = max(tardinessV0, tardinessV1);

      }
   }

   task.cachedCost =
      COEFS[0] * (dist + task.incDist) +
      COEFS[1] * (tard + task.incTard) +
      COEFS[2] * max(tmax, task.currTmax)
   ;

   return task.cachedCost;
}

void Solution::updateRoutes(const Task &task) {
   assert(task.skills[0] != -1 && "First skill for simultaneous double service patient unset.");
   assert(task.vehi[0] != -1 && "Vehicle for the first skill unset.");

   routes[task.vehi[0]].push_back(make_tuple(task.node, task.skills[0]));
   vehiPos[task.vehi[0]] = task.node;
   vehiLeaveTime[task.vehi[0]] = task.leaveTime[0];

   if (task.skills[1] != -1) {
      assert(task.skills[1] != -1 && "Second skill for simultaneous double service patient unset.");
      assert(task.vehi[1] != -1 && "Vehicle for the second skill unset.");

      routes[task.vehi[1]].push_back(make_tuple(task.node, task.skills[1]));
      vehiPos[task.vehi[1]] = task.node;
      vehiLeaveTime[task.vehi[1]] = task.leaveTime[1];
   }

   dist += task.incDist;
   tard += task.incTard;
   tmax = max(tmax, task.currTmax);

   cachedCost = task.cachedCost;
}

void Solution::finishRoutes() {
   // Add the depot node at the end of each vehicle route.
   for (int v = 0; v < inst.numVehicles(); ++v) {
      int lastNode = get<0>(routes[v].back());
      dist += inst.distance(lastNode, 0);
      routes[v].push_back(make_tuple(0, 0));
   }

   // Update the solution to take into account the distances on returning to depot.
   cachedCost =
      COEFS[0] * dist +
      COEFS[1] * tard +
      COEFS[2] * tmax;
}

void Solution::writeTxt(const char* fname) const {
   ofstream fid(fname);
   if (!fid) {
      cout << "Solution file '" << fname << "' can not be written." << endl;
      exit(EXIT_FAILURE);
   }

   fid << "# Solution for " << inst.fileName() << "\n";
   fid << "# Cost = " << cachedCost << " Dist = " << dist << " Tard = " <<
      tard << " TMax = " << tmax << "\n";
   fid << "# <vehicle> <route length>\n";
   fid << "# <origin node> <dest node> <vehicle> <service type>\n";

   for (int v = 0; v < inst.numVehicles(); ++v) {
      fid << v << " " << routes[v].size()-1 << "\n";
      for (unsigned pos = 1; pos < routes[v].size(); ++pos) {
         fid << get<0>(routes[v][pos-1]) << ' ';
         fid << get<0>(routes[v][pos]) << ' ';
         fid << v << ' ';
         fid << get<1>(routes[v][pos]) << '\n';
      }
   }

}
