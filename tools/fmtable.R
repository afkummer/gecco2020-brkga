#!/usr/bin/env Rscript

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

