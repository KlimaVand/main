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
Private Const PARAMETER_FE_CERNAL       = 1   'Cernal yield in parameter table
Private Const PARAMETER_FE_STRAW        = 2   'Straw yield in parameter table   
Private Const PARAMETER_FE_SECONDCROP   = 3   'Second crop yield in parameter table 
Private Const PARAMETER_N_CERNAL        = 4   'Cernal nitrogen content in parameter table 
Private Const PARAMETER_N_STRAW         = 5   'Straw nitrogen content in parameter table 
Private Const PARAMETER_N_SECONDCROP    = 6   'SecondCrop nitrogen content in parameter table 
Private Const PARAMETER_P_CERNAL        = 7  'Cernal Phosphor content in parameter table 
Private Const PARAMETER_P_STRAW         = 8  'Straw Phosphor content in parameter table 
Private Const PARAMETER_P_SECONDCROP    = 9  'Straw Phosphor content in parameter table 

Dim nCrop, NumberCrops, test
Dim CropArea,i  ' array of crop areas NUMBERCROPS
Dim arrCropAndArea 
Dim aFieldPlanRotationID
dim c, aFarmNumber, aScenarioID, aRotationID, aFieldPlanID, arrFieldPlan, returnValue

aFarmNumber  = Session("farmNumber")
aScenarioID  = Session("scenarioNumber")
aRotationID  = Request.QueryString("RotationID")
aFieldPlanID = Request.QueryString("FieldPlanID")

InitFarmNDB

' delete field plan record

Dim arrParameter
redim arrParameter(3)

	 arrParameter(0) = aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aRotationID
	 arrParameter(3) = aFieldPlanID

   returnValue = CallSPReturn ("DeleteFieldPlan",arrParameter)

redim arrParameter(2)

	 arrParameter(0) = aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aRotationID

    returnValue = CallSPReturn ("DeleteFieldPlanRotation",arrParameter)


    Dim dictFieldPlan, arrCropAndArea2, X
    set dictFieldPlan = CreateObject("Scripting.Dictionary")
    NumberCrops = CallSP ("GetFieldPlanCropAndAreaList " & Session("farmNumber")& "," & Session("scenarioNumber") & "," & aRotationID , arrCropAndArea2, dictFieldPlan)

if CInt(NumberCrops) = CInt(0) then 

   returnValue = CallSPReturn ("DeleteRotation",arrParameter)
'   DeleteRotation aFarmNumber, aScenarioID, aRotationID
else 

dim oSoapClient, resultVal
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

'    LPRotationModel arrCropAndArea2, dictFieldPlan

end if 'if nFieldPlan = 0 

redim preserve arrParameter(1)

   returnValue = CallSPReturn ("CalcFarmTotal",arrParameter)
'CalcFarmTotal aFarmNumber,aScenarioID

CloseFarmNDB
Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
'**************************************************************************************************
%>