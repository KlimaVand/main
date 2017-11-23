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
dim aFarmNumber, aScenarioID, DeleteSoldManureID, returnValue, arr

aFarmNumber = Session("farmNumber")
aScenarioID = Session("scenarioNumber")
DeleteSoldManureID = Request("DeleteSoldManureID")

InitFarmNDB

redim arr(2)

	 arr(0) = aFarmNumber
	 arr(1) = aScenarioID
	 arr(2) = DeleteSoldManureID

   returnValue = CallSPReturn ("DeleteSoldManure",arr)
'returnValue = DeleteSoldManure(aFarmNumber, aScenarioID, DeleteSoldManureID)

CloseFarmNDB
Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
%>