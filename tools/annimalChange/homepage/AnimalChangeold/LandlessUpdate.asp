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
<!-- #include file="includes/FunctionField.asp" -->
<%

dim aFarmNumber, aScenarioID, aLandless, returnValue

aFarmNumber   = Session("farmNumber")
aScenarioID   = Session("scenarioNumber")
aLandless   = request("landless")
if Cstr(aLandless) = "False" then
 aLandless = "True"
else
 aLandless = "False"
end if

InitFarmNDB

redim arrParameter(2)
	 arrParameter(0) = aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aLandless
   returnValue = CallSPReturn ("UpdateScenarioLandless",arrParameter)


CloseFarmNDB
Response.Redirect("data.asp?TB=2")
'Response.redirect("start.asp")
Response.End
%>