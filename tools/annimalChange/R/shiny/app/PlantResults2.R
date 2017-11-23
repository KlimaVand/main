PlantResults2<-function(Crop1,obs)
{
    #filename = paste(fileroot,"plantfile.xls", sep = "")
    #Crop1<-read.delim("outputFarm117050BaselineScenarioNr5plantfile.xls", header = T, stringsAsFactors = FALSE)
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
    
    histogramList <- vector('list', NumSeq)
    i <-obs
    title_string_root <- c("crop seq ",as.character(SeqNumbers[i]))
    title_string <-
        c("Crop C outputs ",title_string_root) ##add crop seq number
    title_string <-
        paste(title_string, collapse = " ") ## concatenate
    Crop2<-filter(Crop1,CropSeq==SeqNumbers[i])##choose a single crop sequence
    Crop3<-select(Crop2,Identity,subsurfaceResidueC,surfaceResidueC,harvestedC,burntResidueC,residueCtoNextCrop)
    Crop4<-as.data.frame(lapply(Crop3, as.numeric))
    Crop5<-gather(Crop4,inputs,value,burntResidueC,residueCtoNextCrop,harvestedC,surfaceResidueC,subsurfaceResidueC)
    title_string<-c("Crop C outputs ",title_string_root) ##add crop seq number
    title_string<-paste(title_string, collapse=" ") ## concatenate
    return(
        ggplot(
            Crop5, aes(
                x = Identity, value, fill = inputs, order = -as.numeric(inputs)
            )
        ) + ##,colour="C fixed"))+
            geom_bar(stat = 'identity') + #ylim(0,maxC) +
            ylab(label = "kg C/ha/yr") + ggtitle(title_string) +
            xlab("Crop no")
    )
} 