<%
'**************************************************************************************************
' Author: Poul Lassen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
option explicit
%>
<!-- #include file="includes/NonPublicInc.asp" -->
<!-- #include file="includes/General.asp" -->
<!-- #include file="includes/adovbs.asp" -->
<!-- #include file="includes/dbInterface.asp" -->
<!-- #include file="includes/LanguageDictionary.asp" -->
<!-- #include file="CattleHerd.asp" -->
<!-- #include file="Farm.asp" -->
<!-- #include file="FieldPlan.asp" -->
<!-- #include file="Rotation.asp" -->
<!-- #include file="ManureFertilizer.asp" -->
<!-- #include file="PigHerd.asp" -->
<!-- #include file="Balance.asp" -->
<!-- #include file="Result.asp" -->
<!-- #include file="Documentation.asp" -->
<%
Private Const TITLE = "FarmNTool"
Server.ScriptTimeout=500

'dim nFarm, arrFarm
'dim nScenario, arrScenario
'dim nSoilType, arrSoilType
'dim nFarmType, arrFarmType
'
'dim nRotation, arrRotation
'dim nFieldPlanCrop, arrFieldPlanCrop
'dim nFieldPlanTotal, arrFieldPlanTotal
'dim nCrop, arrCrop
'dim nParameterCattle, arrParameterCattle
'dim nStrawUse, arrStrawUse
'dim nStraw, arrStraw
'dim nCattleHerd, arrCattleHerd
'dim nCattleType, arrCattleType
'dim nCattleHerdTotal, arrCattleHerdTotal
'dim nCattleHerdCattleType, arrCattleHerdCattleType
'dim nPigHerd, arrPigHerd
'dim nPigType, arrPigType
'dim nFarmTotal, arrFarmTotal
'dim nStorageType, arrStorageType
'dim nBalanceFeed, arrBalanceFeed
'dim nBalanceStorage, arrBalanceStorage
'dim nResultInput, arrResultInput, nResultOutput, arrResultOutput
'dim nResultSoilChange,arrResultSoilChange
'dim nResultNleaching, arrResultNleaching
'
'InitFarmNDB
'nFarm                 = GetFarmData(CLng(Session("farmNumber")),arrFarm)
'nScenario             = GetScenarioList(CLng(Session("farmNumber")),arrScenario)
'nSoilType             = GetSoilTypeList(arrSoilType)
'nFarmType             = GetFarmTypeList(arrFarmType)
'
'nRotation             = GetRotationList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation)
'nFieldPlanCrop        = GetFieldPlanCropList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrFieldPlanCrop)
'nFieldPlanTotal       = GetFieldPlanTotalList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrFieldPlanTotal)
'nCrop                 = GetCropList(arrCrop)
'nStrawUse             = GetStrawUseTypeList(arrStrawUse)
'nCattleHerd           = GetCattleHerdList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrCattleHerd)
'nCattleHerdCattleType = GetCattleHerdCattleTypeList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrCattleHerdCattleType)
'nCattleType           = GetCattleTypeList(arrCattleType)
'nParameterCattle      = GetParameterList(3,arrParameterCattle)
'nCattleHerdTotal      = GetCattleHerdTotalList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrCattleHerdTotal)
'nPigHerd              = GetPigHerdList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrPigHerd)
'nPigType              = GetPigTypeList(arrPigType)
'nFarmTotal            = GetFarmTotalList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),3,arrFarmTotal)
'nStorageType          = GetStorageTypeList(arrStorageType)
'nBalanceFeed          = GetBalanceFeedList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrBalanceFeed)
'nBalanceStorage       = GetBalanceStorageList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrBalanceStorage)
'nResultInput          = GetResultList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),1,arrResultInput)
'nResultOutput         = GetResultList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),2,arrResultOutput)
'nResultSoilChange     = GetResultList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),3,arrResultSoilChange)
'nResultNleaching      = GetResultList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),4,arrResultNleaching)
'nStraw                = GetSummarizedStrawKg(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrStraw)
'CloseFarmNDB
dim version

Session("version") = Request.QueryString("version")
if Session("version") = "" then Session("version") = 1

Session("tabNumber") = Request.QueryString("TB")

BeginHeader TITLE,"",""
CreateStyleLink "styles/stylesheet.css"
EndHeader
BeginBodyData
CreatePage
EndBody
'**************************************************************************************************
sub CreatePage()
'   Response.Write("nScenario: "&nScenario)
   select case CInt(Session("tabNumber"))
      case 1 CreateTabFarm()
      case 2 CreateTabField()  
      case 3 CreateTabRotation()  
      case 4 CreateTabCattle() 
      case 5 CreateTabPig()    
      case 6 CreateTabManure() 
      case 7 CreateTabBalance() 
      case 8 CreateTabResult() 
      case 9 CreateTabDocumentation() 
   end select
end sub
'**************************************************************************************************
%>
