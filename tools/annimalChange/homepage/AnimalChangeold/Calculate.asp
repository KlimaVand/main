<%
'**************************************************************************************************
' Author: Poul Lassen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
option explicit

dim aFarmNumber, aScenarioID, aCattleHerdID, returnValue
dim nFarmTotal, arrFarmTotal
dim nStorage, arrStorage
dim nRotation, arrRotation
dim nLoss, arrLoss
dim nBoughtFeed, arrBoughtFeed
dim totalFixation, totalSoldManure
dim storageID, balanceID, cropGroupID, arrSoldCrop
   
aFarmNumber = Session("farmNumber")
aScenarioID = Session("scenarioNumber")
InitFarmNDB
nFarmTotal = GetFarmTotalList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),3,arrFarmTotal)
nStorage   = GetStorageList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrStorage)
nRotation  = GetRotationList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation)

storageID  = 1
balanceID  = 6
nLoss      = GetBalanceStorageData(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), storageID, balanceID, arrLoss)
totalSoldManure  = arrLoss(6,0)
storageID  = 2
balanceID  = 6
nLoss      = GetBalanceStorageData(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), storageID, balanceID, arrLoss)
totalSoldManure  = totalSoldManure + arrLoss(6,0)

'cropGroupID = 4
'balanceID   = 13
'nBoughtFeed = GetBalanceFeedData(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), cropGroupID, balanceID, arrBoughtFeed)
'boughtFeed  = arrBoughtFeed(4,0)
'cropGroupID = 2
'balanceID   = 13
'nBoughtFeed = GetBalanceFeedData(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), cropGroupID, balanceID, arrBoughtFeed)
'boughtFeed  = boughtFeed + arrBoughtFeed(4,0)

Calculate
CloseFarmNDB
Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
'**************************************************************************************************
' Author: Margit Jorgensen, Danish Institute of Agricultural Sciences
' Written: 2004
' Function: The function returns a floating point number giving the total sum of 
' denitrification (Kg N) for all rotations
' Parameters: 
' aFarmNumber is a long integer
'**************************************************************************************************
function Denitrification(aFarmNumber,aScenarioID,aFarmType)
   dim aDenitrification,aFixation,aGrazingManure,aManureNSpread,aManureNIncorp,aFertilizerAfterLoss,i,j,temp,tempDenitri
   dim nFieldPlanRotation, arrFieldPlanRotation
   dim aFertilizerDeliveryLoss',aArea
   
   aFertilizerDeliveryLoss=0.03

   for i=0 to nRotation-1
       nFieldPlanRotation	 = GetFieldPlanRotationList(aFarmNumber,aScenarioID,arrRotation(0,i),arrFieldPlanRotation)
       aFertilizerAfterLoss=GetFertilizerAfterLoss(aFarmNumber,aScenarioID,arrRotation(0,i),aFertilizerDeliveryLoss) 
       aManureNIncorp=GetManureNIncorp(aFarmNumber,aScenarioID,arrRotation(0,i))
       aManureNSpread=GetManureNSpread(aFarmNumber,aScenarioID,arrRotation(0,i))
       aGrazingManure=GetGrazingManure(aFarmNumber,aScenarioID,arrRotation(0,i))
	   aManureNSpread=aManureNSpread+aGrazingManure
	   aFixation=0 
       for j=0 to nFieldPlanRotation-1
          temp=GetFixation(arrFieldPlanRotation(14,j),Csng(arrFieldPlanRotation(15,j)))
		  aFixation=aFixation+temp
	   next
'	   response.write "DenitrificationPrRotation("&arrRotation(2,i)&", "&aFarmType&", "&aFertilizerAfterLoss&", "&aManureNIncorp&", "&aManureNSpread&", "&aFixation&")"
       tempDenitri=DenitrificationPrRotation(arrRotation(2,i), aFarmType, aFertilizerAfterLoss, aManureNIncorp, aManureNSpread, aFixation)
       totalFixation=totalFixation+aFixation
       aDenitrification=aDenitrification+tempDenitri
   next
   
   Denitrification = aDenitrification
end function
'**************************************************************************************************
sub Calculate()
   dim aDenitrification, aBoughtFertilizer, tempA, tempB, aIncorpFertilizer, aFertilizerDeliveryLoss
   dim i,totalCarbonManure,totalCarbonCrop,aSoilChange,r,temp
   dim totalGrazing,grazingConversion
   dim aNInSeed, nArea, arrArea,totalLoss
   dim outputResult,InputResult
   dim aBalance,Nleaching,aSoldCrop

   
   aDenitrification  = Denitrification(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),CInt(Session("farmType")))
   for i = 0 to nRotation-1
      tempA = GetFertilizerBeforeLoss(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation(0,i))
      aBoughtFertilizer = aBoughtFertilizer+tempA
      tempB = GetFertilizerAfterLoss(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation(0,i),0.03) 
      aIncorpFertilizer = aIncorpFertilizer+tempB
   next
   aFertilizerDeliveryLoss=aBoughtFertilizer-aIncorpFertilizer

   totalCarbonManure=GetTotalCarbonFromManure(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   totalGrazing=GetTotalGrazingManure(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   grazingConversion=GetConversionFactor(3)
   totalCarbonManure=((totalGrazing*grazingConversion)/1000)+totalCarbonManure
   totalCarbonCrop=GetTotalCarbonFromCrops(CLng(Session("farmNumber")),CInt(Session("scenarioNumber"))) 
   for r = 0 to nRotation-1
   Response.write "<br> rotations ID " & r
     temp=SoilChange(arrRotation(2,0), arrRotation(0,0),TotalCarbonCrop,totalCarbonManure)
	 aSoilChange=aSoilChange+temp
   next

   nArea 				= CalcSumArea(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrArea)
   
   aSoilChange=aSoilChange/(nRotation)*arrArea(0,0)
   
   
   inputResult			= CalcSumResult(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),1)
   outputResult			= CalcSumResult(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),2)
   
   aBalance				= inputResult-outputResult
   NLeaching			= NitrogenLeaching(aBalance, aSoilChange, arrArea(0,0))
   
   aNInSeed				= GetTotalNInSeed(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   
   
   totalLoss			= GetTotalLoss(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))

   GetSummarizedSoldCrop CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), arrSoldCrop
   aSoldCrop			= arrSoldCrop(4,0)

   Response.Write("<h3>Calculate function activated</h3>" & vbCr)
   InsertUpdateResult aFarmNumber, aScenarioID, 1, aBoughtFertilizer'bought mineralfertilizer
   InsertUpdateResult aFarmNumber, aScenarioID, 2, 0'bought manure
'   InsertUpdateResult aFarmNumber, aScenarioID, 3, boughtFeed 'bought fodder
   InsertUpdateResult aFarmNumber, aScenarioID, 4, 0'bought straw
   InsertUpdateResult aFarmNumber, aScenarioID, 5, 0'bought livestock
   InsertUpdateResult aFarmNumber, aScenarioID, 6, aNInSeed'seed
   InsertUpdateResult aFarmNumber, aScenarioID, 7, 0'Ammonium for straw
   InsertUpdateResult aFarmNumber, aScenarioID, 8, totalFixation'N-fixation
   InsertUpdateResult aFarmNumber, aScenarioID, 9, 15*arrArea(0,0)'precipitation
   InsertUpdateResult aFarmNumber, aScenarioID, 10, arrSoldCrop(3,0)'sold crop
   InsertUpdateResult aFarmNumber, aScenarioID, 11, 0'sold livestock
   InsertUpdateResult aFarmNumber, aScenarioID, 12, arrFarmTotal(5,15)'meat
   InsertUpdateResult aFarmNumber, aScenarioID, 13, arrFarmTotal(5,14)'milk
   InsertUpdateResult aFarmNumber, aScenarioID, 14, totalSoldManure 'sold manure
'   InsertUpdateResult aFarmNumber, aScenarioID, 15, soldFeed 'sold fodder
   InsertUpdateResult aFarmNumber, aScenarioID, 16, arrStorage(4,0)+arrStorage(4,1)'Amm. loss stable
   InsertUpdateResult aFarmNumber, aScenarioID, 17, arrStorage(6,0)+arrStorage(6,1)'Amm. loss storage
   InsertUpdateResult aFarmNumber, aScenarioID, 18, arrFarmTotal(5,17)*0.07'Amm. loss grazing
   InsertUpdateResult aFarmNumber, aScenarioID, 19, totalLoss'Amm. loss manuredelivery
   InsertUpdateResult aFarmNumber, aScenarioID, 20, aFertilizerDeliveryLoss'Amm. loss mineral fertilizer
   InsertUpdateResult aFarmNumber, aScenarioID, 21, aDenitrification'denitrification
   InsertUpdateResult aFarmNumber, aScenarioID, 22, aSoilChange'soilchange
   InsertUpdateResult aFarmNumber, aScenarioID, 23, NLeaching'N-leaching
end sub
'**************************************************************************************************
%>