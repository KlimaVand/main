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

aFarmNumber = Session("farmNumber")
aScenarioID = Session("scenarioNumber")
aPigHerdID  = Request.QueryString("PigHerdID")
InitFarmNDB


Dim arrParameter
redim arrParameter(2)

	 arrParameter(0) = aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aPigHerdID

   returnValue = CallSPReturn ("DeletePigHerd",arrParameter)

'returnValue = DeletePigHerd(aFarmNumber, aScenarioID, aPigHerdID)

CalcPigHerdTotal

redim preserve arrParameter(1)

   returnValue = CallSPReturn ("CalcFarmTotal",arrParameter)

'CalcFarmTotal aFarmNumber,aScenarioID
CloseFarmNDB
Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
%>