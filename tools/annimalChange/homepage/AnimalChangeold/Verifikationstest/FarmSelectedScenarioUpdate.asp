<%
'**************************************************************************************************
' Author: Margit Jørgensen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
option explicit
%>
<!-- #include file="includes/General.asp" -->
<!-- #include file="includes/adovbs.asp" -->
<!-- #include file="includes/dbInterface.asp" -->
<%
dim aFarmNumber, aSelectedScenario, returnValue, nFarm,arrFarm,dictFarm,arr

aFarmNumber 				= Session("farmNumber")
'Response.write request.form

InitFarmNDB
'Response.write "<br> Session(scenarioNumber) " & Session("scenarioNumber") 

aSelectedScenario = Request.Form("selectScenario")
'Response.write "<br> aSelectedScenario " & aSelectedScenario

	redim arr(1)
	 arr(0)= aFarmNumber
	 arr(1) = aSelectedScenario
     returnValue = CallSPReturn ("UpdateSelectedScenario",arr)
'returnValue  = UpdateSelectedScenario(aFarmNumber, aSelectedScenario)



   set dictFarm = CreateObject("Scripting.Dictionary")
   nFarm = CallSP ("GetFarmData " & aFarmNumber,arrFarm,dictFarm)
'nFarm = GetFarmData(aFarmNumber,arrFarm)
Session("scenarioName")   = arrFarm(0,dictFarm.item("ScenarioName"))
Session("scenarioNumber") = aSelectedScenario

'Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.Redirect("start.asp")
Response.End
%>