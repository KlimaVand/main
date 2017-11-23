CToolResults<-function(CTool,obs)
{
  CTool1 <- CTool[-c(1),] ##remove second header line
  CTool2 <- mutate(CTool1,year = as.numeric(startDay) / 365)
  Sequences <- select(CTool2,CropSeqID)
  Sequences <- as.numeric(as.character(Sequences$CropSeq))
  i=1
  NumSeq=0
  oldSeq=0
  SeqNumbers <- rep(0, times = 50)
  while (i <= length(Sequences))
  {
    if (Sequences[i]!=oldSeq)
    {
      NumSeq=NumSeq+1
      SeqNumbers[NumSeq]<-Sequences[i]
      oldSeq=Sequences[i]
    }
    i=i+1
  }
  
  i = 1
  histogramList <- vector('list', NumSeq)
  i <-obs
  CTool3 <- filter(CTool2,CropSeqID == SeqNumbers[i])
  CTool4 <-
    select(CTool3,FOMCStoredStart,HUMCStoredStart,ROMCStoredStart,year)
  CTool5 <- melt(CTool4,id = "year")
  CTool5$value <- as.numeric(CTool5$value)
  maxC <- max(CTool5$value)
  title_string_root <- c("crop seq ",as.character(SeqNumbers[i]))
  title_string <-
    c("Soil C pools in ",title_string_root) ##add crop seq number
  title_string <-
    paste(title_string, collapse = " ") ## concatenate
  
  histogramList[[i]] <-
    ggplot(CTool5, aes(
      x = year, y = value,colour = variable
    )) +
    geom_line() + geom_point() + ggtitle(title_string) + ylim(0,maxC) +
    ylab(label = "kg C/ha") +
    xlab("Year")
  
  
  return(histogramList[obs]);
}