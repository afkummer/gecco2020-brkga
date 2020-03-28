#!/usr/bin/env Rscript

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

library(dplyr)

ifam <- list(
   "A" = 10,
   "B" = 25,
   "C" = 50,
   "D" = 75,
   "E" = 100,
   "F" = 200,
   "G" = 300
)

ifamRev <- list(
   "10" = "A",
   "25" = "B",
   "50" = "C",
   "75" = "D",
   "100" = "E",
   "200" = "F",
   "300" = "G"
)

fixName <- function(fname) {
   tk <- strsplit(basename(fname), "_")[[1]]
   tk.ext <- strsplit(tk[4], "\\.")[[1]]
   return(paste0(ifamRev[[tk[3]]], tk.ext[1]))
}

args <- commandArgs(trailingOnly=TRUE)
if (length(args) != 1) {
   cat("\nI need a CSV input file to continue.\n")
   quit()
}

data <- read.csv(args[1]) %>%
   mutate(instance = as.character(instance)) %>%
   dplyr::filter(generation <= 999999999) %>%
   group_by(instance, seed) %>%
   dplyr::filter(generation == max(generation)) %>%
   ungroup() %>%
   mutate(instance = as.character(instance)) %>%
   mutate(instance = Vectorize(fixName)(instance)) %>%
   select(-X)

write.csv(data, "fmtable.out.csv", row.names=F)

