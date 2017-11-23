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
Dim f, c, nFieldPlan, aFarmNumber, aScenarioID, aSoilTypeID, aIrrigation, aRotationIrrigation, aRotationID, aFieldPlanID, aCrop, aArea, aTotalArea, aSecondCrop, returnValue
Dim aRotationName
Dim nFieldPlanRotation, arrFieldPlanRotation
dim aYieldLevel, arrParameter, arrParameterFP
dim aAfterCrop
dim aFeedStuff,aStrawFeedStuff,aGrazingPart
dim aStartDate, aEndDate, rStartDate, rEndDate,rotationSpan
dim changes
dim currentLocale

currentLocale=getLocale()
setLocale("da")
'changes=request("changes")


aFarmNumber   = Session("farmNumber")
aScenarioID   = Session("scenarioNumber")
'aIrrigation   = Session("irrigation")
aSoilTypeID   = Session("soilType")
aYieldLevel	  = Session("yieldLevel")
aRotationID   = Request.querystring("RotationID")
if aRotationID = "" then aRotationID=1
'aRotationName   = Request.querystring("RotationID")
changes=request("changes_"&aRotationID&"")
aRotationIrrigation = Request.Form("Irrigation_"&aRotationID&"")
 if aRotationIrrigation = "" then aRotationIrrigation = 0
 if Request.Form("SoilType_"&aRotationID&"")<>Session("SoilType") then 
   aSoilTypeID  = Request.Form("SoilType_"&aRotationID&"")
 end if
 if Request.Form("YieldLevel_"&aRotationID&"")<>Session("yieldLevel") then 
   aYieldLevel  = Request.Form("YieldLevel_"&aRotationID&"")
 end if
' if aRotationIrrigation<>aIrrigation then 
'   aIrrigation  = aRotationIrrigation
' end if


aRotationName = Request.Form("RotationName_"&aRotationID&"")
nFieldPlan    = Request.Form("hiddenNFieldPlan_"&aRotationID&"")
aTotalArea    = Request.Form("inputArea_"&aRotationID&"")
'response.write Request.Form("Irrigation_"&aRotationID&"")
'response.write Request.QueryString("Irrigation_"&aRotationID&"")
InitFarmNDB
'update rotation record
redim arrParameter(7)
	 arrParameter(0) = aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aRotationID
	 arrParameter(3) = aRotationName
	 arrParameter(4) = aSoilTypeID
	 arrParameter(5) = 0'aIrrigation
	 arrParameter(6) = aYieldLevel
	 arrParameter(7) = Cdbl(aTotalArea)
   returnValue = CallSPReturn ("InsertUpdateRotation",arrParameter)

'InsertUpdateRotation aFarmNumber, aScenarioID, aRotationID, aRotationName, aSoilTypeID, aIrrigation, aYieldLevel

redim arrParameter(11)
	 arrParameter(0) = aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aRotationID
redim arrParameterFP(7)
	 arrParameterFP(0) = aFarmNumber
	 arrParameterFP(1) = aScenarioID
	 arrParameterFP(2) = aRotationID
'' update all field plan records
rStartDate=cstr(Request.Form("startDate_"&aRotationID&"_0"))
rEndDate=cstr(Request.Form("endDate_"&aRotationID&"_"&nFieldPlan-1&""))
'response.write DateDiff("yyyy",rStartDate,rEndDate)
if rStartDate = "" Then rotationSpan = 1
if rEndDate = "" Then 
rotationSpan = 1
else
rStartDate=cdate(cstr(Request.Form("startDate_"&aRotationID&"_0")))
rEndDate=cdate(cstr(Request.Form("endDate_"&aRotationID&"_"&nFieldPlan-1&"")))
rotationSpan = DateDiff("yyyy",rStartDate,rEndDate)
end if
if rotationSpan <= 0 then rotationSpan = 1
for f = 0 to nFieldPlan-1
   aFieldPlanID = Request.Form("hiddenFieldPlanID_"&aRotationID&"_"&f&"")
   aCrop        = Request.Form("selectCrop_"&aRotationID&"_"&f&"")
   aArea        = aTotalArea/rotationSpan'Round(Cdbl(,5))'''Request.Form("inputArea_"&aRotationID&"_"&f&"")
   aAfterCrop   = 0'Request.Form("selectAfterCrop_"&aRotationID&"_"&f&"")
   aFeedstuff   = Request.Form("selectCropFeedStuff_"&aRotationID&"_"&f&"")
   aStrawFeedStuff   = Request.Form("selectStrawFeedStuff_"&aRotationID&"_"&f&"")
   aIrrigation   = Request.Form("Irrigation_"&aRotationID&"_"&f&"")

'   if cint(aFeedstuff) > 1000 or cint(aStrawFeedStuff) > 1000 then 
'   aGrazingPart = 100
'   end if
   if aCrop=31 then
     aFeedstuff=0
	 aStrawFeedStuff=0
   end if
   aStartDate    = cStr(Request.Form("startDate_"&aRotationID&"_"&f&""))
   if aStartDate    <> "" then
    aStartDate = cStr(Mid(aStartDate,7,4)&"-"&Mid(aStartDate,4,2)&"-"&Mid(aStartDate,1,2))
'	response.write Mid(aStartDate,7,4)&"-"&Mid(aStartDate,4,2)&"-"&Mid(aStartDate,1,2)
   else
    aStartDate = null
   end if
   aEndDate      = Request.Form("endDate_"&aRotationID&"_"&f&"")
   if aEndDate    <> "" then
    aEndDate = cStr(Mid(aEndDate,7,4)&"-"&Mid(aEndDate,4,2)&"-"&Mid(aEndDate,1,2))
   else
    aEnddate = null
   end if
'   response.write "aEndDate "&aEndDate&"<br/>"
	 arrParameter(3) = aFieldPlanID
	 arrParameter(4) = aCrop
	 arrParameter(5) = aArea
	 arrParameter(6) = aAfterCrop
	 arrParameter(7) = aFeedstuff
	 arrParameter(8) = aStrawFeedstuff
	 arrParameter(9) = aStartDate
	 arrParameter(10) = aEndDate
     arrParameter(11) = aIrrigation
    arrParameterFP(3)=CInt(aFieldPlanID)
'    arrParameterFP(4)=2'CInt(aPreviousCropID)
'    arrParameterFP(5)=1'CInt(aPreCropOriginalID)
'    arrParameterFP(6)=1'CInt(aPreCrop_AfterCropID)
'    arrParameterFP(7)=0'CDbl(aPreCropSecondCrop)
    arrParameterFP(4)=CInt(aCrop)
    arrParameterFP(5)=CInt(aAfterCrop)
	arrParameterFP(6)=CDbl(aArea)
'    arrParameterFP(11)=0'CBool(aOrganicFertilizer)
'    arrParameterFP(12)=0'CInt(aGrazingPart)
'    arrParameterFP(13)=0'CInt(aSalePart)
'    arrParameterFP(14)=3'CInt(aStrawUseType)
if f=nFieldPlan-1 then
  aSecondCrop=Request.Form("selectCrop_"&aRotationID&"_0")
else
  aSecondCrop=Request.Form("selectCrop_"&aRotationID&"_"&f+1&"")
end if
    arrParameterFP(7)=CInt(aSecondCrop)
	if aStrawFeedStuff=0 then
	redim preserve arrParameterFP(8)
	arrParameterFP(8)=0
	end if
'    arrParameterFP(16)=0'CInt(aPrePreCropID)
	 

   returnValue = CallSPReturn ("InsertUpdateFieldPlan",arrParameter)
   returnValue=CallSPReturn("InsertUpdateFieldPlanRotationMinimum ", arrParameterFP)
   redim preserve arrParameterFP(7)
next

redim preserve arrParameter(3)
   returnValue  = CallSPReturn ("DeleteFieldPlanRotationSurplus ", arrParameter)
'redim preserve arrParameter(1)
'   returnValue = CallSPReturn ("CalcFarmTotal",arrParameter)

setLocale(currentLocale)
CloseFarmNDB
Session("ShowResult")=False
Response.redirect("start.asp")
Response.End
%>