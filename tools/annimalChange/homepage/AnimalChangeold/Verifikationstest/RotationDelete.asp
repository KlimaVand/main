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
<!-- #include file="includes/FunctionField.asp" -->
<%
dim aFarmNumber, aScenarioID, aRotationID 
dim nAreaSum, arrAreaSum, returnValue, arrParameter

aFarmNumber        = Session("farmNumber")
aScenarioID        = Session("scenarioNumber")
aRotationID        = Request.QueryString("RotationID")
InitFarmNDB

redim arrParameter(2)

	 arrParameter(0) = aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aRotationID

   returnValue = CallSPReturn ("DeleteRotation",arrParameter)
'	DeleteRotation aFarmNumber, aScenarioID, aRotationID
    CalcFarmTotal aFarmNumber,aScenarioID
    'CalcBalance aFarmNumber, aScenarioID

CloseFarmNDB
Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
%>