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
<!-- #include file="includes/FunctionField.asp" -->
<%
'<!-- #include file="includes/LPRotationModel.asp" -->
'<!-- #include file="includes/LPRotation2.asp" -->
%>
<%
'Yield
'Private Const PARAMETER_FE_CERNAL       = 1   'Cernal yield in parameter table
'Private Const PARAMETER_FE_STRAW        = 2   'Straw yield in parameter table   
'Private Const PARAMETER_FE_SECONDCROP   = 3   'Second crop yield in parameter table 
'Private Const PARAMETER_N_CERNAL        = 4   'Cernal nitrogen content in parameter table 
'Private Const PARAMETER_N_STRAW         = 5   'Straw nitrogen content in parameter table 
'Private Const PARAMETER_N_SECONDCROP    = 6   'SecondCrop nitrogen content in parameter table 
'Private Const PARAMETER_P_CERNAL        = 7  'Cernal Phosphor content in parameter table 
'Private Const PARAMETER_P_STRAW         = 8  'Straw Phosphor content in parameter table 
'Private Const PARAMETER_P_SECONDCROP    = 9  'Straw Phosphor content in parameter table 

Dim NumberCrops
Dim aFieldPlanRotationID
Dim f, c, nFieldPlan, aFarmNumber, aScenarioID, aSoilTypeID, aIrrigation, aRotationIrrigation, aRotationID, aFieldPlanID, aCrop, aArea, returnValue
Dim aRotationName
Dim nFieldPlanRotation, arrFieldPlanRotation
dim aYieldLevel, arrParameter
dim aAfterCrop

aFarmNumber   = Session("farmNumber")
aScenarioID   = Session("scenarioNumber")
aIrrigation   = Session("irrigation")
aSoilTypeID   = Session("soilType")
aYieldLevel	  = Session("yieldLevel")
aRotationID   = Request.querystring("RotationID")
'aRotationName   = Request.querystring("RotationID")
aRotationIrrigation = Request.Form("Irrigation_"&aRotationID&"")
 if aRotationIrrigation = "" then aRotationIrrigation = 0
 if Request.Form("SoilType_"&aRotationID&"")<>Session("SoilType") then 
   aSoilTypeID  = Request.Form("SoilType_"&aRotationID&"")
 end if
 if Request.Form("YieldLevel_"&aRotationID&"")<>Session("yieldLevel") then 
   aYieldLevel  = Request.Form("YieldLevel_"&aRotationID&"")
 end if
 if aRotationIrrigation<>aIrrigation then 
   aIrrigation  = aRotationIrrigation
 end if
aRotationName = Request.Form("RotationName_"&aRotationID&"")
nFieldPlan    = Request.Form("hiddenNFieldPlan_"&aRotationID&"")
'response.write Request.Form("Irrigation_"&aRotationID&"")
'response.write Request.QueryString("Irrigation_"&aRotationID&"")
InitFarmNDB
'update rotation record

redim arrParameter(6)
	 arrParameter(0) = aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aRotationID
	 arrParameter(3) = aRotationName
	 arrParameter(4) = aSoilTypeID
	 arrParameter(5) = aIrrigation
	 arrParameter(6) = aYieldLevel
   returnValue = CallSPReturn ("InsertUpdateRotation",arrParameter)

'InsertUpdateRotation aFarmNumber, aScenarioID, aRotationID, aRotationName, aSoilTypeID, aIrrigation, aYieldLevel

redim arrParameter(6)
	 arrParameter(0) = aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aRotationID
' update all field plan records
for f = 0 to nFieldPlan-1
   aFieldPlanID = Request.Form("hiddenFieldPlanID_"&aRotationID&"_"&f&"")
   aCrop        = Request.Form("selectCrop_"&aRotationID&"_"&f&"")
   aArea        = Request.Form("inputArea_"&aRotationID&"_"&f&"")
   aAfterCrop   = Request.Form("selectAfterCrop_"&aRotationID&"_"&f&"")
	 arrParameter(3) = aFieldPlanID
	 arrParameter(4) = aCrop
	 arrParameter(5) = aArea
	 arrParameter(6) = aAfterCrop

   returnValue = CallSPReturn ("InsertUpdateFieldPlan",arrParameter)
'   returnValue  = InsertUpdateFieldPlan(aFarmNumber, aScenarioID, aRotationID, aFieldPlanID, aCrop, aArea)

next


'Dim dictFieldPlan, arrCropAndArea2, X
'set dictFieldPlan = CreateObject("Scripting.Dictionary")
'NumberCrops = CallSP ("GetFieldPlanCropAndAreaList " & Session("farmNumber")& "," & Session("scenarioNumber") & "," & aRotationID , arrCropAndArea2, dictFieldPlan)

dim oSoapClient, resultVal, tid1,tid2
'Kræver at SoapToolkit 3.0 er installeret på serveren
set oSoapClient = server.CreateObject("MSSOAP.SoapClient30")
oSoapClient.ClientProperty("ServerHTTPRequest") = True
if Cstr(request.servervariables("SERVER_NAME")) =   "172.20.107.138" then
  oSoapClient.MSSoapInit("http://172.23.173.223/FarmN_webservice/XPress.asmx?WSDL")'opdaterer FarmNTest - gælder på Freja2
else
  oSoapClient.MSSoapInit("http://172.23.173.223/FarmN_WebService_ver2/Farmnwebservice.asmx?WSDL")'opdaterer FarmNTest - gælder på Blade6
end If
'tid1 = Timer
resultVal = oSoapClient.RotXPress(Clng(aFarmNumber), Cint(aScenarioID), Cint(aRotationID))
set oSoapClient = Nothing
'tid2 = Timer
'Response.write("Test af tid " & tid2-tid1)

'response.write "<br> test af server " & request.servervariables("SERVER_NAME")

'LPRotationModel arrCropAndArea2, dictFieldPlan

redim preserve arrParameter(1)

   returnValue = CallSPReturn ("CalcFarmTotal",arrParameter)
'CalcFarmTotal aFarmNumber,aScenarioID

CloseFarmNDB
'Response.write("data.asp?TB="&session("tabNumber")&"")
Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
%>