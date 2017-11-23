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
<%
dim aFarmNumber, aScenario, aNPercent, d

aFarmNumber       = Session("farmNumber")
aScenario = Session("scenarioNumber")

aNPercent         = Request.Form("selectNPercent")

InitFarmNDB


 redim arrParameter(2)
 arrParameter(0)=aFarmNumber
 arrParameter(1)=aScenario
 arrParameter(2)=aNPercent
  
 d=CallSPReturn("InsertUpdateNNeedPercent", arrParameter)
 
CloseFarmNDB

Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
%>