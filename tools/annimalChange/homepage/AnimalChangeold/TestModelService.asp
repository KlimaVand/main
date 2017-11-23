<html>
<head>
<title>Farm_AC Create input-xml</title>
</head>
<body>
<%
'response.write(Request.Form)
Dim xmlhttp
Dim DataToSend
dim fs,fo,f,show,showresult
'If Request.ServerVariables("REQUEST_METHOD") = "POST" Then
'if Request.Form("FarmNumber") then
DataToSend="FarmNumber="&Session("FarmNumber")&"&ScenarioID="&Session("scenarioNumber")
  set fs=Server.CreateObject("Scripting.FileSystemObject")
  set fo = fs.GetFolder("c:\upload\AnimalChange\")
  for each f in fo.files
  
  if f.Name="farm_"&Session("FarmNumber")&"_"&Request.Form("ScenarioID")&".xml" then
      fs.DeleteFile("c:\upload\AnimalChange\farm_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".xml") 
  end if
  next
'  set fo = fs.GetFolder("c:\upload\AnimalChange\ModelOutput")
'  for each f in fo.files
'  
'  if f.Name="error_"&Session("FarmNumber")&"_"&Request.Form("ScenarioID")&".txt" then
'      fs.DeleteFile("c:\upload\AnimalChange\ModelOutput\error_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt") 
'  end if
'  next
show=0
showresult=0
Dim postUrl
postUrl = "http://localhost/AnimalChange/ModelService/Model.asmx/GenerateInputXML"
Set xmlhttp = server.Createobject("MSXML2.XMLHTTP")
xmlhttp.Open "POST",postUrl,false
xmlhttp.setRequestHeader "Content-Type","application/x-www-form-urlencoded"
xmlhttp.send DataToSend

'Response.Write DataToSend & "<br>"
'Response.Write(xmlhttp.responseText)
'xmlhttp.send DataToSend
  for each f in fo.files
  if f.Name="farm_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".xml" then
   show=1
  end if
  next
'  set fo = fs.GetFolder("c:\upload\AnimalChange\ModelOutput\")
'  for each f in fo.files
'  if f.Name=" outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml" then
'   showresult=1
'  end if
'  next
  
  if show=1 then
   response.write "<a href=""upload/AnimalChange/farm_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".xml"">view file</a>"
   if showresult = 1 then
   	Response.Write "<br/><a href=""modelservice/showresult.aspx?filename=outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml""> Show result</a><br>"

   end if
  else 
   response.write "<br/><br/>The file has not been generated"
  end if
' end if 
'Else
'Response.Write "Loading for first Time" 
'End If
%>
<FORM method=POST name="form1" ID="Form1">
<!--Enter the two Values to perform creation of xml<br/><br/><br/>
<select name="Operation">Select Operation<option value="Sum">Sum</option><option value="Subtraction">Subtraction</option></select> -->
FarmNumber<br/>
<input type="text" name="FarmNumber" ID="FarmNumber" value="<%=Request.Form("FarmNumber")%>" disabled/><br/>
ScenarioID<br/>
<input type="text" name="ScenarioID" ID="ScenarioID" value="<%=Request.Form("ScenarioID")%>" disabled/>
<br/><br/>
<!--<input type="submit" value="GO" name="submit1" ID="Submit1"/>-->
</form>
<!--<FORM METHOD="POST" ENCTYPE="multipart/form-data" ACTION="uploadexmple.asp">
	<TABLE BORDER=0>
	<tr><td><b>Select a result-file to upload:</b><br><INPUT TYPE=FILE SIZE=50 NAME="FILE1"></td></tr>
	<tr><td align="center"><INPUT TYPE=SUBMIT VALUE="Upload!"></td></tr>
	</TABLE>
</FORM>-->
<a href="start.asp">Back</a>
</body>
</html>

