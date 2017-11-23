<%
'**************************************************************************************************
' Author: Margit Jorgensen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
option explicit
%>
<!-- #include file="includes/General.asp" -->
<!-- #include file="includes/adovbs.asp" -->
<!-- #include file="includes/dbInterface.asp" -->
<%
dim aFarmNumber, aScenarioID, returnValue, aSelectedScenario, nScenario, arrScenario
Dim dictScenario, arr

aFarmNumber = Session("farmNumber")
aScenarioID = Request("selectScenario")

InitFarmNDB

   set dictScenario = CreateObject("Scripting.Dictionary")
   nScenario = CallSP ("GetSelectListScenario " & aFarmNumber,arrScenario,dictScenario)

'nScenario        = GetSelectListScenario(CLng(Session("farmNumber")),arrScenario)

if Cint(aScenarioID) <> Cint(arrScenario(0,0)) then
     redim arr(1)

	 arr(0) = aFarmNumber
	 arr(1) = aScenarioID

     returnValue = CallSPReturn ("DeleteScenario",arr)
     'returnValue = DeleteScenario(aFarmNumber, aScenarioID)
end if 

Session("scenarioNumber") =  Cint(arrScenario(0,0))
Session("scenarioName") = arrScenario(1,0)

'Opdatere det selectede scenario i databasen

aSelectedScenario = Session("scenarioNumber")
Response.write "<br> aSelectedScenario " & aSelectedScenario

     redim arr(1)

	 arr(0) = aFarmNumber
	 arr(1) = aSelectedScenario

     returnValue = CallSPReturn ("UpdateSelectedScenario",arr)

'returnValue  = UpdateSelectedScenario(aFarmNumber, aSelectedScenario)


CloseFarmNDB
Response.Redirect("start.asp")
Response.End
%>