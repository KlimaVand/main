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
dim aFarmNumber, aScenarioID, DeleteBoughtManureID, returnValue, arr

aFarmNumber = Session("farmNumber")
aScenarioID = Session("scenarioNumber")
DeleteBoughtManureID = Request("DeleteBoughtManureID")

InitFarmNDB

redim arr(2)

	 arr(0) = aFarmNumber
	 arr(1) = aScenarioID
	 arr(2) = DeleteBoughtManureID

   returnValue = CallSPReturn ("DeleteBoughtManure",arr)

'returnValue = DeleteBoughtManure(aFarmNumber, aScenarioID, DeleteBoughtManureID)

CloseFarmNDB
Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
%>