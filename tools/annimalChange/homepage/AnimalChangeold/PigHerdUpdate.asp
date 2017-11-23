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
<!-- #include file="includes/FunctionPig.asp" -->

<%
dim h, nPigHerd, aFarmNumber, aScenarioID, aPigHerdID, aPigType, aAnimalYear, aProductionVar1, aProductionVar2, aProductionVar3, aFeedLevel, aProteinLevel, aStableType1, aStableType2, returnValue, feedNitrogenAnimalYearNorm
dim arrPigHerd, feedEnergyTotal, feedNitrogenAnimalYear, feedNitrogenTotal, GrowthNitrogenType, GrowthNitrogenTypeNorm, GrowthNitrogenTotal,SowGrowthNitrogen,SowGrowthNitrogenTotal, GrowthNitrogenIn, GrowthNitrogenOut, GrowthNitrogenInTotal, GrowthNitrogenOutTotal, manureNitrogenAnimalYear, manureNitrogenTotal, manureNitrogenFaecesAnimalYear, manureNitrogenFaecesTotal, manureNitrogenUrineAnimalYear, manureNitrogenUrineTotal, manureNitrogenAnimalYearNorm, manureNitrogenTotalNorm
Dim arrParameter
dim aFeedstuffList, aAmountList
dim aManureStorageString1,aManureStorageString2
dim originalLocale, currentLocale
dim nStorageType, arrStorageType, aStorageTypeID, dictStorageType, s


originalLocale = GetLocale()        ' Get the original locale
currentLocale  = SetLocale("en-gb") ' Set the locale to UK
response.write request.form
redim arrParameter(11)

aFarmNumber = Session("farmNumber")
aScenarioID = Session("scenarioNumber")
nPigHerd = Request.Form("hiddenNPigHerd")
InitFarmNDB
for h = 0 to nPigHerd-1
   aPigHerdID  = Request.Form("hiddenPigHerdID_"&h&"")
   aPigType    = Request.Form("selectPigType_"&h&"")
   aAnimalYear = Request.Form("inputAnimalYear_"&h&"")
   aProductionVar1 = Request.Form("inputProductionVar1_"&h&"")
   aProductionVar2 = Request.Form("inputProductionVar2_"&h&"")
   aProductionVar3 = Request.Form("inputProductionVar3_"&h&"")
   aFeedLevel    = Request.Form("inputFeedLevel_"&h&"")
   aProteinLevel = Request.Form("inputProteinLevel_"&h&"")
   aStableType1  = Request.Form("selectStableType1_"&h&"")
   aStableType2  = Request.Form("selectStableType2_"&h&"")
   if aStableType2="" then 
   aStableType2 = 0
   end if


	 arrParameter(0)= aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aPigHerdID
	 arrParameter(3) = aPigType ' aCattleType 
	 arrParameter(4) = aAnimalYear ' aAnimalYear
	 arrParameter(5) = aProteinLevel ' 
	 arrParameter(6) = aFeedLevel
	 arrParameter(7) = aProductionVar1
	 arrParameter(8) = aProductionVar2
	 arrParameter(9) = aProductionVar3
	 arrParameter(10) = aStableType1
	 arrParameter(11) = aStableType2

   returnValue = CallSPReturn ("InsertUpdatePigHerd",arrParameter)
   aFeedstuffList = Request.Form("hiddenFeedstuffList_"&aPigHerdID&"")
   aAmountList = Request.Form("hiddenAmountList_"&aPigHerdID&"")
   aManureStorageString1 = Request.Form("selectManureStorageType1_"&h&"")
   aManureStorageString2 = Request.Form("selectManureStorageType2_"&h&"")
   if cstr(aManureStorageString1)="" then aManureStorageString1=0
   if cstr(aManureStorageString2)="" then aManureStorageString2=0
   if aStableType1=0 then aManureStorageString1=0
   if aStableType2=0 then aManureStorageString2=0
'   response.write "aFeedstuffList : "&aFeedstuffList
'   if aFeedstuffList <> "" then
'   response.write "<br/> InsertUpdateFeedstuff "&aFarmNumber&"."&aScenarioID&"."&aPigHerdID&"."&aFeedstuffList&"."&aAmountList
    InsertUpdateFeedstuff aFarmNumber,aScenarioID,aPigHerdID,aFeedstuffList, aAmountList
'   end if
   InsertUpdateManureStorage aFarmNumber,aScenarioID,aPigHerdID,1,aManureStorageString1
   InsertUpdateManureStorage aFarmNumber,aScenarioID,aPigHerdID,2,aManureStorageString2
'   aFeedstuffList = aFeedstuffList & Request.Form("hiddenFeedstuffList_2_"&h&"")
'   aAmountList = aAmountList & Request.Form("hiddenAmountList_2_"&h&"")
'   if aFeedstuffList <> "" then
'     InsertUpdateFeedstuff aFarmNumber,aScenarioID,aPigHerdID,aFeedstuffList, aAmountList
'   end if

'   returnValue = InsertUpdatePigHerd(aFarmNumber, aScenarioID, aPigHerdID, aPigType, aAnimalYear, aProteinLevel, aFeedLevel, aProductionVar1, aProductionVar2, aProductionVar3, aStableType1, aStableType2)
next   

CalcPigHerdTotal
redim preserve arrParameter(1)

 '  returnValue = CallSPReturn ("CalcFarmTotal",arrParameter)
'CalcFarmTotal aFarmNumber,aScenarioID
CloseFarmNDB
Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
'
'**************************************************************************************************
sub InsertUpdateFeedstuff(aFarmNumber,aScenarioID,aPigHerdID,aFeedstuffList,aAmountList)
   dim arrFeedstuff,f,Feedstuff,arrParameter1,arrAmount, amount
   redim arrParameter1(5)
	 arrParameter1(0)= aFarmNumber
	 arrParameter1(1) = aScenarioID
	 arrParameter1(2) = aPigHerdID
   arrFeedstuff=split(aFeedstuffList,",")
   arrAmount = split(aAmountList,",")
'response.write "arrFeedstuff(0) "&arrFeedstuff(0)
   
   for f = 0  to Ubound(arrFeedstuff)-1
'response.write "arrFeedstuff(f) "&arrFeedstuff(f)
     if arrFeedstuff(f)<>"" and arrFeedstuff(f)<>"-1" and not IsNull(arrFeedstuff(f)) then
	 if Ubound(arrAmount)>= f then
	 amount = arrAmount(f)
	 else
	 amount = 0
	 end if
        arrParameter1(3) =  f+1
        arrParameter1(4) =  arrFeedstuff(f)
        arrParameter1(5) =  cstr(amount)
        CallSPReturn "insertUpdatePigHerdFeedstuff ",arrParameter1
response.write "<br/><br/>amount"&amount
     end if
   next
   redim preserve arrParameter1(3)
   arrParameter1(3) =  Ubound(arrFeedstuff)
        CallSPReturn "DeletePigHerdFeedstuff ",arrParameter1
   
end sub
'**************************************************************************************************
sub InsertUpdateManureStorage(aFarmNumber,aScenarioID,aPigHerdID,aStableSubTypeID,aManureStorageString)
   dim arrManureStorage,s,ManureStorage,arrParameter1
   redim arrParameter1(5)
	 arrParameter1(0)= aFarmNumber
	 arrParameter1(1) = aScenarioID
	 arrParameter1(2) = aPigHerdID
	 arrParameter1(3) = aStableSubTypeID
   arrManureStorage=split(aManureStorageString,",")
   for s = 0  to Ubound(arrManureStorage)
'     if arrManureStorage(s)<>"" and arrFeedstuff(f)<>"-1" and not IsNull(arrFeedstuff(f)) then
        arrParameter1(4) =  s+1
        arrParameter1(5) =  arrManureStorage(s)
        CallSPReturn "InsertUpdatePigManureStorage ",arrParameter1
'     end if
   next
   redim preserve arrParameter1(4)
   arrParameter1(4) =  Ubound(arrManureStorage)+1
        CallSPReturn "DeletePigManureStorage ",arrParameter1
   
end sub
'**************************************************************************************************
%>