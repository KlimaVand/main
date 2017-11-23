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
dim aFarmNumber, aScenarioID, aScenarioName, arrID, nID, aFarmOwnerName, aFarmOwnerAddress, aFarmOwnerZipCode, aFarmOwnerTown
dim aSoilType, aIrrigationType, aDelivery, aFarmType, aSelectedScenario, returnValue
dim aYieldLevel
dim MaxScenarioID, NewScenarioID, sc, dictMaxID, arr

InitFarmNDB
CreateLanguageDictionary
if request("newScenarioName")<>"" then

redim arr(2)

	 arr(0) = Session("farmNumber")
	 arr(1) = Session("scenarioNumber")
	 arr(2) = request("newScenarioName")

   returnValue = CallSPReturn ("InsertUpdateScenario",arr)

aFarmNumber       = Session("farmNumber")
aSelectedScenario = Session("scenarioNumber")
'returnValue   = InsertUpdateScenario(aFarmNumber, aSelectedScenario, request("newScenarioName"))
Session("scenarioName") = request("newScenarioName")
end if

CloseFarmNDB
BeginHeader str.Item("strRENAME"),"",""
CreateStyleLink "styles/stylesheet.css"
EndHeader
BeginBodyData

'   Response.Write("<form action=""FarmScenarioNameUpdate.asp"" method = ""post"" target=""window.opener.name"" onSubmit=""window.close();"">")   
   Response.Write("<form action=""FarmScenarioNameUpdate.asp"" method = ""post"" target=""FarmNparent"" onSubmit=""window.close();"">")   
   Response.Write("<input type = ""text"" size=""30"" name=""newScenarioName"" value="""&Session("scenarioName")&"""  />")   
   Response.Write("<input type = ""submit"" value=""OK""  />")   
   Response.Write("</form>")   
   Response.Write("<form name=""update"">")   
   Response.Write("<input type = ""hidden"" name=""saveUpdates"" value=""true""  />")   
   Response.Write("</form>")   
EndBody

if request("newScenarioName")<>"" then
  Response.Redirect("start.asp")
  Response.End
end if
%>