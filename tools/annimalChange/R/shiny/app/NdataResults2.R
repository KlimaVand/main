NdataResults2<-function(Crop1,obs)
{
  #stub = "plantfile.xls"
  #filename = paste(fileroot,stub, sep = '')
 # Crop1 <-read.delim(filename, header = T, stringsAsFactors = FALSE)
  Crop1 <- Crop1[-c(1),] ##remove second header line
  Sequences <-select(Crop1,CropSeq)####copy crop sequence number to separate dataframe
  Sequences <-as.numeric(as.character(Sequences$CropSeq))##convert to numeric
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
  title_string_root <- c("crop seq ",as.character(SeqNumbers[i]))
  Crop2 <-
      filter(Crop1,CropSeq == SeqNumbers[i])##choose a single crop sequence

  CropN6<-select(Crop2,Identity,harvestedN,surfaceResidueN,subsurfaceResidueN,residueNtoNextCrop)
  CropN7<-as.data.frame(lapply(CropN6, as.numeric))
  CropN7.long<-gather(CropN7,inputs,value,residueNtoNextCrop,harvestedN,surfaceResidueN,subsurfaceResidueN)
  title_string <-
      c("Crop N outputs ",title_string_root) ##add crop seq number
  title_string <-
      paste(title_string, collapse = " ") ## concatenate
  maxN = max(CropN7.long$value)

  
      return(
          ggplot(
              CropN7.long, aes(
                  x = Identity, value, fill = inputs, order = -as.numeric(inputs)
              )
          ) + ##,colour="C fixed"))+
              geom_bar(stat = 'identity') + 
              ylab(label = "kg N/ha/yr") + ggtitle(title_string) +
              xlab("Crop no") + scale_fill_brewer(palette =
                                                      "Spectral")
      )
  

} 