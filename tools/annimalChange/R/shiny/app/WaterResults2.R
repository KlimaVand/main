WaterResults2<-function(Water1,obs)
{
   Sequences<-select(Water1,CropSeq)
   Sequences<-as.numeric(as.character(Sequences$CropSeq))
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
   Water2<-filter(Water1,CropSeq==SeqNumbers[i])
   StartDay<-as.numeric(Water2$day[1])
   Water2$day<-(Water2$day - StartDay + 1)
   Water6<-select(Water2,day,plantwaterInSoil,waterInSoil)
   Water7<-gather(Water6,inputs,value,plantwaterInSoil,waterInSoil)
   title_string_root<-c("crop seq ",as.character(SeqNumbers[i]))
   title_string<-c("water in ",title_string_root) ##add crop seq number
   title_string<-paste(title_string, collapse=" ") ## concatenate
   
   histogramList[[i]] <-
     ggplot(Water7, aes(x=day, value, fill=inputs,colour=inputs))+
     geom_line() + geom_point() + ggtitle(title_string) +
     ylab(label="mm") + 
     xlab("Day")  
  
  return(histogramList[obs]);
}