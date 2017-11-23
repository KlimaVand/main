PlantResults3<-function(Crop1,obs)
{
   # filename = paste(fileroot,"plantfile.xls", sep = "")
    #Crop1 <-
     #   read.delim(filename, header = T, stringsAsFactors = FALSE)
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
    Crop2 <-
        filter(Crop1,CropSeq == SeqNumbers[i])##choose a single crop sequence
    Crop3 <-
        select(
            Crop2,Identity,surfaceResidueC,harvestedC,burntResidueC,residueCtoNextCrop
        )
    Crop4 <- as.data.frame(lapply(Crop3, as.numeric))
    Crop5 <-
        gather(
            Crop4,inputs,value,surfaceResidueC,harvestedC,burntResidueC,residueCtoNextCrop
        )
    title_string_root <- c("crop seq ",as.character(SeqNumbers[i]))
    title_string <-
        c("Crop C outputs ",title_string_root) ##add crop seq number
    title_string <-
        paste(title_string, collapse = " ") ## concatenate
    CropDM1 <-
        select(
            Crop2,Identity,harvestedDM,unutilisedGrazableDM,storageProcessingDMLoss
        )
    CropDM2 <-
        gather(
            CropDM1,inputs,value,unutilisedGrazableDM,storageProcessingDMLoss,harvestedDM
        )
    CropDM2$value <- as.numeric(CropDM2$value)
    CropDM2$Identity <- as.numeric(CropDM2$Identity)
    title_string <-
        c("DM budget ",title_string_root) ##add crop seq number
    title_string <-
        paste(title_string, collapse = " ") ## concatenate
    iteration <- i
    name = paste("plant",iteration)

        return(
            ggplot(
                CropDM2, aes(
                    x = Identity, value, fill = inputs, order = -as.numeric(inputs)
                )
            ) + ##,colour="C fixed"))+
                geom_bar(stat = 'identity')  +
                ylab(label = "kg DM/ha") + ggtitle(title_string) +
                xlab("Crop no")
        )
} 