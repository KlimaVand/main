<!-- #include file="includes/adovbs.asp" -->
<!-- #include file="includes/dbInterface.asp" -->
<html>
<head>
<title>Farm_AC Create input-xml and run model</title>
</head>
<body>
<%
Dim xmlhttp
Dim DataToSend
dim fs,fo,f,show,showresult,message,version 
dim originalLocale, currentLocale
Dim arrParameter
dim dictBaseline, nBaseline, arrBaseline

originalLocale = GetLocale()        ' Get the original locale
currentLocale  = SetLocale("en-gb") ' Set the locale to UK
version = request.querystring("version")
DataToSend="FarmNumber="&Session("FarmNumber")&"&ScenarioID="&Session("scenarioNumber")

InitFarmNDB
set dictBaseline = CreateObject("Scripting.Dictionary")
nBaseline = CallSP("GetBaselineScenario "&Session("FarmNumber"),arrBaseline,dictBaseline)
if request.form("projection") then
	if nBaseline = 0 then
	Response.Redirect("data.asp?TB="&session("tabNumber")&"&message=A scenario with a name containing 'baseline' was not found") 
	response.end 
	else
	Session("Baseline") = arrBaseline(0,0)
	end if
else
Session("Baseline") = -1
end if
'response.write request.form
'
'response.write(Session("Baseline"))
'response.end


  set fs=Server.CreateObject("Scripting.FileSystemObject")
  set fo = fs.GetFolder("c:\upload\AnimalChange\")
  for each f in fo.files
  
  if f.Name="farm_"&Request.Form("FarmNumber")&"_"&Request.Form("ScenarioID")&".xml" then
      fs.DeleteFile("c:\upload\AnimalChange\farm_"&Request.Form("FarmNumber")&"_"&Request.Form("ScenarioID")&".xml") 
  end if
  next
  set fo = fs.GetFolder("c:\upload\AnimalChange\ModelOutput")
  for each f in fo.files
  
  if f.Name=" error_"&Session("FarmNumber")&"_"&Request.Form("ScenarioID")&".txt" then
      fs.DeleteFile("c:\upload\AnimalChange\ModelOutput\ error_"&Session("FarmNumber")&"_"&Request.Form("ScenarioID")&".txt") 
  end if
  next
show=0
showresult=0
Dim postUrl
postUrl = "http://localhost/AnimalChange/ModelService/Model.asmx/RunModelFarmN_DB_2"
'if cint(version) = 1 then
DataToSend=  "FarmNumber="&Session("FarmNumber")&"&ScenarioID="&Session("scenarioNumber")&"&partParameter1=1&partParameter2=1&partParameter3="&Session("Baseline")&""
'else
'DataToSend="FarmNumber="&Session("FarmNumber")&"&ScenarioID="&Session("scenarioNumber")&"&partParameter1=1&partParameter2=1"
'end if
Set xmlhttp = server.Createobject("MSXML2.XMLHTTP")
xmlhttp.Open "POST",postUrl,false
xmlhttp.setRequestHeader "Content-Type","application/x-www-form-urlencoded"
xmlhttp.send DataToSend

'Response.Write DataToSend & "<br>"
'Response.Write(xmlhttp.responseText)
  set fo = fs.GetFolder("c:\upload\AnimalChange\")
time10=now
while (abs(datediff("s",now,time10)) <15)
wend

  for each f in fo.files
time10=now
  if f.Name="farm_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".xml" then
   show=1
  end if
  next
'response.write "show: "&show
  set fo = fs.GetFolder("c:\upload\AnimalChange\ModelOutput\")
  for each f in fo.files
  if (f.Name=" outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml")or(f.Name="outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml") then
	    showresult=3
  end if
  next
'response.write "showresult output: "&showresult
  for each f in fo.files
  if (f.Name=" error_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt")or(f.Name="error_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt") then
    showresult=2
	exit for
  end if
  next
'response.write "showresult error: "&showresult
  if showresult=3 then
    set objXMLDOM = server.CreateObject("Scripting.FileSystemObject")
    if objXMLDOM.GetFile("c:\upload\AnimalChange\ModelOutput\ outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml").Size > 49 then
	showresult=1
  else
		time10=now
		while (abs(datediff("s",now,time10)) <20)
		wend
	  for each f in fo.files
		if (f.Name=" outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml")or(f.Name="outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml") then
		  if showresult<>2 then showresult=1
		end if
		if (f.Name=cstr(" error_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt"))or(f.Name=cstr("error_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt")) then
		  showresult=2
		'  exit for 
		end if
	  next
  end if
  end if
  if show=1 then
   if showresult = 1 then
	saveResults
   end if
	  if showresult=2 then
	  set objXMLDOM = nothing
	  set objXMLDOM =Server.CreateObject("Scripting.FileSystemObject")
	  Set f=objXMLDOM.OpenTextFile("c:\upload\AnimalChange\ModelOutput\ error_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt", 1)
	
	  if objXMLDOM.GetFile("c:\upload\AnimalChange\ModelOutput\ error_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt").Size<>0 then
	  message="Model error :" &f.readAll
	  else
	  message="Empty error-file"
	  end if
	  set objXMLDOM = nothing
	    Response.Redirect("data.asp?TB="&session("tabNumber")&"&message="&message)
	  end if
  else
   Session("ShowResult")=False
   response.write "<br/><br/>The file has not been generated"
  end if
'**************************************************************************************************
sub saveResults()
dim producedManure, exportedManure, importedManure, nFieldPlanRotation, arrFieldPlanRotation, dictFieldPlanRotation
dim ExpectedYield, CropSequence, yieldcoll, i, elem, mainyield, secondyield, count, returnValue
dim FedstuffProductItemU,FedstuffProductItemP,FedstuffProductItemB, outputStrf, itmf, itm,it
InitFarmNDB
set dictFieldPlanRotation = CreateObject("Scripting.Dictionary")
nFieldPlanRotation = CallSP("GetFieldPlanRotationsForExpectedYield "&Session("FarmNumber")&","&Session("scenarioNumber"),arrFieldPlanRotation,dictFieldPlanRotation)
redim arrParameter(1)
	 arrParameter(0)= Session("FarmNumber")
	 arrParameter(1) = Session("scenarioNumber")
     returnValue = CallSPReturn ("DeleteManureTotal",arrParameter)
     returnValue = CallSPReturn ("DeleteFeedstuffProductBalance",arrParameter)'sletter alt i FeedstuffProductBalance-tabellen
redim preserve arrParameter(5)
	 set objXMLDOM = nothing
	 set objXMLDOM =Server.CreateObject("Scripting.FileSystemObject")
if objXMLDOM.GetFile("c:\upload\AnimalChange\ModelOutput\ outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml").Size<>0 then
	  set objXMLDOM = nothing
  set objXMLDOM = server.CreateObject("Microsoft.XMLDOM")
  objXMLDOM.load("c:\upload\AnimalChange\ModelOutput\ outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml")
  Set producedManure = objXMLDOM.getElementsByTagName("producedManure")
  Set exportedManure = objXMLDOM.getElementsByTagName("exportedManure")
  Set importedManure = objXMLDOM.getElementsByTagName("importedManure")
  Set ExpectedYield = objXMLDOM.getElementsByTagName("ExpectedYield")
  Set FedstuffProductItemU = objXMLDOM.getElementsByTagName("FeedAndProductsUsed")
  Set FedstuffProductItemP = objXMLDOM.getElementsByTagName("FeedAndProductsProduced")
  Set FedstuffProductItemB = objXMLDOM.getElementsByTagName("FeedAndProductTradeBalance")
'  yieldcoll = ExpectedYield.item(0).childNodes
  
for each item in producedManure
  if producedManure.item(0).text <>"" then
'  response.write "producedManure= "& producedManure.item(0).childNodes(0).childNodes(1).childNodes(3).text&"-"&cdbl(producedManure.item(0).childNodes(0).childNodes(9).childNodes(3).text)+cdbl(producedManure.item(0).childNodes(0).childNodes(10).childNodes(3).text)
	 arrParameter(2) = producedManure.item(0).childNodes(0).childNodes(1).childNodes(3).text
	 arrParameter(3) = 1100
	 arrParameter(4) = cdbl(producedManure.item(0).childNodes(0).childNodes(9).childNodes(3).text)+cdbl(producedManure.item(0).childNodes(0).childNodes(10).childNodes(3).text)'TAN +Norg from the xml-file
	 arrParameter(5) = cstr(producedManure.item(0).childNodes(0).childNodes(0).childNodes(3).text)
     returnValue=CallSPReturn("InsertUpdateManureTotal ", arrParameter) 
  end if
next
for each item in exportedManure
  if exportedManure.item(0).text <>"" then
'  response.write "<br/>exportedManure= "& exportedManure.item(0).childNodes(0).childNodes(1).childNodes(3).text&"-"&cdbl(exportedManure.item(0).childNodes(0).childNodes(9).childNodes(3).text)+cdbl(exportedManure.item(0).childNodes(0).childNodes(10).childNodes(3).text)
	 arrParameter(2) = exportedManure.item(0).childNodes(0).childNodes(1).childNodes(3).text
	 arrParameter(3) = 1101
	 arrParameter(4) = cdbl(exportedManure.item(0).childNodes(0).childNodes(9).childNodes(3).text)+cdbl(exportedManure.item(0).childNodes(0).childNodes(10).childNodes(3).text)'TAN +Norg from the xml-file
	 arrParameter(5) = cstr(exportedManure.item(0).childNodes(0).childNodes(0).childNodes(3).text)
     returnValue=CallSPReturn("InsertUpdateManureTotal ", arrParameter) 
  end if
next
for each item in importedManure
  if importedManure.item(0).text <>"" then
'  response.write "<br/>importedManure= "& importedManure.item(0).childNodes(0).childNodes(1).childNodes(3).text&"-"&cdbl(importedManure.item(0).childNodes(0).childNodes(9).childNodes(3).text)+cdbl(importedManure.item(0).childNodes(0).childNodes(10).childNodes(3).text)
	 arrParameter(2) = importedManure.item(0).childNodes(0).childNodes(1).childNodes(3).text
	 arrParameter(3) = 1102
	 arrParameter(4) = cdbl(importedManure.item(0).childNodes(0).childNodes(9).childNodes(3).text)+cdbl(importedManure.item(0).childNodes(0).childNodes(10).childNodes(3).text)'TAN +Norg from the xml-file
	 arrParameter(5) = cstr(importedManure.item(0).childNodes(0).childNodes(0).childNodes(3).text)
     returnValue=CallSPReturn("InsertUpdateManureTotal ", arrParameter) 
  end if
next
i=0
if nFieldPlanRotation > 0 then
set elem = ExpectedYield.item(0).childNodes(0).childNodes(0).childNodes '''første "ExpectedYield" - tag indeholder alle de efterfølgende elementer. De næste ExpectedYield tags forkommer der hvor der er modelleret yield
for each item in elem
  if cstr(elem.item(0).lastChild.nodeName)="product" then
	 if cstr(elem.item(0).lastChild.previousSibling.nodeName)="product" then
	  mainyield = Cdbl(elem.item(0).lastChild.previousSibling.text)
	  secondyield = Cdbl(elem.item(0).lastChild.text)
	  if i > 0 then i=i+1
	 else
	  mainyield = Cdbl(elem.item(0).lastChild.text)
	  secondyield = 0
	  if i > 0 then i=i+1
	 end if
  else
  mainyield = 0
  secondyield = 0
  if i > 0 then i=i+1
  end if
'	  response.write "<br/>ExpectedYield= main"& mainyield &"- second="&secondyield &"- i="&i
redim preserve arrParameter(5)
  arrParameter(2) = CInt(arrFieldPlanRotation(i,dictFieldPlanRotation("RotationID")))
  arrParameter(3) = CInt(arrFieldPlanRotation(i,dictFieldPlanRotation("FieldPlanRotationID")))
  arrParameter(4) = mainyield
  arrparameter(5) = secondyield
  returnValue=CallSPReturn("UpdateFieldPlanRotationModelledYield ", arrParameter) 
'	 i=i+1

next
end if
redim preserve arrParameter(3)
  for each itmf in FedstuffProductItemU
  set it=itmf.childNodes
  for each itm in it
'	  response.write "<br/>itm.childNodes(1).childNodes(3).text="&Cstr(itm.childNodes(1).childNodes(3).text)
		arrParameter(2)=Cstr(itm.childNodes(0).childNodes(3).text)
		arrParameter(3)=cdbl(itm.childNodes(1).childNodes(3).text)
        CallSPReturn "InsertUpdateFeedstuffProductBalanceF ",arrParameter
  next
  next
  for each itmf in FedstuffProductItemP
  set it=itmf.childNodes
  for each itm in it
	  response.write "<br/>itm.childNodes(1).childNodes(3).text="&Cstr(itm.childNodes(1).childNodes(3).text)
		arrParameter(2)=Cstr(itm.childNodes(0).childNodes(3).text)
		arrParameter(3)=cdbl(itm.childNodes(1).childNodes(3).text)
        CallSPReturn "InsertUpdateFeedstuffProductBalanceP ",arrParameter
  next
  next

	  set objXMLDOM = nothing
    Session("ShowResult")=True 'disse tre linier udkommenteres, når der skal debugges
    session("tabNumber")=8 'disse tre linier udkommenteres, når der skal debugges
    Response.Redirect("start.asp") 'disse tre linier udkommenteres, når der skal debugges
else
	  set objXMLDOM = nothing
	    Response.Redirect("data.asp?TB="&session("tabNumber")&"&message=The model result-file is empty")
end if
CloseFarmNDB
end sub
'**************************************************************************************************
%>
