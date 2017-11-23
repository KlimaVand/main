NdataResults5<-function(Crop1,obs)
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
    title_string_root <- c("crop seq ",as.character(SeqNumbers[i]))
    Crop2 <-filter(Crop1,CropSeq == SeqNumbers[i])##choose a single crop sequence

    CropN11 <-select(Crop2,Identity,deltaSoilN,manureNH3emission,fertiliserNH3emission,urineNH3emission,harvestedN,storageProcessingNLoss,residueNtoNextCrop,manureN2OEmission,soilN2OEmission,fertiliserN2OEmission,cropResidueN2O,burningN2ON,N2Nemission,burningNH3N,burningNOxN,burningOtherN,N2ONemission,nitrateLeaching,mineralNToNextCrop)
    CropN11 <- as.data.frame(lapply(CropN11, as.numeric))
    CropN11$BurningN <-(CropN11$burningN2ON + CropN11$burningNH3N + CropN11$burningNOxN + CropN11$burningOtherN)
    for (j in 1:nrow(CropN11))
    {
      if (CropN11[j,"deltaSoilN"]<0)
        CropN11[j,"deltaSoilN"]=CropN11[j,"deltaSoilN"] * -1
      else
        CropN11[j,"deltaSoilN"]=0
    }
    CropN11.long <-gather(CropN11,inputs,value,deltaSoilN,manureNH3emission,fertiliserNH3emission,urineNH3emission,storageProcessingNLoss,N2Nemission,N2ONemission,nitrateLeaching,BurningN)
    title_string <-c("N losses ",title_string_root) ##add crop seq number
    title_string <-paste(title_string, collapse = " ") ## concatenate
    
    return(ggplot(CropN11.long, aes(x = Identity, value, fill = inputs, order = -as.numeric(inputs))) + ##,colour="C fixed"))+
                geom_bar(stat = 'identity') + 
                ylab(label = "kg N/ha/yr") + ggtitle(title_string) +
                xlab("Crop no") + scale_fill_brewer(palette =
                                                        "Spectral")
        )
    

} 