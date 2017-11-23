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
<%
dim aFarmNumber, aScenarioID, aScenarioName, arrID, nID, aFarmOwnerName, aFarmOwnerAddress, aFarmOwnerZipCode, aFarmOwnerTown
dim aSoilType, aIrrigationType, aDelivery, aFarmType, aSelectedScenario, returnValue
dim aYieldLevel, nFarm, arrFarm
dim MaxScenarioID, NewScenarioID, sc, dictMaxID
Dim dictID, arr, dictFarm, agrozone, farmDescription, atmosphericDeposition

agrozone = request("selectAgroZone")
if agrozone="" then agrozone=1
aFarmNumber 				= Session("farmNumber")

InitFarmNDB
if request("CopyScenario")<>"" then
  set dictMaxID = CreateObject("Scripting.Dictionary")

'	redim arrID(0)
'	arrID(0)= aFarmNumber
'	InitFarmNDB
   MaxScenarioID = CallSP ("GetMaxScenarioIDPrFarm "&aFarmNumber,arrID,dictMaxID)
   NewScenarioID = arrID(0,0)+1
	 set dictMaxID = nothing
	redim arr(2)
	 arr(0)= aFarmNumber
	 arr(1) = request("selectScenario")
   arr(2) = NewScenarioID
   sc = CallSPReturn ("copy_Scenario",arr)
else
	'Response.write "<br> Session(scenarioNumber) " & Session("scenarioNumber") 

   set dictID = CreateObject("Scripting.Dictionary")
   nID = CallSP ("GetScenarioMaxID " & aFarmNumber,arrID,dictID)
'	nID           = GetScenarioMaxID(aFarmNumber, arrID)
	Response.write "<br> nID " & nID 
	NewScenarioID = arrID(0,dictID.item("MaxID"))+1
	Response.write "<br> aScenarioID " & aScenarioID
	aScenarioName = "Scenario "& NewScenarioID

	 redim arr(2)

	 arr(0) = aFarmNumber
	 arr(1) = NewScenarioID
	 arr(2) = aScenarioName

     returnValue = CallSPReturn ("InsertUpdateScenario",arr)

	Response.write "<br> InsertUpdateScenario " & returnValue 
'	returnValue   = InsertUpdateScenario(aFarmNumber, NewScenarioID, aScenarioName)
end If

     redim arr(1)

	 arr(0) = aFarmNumber
	 arr(1) = NewScenarioID

     returnValue = CallSPReturn ("UpdateSelectedScenario",arr)
	Response.write "<br> UpdateSelectedScenario " & returnValue 

'returnValue  = UpdateSelectedScenario(aFarmNumber, NewScenarioID)


   set dictFarm = CreateObject("Scripting.Dictionary")
   nFarm = CallSP ("GetFarmData " & aFarmNumber,arrFarm,dictFarm)
'nFarm = GetFarmData(aFarmNumber,arrFarm)

Session("scenarioName")   = arrFarm(0,dictFarm.item("ScenarioName"))
'Session("scenarioNumber") = NewScenarioID
	Response.write "<br> scenarioName " & Session("scenarioName") 


aFarmNumber       = Session("farmNumber")
aFarmOwnerName    = Request.Form("inputName")
aFarmOwnerAddress = Request.Form("inputAddress")
aFarmOwnerZipCode = Request.Form("inputZip")
aFarmOwnerTown    = Request.Form("inputTown")
aSoilType         = Request.Form("selectSoilType")
aIrrigationType   = Request.Form("selectIrrigation")
if aIrrigationType = "" then aIrrigationType = 0
aDelivery         = 1 'Request.Form("")
aFarmType         = Request.Form("selectFarmType")
aSelectedScenario = NewScenarioID
aYieldLevel       = Request.Form("selectYieldLevel")
farmDescription   = Request.Form("inputDescription")
atmosphericDeposition = Request.Form("atmosphericDeposition") 

Session("farmType")       = aFarmType
Session("scenarioNumber") = aSelectedScenario
'Session("scenarioName")   = aScenarioName
Session("soilType")       = aSoilType
Session("irrigation")     = aIrrigationType
Session("yieldLevel")     = aYieldLevel

   redim arr(12)
	arr(0)=aFarmNumber
	arr(1)=aFarmOwnerName
	arr(2)=aFarmOwnerAddress
	arr(3)=aFarmOwnerZipCode
	arr(4)=aFarmOwnerTown
	arr(5)=aSoilType
	arr(6)=aIrrigationType
	arr(7)=aFarmType
    arr(8)=aSelectedScenario
	arr(9)=aYieldLevel
	arr(10)=agrozone
    arr(11)=Cstr(farmDescription)
	arr(12)=atmosphericDeposition
	
     returnValue = CallSPReturn ("InsertUpdate_Farm",arr)
	Response.write "<br> InsertUpdateFarm " & returnValue 


'returnValue = InsertUpdateFarm(aFarmNumber, aFarmOwnerName, aFarmOwnerAddress, aFarmOwnerZipCode, aFarmOwnerTown, aSoilType, aIrrigationType, aDelivery, aFarmType, aSelectedScenario,aYieldLevel)
CloseFarmNDB


'Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.Redirect("start.asp")
Response.End
%>