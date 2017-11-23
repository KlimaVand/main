#Reads FarmAC output files (CTool, plant, farm, debug)
#outputs figures for crop and soil processes, water processes and farm C and N budgets

#####load libraries ######
#+ echo=FALSE
library(tidyr)
library(dplyr)
library(ggplot2)
library(readxl)
library(xlsx)
####### read CTool data ##########
library(reshape2)
##First read in the arguments listed at the command line

args=(commandArgs(TRUE))

##args is now a list of character vectors
## First check to see if arguments are passed.
if(length(args)==0){
  print("No arguments supplied.")
  ##supply default values
  #setwd("c:/workspace/tools/annimalChange/scenarios/Senegal/Output")
  #setwd("c:/Users/njh/Dropbox/AnimalChangeCP3 (1)/FarmAC/S-EU-008-Maritime_Dairy_Torr/Baseline/")
  setwd("c:/workspace/tools/annimalChange/scenarios/Ireland/Output")
  #fileroot="outputFarm325148ScenarioNr1"
  #fileroot = "http://gefionau.dk/animalchange/upload/AnimalChange/modeloutput/%20outputFarm117050ScenarioNr5"
  fileroot="outputFarm116540ScenarioNr12"
}else  {
  setwd(args[1])
  fileroot=args[2]
}
stub="CtoolFile.xls"
filename =paste(fileroot,stub, sep='')
CTool<-read.delim(filename, header = T, stringsAsFactors = FALSE)
CTool1<-CTool[-c(1),] ##remove second header line
CTool2<-mutate(CTool1,year=as.numeric(startDay)/365)
Sequences<-select(CTool2,CropSeqID)
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
i=1
while (SeqNumbers[i]!=0)
{
  CTool3<-filter(CTool2,CropSeqID==SeqNumbers[i])
  CTool4<-select(CTool3,FOMCStoredStart,HUMCStoredStart,ROMCStoredStart,year)
  CTool5<-melt(CTool4,id="year")
  CTool5$value<-as.numeric(CTool5$value)
  maxC<-max(CTool5$value)
  title_string_root<-c("crop seq ",as.character(SeqNumbers[i]))
  title_string<-c("Soil C pools in ",title_string_root) ##add crop seq number
  title_string<-paste(title_string, collapse=" ") ## concatenate
  print(ggplot(CTool5, aes(x=year, y=value,colour=variable))+
  geom_line() + geom_point() + ggtitle(title_string) + ylim(0,maxC)+
   ylab(label="kg C/ha") + 
   xlab("Year"))
  #ggsave("plot.pdf")
  
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
  title_string <-c("Soil N pools in ",title_string_root) ##add crop seq number
  title_string <-
    paste(title_string, collapse = " ") ## concatenate
  
  print(ggplot(CTool9, aes(x = year, y = value,colour = variable)) +
    geom_line() + geom_point() + ggtitle(title_string) + #ylim(0,maxN) +
    ylab(label = "kg N/ha") +
    xlab("Year"))
  i=i+1
}
####### Read crop file ##########
filename =paste(fileroot,"plantfile.xls", sep="")
Crop1<-read.delim(filename, header = T, stringsAsFactors = FALSE)
Crop1<-Crop1[-c(1),] ##remove second header line
Sequences<-select(Crop1,CropSeq)####copy crop sequence number to separate dataframe
Sequences<-as.numeric(as.character(Sequences$CropSeq))##convert to numeric
CFIX<-select(Crop1,CropSeq,Identity,CFixed)
CFIX$CFixed<-as.numeric(CFIX$CFixed)
CFIX$Identity<-as.numeric(CFIX$Identity)
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
i=1
while (SeqNumbers[i]!=0)
{
  title_string_root<-c("crop seq ",as.character(SeqNumbers[i]))
  title_string<-c("C fixation in ",title_string_root) ##add crop seq number
  title_string<-paste(title_string, collapse=" ") ## concatenate
  CFixSeq<-filter(CFIX,CropSeq==SeqNumbers[i])##choose a single crop sequence
  maxC<-max(as.numeric(CFIX$CFixed))
  print(ggplot(CFixSeq, aes(x=Identity, y=CFixed))+##,colour="C fixed"))+
          geom_bar(stat='identity',position="dodge") +
          ylab(label="kg C/ha/yr") + ggtitle(title_string)+ylim(0,maxC) +
          xlab("Crop no"))
  
  Crop2<-filter(Crop1,CropSeq==SeqNumbers[i])##choose a single crop sequence
  Crop3<-select(Crop2,Identity,subsurfaceResidueC,surfaceResidueC,harvestedC,burntResidueC,residueCtoNextCrop)
  Crop4<-as.data.frame(lapply(Crop3, as.numeric))
  Crop5<-gather(Crop4,inputs,value,burntResidueC,residueCtoNextCrop,harvestedC,surfaceResidueC,subsurfaceResidueC)
  title_string<-c("Crop C outputs ",title_string_root) ##add crop seq number
  title_string<-paste(title_string, collapse=" ") ## concatenate
  print(ggplot(Crop5, aes(x=Identity, value, fill=inputs, order = -as.numeric(inputs)))+##,colour="C fixed"))+
          geom_bar(stat='identity') + ylim(0,maxC) +
          ylab(label="kg C/ha/yr") + ggtitle(title_string)+
          xlab("Crop no") + scale_fill_brewer(palette="Spectral"))
  
  Crop6<-select(Crop2,Identity,CFixed,urineCCropClass,manureFOMCsurface,faecalCCropClass,fertiliserC)
  Crop7<-gather(Crop6,inputs,value,CFixed,urineCCropClass,manureFOMCsurface,faecalCCropClass,fertiliserC)
  Crop7$value<-as.numeric(Crop7$value)
  Crop7$Identity<-as.numeric(Crop7$Identity)
  title_string<-c("Seq C inputs ",title_string_root) ##add crop seq number
  title_string<-paste(title_string, collapse=" ") ## concatenate
  print(ggplot(Crop7, aes(x=Identity, value, fill=inputs, order = -as.numeric(inputs)))+##,colour="C fixed"))+
          geom_bar(stat='Identity') + #ylim(0,maxC) +
          ylab(label="kg C/ha/yr") + ggtitle(title_string)+
          xlab("Crop no"))
  
  Crop8<-select(Crop2,Identity,harvestedC,soilCO2_CEmission,urineCCropClass,fertiliserC,burntResidueC)
  Crop9<-gather(Crop8,inputs,value,harvestedC,soilCO2_CEmission, urineCCropClass,fertiliserC,burntResidueC)
  Crop9$value<-as.numeric(Crop9$value)
  Crop9$Identity<-as.numeric(Crop9$Identity)
  title_string<-c("Seq C outputs ",title_string_root) ##add crop seq number
  title_string<-paste(title_string, collapse=" ") ## concatenate
  print(ggplot(Crop9, aes(x=Identity, value, fill=inputs, order = -as.numeric(inputs)))+##,colour="C fixed"))+
          geom_bar(stat='Identity') + #ylim(0,maxC) +
          ylab(label="kg C/ha/yr") + ggtitle(title_string)+
          xlab("Crop no"))
  
  CropDM1<-select(Crop2,Identity,harvestedDM,unutilisedGrazableDM,storageProcessingDMLoss)
  CropDM2<-gather(CropDM1,inputs,value,unutilisedGrazableDM,storageProcessingDMLoss,harvestedDM)
  CropDM2$value<-as.numeric(CropDM2$value)
  CropDM2$Identity<-as.numeric(CropDM2$Identity)
  title_string<-c("DM budget ",title_string_root) ##add crop seq number
  title_string<-paste(title_string, collapse=" ") ## concatenate
  print(ggplot(CropDM2, aes(x=Identity, value, fill=inputs, order = -as.numeric(inputs)))+##,colour="C fixed"))+
          geom_bar(stat='identity')  +
          ylab(label="kg DM/ha") + ggtitle(title_string)+
          xlab("Crop no"))
  i=i+1
}
######### N data ###############
filename =paste(fileroot,"plantfile.xls", sep="")
Crop1<-read.delim(filename, header = T, stringsAsFactors = FALSE)
Crop1<-Crop1[-c(1),] ##remove second header line
Sequences<-select(Crop1,CropSeq)####copy crop sequence number to separate dataframe
Sequences<-as.numeric(as.character(Sequences$CropSeq))##convert to numeric
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
i=1
while (SeqNumbers[i]!=0)
{
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
  
  print(ggplot(CropN5.long, aes(x=Identity, value, fill=inputs, order = -as.numeric(inputs)))+##,colour="C fixed"))+
           geom_bar(stat='identity')  +
           ylab(label="kg N/ha/yr") + ggtitle(title_string)+
           xlab("Crop no"))

  CropN6<-select(Crop2,Identity,harvestedN,surfaceResidueN,subsurfaceResidueN,residueNtoNextCrop)
  CropN7<-as.data.frame(lapply(CropN6, as.numeric))
  CropN7.long<-gather(CropN7,inputs,value,residueNtoNextCrop,harvestedN,surfaceResidueN,subsurfaceResidueN)
  title_string<-c("Crop N outputs ",title_string_root) ##add crop seq number
  title_string<-paste(title_string, collapse=" ") ## concatenate
  print(ggplot(CropN7.long, aes(x=Identity, value, fill=inputs, order = -as.numeric(inputs)))+##,colour="C fixed"))+
          geom_bar(stat='identity') +
          ylab(label="kg N/ha/yr") + ggtitle(title_string)+
          xlab("Crop no") + scale_fill_brewer(palette="Spectral"))


  CropN8 <-select(Crop2,Identity,deltaSoilN,Nfixed,nAtm,fertiliserNinput,totalManureNApplied,mineralNFromLastCrop,residueNfromLastCrop, excretaNInput)
  CropN8 <- as.data.frame(lapply(CropN8, as.numeric))
  CropN9<-as.data.frame(CropN8)
  for (j in 1:nrow(CropN8))
  {
    if (CropN9[j,"deltaSoilN"]<0)
      CropN9[j,"deltaSoilN"]=0
  }
  CropN9.long <-gather(CropN9,inputs,value,deltaSoilN,Nfixed,nAtm,fertiliserNinput,totalManureNApplied,mineralNFromLastCrop,residueNfromLastCrop, excretaNInput)
  title_string <-c("Crop sequence N inputs ",title_string_root) ##add crop seq number
  title_string <-paste(title_string, collapse = " ") ## concatenate
  
  print(ggplot(CropN9.long, aes(x=Identity, value, fill=inputs, order = -as.numeric(inputs)))+##,colour="C fixed"))+
          geom_bar(stat='identity') +
          ylab(label="kg N/ha/yr") + ggtitle(title_string)+
          xlab("Crop no") + scale_fill_brewer(palette="Spectral"))
  
  CropN10 <-select(Crop2,Identity,harvestedN,residueNtoNextCrop,mineralNToNextCrop)
  CropN10 <- as.data.frame(lapply(CropN10, as.numeric))
  CropN10.long <-gather(CropN10,inputs,value,harvestedN,residueNtoNextCrop,mineralNToNextCrop)
  title_string <-c("Crop sequence N outputs ",title_string_root) ##add crop seq number
  title_string <-paste(title_string, collapse = " ") ## concatenate
  
  print(ggplot(CropN10.long, aes(x=Identity, value, fill=inputs, order = -as.numeric(inputs)))+##,colour="C fixed"))+
           geom_bar(stat='identity') +
           ylab(label="kg N/ha/yr") + ggtitle(title_string)+
           xlab("Crop no") + scale_fill_brewer(palette="Spectral"))
  
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
  
  print(ggplot(CropN11.long, aes(x=Identity, value, fill=inputs, order = -as.numeric(inputs)))+##,colour="C fixed"))+
          geom_bar(stat='identity') +
          ylab(label="kg N/ha/yr") + ggtitle(title_string)+
          xlab("Crop no") + scale_fill_brewer(palette="Spectral"))
  i=i+1
}
  ####### crop sequence #######
if (file.exists("c:/Home/Projects/Animal change/Publications/FarmAC/FarmAC/seq.xlsx"))
  file.remove("c:/Home/Projects/Animal change/Publications/FarmAC/FarmAC/seq.xlsx")
  filename =paste(fileroot,"plantfile.xls", sep="")
  thesheetName="SeqN" #<-paste(title_string_root, collapse=" ") ## concatenate
  Crop1<-read.delim(filename, header = T, stringsAsFactors = FALSE)
  Crop1<-Crop1[-c(1),] ##remove second header line
  Sequences<-select(Crop1,CropSeq)####copy crop sequence number to separate dataframe
  Sequences<-as.numeric(as.character(Sequences$CropSeq))##convert to numeric
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
  i=1
  while (SeqNumbers[i]!=0)
  {
    title_string_root<-c("crop seq ",as.character(SeqNumbers[i]))
    title_string<-c("C fixation in ",title_string_root) ##add crop seq number
    title_string<-paste(title_string, collapse=" ") ## concatenate
    Crop2<-filter(Crop1,CropSeq==SeqNumbers[i])##choose a single crop sequence
    CropSeq1<-select(Crop2,Identity,area,duration,deltaSoilN,soilNMineralisation,Nfixed,nAtm,fertiliserNinput,totalManureNApplied,urineNCropClass,faecalNCropClass,mineralNFromLastCrop, +
        manureNH3emission,fertiliserNH3emission,urineNH3emission,harvestedN,grazedN,storageProcessingNLoss,residueNtoNextCrop,manureN2OEmission,soilN2OEmission,+
          fertiliserN2OEmission,cropResidueN2O,burningN2ON,N2Nemission,burningNH3N,burningNOxN,burningOtherN,N2ONemission,nitrateLeaching,mineralNToNextCrop)
    CropSeq2<-as.data.frame(lapply(CropSeq1, as.numeric))
    years<-(sum(CropSeq2$duration))/365
    if (i==0)
    {
      CropSeq2$grazedN<-0
      CropSeq2$BurningN<-0
    }
    CropSeq2$BurningN<-(CropSeq2$burningN2ON+CropSeq2$burningNH3N+CropSeq2$burningNOxN+CropSeq2$burningOtherN)
    CropSeq2$nonGrazedHarvest<-(CropSeq2$harvestedN-CropSeq2$grazedN)
    CropSeq2$ExcretaN<-CropSeq2$urineNCropClass+CropSeq2$faecalNCropClass
    CropSeq2$DenitifN<-CropSeq2$soilN2OEmission+CropSeq2$N2Nemission
    CropSeq2<-CropSeq2*CropSeq2$area  #multiply values by area
    CropSeq4<-data.frame(colSums(CropSeq2)/years)
    columnName<-as.character(i)
    if (i==1)
      CropSeq5<-data.frame(CropSeq4)
    else
      CropSeq5<-cbind(CropSeq5,CropSeq4)
    colnames(CropSeq5)[i]=columnName
    i=i+1
  }
  write.xlsx(CropSeq5,"c:/Home/Projects/Animal change/Publications/FarmAC/FarmAC/seq.xlsx",sheetName=thesheetName)
  
####### livestock #######
  filename =paste(fileroot,"livetockfile.xls", sep="")
  Live1<-read.delim(filename, header = T, stringsAsFactors = FALSE)
  Live1<-Live1[-c(1),] ##remove second header line
  Live2<-select(Live1,avgNumberOfAnimal,Nintake:NexcretionToPasture)
  Live3<-as.data.frame(lapply(Live2, as.numeric))
  Live3$nonGrazedIntakeN<-(Live3$Nintake-Live3$grazedN)
  Live4<-Live3*Live3$avgNumberOfAnimal
  Live4$avgNumberOfAnimal<-NULL
  Live5<-data.frame(colSums(Live4))
  write.xlsx(Live5,"c:/Home/Projects/Animal change/Publications/FarmAC/FarmAC/seq.xlsx",sheetName="LiveN", append=TRUE)
  
  ##### farm N ######
  filename =paste(fileroot,".xls", sep="")
  Farm1<-read.delim(filename, header = F, stringsAsFactors = FALSE)
  dummy=c(1)
  Farm2=data.frame(dummy)
  Farm2$dummy<-NULL
  
  Farm2$ImportedManure<-Farm1[97,8]
  Farm2$Nfixation<-Farm1[98,8]
  Farm2$Natm<-Farm1[99,8]
  Farm2$Fertiliser<-Farm1[100,8]
  Farm2$ImportedBedding<-Farm1[101,8]
  Farm2$FeedImport<-Farm1[102,8]
  Farm2$ProcessLoss<-Farm1[103,8]
  Farm2$ExportedCrop<-Farm1[104,8]
  Farm2$ExportedMilk<-Farm1[105,8]
  Farm2$ExportedMeat<-Farm1[106,8]
  Farm2$Mortalities<-Farm1[107,8]
  Farm2$ExportedManure<-Farm1[108,8]
  Farm2$NH3Housing<-Farm1[110,8]
  Farm2$N2Storage<-Farm1[111,8]
  Farm2$N2OStorage<-Farm1[112,8]
  Farm2$NH3Storage<-Farm1[113,8]
  Farm2$NH3Urine<-Farm1[114,8]
  Farm2$Runoff<-Farm1[115,8]
  Farm2$N2Field<-Farm1[116,8]
  Farm2$N2OField<-Farm1[117,8]
  Farm2$NH3Fert<-Farm1[118,8]
  Farm2$NH3ManureField<-Farm1[119,8]
  Farm2$Leaching<-Farm1[120,8]
  Farm2$BurningN2O<-Farm1[122,8]
  Farm2$BurningNH3<-Farm1[123,8]
  Farm2$BurningNOx<-Farm1[124,8]
  Farm2$BurningNOther<-Farm1[125,8]
  Farm2$DeltaSoil<-Farm1[126,8]
  Farm2$GrazedN<-Farm1[25,8]
  Farm2$ExcretaHousing<-Farm1[26,8]
  Farm2$ExcretaField<-Farm1[27,8]
  Farm2$SentToStorage<-Farm1[35,8]
  Farm2$WasteFeed<-Farm1[39,8]
  Farm2$ManureExStore<-Farm1[42,8]
  Farm2$Harvested<-Farm1[50,8]
  Farm2$FeedInHousing<-Farm1[24,8]
  Farm3<-as.data.frame(lapply(Farm2, as.numeric))
  Farm3$BurningNTot<-(Farm3$BurningN2O+Farm3$BurningNH3+Farm3$BurningNOx + Farm3$BurningNOther)
  Farm3$SoilNReduction<-0
  Farm3$SoilNSequestration<-0
  Farm3$HomeGrownNonGrazed<-0
  if (Farm3$DeltaSoil<0)  {
    Farm3$SoilNReduction<-(Farm3$DeltaSoil * -1)
  } else {
      Farm3$SoilNSequestration<-(Farm3$DeltaSoil)
  }
  Farm3$HomeGrownNonGrazed<-(Farm3$FeedInHousing-Farm3$FeedImport)
  Farm4<-data.frame(colSums(Farm3))
  colnames(Farm4)[1]<-"FarmTotals"
  Farm4$FarmPerHa<-0
  TotalFarmArea<-as.numeric(Farm1[213,8])
  Farm4$FarmPerHa<-(Farm4$FarmTotals/TotalFarmArea)
  write.xlsx(Farm4,"c:/Home/Projects/Animal change/Publications/FarmAC/FarmAC/seq.xlsx",sheetName="FarmN", append=TRUE)
  
  ###### Farm C ################
  filename =paste(fileroot,".xls", sep="")
  Farm1<-read.delim(filename, header = F, stringsAsFactors = FALSE)
  dummy=c(1)
  Farm5=data.frame(dummy)
  Farm5$dummy<-NULL
  
  Farm5$Cfixation<-Farm1[55,8]
  Farm5$ImportedManure<-Farm1[56,8]
  Farm5$FeedImport<-Farm1[57,8]
  Farm5$ImportedBedding<-Farm1[58,8]
  Farm5$ExportedMilk<-Farm1[59,8]
  Farm5$ExportedMeat<-Farm1[60,8]
  Farm5$Mortalities<-Farm1[61,8]
  Farm5$ExportedCrop<-Farm1[62,8]
  Farm5$ExportedManure<-Farm1[63,8]
  Farm5$LivestockCH4<-Farm1[64,8]
  Farm5$LivestockCO2<-Farm1[65,8]
  Farm5$HousingCO2<-Farm1[66,8]
  Farm5$ManureCH4<-Farm1[67,8]
  Farm5$ManureCO2<-Farm1[68,8]
  Farm5$BiogasCH4<-Farm1[69,8]
  Farm5$BiogasCO2<-Farm1[70,8]
  Farm5$ProcessLoss<-Farm1[71,8]
  Farm5$FieldCO2<-Farm1[72,8]
  Farm5$BurningCO<-Farm1[74,8]
  Farm5$BurningCO2<-Farm1[75,8]
  Farm5$BurningBC<-Farm1[76,8]
  Farm5$DeltaSoilC<-Farm1[77,8]
  
  Farm6<-as.data.frame(lapply(Farm5, as.numeric))
  Farm6$BurningCTot<-(Farm6$BurningCO+Farm6$BurningCO2+Farm6$BurningBC)
  Farm6$SoilCReduction<-0
  Farm6$SoilCSequestration<-0
  if (Farm6$DeltaSoilC<0)  {
    Farm6$SoilCReduction<-(Farm6$DeltaSoilC * -1)
  } else {
    Farm6$SoilCSequestration<-(Farm6$DeltaSoilC)
  }
  Farm7<-data.frame(colSums(Farm6))
  colnames(Farm7)[1]<-"FarmTotals"
  Farm7$FarmPerHa<-0
  TotalFarmArea<-as.numeric(Farm1[213,8])
  Farm7$FarmPerHa<-(Farm7$FarmTotals/TotalFarmArea)
  write.xlsx(Farm7,"c:/Home/Projects/Animal change/Publications/FarmAC/FarmAC/seq.xlsx",sheetName="FarmC", append=TRUE)
  
  ####### water flows ######
  filename =paste(fileroot,"Debug.xls", sep="")
  Water1<-read.delim(filename, header = T, stringsAsFactors = FALSE)
  Sequences<-select(Water1,CropSeq)
  Sequences<-as.numeric(as.character(Sequences$CropSeq))
  NumSeq<-max(Sequences)
  for (i in 1:NumSeq)
  {
    Water2<-filter(Water1,CropSeq==i)
    StartDay<-as.numeric(Water2$day[1])
    Water2$day<-(Water2$day - StartDay + 1)
    Water3<-select(Water2,day,precip,irrigation,evap,transpire,drainage)
    Water4<-gather(Water3,inputs,value,precip,irrigation)
    title_string_root<-c("crop seq ",as.character(i))
    title_string<-c("water flows into ",title_string_root) ##add crop seq number
    title_string<-paste(title_string, collapse=" ") ## concatenate
    print(ggplot(Water4, aes(x=day, value, fill=inputs,colour=inputs))+
            geom_line() + geom_point() + ggtitle(title_string) +
            ylab(label="mm") + 
            xlab("Day"))
    Water5<-gather(Water3,inputs,value,evap,transpire,drainage)
    title_string_root<-c("crop seq ",as.character(i))
    title_string<-c("water flows out of ",title_string_root) ##add crop seq number
    title_string<-paste(title_string, collapse=" ") ## concatenate
#    print(ggplot(Water5, aes(x=day, value, fill=inputs))+
#            geom_bar(stat='identity')  +
    print(ggplot(Water5, aes(x=day, value, fill=inputs,colour=inputs))+
            geom_line() + geom_point() + ggtitle(title_string) +
            ylab(label="mm") + 
            xlab("Day"))
  
    Water6<-select(Water2,day,plantwaterInSoil,waterInSoil)
    Water7<-gather(Water6,inputs,value,plantwaterInSoil,waterInSoil)
    title_string_root<-c("crop seq ",as.character(i))
    title_string<-c("water in ",title_string_root) ##add crop seq number
    title_string<-paste(title_string, collapse=" ") ## concatenate
    print(ggplot(Water7, aes(x=day, value, fill=inputs,colour=inputs))+
            geom_line() + geom_point() + ggtitle(title_string) +
            ylab(label="mm") + 
            xlab("Day"))
  
    Water8<-select(Water2,day,droughtFactorPlant,droughtFactorSoil)
    Water9<-gather(Water8,inputs,value,droughtFactorPlant,droughtFactorSoil)
    title_string_root<-c("crop seq ",as.character(i))
    title_string<-c("Drought factors in ",title_string_root) ##add crop seq number
    title_string<-paste(title_string, collapse=" ") ## concatenate
    print(ggplot(Water9, aes(x=day, value, fill=inputs,colour=inputs))+
            geom_line() + geom_point() + ggtitle(title_string) + ylim(0,1) +
            ylab(label="") + 
            xlab("Day"))

    Water10<-select(Water2,day,LAI)
    Water11<-gather(Water10,inputs,value,LAI)
    title_string_root<-c("crop seq ",as.character(i))
    title_string<-c("LAI in ",title_string_root) ##add crop seq number
    title_string<-paste(title_string, collapse=" ") ## concatenate
    print(ggplot(Water11, aes(x=day, value, fill=inputs,colour=inputs))+
            geom_line() + geom_point() + ggtitle(title_string) +
            ylab(label="LAI") + 
            xlab("Day"))
  }
  