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
dim fLoginname, fPassword, returnURL
dim nUser, arrUser, nFarm, arrFarm, dictUser, dictFarm
dim aUserID

fLoginName = Request.Form("inputLoginName")
fPassword  = Request.Form("inputPassword")

InitFarmNDB
   set dictUser = CreateObject("Scripting.Dictionary")
   nUser = CallSP ("GetUserByLogin " & fLoginName & "," & fPassword ,arrUser,dictUser)
   'nUser = GetUserByLogin(fLoginName, fPassword, arrUser)
if CInt(nUser) = CInt(1) then
   aUserID = arrUser(0,dictUser.item("UserID"))
   set dictFarm = CreateObject("Scripting.Dictionary")
   nFarm = CallSP ("GetFarmList '" & aUserID &"'" ,arrFarm,dictFarm)
'   nFarm = GetFarmList(aUserID,arrFarm)
end if
CloseFarmNDB

if CInt(nUser) = CInt(1) then
   Session("hasLogin")       = "yes"
''   
   Session("userID")         = arrUser(0,dictUser.item("UserID"))
   Session("nFarm")          = nFarm
''   
   Session("user")           = arrUser(0, dictUser.item("UserName"))
   Session("userType")       = arrUser(0, dictUser.item("UserType"))'new Session-variable 14-06-06
   Session("farmNumber")     = arrFarm(0,dictFarm.item("FarmNumber"))
   Session("farmOwner")      = arrFarm(0,dictFarm.item("FarmOwner"))
   Session("scenarioNumber") = arrFarm(0,dictFarm.item("SelectedScenario"))
   Session("scenarioName")   = arrFarm(0,dictFarm.item("ScenarioName"))
   Session("soilType")       = arrFarm(0,dictFarm.item("SoilType"))
   Session("irrigation")     = arrFarm(0,dictFarm.item("Irrigation"))
   if Session("irrigation") = "true" then 
      Session("irrigation") = 1
   else
      Session("irrigation") = 0
   end if
   Session("farmType")       = arrFarm(0,dictFarm.item("FarmType"))
   Session("yieldLevel")     = arrFarm(0,dictFarm.item("YieldLevel"))
   Session("tabNumber")      = 1
else
   Session("hasLogin")       = "no"
end if

if Session("hasLogin") = "yes" then
   Session("loginTry") = "yes"
'if (Session("userID") = "2") or (Session("userID") = "6") or (Session("userID") = "7") or (Session("userID") = "19") or (Session("userID") = "21") or (Session("userID") = "25") or (Session("userType") = 50) or (Session("userID") = "5050") or (Session("userID") = "6060") or (Session("userID") = "7070") or (Session("userID") = "8080") or (Session("userID") = "9090") then'bruges når der opdateres mellem servere 
'   if nFarm >1 then
   if Session("userType") >=100 then
	   Session.TimeOut					 = 40
     returnURL = "SelectFarm.asp"
   else
     returnURL = "start.asp"
   end if
'else'bruges når der opdateres mellem servere 
'   Session("loginTry") = "no"'bruges når der opdateres mellem servere 
'   returnURL = "default.asp"'bruges når der opdateres mellem servere 
'end if'bruges når der opdateres mellem servere 
else
   Session("loginTry") = "no"
   returnURL = "default.asp"
end if

Response.Redirect(returnURL)
Response.End
%>