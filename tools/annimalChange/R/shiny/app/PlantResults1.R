PlantResults1<-function(Crop1,obs)
{
   # filename = paste(fileroot,"plantfile.xls", sep = "")
    #Crop1<-read.delim("outputFarm117050BaselineScenarioNr5plantfile.xls", header = T, stringsAsFactors = FALSE)
    #Crop1 <-
        #read.delim(filename, header = T, stringsAsFactors = FALSE)
    Crop1 <- Crop1[-c(1),] ##remove second header line
    Sequences <-
        select(Crop1,CropSeq)####copy crop sequence number to separate dataframe
    Sequences <-
        as.numeric(as.character(Sequences$CropSeq))##convert to numeric
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
    
    CFIX <- select(Crop1,CropSeq,Identity,CFixed)
    CFIX$CFixed <- as.numeric(CFIX$CFixed)
    CFIX$Identity <- as.numeric(CFIX$Identity)
    maxC <- max(as.numeric(CFIX$CFixed))
    
    histogramList <- vector('list', NumSeq)
    i <-obs
    title_string_root <- c("crop seq ",as.character(SeqNumbers[i]))
    title_string <-
        c("C fixation in ",title_string_root) ##add crop seq number
    title_string <-
        paste(title_string, collapse = " ") ## concatenate
    CFixSeq <- filter(CFIX,CropSeq == SeqNumbers[i])
    return(
        ggplot(CFixSeq, aes(x = Identity, y = CFixed)) + 
            geom_bar(stat = 'identity',position = "dodge") +
            ylab(label = "kg C/ha/yr") + ggtitle(title_string) +
            ylim(0,maxC) +
            xlab("Crop no")
    )
}