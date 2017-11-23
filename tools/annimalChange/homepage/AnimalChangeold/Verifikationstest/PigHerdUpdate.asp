<%
'**************************************************************************************************
' Author: Poul Lassen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
option explicit
%>
<!-- #include file="includes/General.asp" -->
<!-- #include file="includes/adovbs.asp" -->
<!-- #include file="includes/dbInterface.asp" -->
<!-- #include file="includes/FunctionPig.asp" -->

<%
dim h, nPigHerd, aFarmNumber, aScenarioID, aPigHerdID, aPigType, aAnimalYear, aProductionVar1, aProductionVar2, aProductionVar3, aFeedLevel, aProteinLevel, aStableType1, aStableType2, returnValue, feedNitrogenAnimalYearNorm
dim arrPigHerd, feedEnergyTotal, feedNitrogenAnimalYear, feedNitrogenTotal, GrowthNitrogenType, GrowthNitrogenTypeNorm, GrowthNitrogenTotal,SowGrowthNitrogen,SowGrowthNitrogenTotal, GrowthNitrogenIn, GrowthNitrogenOut, GrowthNitrogenInTotal, GrowthNitrogenOutTotal, manureNitrogenAnimalYear, manureNitrogenTotal, manureNitrogenFaecesAnimalYear, manureNitrogenFaecesTotal, manureNitrogenUrineAnimalYear, manureNitrogenUrineTotal, manureNitrogenAnimalYearNorm, manureNitrogenTotalNorm
Dim arrParameter
redim arrParameter(11)

aFarmNumber = Session("farmNumber")
aScenarioID = Session("scenarioNumber")
nPigHerd = Request.Form("hiddenNPigHerd")
InitFarmNDB
for h = 0 to nPigHerd-1
   aPigHerdID  = Request.Form("hiddenPigHerdID_"&h&"")
   aPigType    = Request.Form("selectPigType_"&h&"")
   aAnimalYear = Request.Form("inputAnimalYear_"&h&"")
   aProductionVar1 = Request.Form("inputProductionVar1_"&h&"")
   aProductionVar2 = Request.Form("inputProductionVar2_"&h&"")
   aProductionVar3 = Request.Form("inputProductionVar3_"&h&"")
   aFeedLevel    = Request.Form("inputFeedLevel_"&h&"")
   aProteinLevel = Request.Form("inputProteinLevel_"&h&"")
   aStableType1  = Request.Form("selectStableType1_"&h&"")
   aStableType2  = Request.Form("selectStableType2_"&h&"")
   if aStableType2="" then 
   aStableType2 = 1
   end if


	 arrParameter(0)= aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aPigHerdID
	 arrParameter(3) = aPigType ' aCattleType 
	 arrParameter(4) = aAnimalYear ' aAnimalYear
	 arrParameter(5) = aProteinLevel ' 
	 arrParameter(6) = aFeedLevel
	 arrParameter(7) = aProductionVar1
	 arrParameter(8) = aProductionVar2
	 arrParameter(9) = aProductionVar3
	 arrParameter(10) = aStableType1
	 arrParameter(11) = aStableType2

   returnValue = CallSPReturn ("InsertUpdatePigHerd",arrParameter)

'   returnValue = InsertUpdatePigHerd(aFarmNumber, aScenarioID, aPigHerdID, aPigType, aAnimalYear, aProteinLevel, aFeedLevel, aProductionVar1, aProductionVar2, aProductionVar3, aStableType1, aStableType2)
next   

CalcPigHerdTotal
redim preserve arrParameter(1)

   returnValue = CallSPReturn ("CalcFarmTotal",arrParameter)
'CalcFarmTotal aFarmNumber,aScenarioID
CloseFarmNDB
Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End

'**************************************************************************************************

%>