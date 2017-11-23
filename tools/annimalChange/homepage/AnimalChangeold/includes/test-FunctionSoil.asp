<!-- #include file="FunctionSoil.asp" -->
<!-- #include file="adovbs.asp" -->
<!-- #include file="dbInterface.asp" -->
<%
dim aDenitrification,totalCarbonManure,totalCarbonCrop,ASoilChange,aFixation,aGetGrazingManure,aGetFertilizerAfterLoss,aGetManureNSpread,aGetManureNIncorp
dim totalGrazing,grazingConversion
dim nRotation, arrRotation, r,temp
dim outputResult,InputResult
dim aBalance, deltaNInSoil
dim NLeaching
   
   InitFarmNDB
   nArea 				= CalcSumArea(12345,1,arrArea)
   nRotation            = GetRotationList(12345,1,arrRotation)
   inputResult			= CalcSumResult(12345,1,1)
   outputResult			= CalcSumResult(12345,1,2)
   aBalance				= inputResult-outputResult
   nNeed				= GetTotalNNeed(12345,1)
   response.write nNeed
'   deltaNInSoil			= CalcSumResult(12345,1,3)
'   response.write deltaNInSoil&"<br>"
'   NLeaching			= NitrogenLeaching(aBalance, DeltaNInSoil,arrArea(0,0))
'   response.write NLeaching&"<br>"
'  response.write deltaNInSoil&"<br>"


'   GetSummarizedCerealYield 12345,1, arrCereal 
'   GetSummarizedGrazingYield 12345,1, arrGrazing
'   GetSummarizedNonCerealYield 12345,1, arrNonCereal
'   GetSummarizedNonGrazedYield 12345,1, arrNonGrazing
'
'   N_indhold=arrGrazing(3,0)+arrGrazing(5,0)+arrNonCereal(3,0)+arrNonCereal(5,0)+arrNonGrazing(3,0)+arrNonGrazing(5,0)+arrCereal(5,0)
'   response.write N_indhold&"<br>"

'   Denitrification(intSoilCode, intFarmType, fltFertiliserN, fltManureNincorp, fltManureNspread, fltNFixation)
'   aDenitrification=Denitrification(12344,7,1)
'   totalCarbonManure=GetTotalCarbonFromManure(12345,1)
'   totalGrazing=GetTotalGrazingManure(12345,1)
'   grazingConversion=GetConversionFactor(3)
'   totalCarbonManure=((totalGrazing*grazingConversion)/1000)+totalCarbonManure
'   totalCarbonCrop=GetTotalCarbonFromCrops(12345,1) 
'   for r = 0 to nRotation-1
'     temp=SoilChange(arrRotation(2,0), arrRotation(0,0),TotalCarbonCrop,totalCarbonManure)
'	  aSoilChange=aSoilChange+temp
'   next
'   aSoilChange=aSoilChange/(nRotation)*arrArea(0,0)
'   response.write aSoilChange&"<br>"
'   NLeaching			= NitrogenLeaching(aBalance, aSoilChange,arrArea(0,0))
'   response.write NLeaching&"<br>"
'   response.write aSoilChange&"<br>"
'
'   aFixation=GetFixation(18,3)
'   aGetManureNSpread=GetManureNSpread(12344,6,1)
'   aGetGrazingManure=GetGrazingManure(12344,6,1) 
'   aGetManureNIncorp=GetManureNIncorp(12344,6,1) 
'   aGetFertilizerAfterLoss=GetFertilizerAfterLoss(12344,6,1,0.03) 
   CloseFarmNDB
   
'   Response.write "Denitrification = "&aDenitrification &"<br>"
'   Response.write "GetTotalCarbonFromManure = "&totalCarbonManure&"<br>"
'   Response.write "GetTotalCarbonFromCrops = "&totalCarbonCrop&"<br>"
'   Response.write "totalGrazing = "&totalGrazing&"<br>"
'   Response.write "aSoilChange = "&aSoilChange&"<br>"
'   Response.write "aGetManureNSpread = "&aGetManureNSpread&"<br>"
'   Response.write "aGetGrazingManure = "&aGetGrazingManure&"<br>"
'   Response.write "aGetManureNIncorp = "&aGetManureNIncorp&"<br>"
'   Response.write "aGetFertilizerAfterLoss = "&aGetFertilizerAfterLoss&"<br>"
   

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
'	   response.write aManureNSpread&"<br>"
       aGrazingManure=GetGrazingManure(aFarmNumber,aScenarioID,arrRotation(0,i))
	   aManureNSpread=aManureNSpread+aGrazingManure
'	   response.write aManureNIncorp&"<br>"
	   aFixation=0 
       for j=0 to nFieldPlanRotation-1
          temp=GetFixation(arrFieldPlanRotation(14,j),Csng(arrFieldPlanRotation(15,j)))
		  aFixation=aFixation+temp
	   next
'	   response.write "DenitrificationPrRotation("&arrRotation(2,i)&", "&aFarmType&", "&aFertilizerAfterLoss&", "&aManureNIncorp&", "&aManureNSpread&", "&aFixation&")"
       tempDenitri=DenitrificationPrRotation(arrRotation(2,i), aFarmType, aFertilizerAfterLoss, aManureNIncorp, aManureNSpread, aFixation)
       aDenitrification=aDenitrification+tempDenitri
   next
   
   Denitrification = aDenitrification
end function
'**************************************************************************************************
%>