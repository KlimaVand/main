<%
option explicit
%>
<!-- #include file="includes/General.asp" -->
<!-- #include file="includes/adovbs.asp" -->
<!-- #include file="includes/dbInterface.asp" -->
<%
dim skemaID, arrVVM, dictVVM, x
dim aUserLoginName,aUserPassword,dictUser,arrUser,nUser,returnURL,aUserID,nFarm,arrFarm,dictFarm,version

 skemaID=request("skemaID")
 aUserPassword = ""
' response.write cstr(Request.ServerVariables("HTTP_REFERER"))
' response.write cstr(Request.ServerVariables("REMOTE_HOST"))
'if request.servervariables("REMOTE_HOST") = "172.20.153.187" or instr(cstr(Request.ServerVariables("HTTP_REFERER")),"servervariableslink.asp")<>0 then'hvis forespørgsel kommer fra husdyrgodkendelse eller fra mig
'if request.servervariables("REMOTE_HOST") = "172.20.153.187" or request.servervariables("REMOTE_HOST") ="130.226.173.129" or instr(cstr(Request.ServerVariables("HTTP_REFERER")),"husdyrgodkendelse.dk")<>0 then'hvis forespørgsel kommer fra husdyrgodkendelse eller fra mig
 if skemaID<>"" then 
   InitFarmNDB
   set dictVVM = CreateObject("Scripting.Dictionary")
   x = CallSP ("GetVVM_ID " & Cstr(skemaID), arrVVM, dictVVM)	   
   if x>0 then aUserPassword = arrVVM(0,dictVVM.item("FarmNumber"))
 end if



    set dictUser = CreateObject("Scripting.Dictionary")
    nUser = CallSP ("GetUserByLogin 'VVM','"&aUserPassword&"'", arrUser, dictUser)
	if CInt(nUser) = CInt(1) then
	   aUserID = arrUser(0,dictUser.item("UserID"))
	
       set dictFarm = CreateObject("Scripting.Dictionary")
       nFarm = CallSPStr ("GetFarmList '" & aUserID&"'", arrFarm, dictFarm)	   
	end if
if CInt(nUser) = CInt(1) then
version=arrFarm(0,dictFarm.item("ManureVersion"))
if version = "" then version = request("version")
if version = "" then version = 1
Session("version")=version

   Session("hasLogin")       = "yes"
   Session("userID")         = arrUser(0,dictUser.item("UserID"))
   Session("nFarm")          = nFarm
   Session("user")           = arrUser(0,dictUser.item("UserName"))
   Session("userType")       = arrUser(0, dictUser.item("UserType"))'new Session-variable 14-06-06
   Session("farmNumber")     = arrFarm(0,dictFarm.item("FarmNumber"))
   Session("farmOwner")      = arrFarm(0,dictFarm.item("FarmOwnerName"))
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
	 Session.TimeOut					 = 40
else
   Session("hasLogin")       = "no"
end if
'end if
if Session("hasLogin") = "yes" then
   Session("loginTry") = "yes"
   returnURL = "start.asp?version="&session("version")&""
else
   Session("loginTry") = "no"
   returnURL = "default.asp"
end if
response.redirect(returnURL)
response.end
%>
