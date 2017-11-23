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
dim nID, arrID, nPigType, arrPigType, i, dictID, dictPigType

aFarmNumber = session("farmNumber")
aScenarioID = Session("scenarioNumber")
aPigType = Request.Form("hiddenPigType")

Response.write " <br> aPigType "& aPigType
InitFarmNDB

  set dictID = CreateObject("Scripting.Dictionary")
  set dictPigType = CreateObject("Scripting.Dictionary")

   nID = CallSP ("GetPigHerdMaxID "&aFarmNumber & ", " & aScenarioID, arrID,dictID)
  set dictPigType = CreateObject("Scripting.Dictionary")

nPigType  = CallSP ("GetPigTypeList", arrPigType, dictPigType)

'nID = GetPigHerdMaxID(aFarmNumber, aScenarioID, arrID)
'nPigType  = GetPigTypeList(arrPigType)

if arrID(0,0) > 0 then
   aPigHerdID  = arrID(0,0)+1
else
   aPigHerdID = 1
end if

for i=0 to Ubound(arrPigType)
  if Cint(aPigType)=Cint(arrPigType(i,0)) then 

aAnimalYear = 1
aProductionVar1 = arrPigType(i,13)
aProductionVar2 = arrPigType(i,11)
aProductionVar3 = arrPigType(i,9)
aFeedLevel  = arrPigType(i,3)
aProteinLevel   = arrPigType(i,5)
aStableType1  = null
aStableType2  = null
  
  end if

next
Dim arrParameter
redim arrParameter(11)

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

'returnValue = InsertUpdatePigHerd(aFarmNumber, aScenarioID, aPigHerdID, aPigType, aAnimalYear, aProteinLevel, aFeedLevel, aProductionVar1, aProductionVar2, aProductionVar3,  aStableType1, aStableType2)

CalcPigHerdTotal
redim preserve arrParameter(1)

   returnValue = CallSPReturn ("CalcFarmTotal",arrParameter)
'CalcFarmTotal aFarmNumber,aScenarioID
CloseFarmNDB
Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
%>