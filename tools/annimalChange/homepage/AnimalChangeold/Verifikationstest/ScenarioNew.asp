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
dim aFarmNumber, aScenarioID, aCattleType, p, c, n

aFarmNumber = session("farmNumber")
aScenarioID = Session("scenarioNumber")

InitFarmNDB
for c=1 to 6
   for p=10 to 20
      n = InsertUpdateCattleHerdTotal(aFarmNumber, aScenarioID, c, p, 0.0)
   next
next
for p=1 to 20
   n = InsertUpdateFarmTotal(aFarmNumber, aScenarioID, p, 0.0)
next
CloseFarmNDB

Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
%>