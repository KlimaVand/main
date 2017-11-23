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

InitFarmNDB

Dim arrParameter
redim arrParameter(2)
Response.Write("<h5>farmNumber "&Session("farmNumber")&"</h5>")
Response.Write("<h5>scenarioNumber "&Session("farmNumber")&"</h5>")
Response.Write("<h5>aCattleHerdID: "&Request.QueryString("CattleHerdID")&"</h5>")

	 arrParameter(0) = Session("farmNumber")
	 arrParameter(1) = Session("scenarioNumber")
	 arrParameter(2) = Request.QueryString("CattleHerdID")

   returnValue = CallSPReturn ("DeleteCattleHerd",arrParameter)

CalcCattleHerdTotal
redim preserve arrParameter(1)

   returnValue = CallSPReturn ("CalcFarmTotal",arrParameter)


CloseFarmNDB
Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
%>