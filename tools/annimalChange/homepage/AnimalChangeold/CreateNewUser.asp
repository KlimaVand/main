<%
'**************************************************************************************************
' Author: Margit Jorgensen, Danish Institute of Agricultural Sciences
' Description: Create new user, new farm and new scenario, and logging in new user
'**************************************************************************************************
option explicit
%>
<!-- #include file="includes/General.asp" -->
<!-- #include file="includes/adovbs.asp" -->
<!-- #include file="includes/dbInterface.asp" -->
<%
dim aUserID
dim nUser, arrUser, nFarm, arrFarm,returnURL
Dim arrParameter, returnValue
Dim dictUser, dictFarm

aUserID=Right(CStr(round(CDbl(Cdate(date+time))*100000)),9)
'response.write "New user:" & aUserID
InitFarmNDB

redim arrParameter(3)

	 arrParameter(0) = aUserID
	 arrParameter(1) = "zzz"
	 arrParameter(2) = aUserID
	 arrParameter(3) = "Demo User"


   returnValue = CallSPReturn ("InsertNewUser",arrParameter)

    set dictUser = CreateObject("Scripting.Dictionary")
    nUser = CallSP ("GetUserByLogin " & "zzz" & "," & aUserID, arrUser, dictUser)
response.write "<br> nUser "& nUser 
'	nUser = GetUserByLogin("zzz", aUserID, arrUser)
	if CInt(nUser) = CInt(1) then
	   aUserID = arrUser(0,dictUser.item("UserID"))
	
       set dictFarm = CreateObject("Scripting.Dictionary")
       nFarm = CallSPStr ("GetFarmList " & aUserID, arrFarm, dictFarm)	   
'	   nFarm = GetFarmList(aUserID,arrFarm)
	end if

 
CloseFarmNDB
if CInt(nUser) = CInt(1) then
   Session("hasLogin")       = "yes"
   Session("user")           = arrUser(0,dictUser.item("UserName"))
   Session("farmNumber")     = arrFarm(0,dictFarm.item("FarmNumber"))
   Session("farmOwner")      = arrFarm(0,dictFarm.item("FarmOwnerName"))
   Session("scenarioNumber") = arrFarm(0,dictFarm.item("SelectedScenario"))
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
'	 Session.TimeOut					 = 40
else
   Session("hasLogin")       = "no"
end if

if Session("hasLogin") = "yes" then
   Session("loginTry") = "yes"
   returnURL = "start.asp"
else
   Session("loginTry") = "no"
   returnURL = "default.asp"
end if

Response.Redirect(returnURL)
Response.End
%>