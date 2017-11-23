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
<!-- #include file="includes/FunctionCattle.asp" -->
<%
dim h, aFarmNumber, aScenarioID 
Dim returnValue
dim feedEnergyAnimalYear, feedEnergyTotal, feedEnergyTotalNorm, feedDryMatterAnimalYear, feedDryMatterTotal, feedNitrogenAnimalYear, feedNitrogenTotal, feedNitrogenAnimalYearNorm, feedNitrogenTotalNorm
dim productionMilkNitrogenAnimalYear, productionMilkNitrogenTotal, productionGrowthNitrogenAnimalYear, productionGrowthNitrogenTotal
dim manureNitrogenAnimalYear, manureNitrogenTotal, manureNitrogenOnField, manureNitrogenAnimalYearNorm, manureNitrogenTotalNorm
dim manureNitrogenOnFieldAnimalYear, manureNitrogenOnFieldTotal
dim manureNitrogenFaecesAnimalYear, manureNitrogenFaecesTotal
dim manureNitrogenUrineAnimalYear, manureNitrogenUrineTotal
dim nCattleHerd, arrCattleHerd

aFarmNumber = Session("farmNumber")
aScenarioID = Session("scenarioNumber")
nCattleHerd = Request.Form("hiddenNCattleHerd")

dim originalLocale, currentLocale
Dim arrParameter
redim arrParameter(9)

originalLocale = GetLocale()        ' Get the original locale
currentLocale  = SetLocale("en-gb") ' Set the locale to UK

InitFarmNDB
for h = 0 to nCattleHerd-1

	 arrParameter(0)= aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = Request.Form("hiddenCattleHerdID_"&h&"") ' aCattleHerdID
	 arrParameter(3) = Request.Form("selectCattleType_"&h&"") ' aCattleType 
	 arrParameter(4) = Request.Form("inputAnimalYear_"&h&"") ' aAnimalYear
	 arrParameter(5) = Request.Form("inputGrazing_"&h&"") ' aGrazing
	 arrParameter(6) = Request.Form("inputProductionLevel_"&h&"") ' aProductionLevel
	 arrParameter(7) = Request.Form("inputProductionLevel2_"&h&"")' aProductionLevel2
	 arrParameter(8) = Request.Form("inputEffectivity_"&h&"") ' aEffectivity
	 arrParameter(9) = Request.Form("selectStableType_"&h&"")  ' aStableType

   returnValue = CallSPReturn ("InsertUpdateCattleHerd",arrParameter)

next
CalcCattleHerdTotal
redim preserve arrParameter(1)

   returnValue = CallSPReturn ("CalcFarmTotal",arrParameter)

CloseFarmNDB

currentLocale  = SetLocale(originalLocale) ' Restore the original locale

Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
'**************************************************************************************************
%>
