SeqResults2<-function(Crop1,obs)
{
   # filename = paste(fileroot,"plantfile.xls", sep = "")
   # Crop1 <-
    #    read.delim(filename, header = T, stringsAsFactors = FALSE)
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
    
    i <-obs
    Crop2<-filter(Crop1,CropSeq==SeqNumbers[i])##choose a single crop sequence
    
    histogramList <- vector('list', NumSeq)
    Crop8<-select(Crop2,Identity,harvestedC,soilCO2_CEmission,urineCCropClass,fertiliserC,burntResidueC)
    Crop9<-gather(Crop8,inputs,value,harvestedC,soilCO2_CEmission, urineCCropClass,fertiliserC,burntResidueC)
    Crop9$value<-as.numeric(Crop9$value)
    Crop9$Identity<-as.numeric(Crop9$Identity)
    title_string_root<-c("crop seq ",as.character(SeqNumbers[i]))
    title_string<-c("Seq C outputs ",title_string_root) ##add crop seq number
    title_string<-paste(title_string, collapse=" ") ## concatenate
    return(
            ggplot(Crop9, aes(x = Identity, value, fill = inputs, order = -as.numeric(inputs))) + 
                geom_bar(stat = 'identity')  +
                ylab(label = "kg C/ha") + ggtitle(title_string) +
                xlab("Crop no")
        )
} 