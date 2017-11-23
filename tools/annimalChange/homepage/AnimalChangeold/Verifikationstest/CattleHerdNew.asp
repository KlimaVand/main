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
dim h, aFarmNumber, aScenarioID, aCattleHerdID
Dim returnValue
dim feedEnergyAnimalYear, feedEnergyTotal, feedEnergyTotalNorm, feedDryMatterAnimalYear, feedDryMatterTotal, feedNitrogenAnimalYear, feedNitrogenTotal, feedNitrogenAnimalYearNorm, feedNitrogenTotalNorm
dim productionMilkNitrogenAnimalYear, productionMilkNitrogenTotal, productionGrowthNitrogenAnimalYear, productionGrowthNitrogenTotal
dim manureNitrogenAnimalYear, manureNitrogenTotal, manureNitrogenOnField, manureNitrogenAnimalYearNorm, manureNitrogenTotalNorm
dim manureNitrogenOnFieldAnimalYear, manureNitrogenOnFieldTotal
dim manureNitrogenFaecesAnimalYear, manureNitrogenFaecesTotal
dim manureNitrogenUrineAnimalYear, manureNitrogenUrineTotal
dim nCattleHerd, arrCattleHerd
dim nID, arrID, dictID
dim nCattleType, arrCattleType, dictCattleType

aFarmNumber = session("farmNumber")
aScenarioID = Session("scenarioNumber")
InitFarmNDB

  set dictID = CreateObject("Scripting.Dictionary")

   nID = CallSP ("GetCattleHerdMaxID "&aFarmNumber & ", " & aScenarioID,arrID,dictID)
  set dictCattleType = CreateObject("Scripting.Dictionary")

nCattleType  = CallSP ("GetCattleTypeList", arrCattleType, dictCattleType)

if CInt(arrID(0,dictID.Item("MaxID"))) > 0 then
   aCattleHerdID = arrID(0,dictID.Item("MaxID"))+1
else
   aCattleHerdID = 1
end if
set dictID = nothing

Dim arrParameter
redim arrParameter(9)

	 arrParameter(0)= aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aCattleHerdID
	 arrParameter(3) = 1 ' aCattleType 
	 arrParameter(4) = 1 ' aAnimalYear
	 arrParameter(5) = arrCattleType(0,dictCattleType.Item("GrazingDefault")) ' aGrazing
	 arrParameter(6) = arrCattleType(0,dictCattleType.Item("ProductionLevelDefault")) ' aProductionLevel
	 arrParameter(7) = arrCattleType(0,dictCattleType.Item("ProductionLevel2Default")) ' aProductionLevel2
	 arrParameter(8) = 100 ' aEffectivity
	 arrParameter(9) = 1  ' aStableType

   returnValue = CallSPReturn ("InsertUpdateCattleHerd",arrParameter)

  set dictCattleType = Nothing

CalcCattleHerdTotal
redim preserve arrParameter(1)

returnValue = CallSPReturn ("CalcFarmTotal",arrParameter)

CloseFarmNDB
Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
%>