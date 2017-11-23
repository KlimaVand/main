library(tidyr)
library(dplyr)
library(ggplot2)
library(readxl)
library(reshape2)
library(shiny)



options(echo=FALSE)
##First read in the arguments listed at the command line

args <- commandArgs(trailingOnly = TRUE)

##args is now a list of character vectors
## First check to see if arguments are passed.
if(length(args)==0){
  print("No arguments supplied.")
  ##supply default values
  setwd("D:/main/tools/annimalChange/R/shiny")
  source("BrowserFunction.R")
  fileroot="outputFarm117050BaselineScenarioNr5"
  #fileroot = "http://gefionau.dk/animalchange/upload/AnimalChange/modeloutput/%20outputFarm117050ScenarioNr5"
  #  fileroot="outputFarm117051ScenarioNr1"
}else  {

  setwd(args[1])
    source("BrowserFunction.R")
  fileroot=args[2]
}
BrowserFunction(fileroot)
