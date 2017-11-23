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
dim aFarmNumber, aScenarioID, aSoldManureID, aSoldManureType, aSoldManureAmount, returnValue, arrID, nID
Dim dictID, arr
aFarmNumber 				= Session("farmNumber")
aScenarioID 				= Session("scenarioNumber")
'aSoldManureID 	                = Request("selectSoldManure")
InitFarmNDB

   set dictID = CreateObject("Scripting.Dictionary")
   nID = CallSP ("GetSoldManureMaxID " & aFarmNumber & "," & aScenarioID ,arrID,dictID)
'nID          = GetSoldManureMaxID(aFarmNumber, aScenarioID, arrID)
aSoldManureID = arrID(0,dictID.item("MaxID"))+1
aSoldManureType   = 1
aSoldManureAmount = 0

redim arr(4)

	 arr(0) = aFarmNumber
	 arr(1) = aScenarioID
	 arr(2) = aSoldManureID
	 arr(3) = aSoldManureAmount
	 arr(4) = aSoldManureType

   returnValue = CallSPReturn ("InsertUpdateSoldManure",arr)
'returnValue  = InsertUpdateSoldManure(aFarmNumber, aScenarioID, aSoldManureID, aSoldManureAmount, aSoldManureType)

CloseFarmNDB
Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
%>