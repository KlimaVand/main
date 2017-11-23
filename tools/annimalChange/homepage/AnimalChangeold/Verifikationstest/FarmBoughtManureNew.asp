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
dim aFarmNumber, aScenarioID, aBoughtManureID, aBoughtManureType, aBoughtManureAmount, returnValue, arrID, nID
Dim dictID, arr

aFarmNumber 				= Session("farmNumber")
aScenarioID 				= Session("scenarioNumber")

InitFarmNDB

   set dictID = CreateObject("Scripting.Dictionary")
   nID = CallSP ("GetBoughtManureMaxID " & aFarmNumber & "," & aScenarioID ,arrID,dictID)
response.write "<br> GetBoughtManureMaxID "& nID &" " & arrID(0,dictID.item("MaxID"))
'nID          = GetBoughtManureMaxID(aFarmNumber, aScenarioID, arrID)
aBoughtManureID = arrID(0,dictID.item("MaxID"))+1
aBoughtManureType   = 1
aBoughtManureAmount = 0

'response.write "InsertUpdateBoughtManure "&aFarmNumber&", "&aScenarioID&", "&aBoughtManureID&", "&aBoughtManureAmount&", "&aBoughtManureType

redim arr(4)

	 arr(0) = aFarmNumber
	 arr(1) = aScenarioID
	 arr(2) = aBoughtManureID
	 arr(3) = aBoughtManureAmount
	 arr(4) = aBoughtManureType

   returnValue = CallSPReturn ("InsertUpdateBoughtManure",arr)
'returnValue  = InsertUpdateBoughtManure(aFarmNumber, aScenarioID, aBoughtManureID, aBoughtManureAmount, aBoughtManureType)

CloseFarmNDB
Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
%>