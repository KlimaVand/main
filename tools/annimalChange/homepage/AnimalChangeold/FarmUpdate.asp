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
dim aFarmNumber, aFarmOwnerName, aFarmOwnerAddress, aFarmOwnerZipCode, aFarmOwnerTown, arrFarm, nFarm
dim aSoilType, aIrrigationType, aDelivery, aFarmType, aSelectedScenario, returnValue
dim arrDelivery,d,arrParameter,q
dim aYieldLevel, sm, SMID, BMID, bm, aSoldManureID, aSoldManureAmount, aSoldManureType, aBoughtManureID, aBoughtManureAmount, aBoughtManureType, aBoughtManureUtilization, aBoughtManureConversion
Dim dictFarm, arr
dim agroZone, farmDescription, atmosphericDeposition
response.write request.form

aFarmNumber       = Session("farmNumber")
aFarmOwnerName    = Request.Form("inputName")
aFarmOwnerAddress = Request.Form("inputAddress")
aFarmOwnerZipCode = ""'Request.Form("inputZip")
aFarmOwnerTown    = ""'Request.Form("inputTown")
aSoilType         = Request.Form("selectSoilType")'1'
aIrrigationType   = Request.Form("selectIrrigation")
if aIrrigationType = "" then aIrrigationType = 0
aDelivery         = Request.Form("deliveryTypeID")'+",4,5"
aFarmType         = Request.Form("selectFarmType")
aSelectedScenario = Request.Form("selectScenario")
aYieldLevel       = Request.Form("selectYieldLevel")
agroZone          = Request.Form("selectAgroZone")
farmDescription   = Request.Form("inputDescription")
atmosphericDeposition = Request.Form("atmosphericDeposition") 
if agroZone = "" then agroZone = 1
response.write "farmDescription : "&farmDescription

Session("farmType")       = aFarmType
Session("scenarioNumber") = aSelectedScenario
Session("soilType")       = aSoilType
Session("irrigation")     = aIrrigationType
Session("yieldLevel")     = aYieldLevel
SMID          = Request.Form("nSM")
BMID          = Request.Form("nBM")
InitFarmNDB
 for sm=0 to SMID 
   aSoldManureID = Request.Form("SMID_"&sm&"")
   aSoldManureAmount = Request.Form("SoldAmount_"&sm&"")
   aSoldManureType = Request.Form("selectSoldManure_"&sm&"")

   redim arr(4)
	 arr(0) = aFarmNumber
	 arr(1) = aSelectedScenario
	 arr(2) = aSoldManureID
	 arr(3) = aSoldManureAmount
	 arr(4) = aSoldManureType
     returnValue = CallSPReturn ("InsertUpdateSoldManure",arr)  
'   returnValue  = InsertUpdateSoldManure(aFarmNumber, aSelectedScenario, aSoldManureID, aSoldManureAmount, aSoldManureType)
 next

 for bm=0 to BMID 
   aBoughtManureID = Request.Form("BMID_"&bm&"")
   aBoughtManureAmount = Request.Form("BoughtAmount_"&bm&"")
   aBoughtManureType = Request.Form("selectBoughtManure_"&bm&"")
   aBoughtManureUtilization = Request.Form("BoughtUtilization_"&bm&"")
   aBoughtManureConversion = Request.Form("BoughtConversion_"&bm&"")
   redim arr(6)
	 arr(0) = aFarmNumber
	 arr(1) = aSelectedScenario
	 arr(2) = aBoughtManureID
	 arr(3) = aBoughtManureAmount
	 arr(4) = aBoughtManureType
	 arr(5) = aBoughtManureUtilization
	 arr(6) = aBoughtManureConversion
response.write aBoughtManureID
response.write aBoughtManureType
     returnValue = CallSPReturn ("InsertUpdateBoughtManure",arr) 
'	 response.write returnValue 
'   returnValue  = InsertUpdateBoughtManure(aFarmNumber, aSelectedScenario, aBoughtManureID, aBoughtManureAmount, aBoughtManureType)
 next

 redim arrParameter(0)
 arrParameter(0)=aFarmNumber
 d=CallSPReturn("DeleteFarmDeliveryAll", arrParameter)
 
 arrDelivery=split(aDelivery,",")
 redim arrParameter(1)
 for q=0 to Ubound(arrDelivery)
   arrParameter(0)=aFarmNumber
   arrParameter(1)=arrDelivery(q)
   d=CallSPReturn("InsertFarmDelivery", arrParameter) 
 next
 arrParameter(0)=aFarmNumber
 arrParameter(1)=4
 d=CallSPReturn("InsertFarmDelivery", arrParameter)
 arrParameter(0)=aFarmNumber
 arrParameter(1)=5
 d=CallSPReturn("InsertFarmDelivery", arrParameter)
 arrParameter(1)=6
 d=CallSPReturn("InsertFarmDelivery", arrParameter)
 
'   redim arr(9)
'	arr(0)=aFarmNumber
'	arr(1)=aFarmOwnerName
'	arr(2)=aFarmOwnerAddress
'	arr(3)=aFarmOwnerZipCode
'	arr(4)=aFarmOwnerTown
'	arr(5)=aSoilType
'	arr(6)=aIrrigationType
'	arr(7)=aFarmType
'    arr(8)=aSelectedScenario
'	arr(9)=aYieldLevel
'	
'     returnValue = CallSPReturn ("InsertUpdateFarm",arr)


   redim arr(12)
	arr(0)=aFarmNumber
	arr(1)=aFarmOwnerName
	arr(2)=Cstr(aFarmOwnerAddress)
	arr(3)=aFarmOwnerZipCode
	arr(4)=aFarmOwnerTown
	arr(5)=aSoilType
	arr(6)=aIrrigationType
	arr(7)=aFarmType
    arr(8)=aSelectedScenario
	arr(9)=aYieldLevel
    arr(10)=agroZone
    arr(11)=Cstr(farmDescription)
	arr(12)=atmosphericDeposition

	
     returnValue = CallSPReturn ("InsertUpdate_Farm",arr)
'	Response.write "<br> InsertUpdateFarm " & returnValue 

'returnValue = InsertUpdateFarm(aFarmNumber, aFarmOwnerName, aFarmOwnerAddress, aFarmOwnerZipCode, aFarmOwnerTown, aSoilType, aIrrigationType, aDelivery, aFarmType, aSelectedScenario,aYieldLevel)

  set dictFarm = CreateObject("Scripting.Dictionary")
   nFarm = CallSP ("GetFarmData " & aFarmNumber,arrFarm,dictFarm)
'nFarm        = GetFarmData(aFarmNumber,arrFarm)
Session("scenarioName")   = arrFarm(0,dictFarm.item("ScenarioName"))
'response.write Session("scenarioName")
CloseFarmNDB

Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
%>