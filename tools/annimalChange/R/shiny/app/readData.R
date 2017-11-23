readData <- function(filename){
  Crop1 <-
  read.delim(filename, header = T, stringsAsFactors = FALSE)
  return(Crop1)
  }