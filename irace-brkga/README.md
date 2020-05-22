## Tunning BRKGA parameters with the irace package

This directory contains all data files you need to run the calibration of the BRKGA algorithm with the R package `irace`. All the instructions below suppose you already have the package installed and the `$PATH` configured to allow access to the `irace` script.

To run the calibration, do the following steps.

1. Copy the instances you want to use as training set from `instances-HHCRSP` to the `irace-brkga/train` directory.

2. Runs `irace` from the directory `irace-bkga` to start the calibration.

__Note 1:__ Take care to only introduce instance files in the directory `irace-brkga/train` since the `irace` will attempt to use them as a HHCRSP instance.

__Note 2:__ You may want to adjust the parallel processing of the experiments according to you processor capability. To to this, adjust the value of `OMP_NUM_THREADS` in the [target runner](target-runner), and the value of `parallel` in the [scenario](scenario.txt) file. Ideally, the value of `OMP_NUM_THREADS` * `parallel` should be smaller than or equals to the number of cores you processor have. The default configuration is set to a octa-core processor.

__Note 3:__ Often is more efficient to run several executions of the `brkga` than a single execution of `brkga` with several decoding threads. This is related to the number of synchronizations made by the multithreading.

You can extract the best configurations found be the calibration on the last lines output by the `irace` execution. You can also recover this data from the .Rdata file of the `arena` directory.
