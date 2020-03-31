#!/bin/bash

mkdir solverlog

for n in 10 25 50 75; do
   for i in {1..10}; do
      for seed in {1..15}; do
         export INST="../instances-HHCRSP/InstanzCPLEX_HCSRP_"$n"_"$i".txt"
         printf "\n\nSolving "$INST" with seed "$seed"\n\n"
         ./brkga $INST $seed 885 1823 0.20655 0.05408 0.32728 | tee "solverlog/solverlog-brkga_"$n"_"$i"_"$seed".txt"
      done
   done
done

for n in 100 200 300; do
   for i in {1..10}; do
      for seed in {1..15}; do
         export INST="../instances-HHCRSP/InstanzVNS_HCSRP_"$n"_"$i".txt"
         printf "\n\nSolving "$INST" with seed "$seed"\n\n"
         $ROOTDIR/build/brkga $INST $seed 885 1823 0.20655 0.05408 0.32728 | tee "solverlog/solverlog-brkga_"$n"_"$i"_"$seed".txt"
      done
   done
done

