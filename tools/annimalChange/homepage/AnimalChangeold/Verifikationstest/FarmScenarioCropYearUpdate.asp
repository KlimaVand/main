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
dim aFarmNumber, aSelectedScenario, aScenarioCropYear, returnValue, nFarm,arrFarm,dictFarm,arr

aFarmNumber 				= Session("farmNumber")


aSelectedScenario = Session("scenarioNumber")
InitFarmNDB

aScenarioCropYear = request("selectCropYear")

	redim arr(2)
	 arr(0)= aFarmNumber
	 arr(1) = aSelectedScenario
	 arr(2) = aScenarioCropYear
     returnValue = CallSPReturn ("UpdateScenarioCropYear",arr)


Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
%>