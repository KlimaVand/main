<%
'**************************************************************************************************
'**************************************************************************************************
option explicit
%>
<!-- #include file="includes/General.asp" -->
<!-- #include file="includes/adovbs.asp" -->
<!-- #include file="includes/dbInterface.asp" -->
<!-- #include file="includes/FunctionCattle.asp" -->
<%
dim h, aFarmNumber, aScenarioID, i 
Dim returnValue
dim feedEnergyAnimalYear, feedEnergyTotal, feedEnergyTotalNorm, feedDryMatterAnimalYear, feedDryMatterTotal, feedNitrogenAnimalYear, feedNitrogenTotal, feedNitrogenAnimalYearNorm, feedNitrogenTotalNorm
dim productionMilkNitrogenAnimalYear, productionMilkNitrogenTotal, productionGrowthNitrogenAnimalYear, productionGrowthNitrogenTotal,avgProductionMilk, avgProductionMeat
dim manureNitrogenAnimalYear, manureNitrogenTotal, manureNitrogenOnField, manureNitrogenAnimalYearNorm, manureNitrogenTotalNorm
dim manureNitrogenOnFieldAnimalYear, manureNitrogenOnFieldTotal
dim manureNitrogenFaecesAnimalYear, manureNitrogenFaecesTotal
dim manureNitrogenUrineAnimalYear, manureNitrogenUrineTotal
dim nCattleHerd, arrCattleHerd, aCattleHerdID
dim aFeedstuffList, aAmountList
dim aManureStorageString, message
dim nStorageType, arrStorageType, aStorageTypeID, dictStorageType, s
dim GrazedItem, outputStr, itm, item, elem

aFarmNumber = Session("farmNumber")
aScenarioID = Session("scenarioNumber")
nCattleHerd = Request.Form("hiddenNCattleHerd")

dim originalLocale, currentLocale
Dim arrParameter
redim arrParameter(9)

originalLocale = GetLocale()        ' Get the original locale
currentLocale  = SetLocale("en-gb") ' Set the locale to UK
response.write "<html><body>" & vbCr
'response.write "FormString= "&Request.Form
'response.write "ManureStorageString2= "&Request.Form("selectManureStorageType_2")

InitFarmNDB
for h = 0 to nCattleHerd-1
aCattleHerdID=Request.Form("hiddenCattleHerdID_"&h&"")
	 arrParameter(0)= aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aCattleHerdID ' aCattleHerdID
	 arrParameter(3) = Request.Form("selectCattleType_"&h&"") ' aCattleType 
	 arrParameter(4) = replace(Request.Form("inputAnimalYear_"&h&""),",",".") ' aAnimalYear
	 arrParameter(5) = Request.Form("inputGrazing_"&h&"") ' aGrazing
	 arrParameter(6) = Request.Form("inputProductionLevel_"&h&"") ' aProductionLevel
	 arrParameter(7) = Request.Form("inputProductionLevel2_"&h&"")' aProductionLevel2
	 arrParameter(8) = 100'Request.Form("inputEffectivity_"&h&"") ' aEffectivity
	 arrParameter(9) = Request.Form("selectStableType_"&h&"")  ' aStableType

   returnValue = CallSPReturn ("InsertUpdateCattleHerd",arrParameter)
'   response.write "<br/>returnValue :"&returnValue
   aFeedstuffList = Request.Form("hiddenFeedstuffList_"&h&"")
'   response.write aFeedstuffList&" f<br/>"
   aAmountList = Request.Form("hiddenAmountList_"&h&"")
'   response.write aAmountList&" aMount<br/>"
   aManureStorageString = Request.Form("selectManureStorageType_"&h&"")
   if cstr(aManureStorageString)="" then aManureStorageString=0
   if Request.Form("storageChanged_"&h&"")=1 then
     set dictStorageType = CreateObject("Scripting.Dictionary")
     nStorageType = CallSP ("GetDefaultManureStorageTypes " & Request.Form("selectCattleType_"&h&"") & "," & Request.Form("selectStableType_"&h&""), arrStorageType, dictStorageType)
     aManureStorageString=""
     for s = 0 to nStorageType-1
	   aManureStorageString = aManureStorageString&arrStorageType(s,0)
	   if s < nStorageType-1 then
	     aManureStorageString=aManureStorageString&","
	   end if
	 next
'response.write "aManureStorageString_"&h&": "&aManureStorageString &"<br/>"
     set dictStorageType = nothing
   end if
'   response.write aManureStorageString&" m<br/>"
   InsertUpdateFeedstuff aFarmNumber,aScenarioID,aCattleHerdID,aFeedstuffList,aAmountList
   InsertUpdateManureStorage aFarmNumber,aScenarioID,aCattleHerdID,aManureStorageString
next

CalcCattleHerdTotal
dim arr(2)
dim arr1(1)
arr(0) = aFarmNumber
arr1(0) = aFarmNumber
arr(1) = aScenarioID
arr1(1) = aScenarioID
arr(2) = -1

 returnValue = CallSPReturn ("DeleteCattleHerdCalculation",arr)'sletter alle gamle beregninger, alt hvor cattleherdID>-1
 returnValue = CallSPReturn ("DeleteGrazingBalance",arr1)'sletter alt i GrazingBalance-tabellen
Dim DataToSend,xmlhttp,objXMLDOM
dim fs,fo,f,showpartresult
if nCattleHerd>0 then
'DataToSend="FarmNumber="&Session("FarmNumber")&"&ScenarioID="&Session("scenarioNumber")&"&partParameter=0"
DataToSend="FarmNumber="&Session("FarmNumber")&"&ScenarioID="&Session("scenarioNumber")&"&partParameter1=0&partParameter2=1&partParameter3=-1"
  set fs=Server.CreateObject("Scripting.FileSystemObject")
  set fo = fs.GetFolder("c:\upload\AnimalChange\")
  for each f in fo.files
  
  if f.Name="farm_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".xml" then
      fs.DeleteFile("c:\upload\AnimalChange\farm_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".xml") 
  end if
  next
  set fo = fs.GetFolder("c:\upload\AnimalChange\ModelOutput")
  for each f in fo.files
  if (f.Name=" error_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt")or(f.Name="error_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt")or(f.Name=" outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml")or(f.Name="outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml") then
      fs.DeleteFile("c:\upload\AnimalChange\ModelOutput\"&f.Name)
  end if
  next
showpartresult=0
Dim postUrl,energyIntake,energyUseForMaintenance,energyUseForGrowth,energyUseForMilk,energyFromRemobilisation
dim producedManure
'response.end
'postUrl = "http://localhost/AnimalChange/ModelService/Model.asmx/RunPartModelFarmN_DB"
postUrl = "http://localhost/AnimalChange/ModelService/Model.asmx/RunModelFarmN_DB_2"
Set xmlhttp = server.Createobject("MSXML2.XMLHTTP")
xmlhttp.Open "POST",postUrl,false
xmlhttp.setRequestHeader "Content-Type","application/x-www-form-urlencoded"
xmlhttp.send DataToSend
time10=now
while (abs(datediff("s",now,time10)) <15)
wend
redim preserve arrParameter(4)

  set fo = fs.GetFolder("c:\upload\AnimalChange\ModelOutput")
  if showpartresult=0 then
  for each f in fo.files
  if (f.Name=" outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml") or (f.Name="outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml") then
    showpartresult=3
  end if
  next
  end if
  for each f in fo.files
  if (f.Name=cstr(" error_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt"))or(f.Name=cstr("error_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt")) then
    showpartresult=2 
  end if
  next
  if showpartresult<>0 then
  if showpartresult=3 then
    set objXMLDOM = server.CreateObject("Scripting.FileSystemObject")
    if objXMLDOM.GetFile("c:\upload\AnimalChange\ModelOutput\ outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml").Size > 49 then
	showpartresult=1
  else
		time10=now
		while (abs(datediff("s",now,time10)) <15)
		wend
	  for each f in fo.files
		if (f.Name=" outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml")or(f.Name="outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml") then
		  if showpartresult<>2 then showpartresult=1
		end if
		if (f.Name=cstr(" error_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt"))or(f.Name=cstr("error_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt")) then
		  showpartresult=2
		  exit for 
		end if
	  next
  end if
  end if
  set objXMLDOM = server.CreateObject("MSXML2.DOMDocument.3.0")
  if showpartresult=1 then
  objXMLDOM.load("c:\upload\AnimalChange\ModelOutput\ outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml")
  Set energyIntake = objXMLDOM.getElementsByTagName("energyIntake")
  Set energyUseForMaintenance = objXMLDOM.getElementsByTagName("energyUseForMaintenance")
  Set energyUseForGrowth = objXMLDOM.getElementsByTagName("energyUseForGrowth")
  Set energyUseForMilk = objXMLDOM.getElementsByTagName("energyUseForMilk")
  Set energyFromRemobilisation = objXMLDOM.getElementsByTagName("energyFromRemobilisation")
  Set avgProductionMilk = objXMLDOM.getElementsByTagName("avgDailyProductionMilk")
  Set avgProductionMeat = objXMLDOM.getElementsByTagName("avgProductionMeat")
  Set GrazedItem = objXMLDOM.getElementsByTagName("GrazedItem")
  Set producedManure = objXMLDOM.getElementsByTagName("producedManure")
'  Set producedManure = objXMLDOM.getElementsByTagName("manure")
  end if
'  if showpartresult=2 then
'  objXMLDOM.load("c:\upload\AnimalChange\ModelOutput\ error_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt")
'  end if
'  

  if showpartresult=1 then
  for h=0 to nCattleHerd-1
  aCattleHerdID=Request.Form("hiddenCattleHerdID_"&h&"")
  arrParameter(2) = aCattleHerdID ' aCattleHerdID
  arrParameter(3)=1001
  arrParameter(4)=cdbl(energyIntake.item(h).childNodes(3).text)
        CallSPReturn "insertUpdateCattleHerdCalculation ",arrParameter
  arrParameter(3)=1002
  if cdbl(energyIntake.item(h).childNodes(3).text)<>0 then
  arrParameter(4)=cdbl(energyUseForMaintenance.item(h).childNodes(3).text)
  else
  arrParameter(4)=0
  end if
        CallSPReturn "insertUpdateCattleHerdCalculation ",arrParameter
  arrParameter(3)=1003
  if cdbl(energyIntake.item(h).childNodes(3).text)<>0 then
  arrParameter(4)=cdbl(energyUseForGrowth.item(h).childNodes(3).text)
  else
  arrParameter(4)=0
  end if
        CallSPReturn "insertUpdateCattleHerdCalculation ",arrParameter
  arrParameter(3)=1004
  if cdbl(energyIntake.item(h).childNodes(3).text)<>0 then
  arrParameter(4)=cdbl(energyUseForMilk.item(h).childNodes(3).text)
  else
  arrParameter(4)=0
  end if
        CallSPReturn "insertUpdateCattleHerdCalculation ",arrParameter
  arrParameter(3)=1005
  if cdbl(energyIntake.item(h).childNodes(3).text)<>0 then
  arrParameter(4)=cdbl(energyFromRemobilisation.item(h).childNodes(3).text)
  else
  arrParameter(4)=0
  end if
        CallSPReturn "insertUpdateCattleHerdCalculation ",arrParameter
  arrParameter(3)=1006
  if cdbl(energyIntake.item(h).childNodes(3).text)<>0 then
  arrParameter(4)=cdbl(avgProductionMilk.item(h).childNodes(3).text)
  else
  arrParameter(4)=0
  end if
        CallSPReturn "insertUpdateCattleHerdCalculation ",arrParameter
  arrParameter(3)=1076
  if cdbl(energyIntake.item(h).childNodes(3).text)<>0 then
  arrParameter(4)=cdbl(avgProductionMeat.item(h).childNodes(3).text)
  else
  arrParameter(4)=0
  end if
        CallSPReturn "insertUpdateCattleHerdCalculation ",arrParameter
  next
  message="Model calculations OK"
'arrParameter(3)=1001
'arrParameter(4)=cdbl(energyIntake.item(0).childNodes(0).text)
'        CallSPReturn "insertUpdateCattleHerdCalculation ",arrParameter
'
  end if
end if'***if nCattleHerd>0
  if showpartresult=1 then
  for each itm in GrazedItem
		arrParameter(2)=Cstr(itm.childNodes(0).childNodes(3).text)
		arrParameter(3)=cdbl(itm.childNodes(2).childNodes(3).text)
		arrParameter(4)=cdbl(itm.childNodes(1).childNodes(3).text)
        CallSPReturn "insertUpdateGrazingBalance ",arrParameter
  next
  
dim arr1Parameter(1)
	 arr1Parameter(0)= Session("FarmNumber")
	 arr1Parameter(1) = Session("scenarioNumber")
     returnValue = CallSPReturn ("DeleteManureTotal",arr1Parameter)
dim xcounter
xcounter=0
dim ChildNodes	 
  redim preserve arrParameter(5)
  for each elem in producedManure
  set ChildNodes=elem.childNodes
  for each item in ChildNodes
' response.write "item.text<br/>"&item.text
  if item.text <>"" then
'  response.write "item= "& item.childNodes(2).childNodes(3).text&"-"&cdbl(item.childNodes(9).childNodes(3).text)+cdbl(item.childNodes(10).childNodes(3).text)
	 arrParameter(2) = item.childNodes(2).childNodes(3).text
	 arrParameter(3) = 1100
	 arrParameter(4) = cdbl(item.childNodes(9).childNodes(3).text)+cdbl(item.childNodes(10).childNodes(3).text)'TAN +Norg from the xml-file
	 arrParameter(5) = cstr(item.childNodes(0).childNodes(3).text)
     returnValue=CallSPReturn("InsertUpdateManureTotal ", arrParameter) 
  end if
  next
  next
  end if

CloseFarmNDB
  if showpartresult=2 then
  set objXMLDOM = nothing
  set objXMLDOM =Server.CreateObject("Scripting.FileSystemObject")
  Set f=objXMLDOM.OpenTextFile("c:\upload\AnimalChange\ModelOutput\ error_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt", 1)

  if objXMLDOM.GetFile("c:\upload\AnimalChange\ModelOutput\ error_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt").Size<>0 then
  message="Model error :" &f.readAll
  else
  message="Empty error-file"
  end if
  end if
  set objXMLDOM = nothing
  end if

currentLocale  = SetLocale(originalLocale) ' Restore the original locale
'response.write outputStr
response.write "<form name=""ret"" action=""data.asp?TB="&session("tabNumber")&""" method=""post"">" & vbCr 
response.write "<input type=""hidden"" name=""message"" value="""&message&"""/>" & vbCr
response.write "</form>" & vbCr
response.write "<script language =""javascript"">document.forms[0].submit();</script>"
response.write "</body></html>" & vbCr
Session("ShowResult")=False

''Response.Redirect("data.asp?TB="&session("tabNumber")&"")
''Response.End
'**************************************************************************************************
sub InsertUpdateFeedstuff(aFarmNumber,aScenarioID,aCattleHerdID,aFeedstuffList,aAmountList)
   dim arrFeedstuff,f,Feedstuff,arrParameter1,arrAmount,amount
   redim arrParameter1(5)
	 arrParameter1(0)= aFarmNumber
	 arrParameter1(1) = aScenarioID
	 arrParameter1(2) = aCattleHerdID
   arrFeedstuff=split(aFeedstuffList,",")
   arrAmount = split(aAmountList,",")
   for f = 0  to Ubound(arrFeedstuff)-1
     if arrFeedstuff(f)<>"" and arrFeedstuff(f)<>"-1" and not IsNull(arrFeedstuff(f)) then
	 if Ubound(arrAmount)> f then
	 amount = (arrAmount(f))'*numberAnimal
	 else
	 amount = 0
	 end if
        arrParameter1(3) =  f+1
        arrParameter1(4) =  arrFeedstuff(f)
        arrParameter1(5) =  amount
        CallSPReturn "insertUpdateCattleHerdFeedstuff ",arrParameter1
     end if
   next
   redim preserve arrParameter1(3)
   arrParameter1(3) =  Ubound(arrFeedstuff)
        CallSPReturn "DeleteCattleHerdFeedstuff ",arrParameter1
   
end sub
'**************************************************************************************************
sub InsertUpdateManureStorage(aFarmNumber,aScenarioID,aCattleHerdID,aManureStorageString)
   dim arrManureStorage,s,ManureStorage,arrParameter1
   redim arrParameter1(3)
	 arrParameter1(0)= aFarmNumber
	 arrParameter1(1) = aScenarioID
	 arrParameter1(2) = aCattleHerdID
     arrParameter1(3) =  0
        CallSPReturn "DeleteManureStorage ",arrParameter1
   redim preserve arrParameter1(4)
'	 arrParameter1(0)= aFarmNumber
'	 arrParameter1(1) = aScenarioID
'	 arrParameter1(2) = aCattleHerdID
   arrManureStorage=split(aManureStorageString,",")
   for s = 0  to Ubound(arrManureStorage)
'     if arrManureStorage(s)<>"" and arrFeedstuff(f)<>"-1" and not IsNull(arrFeedstuff(f)) then
        arrParameter1(3) =  s+1
        arrParameter1(4) =  arrManureStorage(s)
        CallSPReturn "InsertUpdateManureStorage ",arrParameter1
'		response.write "<br/>s"&s
'     end if
   next
   
end sub
'**************************************************************************************************
%>
