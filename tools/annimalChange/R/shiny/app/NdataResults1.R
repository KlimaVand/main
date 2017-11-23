NdataResults1<-function(Crop1,obs)
{
  #stub = "plantfile.xls"
  #filename = paste(fileroot,stub, sep = '')
  #Crop1 <-read.delim(filename, header = T, stringsAsFactors = FALSE)
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
    
    histogramList <- vector('list', NumSeq)
    i <-obs
    title_string_root<-c("crop seq ",as.character(SeqNumbers[i]))
    title_string<-c("C fixation in ",title_string_root) ##add crop seq number
    title_string<-paste(title_string, collapse=" ") ## concatenate
    Crop2<-filter(Crop1,CropSeq==SeqNumbers[i])##choose a single crop sequence
    
    CropN3<-select(Crop2,Identity,cropNuptake,Nfixed)
    CropN4<-as.data.frame(lapply(CropN3, as.numeric))
    CropN4$cropNuptake<-(CropN4$cropNuptake-CropN4$Nfixed)
    title_string<-c("Crop N input ",title_string_root) ##add crop seq number
    title_string<-paste(title_string, collapse=" ") ## concatenate
    CropN5.long<-gather(CropN4,inputs,value,cropNuptake,Nfixed)
    minX=min(0)
    maxX=max(CropN5.long$Identity)
    return(
      ggplot(CropN5.long, aes(x=Identity, value, fill=inputs, order = -as.numeric(inputs)))+##,colour="C fixed"))+
        geom_bar(stat='identity')  +
        ylab(label="kg N/ha/yr") + ggtitle(title_string)+
        xlab("Crop no"))
} 