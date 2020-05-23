# Article solutions

The best route found for each execution of the solver are stored in the (solutions/routes)[solutions/routes] directory.

The solution files are labeled according to the number of nodes _N_, the instance identifier _I_, and the seed _S_ used to generate such solution.
File name pattern is `solution-N-I-S.txt`. For example, the file `solution-200-9-8.txt` refers to the ninth instance with 200 nodes solved with the seed 8. To translate the solution names to the corresponding instance names used in the article, check the table below. Furthermore, the instance identifier can be attached directly to the instance family name. For example, the file `solution-50-8-3.txt` refers to the instance `C8`, solver by the `brkga` using the seed `3`.

| Number of nodes | Instance family |
|:---------------:|:---------------:|
|        10       |        A        |
|        25       |        B        |
|        50       |        C        |
|        75       |        D        |
|       100       |        E        |
|       200       |        F        |
|       300       |        H        |

The file [solutions/brkga-results](solutions/brkga-results.zip) contains the best solution found in each generation, and for each experiment run with the `brkga` solver. The results presented in the article were extracted from this CSV file.
