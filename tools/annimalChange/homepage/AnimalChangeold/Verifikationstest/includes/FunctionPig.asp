<%
'**************************************************************************************************
' Author: Poul Lassen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
function CalcPigGrowthNitrogen(intType, fltProductionVar1, fltProductionVar2, fltProductionVar3, fltNitrogenContentLiveWeight, fltNitrogenContentInput)
   dim returnValue
   
   select case CInt(intType)
      case 1 'Søer
               returnValue = fltProductionVar3*fltNitrogenContentLiveWeight + fltProductionVar1*fltProductionVar2*fltNitrogenContentInput           
      case 2,3 'slagtesvin og smågrise
               returnValue = fltProductionVar2*fltNitrogenContentLiveWeight - fltProductionVar1*fltNitrogenContentInput         
      case else
         returnValue = 0
   end select
   CalcPigGrowthNitrogen = returnValue
end function
'**************************************************************************************************
function CalcSowGrowthNitrogen(intType, fltProductionVar3, fltNitrogenContentLiveWeight)
   dim returnValue
   
   select case CInt(intType)
      case 1 'Søer
               returnValue = fltProductionVar3*fltNitrogenContentLiveWeight          
      case else
         returnValue = 0
   end select
   CalcSowGrowthNitrogen = returnValue
end function
'**************************************************************************************************
function CalcPigGrowthNitrogenIn(intType, fltProductionVar1, fltProductionVar2, fltProductionVar3, fltNitrogenContentLiveWeight, fltNitrogenContentInput)
   dim returnValue
   
   select case CInt(intType)
      case 1 'Søer
               returnValue = 0        
      case 2,3 'slagtesvin og smågrise
               returnValue = fltProductionVar1*fltNitrogenContentInput         
      case else
         returnValue = 0
   end select
   CalcPigGrowthNitrogenIn = returnValue
end function
'**************************************************************************************************
function CalcPigGrowthNitrogenOut(intType, fltProductionVar1, fltProductionVar2, fltProductionVar3, fltNitrogenContentLiveWeight, fltNitrogenContentInput)
   dim returnValue
   
   select case CInt(intType)
      case 1 'Søer
               returnValue = fltProductionVar1*fltProductionVar2*fltNitrogenContentInput        
      case 2,3 'slagtesvin og smågrise
               returnValue = fltProductionVar2*fltNitrogenContentLiveWeight         
      case else
         returnValue = 0
   end select
   CalcPigGrowthNitrogenOut = returnValue
end function
'**************************************************************************************************
sub CalcPigHerdTotal()
   dim currentType, currentAnimalYear, currentProductionVar1, currentProductionVar2, currentProductionVar3
   Dim currentProteinLevel, currentFeedLevel, currentNitrogenContentLiveWeight, currentNitrogenContentInput
   Dim currentStable1, currentStable2, currentNitrogenDigestability, dictPigHerd

   set dictPigHerd = CreateObject("Scripting.Dictionary")
   nPigHerd = CallSP ("GetPigHerdList " & aFarmNumber& "," & aScenarioID , arrPigHerd, dictPigHerd)
'   nPigHerd = GetPigHerdList(aFarmNumber,aScenarioID,arrPigHerd)

    Dim arrParameter
    redim arrParameter(1)

	 arrParameter(0)= aFarmNumber
	 arrParameter(1) = aScenarioID
     returnValue = CallSPReturn ("DeletePigHerdTotal",arrParameter)
     Response.write "<br> returnValue " & returnValue
'   DeletePigHerdTotal aFarmNumber,aScenarioID   
   redim preserve arrParameter(2)
	 arrParameter(2) = 4
     returnValue = CallSPReturn ("DeleteStable",arrParameter)
     returnValue = CallSPReturn ("DeleteStorage",arrParameter)
     returnValue = CallSPReturn ("DeleteStorageNorm",arrParameter)
'   DeleteStable          aFarmNumber,aScenarioID,4
'   DeleteStorage         aFarmNumber,aScenarioID,4
'   DeleteStorageNorm aFarmNumber,aScenarioID,4

   for h = 0 to nPigHerd-1
      currentType            = arrPigHerd(h,6)
      currentAnimalYear      = arrPigHerd(h,3)
      currentProductionVar1  = arrPigHerd(h,12)
      currentProductionVar2  = arrPigHerd(h,13)
      currentProductionVar3  = arrPigHerd(h,14)
      currentProteinLevel    = arrPigHerd(h,4)
      currentFeedLevel       = arrPigHerd(h,5)
      currentNitrogenContentLiveWeight  = arrPigHerd(h,10)
      currentNitrogenContentInput       = arrPigHerd(h,11)
      currentNitrogenDigestability      = arrPigHerd(h,23)
      currentStable1         = arrPigHerd(h,15)
      currentStable2         = arrPigHerd(h,16)
    
      CalcFeedEnergyP currentType,currentAnimalYear,currentFeedLevel
      CalcFeedNitrogenP currentType,currentAnimalYear,currentFeedLevel, currentProteinLevel
      CalcGrowthNitrogenP currentType, currentAnimalYear, currentProductionVar1, currentProductionVar2, currentProductionVar3, currentNitrogenContentLiveWeight, currentNitrogenContentInput  

      CalcManureNitrogenP currentType,currentAnimalYear

      CalcManureNitrogenFaecesUrineP currentType,currentAnimalYear,currentNitrogenDigestability

      CalcManureStableStorageP currentType, currentAnimalYear, currentStable1,currentStable2
   next
end sub
'**************************************************************************************************
sub CalcFeedEnergyP(aType,aAnimalYear,aFeedLevel)
   feedEnergyTotal = aAnimalYear*aFeedLevel
    Dim arrParameter
    redim arrParameter(4)

	 arrParameter(0)= aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aType
	 arrParameter(3) = 402
	 arrParameter(4) = CSng(feedEnergyTotal)
     returnValue = CallSPReturn ("IncrementPigHerdTotal",arrParameter)
'  IncrementPigHerdTotal aFarmNumber, aScenarioID, aType, 402, CSng(feedEnergyTotal)
end sub
'**************************************************************************************************
sub CalcFeedNitrogenP(aType,aAnimalYear,aFeedLevel,aProteinLevel)
   feedNitrogenAnimalYear = aFeedLevel*aProteinLevel/6250
'   Response.write "<br> feedNitrogenAnimalYear " & feedNitrogenAnimalYear
   feedNitrogenAnimalYearNorm = arrPigHerd(h,8)*arrPigHerd(h,9)/6250
'   Response.write "<br> feedNitrogenAnimalYearNorm før korrigering for faktuel tilvækst " & feedNitrogenAnimalYearNorm
   feedNitrogenTotal = aAnimalYear*feedNitrogenAnimalYear
    Dim arrParameter
    redim arrParameter(4)

	 arrParameter(0)= aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aType
	 arrParameter(3) = 404
	 arrParameter(4) = CSng(feedNitrogenTotal)
     returnValue = CallSPReturn ("IncrementPigHerdTotal",arrParameter)
'   IncrementPigHerdTotal aFarmNumber, aScenarioID, aType, 404, CSng(feedNitrogenTotal)
end sub
'**************************************************************************************************
sub CalcGrowthNitrogenP( aType, aAnimalYear, aProductionVar1, aProductionVar2, aProductionVar3, aNitrogenContentLiveWeight, aNitrogenContentInput)
   GrowthNitrogenType = CalcPigGrowthNitrogen(aType, aProductionVar1, aProductionVar2, aProductionVar3, aNitrogenContentLiveWeight, aNitrogenContentInput)
   GrowthNitrogenTypeNorm = CalcPigGrowthNitrogen(aType, arrPigHerd(h,30), arrPigHerd(h,31), arrPigHerd(h,32), aNitrogenContentLiveWeight, aNitrogenContentInput)
   SowGrowthNitrogen = CalcSowGrowthNitrogen(aType, aProductionVar3, aNitrogenContentLiveWeight)
   GrowthNitrogenIn = CalcPigGrowthNitrogenIn(aType, aProductionVar1, aProductionVar2, aProductionVar3, aNitrogenContentLiveWeight, aNitrogenContentInput)
   GrowthNitrogenOut = CalcPigGrowthNitrogenOut(aType, aProductionVar1, aProductionVar2, aProductionVar3, aNitrogenContentLiveWeight, aNitrogenContentInput)
   feedNitrogenAnimalYearNorm = feedNitrogenAnimalYearNorm * GrowthNitrogenType / GrowthNitrogenTypeNorm
'   Response.write "<br> feedNitrogenAnimalYearNorm " & feedNitrogenAnimalYearNorm


   GrowthNitrogenTotal = aAnimalYear*GrowthNitrogenType
   SowGrowthNitrogenTotal = aAnimalYear*SowGrowthNitrogen
   GrowthNitrogenInTotal = aAnimalYear*GrowthNitrogenIn
   GrowthNitrogenOutTotal = aAnimalYear*GrowthNitrogenOut

   Dim arrParameter
    redim arrParameter(4)

	 arrParameter(0)= aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aType
	 arrParameter(3) = 407
	 arrParameter(4) = CSng(GrowthNitrogenTotal)
     returnValue = CallSPReturn ("IncrementPigHerdTotal",arrParameter)
'   IncrementPigHerdTotal aFarmNumber, aScenarioID, aType, 407, CSng(GrowthNitrogenTotal)
 	 arrParameter(3) = 413
	 arrParameter(4) = CSng(SowGrowthNitrogenTotal)
     returnValue = CallSPReturn ("IncrementPigHerdTotal",arrParameter)
'  IncrementPigHerdTotal aFarmNumber, aScenarioID, aType, 413, CSng(SowGrowthNitrogenTotal)
 	 arrParameter(3) = 414
	 arrParameter(4) = CSng(GrowthNitrogenInTotal)
     returnValue = CallSPReturn ("IncrementPigHerdTotal",arrParameter)
'   IncrementPigHerdTotal aFarmNumber, aScenarioID, aType, 414, CSng(GrowthNitrogenInTotal)
 	 arrParameter(3) = 415
	 arrParameter(4) = CSng(GrowthNitrogenOutTotal)
     returnValue = CallSPReturn ("IncrementPigHerdTotal",arrParameter)
'   IncrementPigHerdTotal aFarmNumber, aScenarioID, aType, 415, CSng(GrowthNitrogenOutTotal)

end sub
'**************************************************************************************************
sub CalcManureNitrogenP(aType, aAnimalYear)
   manureNitrogenAnimalYear = feedNitrogenAnimalYear - GrowthNitrogenType
   manureNitrogenAnimalYearNorm = feedNitrogenAnimalYearNorm - GrowthNitrogenType
   manureNitrogenTotal = aAnimalYear*manureNitrogenAnimalYear
   manureNitrogenTotalNorm = aAnimalYear*manureNitrogenAnimalYearNorm
   Dim arrParameter
    redim arrParameter(4)

	 arrParameter(0)= aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aType
	 arrParameter(3) = 408
	 arrParameter(4) = CSng(manureNitrogenTotal)
     returnValue = CallSPReturn ("IncrementPigHerdTotal",arrParameter)
'   IncrementPigHerdTotal aFarmNumber, aScenarioID, aType, 408, CSng(manureNitrogenTotal)
end sub
'**************************************************************************************************
sub CalcManureNitrogenFaecesUrineP(aType, aAnimalYear, aNitrogenDigestability)
   manureNitrogenFaecesAnimalYear = feedNitrogenAnimalYear*aNitrogenDigestability
   manureNitrogenFaecesTotal = aAnimalYear*manureNitrogenFaecesAnimalYear
   Dim arrParameter
    redim arrParameter(4)

	 arrParameter(0)= aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aType
	 arrParameter(3) = 410
	 arrParameter(4) = CSng(manureNitrogenFaecesTotal)
     returnValue = CallSPReturn ("IncrementPigHerdTotal",arrParameter)
'   IncrementPigHerdTotal aFarmNumber, aScenarioID, aType, 410, CSng(manureNitrogenFaecesTotal)

   manureNitrogenUrineAnimalYear = manureNitrogenAnimalYear - manureNitrogenFaecesAnimalYear
   manureNitrogenUrineTotal = aAnimalYear*manureNitrogenUrineAnimalYear
	 arrParameter(3) = 411
	 arrParameter(4) = CSng(manureNitrogenUrineTotal)
     returnValue = CallSPReturn ("IncrementPigHerdTotal",arrParameter)
'   IncrementPigHerdTotal aFarmNumber, aScenarioID, aType, 411, CSng(manureNitrogenUrineTotal)
end sub
'**************************************************************************************************
sub CalcManureStableStorageP(aType, aAnimalYear, aStable1ID, aStable2ID)
   dim nStable, arrStable, dictStable
   dim stableNitrogen, stableNitrogenLoss, stableNitrogenAfterLoss
   dim storageNitrogen, storageNitrogenLossAmmonia,storageNitrogenLossDenitrification, storageNitrogenAfterLoss
   dim stableBeddingNitrogen,stableNitrogenNormAfterLoss, storageNitrogenNormAfterLoss, stableNitrogenNorm
   dim s
   if isnull(aStable1ID) or isnull(aStable2ID) then
   
 '  Response.write "<br> aType: " & aType & " aStable1ID: "&aStable1ID&" aStable2ID: "& aStable2ID
   else
   set dictStable = CreateObject("Scripting.Dictionary")

   nStable = CallSP ("GetPigStableList "&aType & ", " & aStable1ID & ", " &  aStable2ID, arrStable, dictStable)
'   nStable = GetPigStableList(aType,aStable1ID,aStable2ID,arrStable)
   for s = 0 to nStable-1
'   Response.write "<br> aType: " & aType & " arrStable(s,1): " & arrStable(s,1) & " arrStable(s,4): "& arrStable(s,4) & " " & arrStable(s,6)

      stableBeddingNitrogen = arrStable(s,3)*arrStable(s,6)/100*aAnimalYear*0.85*0.005
       
      if isnull(stableBeddingNitrogen) then 
      stableBeddingNitrogen=0
      end if

    '  Response.write "<br> Strøelse: " & stableBeddingNitrogen

   Dim arrParameter
    redim arrParameter(4)

	 arrParameter(0)= aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aType
	 arrParameter(3) = 412
	 arrParameter(4) = CSng(stableBeddingNitrogen)
     returnValue = CallSPReturn ("IncrementPigHerdTotal",arrParameter)
'      IncrementPigHerdTotal aFarmNumber, aScenarioID, aType, 412, CSng(stableBeddingNitrogen)

    '  Response.write "<br> manureNitrogenTotal : " & manureNitrogenTotal 

      stableNitrogen = manureNitrogenTotal*arrStable(s,3)/100*arrStable(s,7)/100 + stableBeddingNitrogen
  '    Response.write "<br> stableNitrogen : " & stableNitrogen 
      stableNitrogenNorm = manureNitrogenTotalNorm*arrStable(s,3)/100*arrStable(s,7)/100 + stableBeddingNitrogen
  '    Response.write "<br> stableNitrogenNorm : " & stableNitrogenNorm 
    
      stableNitrogenLoss = stableNitrogen*arrStable(s,5)/100
      stableNitrogenAfterLoss = stableNitrogen - stableNitrogenLoss
      stableNitrogenNormAfterLoss = stableNitrogenNorm*(100-arrStable(s,5))/100
 '     Response.write "<br> stableNitrogenNormAfterLoss : " & stableNitrogenNormAfterLoss 

       redim preserve arrParameter(7)
 	 arrParameter(2) = 4
	 arrParameter(3) = arrStable(s,1)
	 arrParameter(4) = 0
	 arrParameter(5) = 0
	 arrParameter(6) = stableNitrogenLoss
	 arrParameter(7) = stableNitrogenAfterLoss
     returnValue = CallSPReturn ("IncrementStable",arrParameter)
'     IncrementStable  aFarmNumber,aScenarioID,4,arrStable(s,1),0,0,stableNitrogenLoss,stableNitrogenAfterLoss
      
      storageNitrogen = stableNitrogenAfterLoss
      storageNitrogenLossAmmonia = storageNitrogen*arrStable(s,8)/100
      storageNitrogenLossDenitrification = storageNitrogen*arrStable(s,9)/100
      storageNitrogenAfterLoss = storageNitrogen - storageNitrogenLossAmmonia - storageNitrogenLossDenitrification
      storageNitrogenNormAfterLoss = stableNitrogenNormAfterLoss * (100- arrStable(s,8) - arrStable(s,9))/100
  '    Response.write "<br> storageNitrogenNormAfterLoss : " & storageNitrogenNormAfterLoss 
      
      redim preserve arrParameter(14)
	 arrParameter(3) = arrStable(s,4)
	 arrParameter(4) = stableNitrogen
	 arrParameter(5) = stableNitrogenLoss
	 arrParameter(6) = stableNitrogenAfterLoss
	 arrParameter(7) = storageNitrogenLossAmmonia
	 arrParameter(8) = storageNitrogenLossDenitrification
	 arrParameter(9) = storageNitrogenAfterLoss
	 arrParameter(10) = 0
	 arrParameter(11) = 0
	 arrParameter(12) = 0
	 arrParameter(13) = 0
	 arrParameter(14) = 0  
    returnValue = CallSPReturn ("IncrementStorage",arrParameter)
'	  IncrementStorage aFarmNumber,aScenarioID,4,arrStable(s,4),stableNitrogen,stableNitrogenLoss,stableNitrogenAfterLoss,storageNitrogenLossAmmonia,storageNitrogenLossDenitrification,storageNitrogenAfterLoss,0,0,0,0,0  
	  redim preserve arrParameter(4)
	 arrParameter(3) = arrStable(s,4)
	 arrParameter(4) = storageNitrogenNormAfterLoss
     returnValue = CallSPReturn ("IncrementStorageNorm",arrParameter)

'      IncrementStorageNorm aFarmNumber,aScenarioID,4,arrStable(s,4), storageNitrogenNormAfterLoss
   next
   end if
end sub
'**************************************************************************************************
%>