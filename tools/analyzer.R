#!/usr/bin/env Rscript

library(ggplot2)
library(dplyr)


bks <- NULL
results <- NULL

ifam <- list(
   "A" = 10,
   "B" = 25,
   "C" = 50,
   "D" = 75,
   "E" = 100,
   "F" = 200,
   "G" = 300
)

resortDf <- function(df) {
   df %>%
      mutate(family = substring(instance, 1, 1), iid = as.integer(substring(instance,2))) %>%
      arrange(family, iid) %>%
      select(-family, -iid)
}

readData <- function() {
   args = commandArgs(trailingOnly=TRUE)
   if (length(args) != 2) {
      cat("\nError: Missing arguments: <bks file> <solutions file>\n\n")
      quit()
   }
   cat('\n')

   cat(paste0("Reading BKS data from '", args[1], "'.\n"))
   bks <<- read.csv(args[1]) %>%
      mutate(instance = as.character(instance))

   cat(paste0("Reading solutions from '", args[2], "'.\n"))
   results <<- read.csv(args[2]) %>%
      mutate(instance = as.character(instance))
}


generateMeansTables <- function() {
   meansTbl <- results %>%
      group_by(instance) %>%
      summarize(
         time.mean = mean(time),
         time.sd = sd(time),
         cost.mean = mean(cost),
         cost.sd = sd(cost),
         cost.best = min(cost),
         dist.mean = mean(dist),
         dist.sd = sd(dist),
         tard.mean = mean(tard),
         tard.sd = sd(tard),
         tmax.mean = mean(tmax),
         tmax.sd = sd(tmax),
      )

   meansTbl <- resortDf(meansTbl)

   meansTbl$cost.bks <- bks[match(meansTbl$instance, bks$instance), "bks"]
   meansTbl$time.bks <- bks[match(meansTbl$instance, bks$instance), "time"]

   meansTbl$gapMeanBksCost <- (meansTbl$cost.mean - meansTbl$cost.bks)/meansTbl$cost.bks
   meansTbl$gapBestBksCost <- (meansTbl$cost.best - meansTbl$cost.bks)/meansTbl$cost.bks

   meansTbl$speedupMeanBksTime <- meansTbl$time.bks/meansTbl$time.mean

   write.csv(meansTbl, "means.csv", row.names=F)
   cat("Average results written to 'means.csv'.\n")
}


#====================== Entry point of this script.

readData()

generateMeansTables()
