#!/usr/bin/env python3
# -*- coding:utf-8 -*-

#
# The MIT License (MIT)
#
# Copyright (c) 2020
# Alberto Francisco Kummer Neto (afkneto@inf.ufrgs.br),
# Luciana Salete Buriol (buriol@inf.ufrgs.br) and
# Olinto César Bassi de Araújo (olinto@ctism.ufsm.br)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

#
# Simple script to plot a BRKGA solution as a scatter plot.
# To export a solution, use the method `Solution::writeTxt2` in 
# the C++ code.
#
# @author Alberto Kummer
#

if __name__ == "__main__":
   from sys import argv
   if len(argv) != 2:
      print("Usage: %s <1: solution path>" % argv[0])
      exit(1)

   fid = open(argv[1], "rt")

   fid.readline()
   fid.readline()
   fid.readline()
   fid.readline()

   from matplotlib import pyplot as plt
   depot_x = -1
   depot_y = -1

   while True:
      line = fid.readline()
      if len(line) == 0:
         break

      tks = [int(x) for x in line.split()]
      vehicle = tks[0]
      route_length = tks[1]
      route_x = []
      route_y = []

      for r in range(route_length):
         line = fid.readline()
         tks = [int(x) for x in line.split()]
         route_x.append(tks[0])
         route_y.append(tks[1])

      route_x.append(route_x[0])
      route_y.append(route_y[0])

      depot_x = route_x[0]
      depot_y = route_y[0]

      plt.plot(route_x, route_y, marker='*', label=("v=" + str(vehicle)))

   plt.plot(depot_x, depot_y, marker='s', color='black', markersize=10)
   plt.legend()
   plt.show()



