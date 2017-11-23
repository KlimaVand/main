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

dim f, c, nFieldPlanRotation, aFarmNumber, aScenarioID, aSoilTypeID, aRotationID, aFieldPlanRotationID, aPreviousCropID, aPreCropOriginalID, aCropID, aArea, aOrganicFertilizer, aGrazingPart, aSalePart, aStrawUseType, returnValue
dim nAreaSum, arrAreaSum, aSecondCrop
Dim aPreCropSecondCrop, aPreCrop_AfterCropID, aCrop_AfterCrop, aPrePreCropID
Dim aMainYield, aCatchYield, aStrawYield
Dim arrParameter
ReDim arrParameter(19)
aFarmNumber        = Session("farmNumber")
aScenarioID        = Session("scenarioNumber")
aRotationID        = Request.QueryString("RotationID")
nFieldPlanRotation = Request.Form("hiddenNFieldPlanRotation_"&aRotationID&"")
'response.write request.form
InitFarmNDB

for f = 0 to nFieldPlanRotation-1
   aFieldPlanRotationID = Request.Form("hiddenFieldPlanRotationID_"&aRotationID&"_"&f&"")
   aPreviousCropID      = 2'Request.Form("hiddenPreviousCropID_"&aRotationID&"_"&f&"")
   aPreCropOriginalID   = 1'Request.Form("hiddenPreCropOriginalID_"&aRotationID&"_"&f&"")
   aPreCropSecondCrop   = 0'Request.Form("PreCropSecondCrop_"&aRotationID&"_"&f&"")
   aPreCrop_AfterCropID = 1'Request.Form("hiddenPreCrop_AfterCropID_"&aRotationID&"_"&f&"")
   aPrePreCropID		= 0'Request.Form("hiddenPrePreCropID_"&aRotationID&"_"&f&"")

   aCropID              = Request.Form("hiddenCropID_"&aRotationID&"_"&f&"")
   aCrop_AfterCrop      = Request.Form("hiddenCrop_AfterCropID_"&aRotationID&"_"&f&"")
   aArea                = Request.Form("hiddenArea_"&aRotationID&"_"&f&"")
   aOrganicFertilizer   = Request.Form("checkboxOrganicFertilizer_"&aRotationID&"_"&f&"")
   if aOrganicFertilizer = "" then aOrganicFertilizer = 0
'      aOrganicFertilizer = 1'True
'   else
'      aOrganicFertilizer = False
'   end if
   aGrazingPart         = Request.Form("inputGrazingPart_"&aRotationID&"_"&f&"")
   aSalePart            = Request.Form("inputSalePart_"&aRotationID&"_"&f&"")
   aStrawUseType        = Request.Form("selectStrawUseType_"&aRotationID&"_"&f&"")
   aSecondCrop			= Request.Form("SecondCrop_"&aRotationID&"_"&f&"")
   aMainYield           = Request.Form("mainYield_"&aRotationID&"_"&f&"")
   aCatchYield           = Request.Form("catchYield_"&aRotationID&"_"&f&"")
   aStrawYield           = Request.Form("strawYield_"&aRotationID&"_"&f&"")
'   response.write request.form
	arrParameter(0)=Session("farmNumber")
	arrParameter(1)=CInt(aScenarioID)
	arrParameter(2)=CInt(aRotationID)
    arrParameter(3)=CInt(aFieldPlanRotationID)
    arrParameter(4)=CInt(aPreviousCropID)
    arrParameter(5)=CInt(aPreCropOriginalID)
    arrParameter(6)=CInt(aPreCrop_AfterCropID)
    arrParameter(7)=CDbl(aPreCropSecondCrop)
    arrParameter(8)=CInt(aCropID)
    arrParameter(9)=CInt(aCrop_AfterCrop)
	arrParameter(10)=CDbl(aArea)
    arrParameter(11)=CDbl(aOrganicFertilizer)
    arrParameter(12)=CDbl(aGrazingPart)
    arrParameter(13)=CDbl(aSalePart)
    arrParameter(14)=CInt(aStrawUseType)
    arrParameter(15)=CInt(aSecondCrop)
    arrParameter(16)=CInt(aPrePreCropID)

    arrParameter(17)=Cdbl(aMainYield)
    arrParameter(18)=Cdbl(aCatchYield)
    arrParameter(19)=Cdbl(aStrawYield)
'response.write "<br>"& Session("farmNumber")
'response.write "<br>"& CInt(aScenarioID)
'response.write "<br>"& CInt(aRotationID)
'response.write "<br>"& CInt(aFieldPlanRotationID)
'response.write "<br>"& CInt(aPreviousCropID)
'response.write "<br>"& CInt(aPreCropOriginalID)
'response.write "<br>"& CInt(aPreCrop_AfterCropID)
'response.write "<br>"& CDbl(aPreCropSecondCrop)
'response.write "<br>"& CInt(aCropID)
'response.write "<br>"& CInt(aCrop_AfterCrop)
'response.write "<br>"& CDbl(aArea)
'response.write "<br>"& CBool(aOrganicFertilizer)
'response.write "<br>"& CInt(aGrazingPart)
'response.write "<br>"& CInt(aSalePart)
'response.write "<br>"& CInt(aStrawUseType)
'response.write "<br>"& CInt(aSecondCrop)
'response.write "<br>"& CInt(aPrePreCropID)



	returnValue=CallSPReturn("InsertUpdateFieldPlanRotation ", arrParameter)

'    Response.Write "<br>returnValue " & returnValue & Chr(13)

next


'CalcBalance aFarmNumber, aScenarioID
CloseFarmNDB
Session("ShowResult")=False
Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End

%>