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
Dim CropArea,i, aTotalArea  ' array of crop areas NUMBERCROPS
Dim arrCropAndArea 
Dim aFieldPlanRotationID
dim c, aFarmNumber, aScenarioID, aRotationID, aFieldPlanID, arrFieldPlan, returnValue
Dim nFieldPlan, f, aArea, aSecondCrop


aFarmNumber  = Session("farmNumber")
aScenarioID  = Session("scenarioNumber")
aRotationID  = Request.QueryString("RotationID")
aFieldPlanID = Request.QueryString("FieldPlanID")
aTotalArea   = Request.Form("inputArea_"&aRotationID&"")
nFieldPlan   = Request.Form("hiddenNFieldPlan_"&aRotationID&"")

InitFarmNDB

' delete field plan record

Dim arrParameter
redim arrParameter(3)

	 arrParameter(0) = aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aRotationID
	 arrParameter(3) = aFieldPlanID

   returnValue = CallSPReturn ("DeleteFieldPlan",arrParameter)

'redim arrParameter(2)
'
'	 arrParameter(0) = aFarmNumber
'	 arrParameter(1) = aScenarioID
'	 arrParameter(2) = aRotationID

    returnValue = CallSPReturn ("DeleteFieldPlanRotation",arrParameter)


    Dim dictFieldPlan, arrCropAndArea2, X
    set dictFieldPlan = CreateObject("Scripting.Dictionary")
    NumberCrops = CallSP ("GetFieldPlanCropAndAreaList " & Session("farmNumber")& "," & Session("scenarioNumber") & "," & aRotationID , arrCropAndArea2, dictFieldPlan)

if CInt(NumberCrops) = CInt(0) then 
redim preserve arrParameter(2)

   returnValue = CallSPReturn ("DeleteRotation",arrParameter)
'   DeleteRotation aFarmNumber, aScenarioID, aRotationID
else 
redim preserve arrParameter(4)
'for f = 0 to nFieldPlan-2
'   aFieldPlanID = Request.Form("hiddenFieldPlanID_"&aRotationID&"_"&f&"")
'   aArea        = Round(Cdbl(aTotalArea/(nFieldPlan-1)),5)'Request.Form("inputArea_"&aRotationID&"_"&f&"")
'   arrParameter(3)=aFieldPlanID
'   arrParameter(4)=aArea
'	 
'
''   returnValue = CallSPReturn ("InsertUpdateFieldPlan",arrParameter)
''   returnValue=CallSPReturn("InsertUpdateFieldPlanRotation ", arrParameterFP)
'   returnValue  = CallSPReturn("UpdateFieldPlanArea ",arrParameter)
'   returnValue  = CallSPReturn("UpdateFieldPlanRotationArea ",arrParameter)
'
'next
'   aFieldPlanID   = Request.Form("hiddenFieldPlanID_"&aRotationID&"_"&nFieldPlan-2&"")
'   aSecondCrop    = Request.Form("selectCrop_"&aRotationID&"_0")
'   arrParameter(3)=aFieldPlanID
'   arrParameter(4)=aSecondCrop
'   returnValue  = CallSPReturn("UpdateFieldPlanRotationSecondCrop ",arrParameter)
   

end if 'if nFieldPlan = 0 

redim preserve arrParameter(1)

   returnValue = CallSPReturn ("CalcFarmTotal",arrParameter)
'CalcFarmTotal aFarmNumber,aScenarioID

CloseFarmNDB
Session("ShowResult")=False
Response.Redirect("start.asp")
Response.End
'**************************************************************************************************
%>