## File name format

### Solutions routes

The best route found for each execution of the solver are stored in the (routes)[routes] directory.

The solution files are labeled according to the number of nodes _N_, the instance identifier _I_, and the seed _S_ used to generate such solution. 
File name pattern is `solution-N-I-S.txt`. For example, the file `solution-200-9-8.txt` refers to the ninth instance with 200 nodes solved with the seed 8.

The file [brkga-results](brkga-results.zip) contains the best solution found in each generation, and for each experiment run with the `brkga` solver. The values presented in the article were extracted from this CSV file.

