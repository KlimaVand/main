CToolResultsN<-function(CTool1,obs)
{
  #stub = "CtoolFile.xls"
  #filename = paste(fileroot,stub, sep = '')
  #CTool <-read.delim(filename, header = T, stringsAsFactors = FALSE)
  CTool1 <- CTool1[-c(1),] ##remove second header line
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
  CTool6 <-select(CTool3,FOMnStoredStart,HUMnStoredStart,ROMnStoredStart,FOMnStoredEnd,HUMnStoredEnd,ROMnStoredEnd,year)
  CTool7<-as.data.frame(lapply(CTool6, as.numeric))
  CTool7$ChangeFOMn<-(CTool7$FOMnStoredEnd-CTool7$FOMnStoredStart)
  CTool7$ChangeHUMn<-(CTool7$HUMnStoredEnd-CTool7$HUMnStoredStart)
  CTool7$ChangeROMn<-(CTool7$ROMnStoredEnd-CTool7$ROMnStoredStart)
  names(CTool7)[8]<-"ChangeFOMn"
  names(CTool7)[9]<-"ChangeHUMn"
  names(CTool7)[10]<-"ChangeROMn"
  CTool8 <-select(CTool7,ChangeFOMn,ChangeHUMn,ChangeROMn,year)
  CTool9 <- melt(CTool8,id = "year")
  CTool9$value <- as.numeric(CTool9$value)
  #maxN <- max(CTool8$value)
  title_string_root <- c("crop seq ",as.character(SeqNumbers[i]))
  title_string <-c("Change in soil N pools in ",title_string_root) ##add crop seq number
  title_string <-
    paste(title_string, collapse = " ") ## concatenate
  
  histogramList[[i]] <-ggplot(CTool9, aes(x = year, y = value,colour = variable)) +
    geom_line() + geom_point() + ggtitle(title_string) + #ylim(0,maxN) +
    ylab(label = "kg N/ha") +
    xlab("Year")
  
  
  return(histogramList[obs]);
}