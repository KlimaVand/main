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
<!-- #include file="includes/RotationModel2.asp" -->
<%
Dim nCrop, arrCrop, CropString, NumberCrops, test
Dim nYield, arrYield
Dim ObjectCoeff   ' matrix of objective function coefficients NUMBERCROPS * NUMBERCROPS
                  ' -10000 indicates that it is an impossible combination of precrop and crop
Dim CropArea, i, j, k  ' array of crop areas NUMBERCROPS
Dim arrCropAndArea, arrPreCrop
Dim arrRotationList, aFieldPlanRotationID
dim c, aFarmNumber, aScenarioID, aSoilTypeID, aRotationID,aRotationName, aFieldPlanID, aCrop, aArea, returnValue
dim nID, arrID, nFieldPlan, f, aIrrigation
dim nAreaSum, arrAreaSum, aTotalArea
dim aYieldLevel
Dim arrParameter
dim aAfterCrop, aSecondCrop, aGrazingPart, aOrganicFertilizer
dim aFeedStuff,aStrawFeedStuff
dim aStartDate, aEndDate, rStartDate, rEndDate,rotationSpan
dim changes
dim currentLocale

currentLocale=getLocale()
setLocale("da")
changes=request("changes")

aFarmNumber = Session("farmNumber")
aScenarioID = Session("scenarioNumber")
aSoilTypeID = Session("soilType")
aIrrigation = Session("irrigation")
aYieldLevel	  = Session("yieldLevel")
aRotationID = Request.QueryString("RotationID")
 if Request.Form("SoilType_"&aRotationID&"")<>Session("soilType") then 
   aSoilTypeID  = Request.Form("SoilType_"&aRotationID&"")
 end if
 if Request.Form("Irrigation_"&aRotationID&"")<>Session("irrigation") then 
   aIrrigation  = Request.Form("Irrigation_"&aRotationID&"")
 end if
 if Request.Form("YieldLevel_"&aRotationID&"")<>Session("yieldLevel") then 
   aYieldLevel  = Request.Form("YieldLevel_"&aRotationID&"")
 end if
aRotationName = Request.Form("RotationName_"&aRotationID&"")
nFieldPlan  = Request.Form("hiddenNFieldPlan_"&aRotationID&"")
aTotalArea    = Request.Form("inputArea_"&aRotationID&"")

InitFarmNDB
'update rotation record
redim arrParameter(7)
	 arrParameter(0) = aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aRotationID
	 arrParameter(3) = aRotationName
	 arrParameter(4) = aSoilTypeID
	 arrParameter(5) = aIrrigation
	 arrParameter(6) = aYieldLevel
	 arrParameter(7) = aTotalArea
   returnValue = CallSPReturn ("InsertUpdateRotation",arrParameter)

redim arrParameter(10)
	 arrParameter(0) = aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aRotationID
	 
'' update all field plan records
'for f = 0 to nFieldPlan-1
'   aFieldPlanID = Request.Form("hiddenFieldPlanID_"&aRotationID&"_"&f&"")
'   aCrop        = Request.Form("selectCrop_"&aRotationID&"_"&f&"")
'   aArea        = aTotalArea
''   aArea        = Round(Cdbl(aTotalArea/(nFieldPlan+1)),5)'Request.Form("inputArea_"&aRotationID&"_"&f&"")
'   aAfterCrop   = Request.Form("selectAfterCrop_"&aRotationID&"_"&f&"")
'	 arrParameter(3) = aFieldPlanID
'	 arrParameter(4) = aCrop
'	 arrParameter(5) = aArea
'	 arrParameter(6) = aAfterCrop
'	 arrParameter(7) = Request.Form("selectCropFeedStuff_"&aRotationID&"_"&f&"")

'   returnValue = CallSPReturn ("InsertUpdateFieldPlan",arrParameter) istedet for denne skal opdateringsprocedure fra FieldPlanUpdate.asp bruges
'   returnValue  = InsertUpdateFieldPlan(aFarmNumber, aScenarioID, aRotationID, aFieldPlanID, aCrop, aArea)
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
if changes<>"" then
for f = 0 to nFieldPlan-1
   aFieldPlanID = Request.Form("hiddenFieldPlanID_"&aRotationID&"_"&f&"")
   aCrop        = Request.Form("selectCrop_"&aRotationID&"_"&f&"")
   aArea        = aTotalArea/rotationSpan'Round(Cdbl(,5))'''Request.Form("inputArea_"&aRotationID&"_"&f&"")
   aAfterCrop   = 0'Request.Form("selectAfterCrop_"&aRotationID&"_"&f&"")
   aFeedstuff   = Request.Form("selectCropFeedStuff_"&aRotationID&"_"&f&"")
   aStrawFeedStuff   = Request.Form("selectStrawFeedStuff_"&aRotationID&"_"&f&"")
   
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
	 arrParameter(3) = aFieldPlanID
	 arrParameter(4) = aCrop
	 arrParameter(5) = aArea
	 arrParameter(6) = aAfterCrop
	 arrParameter(7) = aFeedstuff
	 arrParameter(8) = aStrawFeedstuff
	 arrParameter(9) = aStartDate
	 arrParameter(10) = aEndDate
   returnValue = CallSPReturn ("InsertUpdateFieldPlan",arrParameter)

next
end if
' insert new field plan record

Dim dictID, arrA, dictA
   set dictA = CreateObject("Scripting.Dictionary")
   nID = CallSP ("GetFirstUnusedCrop " & aFarmNumber & "," & aScenarioID & "," & aRotationID,arrA,dictA)

'   set dictID = CreateObject("Scripting.Dictionary")
'   nID = CallSP ("GetFieldPlanMaxID " & aFarmNumber & "," & aScenarioID & "," & aRotationID,arrID,dictID)

'nID          = GetFieldPlanMaxID(aFarmNumber, aScenarioID, aRotationID, arrID)
aFieldPlanID = nFieldPlan+1'arrID(0,dictID.item("MaxID"))+1
aCrop        = arrA(0,dictA.item("CropID"))
aArea        = aTotalArea
'aArea        = Round(Cdbl(aTotalArea/(nFieldPlan+1)),5)
aAfterCrop	 = 0

	 arrParameter(3) = aFieldPlanID
	 arrParameter(4) = aCrop
	 arrParameter(5) = aArea
	 arrParameter(6) = aAfterCrop
	 arrParameter(9) = NULL
	 arrParameter(10) = NULL


   returnValue = CallSPReturn ("InsertUpdateFieldPlan",arrParameter)
'   response.write "returnValue "&returnValue
'   response.write aFieldPlanID
redim preserve arrParameter(7)


for f = 0 to nFieldPlan-1
   aFieldPlanID = Request.Form("hiddenFieldPlanID_"&aRotationID&"_"&f&"")
   aCrop        = Request.Form("selectCrop_"&aRotationID&"_"&f&"")
   aArea        = Request.Form("inputArea_"&aRotationID&"_"&f&"")'Round(Cdbl(aTotalArea/(nFieldPlan+1)),5)
   aAfterCrop   = Request.Form("selectAfterCrop_"&aRotationID&"_"&f&"")
'   aOrganicFertilizer 	= Request.Form("checkboxOrganicFertilizer_"&aRotationID&"_"&f&"")
'   aGrazingPart			= Request.Form("inputGrazingPart_"&aRotationID&"_"&f&"")
    arrParameter(3)=CInt(aFieldPlanID)
'    arrParameter(4)=2'CInt(aPreviousCropID)
'    arrParameter(5)=1'CInt(aPreCropOriginalID)
'    arrParameter(6)=1'CInt(aPreCrop_AfterCropID)
'    arrParameter(7)=0'CDbl(aPreCropSecondCrop)
    arrParameter(4)=CInt(aCrop)
    arrParameter(5)=CInt(aAfterCrop)
	arrParameter(6)=CDbl(aArea)
'    arrParameter(11)=-99'CDbl(aOrganicFertilizer)
'    arrParameter(12)=-99'CDbl(aGrazingPart)
'    arrParameter(13)=0'CInt(aSalePart)
'    arrParameter(14)=3'CInt(aStrawUseType)
if f=nFieldPlan-1 then
  aSecondCrop=arrA(0,dictA.item("CropID"))
else
    aSecondCrop=Request.Form("selectCrop_"&aRotationID&"_"&f+1&"")
end if
    arrParameter(7)=CInt(aSecondCrop)
'    arrParameter(16)=0'CInt(aPrePreCropID)
	 

   returnValue=CallSPReturn("InsertUpdateFieldPlanRotationMinimum ", arrParameter)
'   returnValue  = InsertUpdateFieldPlan(aFarmNumber, aScenarioID, aRotationID, aFieldPlanID, aCrop, aArea)
'response.write "returnValue "&returnValue
next
redim preserve arrParameter(16)
    aFieldPlanID = nFieldPlan+1'arrID(0,dictID.item("MaxID"))+1
    aCrop        = arrA(0,dictA.item("CropID"))
    aSecondCrop=Request.Form("selectCrop_"&aRotationID&"_0")
	aAfterCrop	 = 0
    arrParameter(3)=CInt(aFieldPlanID)
    arrParameter(4)=2'CInt(aPreviousCropID)
    arrParameter(5)=1'CInt(aPreCropOriginalID)
    arrParameter(6)=1'CInt(aPreCrop_AfterCropID)
    arrParameter(7)=0'CDbl(aPreCropSecondCrop)
    arrParameter(8)=CInt(aCrop)
    arrParameter(9)=CInt(aAfterCrop)
	arrParameter(10)=CDbl(aArea)
    arrParameter(11)=0'CBool(aOrganicFertilizer)
    arrParameter(12)=0'CInt(aGrazingPart)
    arrParameter(13)=0'CInt(aSalePart)
    arrParameter(14)=3'CInt(aStrawUseType)
    arrParameter(15)=arrA(0,dictA.item("CropID"))
    arrParameter(16)=0'CInt(aPrePreCropID)
    returnValue=CallSPReturn("InsertUpdateFieldPlanRotation ", arrParameter)
  
response.write "returnValue "&returnValue


CloseFarmNDB
Session("ShowResult")=False
Response.Redirect("start.asp")
Response.End
%>