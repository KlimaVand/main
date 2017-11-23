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
dim aFarmNumber, aScenarioID, aSoilTypeID, aIrrigationType, aRotationID, aFieldPlanID, aCrop, aArea, returnValue
dim nID, arrID, dictID
dim nFarm, arrFarm, dictFarm
dim aYieldLevel, arr
dim aAfterCrop

aFarmNumber = Session("farmNumber")
aScenarioID = Session("scenarioNumber")
set dictFarm = CreateObject("Scripting.Dictionary")

InitFarmNDB

nFarm = CallSP ("GetFarmData "&aFarmNumber,arrFarm,dictFarm)
'nFarm = GetFarmData(aFarmNumber, arrFarm)

aSoilTypeID = arrFarm(0,dictFarm.Item("SoilType"))
aIrrigationType = arrFarm(0,dictFarm.Item("Irrigation"))
aYieldLevel = arrFarm(0,dictFarm.Item("YieldLevel"))
'response.write aIrrigationType


set dictID = CreateObject("Scripting.Dictionary")

nID = CallSP ("GetRotationMaxID "&aFarmNumber & ", " & aScenarioID,arrID,dictID)

'nID          = GetRotationMaxID(aFarmNumber, aScenarioID, arrID)
if IsNull(arrID(0,dictID.Item("MaxID"))) then
   aRotationID  = 1
else
   aRotationID  = arrID(0,dictID.Item("MaxID")) + 1
end if

ReDim arr(6)
   arr(0) = aFarmNumber
   arr(1) = aScenarioID
   arr(2) = aRotationID
   arr(3) = aRotationID
   arr(4) = aSoilTypeID
   arr(5) = aIrrigationType
   arr(6) = aYieldLevel

   returnValue = CallSPReturn ("InsertUpdateRotation",arr)

' insert new rotation record
' returnValue  = InsertUpdateRotation(aFarmNumber, aScenarioID, aRotationID, aRotationID, aSoilTypeID, aIrrigationType, aYieldLevel)

aFieldPlanID = 1
aCrop        = 1
aArea        = 0
aAfterCrop	 = 0


ReDim arr(11)
   arr(0) = aFarmNumber
   arr(1) = aScenarioID
   arr(2) = aRotationID
   arr(3) = aFieldPlanID
   arr(4) = aCrop
   arr(5) = aArea
   arr(6) = aAfterCrop
	 arr(7) = 0
	 arr(8) = 0
	 arr(9) = CDate(Now)
	 arr(10) = CDate(Now)
	 arr(11) = false


   returnValue = CallSPReturn ("InsertUpdateFieldPlan",arr)

'returnValue  = InsertUpdateFieldPlan(aFarmNumber, aScenarioID, aRotationID, aFieldPlanID, aCrop, aArea)

'Response.Write("<h3>aFarmNumber: "&aFarmNumber&"</h3>")
'Response.Write("<h3>aScenarioID: "&aScenarioID&"</h3>")
'Response.Write("<h3>nID: "&nID&"</h3>")
'Response.Write("<h3>aRotationID: "&aRotationID&"</h3>")
'Response.Write("<h3>aFieldPlanID: "&aFieldPlanID&"</h3>")
'Response.Write("<h3>aCrop: "&aCrop&"</h3>")
'Response.Write("<h3>aArea: "&aArea&"</h3>")
redim preserve arr(16)
'    aFieldPlanID = nFieldPlan+1'arrID(0,dictID.item("MaxID"))+1
'    aCrop        = arrA(0,dictA.item("CropID"))
'    aSecondCrop=Request.Form("selectCrop_"&aRotationID&"_0")
	aAfterCrop	 = 0
    arr(3)=CInt(aFieldPlanID)
    arr(4)=2'CInt(aPreviousCropID)
    arr(5)=1'CInt(aPreCropOriginalID)
    arr(6)=1'CInt(aPreCrop_AfterCropID)
    arr(7)=0'CDbl(aPreCropSecondCrop)
    arr(8)=CInt(aCrop)
    arr(9)=CInt(aAfterCrop)
	arr(10)=CDbl(aArea)
    arr(11)=0'CBool(aOrganicFertilizer)
    arr(12)=0'CInt(aGrazingPart)
    arr(13)=0'CInt(aSalePart)
    arr(14)=3'CInt(aStrawUseType)
    arr(15)=aCrop'arrA(0,dictA.item("CropID"))
    arr(16)=0'CInt(aPrePreCropID)
    returnValue=CallSPReturn("InsertUpdateFieldPlanRotation ", arr)
'Response.Write("<h3>aFarmNumber: "&aFarmNumber&"</h3>")
'Response.Write("<h3>aScenarioID: "&aScenarioID&"</h3>")
'Response.Write("<h3>nID: "&nID&"</h3>")
'Response.Write("<h3>aRotationID: "&aRotationID&"</h3>")
'Response.Write("<h3>aFieldPlanID: "&aFieldPlanID&"</h3>")
'Response.Write("<h3>aCrop: "&aCrop&"</h3>")
'Response.Write("<h3>aArea: "&aArea&"</h3>")


CloseFarmNDB
Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
%>