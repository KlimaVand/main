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
<!-- #include file="includes/LanguageDictionary.asp" -->
<%
'dim aFarmNumber, aScenarioID, aScenarioName, arrID, nID, aFarmOwnerName, aFarmOwnerAddress, aFarmOwnerZipCode, aFarmOwnerTown
'dim aSoilType, aIrrigationType, aDelivery, aFarmType, aSelectedScenario, returnValue
'dim aYieldLevel
'dim MaxScenarioID, NewScenarioID, sc, dictMaxID, arr
'
'InitFarmNDB
'CreateLanguageDictionary
'if request("newScenarioName")<>"" then
'
'redim arr(2)
'
'	 arr(0) = Session("farmNumber")
'	 arr(1) = Session("scenarioNumber")
'	 arr(2) = request("newScenarioName")
'
'   returnValue = CallSPReturn ("InsertUpdateScenario",arr)
'
'aFarmNumber       = Session("farmNumber")
'aSelectedScenario = Session("scenarioNumber")
''returnValue   = InsertUpdateScenario(aFarmNumber, aSelectedScenario, request("newScenarioName"))
'Session("scenarioName") = request("newScenarioName")
'end if
'
'CloseFarmNDB
BeginHeader "Warning","",""
CreateStyleLink "styles/stylesheet.css"
EndHeader
BeginBodyData
'response.write "amount: "& request("amount")

   Response.Write("Warning: There is not balance between grazing amount on Ruminants page and grazing on Yield page")
   if request("amount") > 0 then
   response.write("<br/>Too much grazing registered for Ruminants")   
   else
   response.write("<br/>Too much grazing registered on Yield")
   end if   
EndBody

%>