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
dim nAreaSum, arrAreaSum
dim aYieldLevel
Dim arrParameter
dim aAfterCrop

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

' insert new field plan record

Dim dictID, arrA, dictA
   set dictA = CreateObject("Scripting.Dictionary")
   nID = CallSP ("GetFirstUnusedCrop " & aFarmNumber & "," & aScenarioID & "," & aRotationID,arrA,dictA)

   set dictID = CreateObject("Scripting.Dictionary")
   nID = CallSP ("GetFieldPlanMaxID " & aFarmNumber & "," & aScenarioID & "," & aRotationID,arrID,dictID)

'nID          = GetFieldPlanMaxID(aFarmNumber, aScenarioID, aRotationID, arrID)
aFieldPlanID = arrID(0,dictID.item("MaxID"))+1
aCrop        = arrA(0,dictA.item("CropID"))
aArea        = 0
aAfterCrop	 = 0

	 arrParameter(3) = aFieldPlanID
	 arrParameter(4) = aCrop
	 arrParameter(5) = aArea
	 arrParameter(6) = aAfterCrop

   returnValue = CallSPReturn ("InsertUpdateFieldPlan",arrParameter)


CloseFarmNDB
Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
%>