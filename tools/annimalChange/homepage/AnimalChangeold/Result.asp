<%
'**************************************************************************************************
'**************************************************************************************************
%>
<!-- #include file="includes/FunctionBalance.asp" -->
<!-- #include file="includes/FunctionSoil.asp" -->
<!-- #include file="includes/FunctionField.asp" -->
<%
'<!-- #include file="includes/FunctionResult.asp" -->
Private Const NUMBER_COL  = 3
Private Const NUMBER_ROW  = 10
Private Const NUMBER_COL_SURPLUS  = 4
Private Const NUMBER_ROW_SURPLUS  = 12
Private Const SURPLUSCORRECTION_HARVEST  = 0.45
Private Const SURPLUSCORRECTION_SOIL     = 0.10
Private Const SURPLUSCORRECTION_DENITR   = 0.10
Private Const SURPLUSCORRECTION_NLES     = 0.35

Private Const RED_NORM_CORRECTION_HARVEST  = 0.45'p33
Private Const RED_NORM_CORRECTION_NLES     = 0.35'p34
Private Const RED_NORM_CORRECTION_SOIL     = 0.11'p35
Private Const RED_NORM_CORRECTION_NH3EVAP  = 0.01'p36
Private Const RED_NORM_CORRECTION_DENITR   = 0.08'p37

'Private Const LOSS_MANURE_N_FIELD      = 0.07

dim arrOutput
dim arrInput
dim arrSurplus
dim surplus, surplusCorr, surplusNcorr
dim arrFeedResult
dim totalArea, animalUnit
dim nRotation, arrRotation
dim boughtFeed, soldFeed, BoughtBeddingN, SoldBeddingN
dim TotalBoughtManure, TotalSoldManure, HighEnergyConc, meanRunOff
dim minNormReduction
dim normPercent, reducedFertilizer,internalGrazingFE
dim fs,fo,f
   InitFarmNDB
   CreateLanguageDictionary
   CloseFarmNDB
version = Session("version")
  set fs=Server.CreateObject("Scripting.FileSystemObject")
  set fo = fs.GetFolder("c:\upload\AnimalChange\ModelOutput")

'normPercent = 100
'Dim arrParameter, returnValue
'**************************************************************************************************
sub CreateTabResult()

  for each f in fo.files
'  response.write f.Name &"<br/>"
  if f.Name=" outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml" then
      response.write "<br/><a href=""\upload\AnimalChange\ModelOutput\ outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml"" target=""new"">"&Str.Item("strViewOutputXml")&"</a>" 
  end if
  if f.Name=" outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xls" then
      response.write "<br/><a href=""\upload\AnimalChange\ModelOutput\ outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xls"" target=""new"">"&Str.Item("strViewOutputXls")&"</a>" 
  end if
  if f.Name=" outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&"livetockfile.xls" then
'  if f.Name=" outputFarm23456ScenarioNr2livetockfile.xls" then
      response.write "<br/><a href=""\upload\AnimalChange\ModelOutput\ outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&"livetockfile.xls"" target=""new"">Livestock result as xls</a>"
  end if
  if f.Name=" outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&"plantfile.xls" then
      response.write "<br/><a href=""\upload\AnimalChange\ModelOutput\ outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&"plantfile.xls"" target=""new"">"&Str.Item("strViewOutputPlantXls")&"</a>" 
  end if
  if f.Name=" dataCtool_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt" then
      response.write "<br/><a href=""\upload\AnimalChange\ModelOutput\ dataCtool_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt"" target=""new"">dataCtool-txt-file</a>"  
  end if
  if f.Name=" outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&"CtoolFile.xls" then
      response.write "<br/><a href=""\upload\AnimalChange\ModelOutput\ outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&"CtoolFile.xls"" target=""new"">"&Str.Item("strViewOutputCtoolXls")&"</a>"  
  end if
  next
'Dim tid0, tid1, tid2, tid3 
'tid0=Timer
'
''tid1=Timer
'   InitFarmNDB
'   CreateLanguageDictionary
'   reDim arrInput(NUMBER_ROW-1,NUMBER_COL-1)
'   reDim arrOutput(NUMBER_ROW-1,NUMBER_COL)
'   reDim arrSurplus(NUMBER_ROW_SURPLUS-1,NUMBER_COL_SURPLUS-1)
'   reDim arrFeedResult(NUMBER_FEED_ROW,NUMBER_FEED_COL-1)
'   
'   ReDim arrParameter(1)
'   arrParameter(0) = CLng(Session("farmNumber"))
'   arrParameter(1) = CInt(Session("scenarioNumber"))
'
'   totalArea = CallSPReturn ("GetTotalArea",arrParameter)
'   normPercent = CallSPReturn ("GetUserNormReductionValue",arrParameter)
''   totalArea = GetTotalArea(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
'   '***********************Dyreenheder for kvæg og grise vha stored procedure.
'   CloseFarmNDB
''   CalculateManure version, NPercent, scale,arrA,arrB,warning, TotalBoughtManure, TotalSoldManure
'   InitFarmNDB
'   Dim arranimal, dictanimal, nanimal
'   Set dictanimal = CreateObject("Scripting.Dictionary")
'   nanimal = CallSP("GetAnimalUnit "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber")), arranimal, dictanimal)
'   animalUnit = arranimal(0,dictanimal.item("DE"))
'   Set dictanimal = Nothing 
''   animalUnit = GetAnimalUnit(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
''   response.write "Test af animalUnit " & animalUnit
'   
'   
'    Set dictRotation = CreateObject("Scripting.Dictionary")
'   nRotation = CallSP("GetRotationList "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber")), arrRotation, dictRotation)
'
''   nRotation  = GetRotationList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation)
''   PrintArrayInTable2 arrRotation
'   DefineNormReductionScript
'   InitTableFeed arrFeedResult, boughtFeed, soldFeed, HighEnergyConc, BoughtBeddingN, SoldBeddingN,internalGrazingFE
'   InitTableInput 'CLng(Session("farmNumber")),CInt(Session("scenarioNumber"))
'   InitTableOutput
''   InitTableSurplus
''' tid2=Timer
'  
'   CreateTableResult
''tid3=Timer
'   CloseFarmNDB
'   CreateUpdateFormResult
'
''response.write "<br>Tid Function  = "&   tid1-tid0
''response.write "<br>Tid Beregninger  = "&   tid2-tid1
''response.write "<br>Tid NLess  = "&   tid3-tid2
'response.write "Session(""ShowResult"") :"&Session("ShowResult")
'response.write "Session(""ShowResult"") :"&Session("ShowResult")
response.write "<iframe class=""data"" name=""iframe"" src=""modelservice/showresult.aspx?filename=outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml&balance=N"" style=""width: 100%; height: 1500px;border: none;""></iframe></td>" & vbCr
'response.end
'   	Response.Redirect("RunModelService.asp")

end sub
'**************************************************************************************************
sub CreateTabResultGHG()
  for each f in fo.files
  if f.Name=" outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml" then
      response.write "<br/><a href=""\upload\AnimalChange\ModelOutput\ outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml"" target=""new"">"&Str.Item("strViewOutputXml")&"</a>" 
  end if
  if f.Name=" outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xls" then
      response.write "<br/><a href=""\upload\AnimalChange\ModelOutput\ outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xls"" target=""new"">"&Str.Item("strViewOutputXls")&"</a>" 
  end if
  if f.Name=" outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&"livetockfile.xls" then
'  if f.Name=" outputFarm23456ScenarioNr2livetockfile.xls" then
      response.write "<br/><a href=""\upload\AnimalChange\ModelOutput\ outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&"livetockfile.xls"" target=""new"">Livestock result as xls</a>"
  end if
  if f.Name=" outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&"plantfile.xls" then
      response.write "<br/><a href=""\upload\AnimalChange\ModelOutput\ outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&"plantfile.xls"" target=""new"">"&Str.Item("strViewOutputPlantXls")&"</a>" 
  end if
  if f.Name=" dataCtool_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt" then
      response.write "<br/><a href=""\upload\AnimalChange\ModelOutput\ dataCtool_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt"" target=""new"">dataCtool-txt-file</a>"  
  end if
  if f.Name=" outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&"CtoolFile.xls" then
      response.write "<br/><a href=""\upload\AnimalChange\ModelOutput\ outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&"CtoolFile.xls"" target=""new"">"&Str.Item("strViewOutputCtoolXls")&"</a>"  
  end if
  next
'Dim tid0, tid1, tid2, tid3 
'tid0=Timer
'
''tid1=Timer
'   InitFarmNDB
'   CreateLanguageDictionary
'   reDim arrInput(NUMBER_ROW-1,NUMBER_COL-1)
'   reDim arrOutput(NUMBER_ROW-1,NUMBER_COL)
'   reDim arrSurplus(NUMBER_ROW_SURPLUS-1,NUMBER_COL_SURPLUS-1)
'   reDim arrFeedResult(NUMBER_FEED_ROW,NUMBER_FEED_COL-1)
'   
'   ReDim arrParameter(1)
'   arrParameter(0) = CLng(Session("farmNumber"))
'   arrParameter(1) = CInt(Session("scenarioNumber"))
'
'   totalArea = CallSPReturn ("GetTotalArea",arrParameter)
'   normPercent = CallSPReturn ("GetUserNormReductionValue",arrParameter)
''   totalArea = GetTotalArea(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
'   '***********************Dyreenheder for kvæg og grise vha stored procedure.
'   CloseFarmNDB
''   CalculateManure version, NPercent, scale,arrA,arrB,warning, TotalBoughtManure, TotalSoldManure
'   InitFarmNDB
'   Dim arranimal, dictanimal, nanimal
'   Set dictanimal = CreateObject("Scripting.Dictionary")
'   nanimal = CallSP("GetAnimalUnit "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber")), arranimal, dictanimal)
'   animalUnit = arranimal(0,dictanimal.item("DE"))
'   Set dictanimal = Nothing 
''   animalUnit = GetAnimalUnit(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
''   response.write "Test af animalUnit " & animalUnit
'   
'   
'    Set dictRotation = CreateObject("Scripting.Dictionary")
'   nRotation = CallSP("GetRotationList "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber")), arrRotation, dictRotation)
'
''   nRotation  = GetRotationList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation)
''   PrintArrayInTable2 arrRotation
'   DefineNormReductionScript
'   InitTableFeed arrFeedResult, boughtFeed, soldFeed, HighEnergyConc, BoughtBeddingN, SoldBeddingN,internalGrazingFE
'   InitTableInput 'CLng(Session("farmNumber")),CInt(Session("scenarioNumber"))
'   InitTableOutput
''   InitTableSurplus
''' tid2=Timer
'  
'   CreateTableResult
''tid3=Timer
'   CloseFarmNDB
'   CreateUpdateFormResult
'
''response.write "<br>Tid Function  = "&   tid1-tid0
''response.write "<br>Tid Beregninger  = "&   tid2-tid1
''response.write "<br>Tid NLess  = "&   tid3-tid2
response.write "<iframe class=""data"" name=""iframe"" src=""modelservice/showresult.aspx?filename=outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml&balance=GHG"" style=""width: 100%; height: 1500px;border: none;""></iframe></td>" & vbCr
'response.end
'   	Response.Redirect("RunModelService.asp")

end sub
'**************************************************************************************************
sub CreateTabResultC()
  for each f in fo.files
  if f.Name=" outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml" then
      response.write "<br/><a href=""\upload\AnimalChange\ModelOutput\ outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml"" target=""new"">"&Str.Item("strViewOutputXml")&"</a>" 
  end if
  if f.Name=" outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xls" then
      response.write "<br/><a href=""\upload\AnimalChange\ModelOutput\ outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xls"" target=""new"">"&Str.Item("strViewOutputXls")&"</a>" 
  end if
  if f.Name=" outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&"livetockfile.xls" then
'  if f.Name=" outputFarm23456ScenarioNr2livetockfile.xls" then
      response.write "<br/><a href=""\upload\AnimalChange\ModelOutput\ outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&"livetockfile.xls"" target=""new"">Livestock result as xls</a>"
  end if
  if f.Name=" outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&"plantfile.xls" then
      response.write "<br/><a href=""\upload\AnimalChange\ModelOutput\ outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&"plantfile.xls"" target=""new"">"&Str.Item("strViewOutputPlantXls")&"</a>" 
  end if
  if f.Name=" dataCtool_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt" then
      response.write "<br/><a href=""\upload\AnimalChange\ModelOutput\ dataCtool_"&Session("FarmNumber")&"_"&Session("scenarioNumber")&".txt"" target=""new"">dataCtool-txt-file</a>"  
  end if
  if f.Name=" outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&"CtoolFile.xls" then
      response.write "<br/><a href=""\upload\AnimalChange\ModelOutput\ outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&"CtoolFile.xls"" target=""new"">"&Str.Item("strViewOutputCtoolXls")&"</a>"  
  end if
  next
	  'Dim tid0, tid1, tid2, tid3 
'tid0=Timer
'
''tid1=Timer
'   InitFarmNDB
'   CreateLanguageDictionary
'   reDim arrInput(NUMBER_ROW-1,NUMBER_COL-1)
'   reDim arrOutput(NUMBER_ROW-1,NUMBER_COL)
'   reDim arrSurplus(NUMBER_ROW_SURPLUS-1,NUMBER_COL_SURPLUS-1)
'   reDim arrFeedResult(NUMBER_FEED_ROW,NUMBER_FEED_COL-1)
'   
'   ReDim arrParameter(1)
'   arrParameter(0) = CLng(Session("farmNumber"))
'   arrParameter(1) = CInt(Session("scenarioNumber"))
'
'   totalArea = CallSPReturn ("GetTotalArea",arrParameter)
'   normPercent = CallSPReturn ("GetUserNormReductionValue",arrParameter)
''   totalArea = GetTotalArea(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
'   '***********************Dyreenheder for kvæg og grise vha stored procedure.
'   CloseFarmNDB
''   CalculateManure version, NPercent, scale,arrA,arrB,warning, TotalBoughtManure, TotalSoldManure
'   InitFarmNDB
'   Dim arranimal, dictanimal, nanimal
'   Set dictanimal = CreateObject("Scripting.Dictionary")
'   nanimal = CallSP("GetAnimalUnit "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber")), arranimal, dictanimal)
'   animalUnit = arranimal(0,dictanimal.item("DE"))
'   Set dictanimal = Nothing 
''   animalUnit = GetAnimalUnit(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
''   response.write "Test af animalUnit " & animalUnit
'   
'   
'    Set dictRotation = CreateObject("Scripting.Dictionary")
'   nRotation = CallSP("GetRotationList "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber")), arrRotation, dictRotation)
'
''   nRotation  = GetRotationList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation)
''   PrintArrayInTable2 arrRotation
'   DefineNormReductionScript
'   InitTableFeed arrFeedResult, boughtFeed, soldFeed, HighEnergyConc, BoughtBeddingN, SoldBeddingN,internalGrazingFE
'   InitTableInput 'CLng(Session("farmNumber")),CInt(Session("scenarioNumber"))
'   InitTableOutput
''   InitTableSurplus
''' tid2=Timer
'  
'   CreateTableResult
''tid3=Timer
'   CloseFarmNDB
'   CreateUpdateFormResult
'
''response.write "<br>Tid Function  = "&   tid1-tid0
''response.write "<br>Tid Beregninger  = "&   tid2-tid1
''response.write "<br>Tid NLess  = "&   tid3-tid2
response.write "<iframe class=""data"" name=""iframe"" src=""modelservice/showresult.aspx?filename=outputFarm"&Session("FarmNumber")&"ScenarioNr"&Session("scenarioNumber")&".xml&balance=C"" style=""width: 100%; height: 1500px;border: none;""></iframe></td>" & vbCr
'response.end
'   	Response.Redirect("RunModelService.asp")

end sub
'**************************************************************************************************
sub CreateTableResult()
   CreateTableInputOutput
   CreateTableTotal
   CreateTableSurplus
end sub
'**************************************************************************************************
sub CreateTableInputOutput()
response.write "<br/>"&Str.Item("strUtilizedN")&":<input type=""text"" id=""d7"" class=""noborder"" size=""5"" value="""&FormatNumber(100*MeanUtilization,1,-1,0,0)&"""/>"
   Response.Write("<form name=""formFarm"" action=""DummyFarm.asp"" method=""post"">" & vbCr)
   Response.Write("<table class=""data"" border=""0"" width=""100%"">" & vbCr)
   CreateHeaderInputOutput
   CreateDataInputOutput
   Response.Write("</table>" & vbCr)
   Response.Write("</form>" & vbCr)
end sub
'**************************************************************************************************
sub CreateHeaderInputOutput()
   Response.Write("  <tr>" & vbCr)   
   Response.Write("    <td width=""21%"">&nbsp;</td>" & vbCr)
   Response.Write("    <td width=""7%"">&nbsp;</td>" & vbCr)
   Response.Write("    <td width=""7%"">&nbsp;</td>" & vbCr)
   Response.Write("    <td width=""4%"">&nbsp;</td>" & vbCr)
   Response.Write("    <td width=""22%"" align=""center""><h1>"&Str.Item("strFARMSCALE")&"</h1></td>" & vbCr)
   Response.Write("    <td width=""4%"">&nbsp;</td>" & vbCr)
   Response.Write("    <td width=""6%"">&nbsp;</td>" & vbCr)
   Response.Write("    <td width=""7%"">&nbsp;</td>" & vbCr)
   Response.Write("    <td width=""6%"">&nbsp;</td>" & vbCr)
   Response.Write("    <td width=""16%"">&nbsp;</td>" & vbCr)
   Response.Write("  </tr>" & vbCr)
   
'   Response.Write("  <tr>" & vbCr)   
'   Response.Write("    <td colspan=""9"" align=""center""><h1>"&Str.Item("strFARMSCALE")&"</h1></td>" & vbCr)
'   Response.Write("  </tr>" & vbCr)
      
   Response.Write("  <tr>" & vbCr)   
   Response.Write("    <td colspan=""4"" align=""center""><h3>"&Str.item("strINPUTKGN")&  " </h3></td>" & vbCr)
'   Response.Write("    <td rowspan="""&rMax+1&""" valign=""middle"" align=""center""><a href=""url""><img src=""images/farm.gif"" width=""200"" height=""128"" border=""0"" alt=""text""></a></td>" & vbCr)
   Response.Write("    <td rowspan="""&NUMBER_ROW+2&""" valign=""middle"" align=""center""><br/><br/><br/><br/><img src=""images/farm.gif"" width=""200"" height=""128"" border=""0"" alt=""text""></td>" & vbCr)
   Response.Write("    <td colspan=""4"" align=""center""><h3>"&Str.Item("strOUTPUTKGN")&"</h3></td>" & vbCr)
   Response.Write("  </tr>" & vbCr)
   
   Response.Write("  <tr>" & vbCr)   
   Response.Write("    <th>&nbsp;</th>" & vbCr)
   Response.Write("    <td align=""right"">"&Str.Item("strkg N/ha")&"</th>" & vbCr)
   Response.Write("    <td align=""right""><i>"&Str.Item("strReducedNNorm")&"</i></th>" & vbCr)
   Response.Write("    <th>&nbsp;</th>" & vbCr)
   Response.Write("    <th>&nbsp;</th>" & vbCr)
   Response.Write("    <td align=""right"">"&Str.Item("strkg N/ha")&"</th>" & vbCr)
   Response.Write("    <td align=""right"">"&Str.Item("strAdjusted")&"</th>" & vbCr)
   Response.Write("    <td align=""right""><i>"&Str.Item("strReducedNNorm")&"</i></th>" & vbCr)
   Response.Write("    <th>&nbsp;</th>" & vbCr)
   Response.Write("  </tr>" & vbCr)
end sub
'**************************************************************************************************
sub CreateDataInputOutput()
   dim r
   
   for r = 0 to NUMBER_ROW-1
      Response.Write("  <tr>" & vbCr)
      Response.Write("    <td align=""left"">"&arrInput(r,0)&"</td>" & vbCr)
         if arrInput(r,1)>=0 then
		     if totalArea <> 0 then
'					  if r<=1 or r=9 then
'		           Response.Write("    <td align=""right""><input type=""text"" id=""c1"&r&""" class=""noborder"" size=""5"" value="""&FormatNumber(arrInput(r,1)/totalArea,1,-1,0,0)&"""/></td>" & vbCr)
'		           Response.Write("    <td align=""right""><input type=""text"" id=""d1"&r&""" class=""noborder"" size=""5"" value="""&FormatNumber(arrInput(r,2)/totalArea,1,-1,0,0)&"""/></td>" & vbCr)
'	             Response.Write("    <td align=""right"">-&gt;</td>" & vbCr)
'					  else
'		           Response.Write("    <td align=""right"">"&FormatNumber(arrInput(r,1)/totalArea,1,-1,0,0)&"</td>" & vbCr)
'		           Response.Write("    <td align=""right"">"&FormatNumber(arrInput(r,2)/totalArea,1,-1,0,0)&"</td>" & vbCr)
'	             Response.Write("    <td align=""right"">-&gt;</td>" & vbCr)
'	          end if
					  if r<=1 or r=9 then
		           Response.Write("    <td align=""right""><input type=""text"" id=""c1"&r&""" class=""noborder"" size=""5"" value="""&FormatNumber(arrInput(r,1),1,-1,0,0)&"""/></td>" & vbCr)
		           Response.Write("    <td align=""right""><input type=""text"" id=""d1"&r&""" class=""noborder"" size=""5"" value="""&FormatNumber(arrInput(r,2),1,-1,0,0)&"""/></td>" & vbCr)
	             Response.Write("    <td align=""right"">-&gt;</td>" & vbCr)
					  else
		           Response.Write("    <td align=""right"">"&FormatNumber(arrInput(r,1),1,-1,0,0)&"</td>" & vbCr)
		           Response.Write("    <td align=""right"">"&FormatNumber(arrInput(r,2),1,-1,0,0)&"</td>" & vbCr)
	             Response.Write("    <td align=""right"">-&gt;</td>" & vbCr)
	          end if
	         else
		           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
		           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
		           Response.Write("    <td align=""right"">-&gt;</td>" & vbCr)
'	             Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
	         end if
'	           Response.Write("    <td align=""right"">"&FormatNumber(arrInput(r,1)/totalArea,1,-1,0,0)&"</td>" & vbCr)
'             Response.Write("    <td align=""right"">-&gt;</td>" & vbCr)
         else
	           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
	           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
	           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
'             Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
         end if
      if arrOutput(r,1)>=0 then
		    if totalArea <> 0 then
				  if r>=8 then
		           if r=9 then
								Response.Write("    <td align=""left"">-&gt;</td>" & vbCr)
'								Response.Write("    <td align=""left"">&nbsp;</td>" & vbCr)
								Response.Write("    <td align=""right"">"&FormatNumber(arrOutput(r,1),1,-1,0,0)&"</td>" & vbCr)
		           else 
								Response.Write("    <td align=""left"">&nbsp;</td>" & vbCr)
'								Response.Write("    <td align=""left"">&nbsp;</td>" & vbCr)
								Response.Write("    <td align=""left"">&nbsp;</td>" & vbCr)
		           end if
		            Response.Write("    <td align=""right""><input type =""text"" id=""i1"&r&""" class=""noborder"" size=""5"" value="""&FormatNumber(arrOutput(r,2),1,-1,0,0)&"""/></td>" & vbCr)
		            Response.Write("    <td align=""right""><input type =""text"" id=""j1"&r&""" class=""noborder"" size=""5"" value="""&FormatNumber(arrOutput(r,3),1,-1,0,0)&"""/></td>" & vbCr)
				  else
		            Response.Write("    <td align=""left"">-&gt;</td>" & vbCr)
		            Response.Write("    <td align=""right"">"&FormatNumber(arrOutput(r,1),1,-1,0,0)&"</td>" & vbCr)
		            Response.Write("    <td align=""left"">&nbsp;</td>" & vbCr)
		            Response.Write("    <td align=""left"">&nbsp;</td>" & vbCr)
		      end if
'		    if totalArea <> 0 then
'               Response.Write("    <td align=""right"">"&FormatNumber(arrOutput(r,1)/totalArea,1,-1,0,0)&"</td>" & vbCr)
'		    else
'               Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
'		    end if
         else
           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
	         Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
	         Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
         end if
        else
          Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
          Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
         Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
         Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
        end if
      Response.Write("    <td align=""right"">"&arrOutput(r,0)&"</td>" & vbCr)
	  Response.Write("  </tr>" & vbCr)
   next 
	  Response.Write("  <input type=""hidden"" id=""p33"" value="""&RED_NORM_CORRECTION_HARVEST&"""/>" & vbCr)   
	  Response.Write("  <input type=""hidden"" id=""p34"" value="""&RED_NORM_CORRECTION_NLES&"""/>" & vbCr) 
	  Response.Write("  <input type=""hidden"" id=""p35"" value="""&RED_NORM_CORRECTION_SOIL&"""/>" & vbCr)  
	  Response.Write("  <input type=""hidden"" id=""p36"" value="""&RED_NORM_CORRECTION_NH3EVAP&"""/>" & vbCr)   
	  Response.Write("  <input type=""hidden"" id=""p37"" value="""&RED_NORM_CORRECTION_DENITR&"""/>" & vbCr)  
	  Response.Write("  <input type=""hidden"" id=""runoff"" value="""&meanRunOff&"""/>" & vbCr)  
end sub
'**************************************************************************************************
sub CreateTableSurplus()
   dim r

   Response.Write("<table class=""data"" align=""center"" border=""0"">" & vbCr)
   Response.Write("  <tr>" & vbCr)   
   Response.Write("    <td>&nbsp;</td>" & vbCr)
   Response.Write("    <td>&nbsp;</td>" & vbCr)
   Response.Write("    <td>&nbsp;</td>" & vbCr)

   Response.Write("  </tr>" & vbCr)
   
   Response.Write("  <tr>" & vbCr)   
   Response.Write("    <td colspan=""3"" align=""center""><h3>"&Str.Item("strFordelingNoverskud")&"</h3></td>" & vbCr)
   Response.Write("    <td>&nbsp;</td>" & vbCr)
   Response.Write("    <td>&nbsp;</td>" & vbCr)
   Response.Write("    <td>&nbsp;</td>" & vbCr)
   Response.Write("    <td>&nbsp;</td>" & vbCr)
   Response.Write("  </tr>" & vbCr)
'   Response.Write("  <tr>" & vbCr)   
'   Response.Write("    <th>&nbsp;</th>" & vbCr)
'   Response.Write("    <td>&nbsp;</td>" & vbCr)
'   Response.Write("    <th align=""right"">"&Str.Item("strTOTAL")&"</th>" & vbCr)
'   Response.Write("    <td>&nbsp;</td>" & vbCr)
'   Response.Write("    <th align=""right"">"&Str.Item("strPERHA")&"</th>" & vbCr)
'   Response.Write("    <td>&nbsp;</td>" & vbCr)
'   Response.Write("    <th align=""right"">"&Str.Item("strPERHA")&"</th>" & vbCr)
'   Response.Write("  </tr>" & vbCr)
   Response.Write("  <tr>" & vbCr)   
   Response.Write("    <td>&nbsp;</th>" & vbCr)
   Response.Write("    <td>&nbsp;</td>" & vbCr)
   Response.Write("    <th align=""right"">"&Str.Item("strkg N/ha")&"</th>" & vbCr)
   Response.Write("    <td>&nbsp;</td>" & vbCr)
   Response.Write("    <th align=""right"">"&Str.Item("strAdjusted")&"</th>" & vbCr)
   Response.Write("    <td>&nbsp;</td>" & vbCr)
'   Response.Write("    <th align=""right""><form name=""norm""><input type=""text"" name=""normreduction"" value=""100"" size=""3"" maxlength=""3"" onChange=""if(InputValidator(document.forms.norm, 1, 100, "normreduction", 3)){return(true);};""></form>"&Str.Item("strReducedNNorm")&"</th>" & vbCr)
'   Response.Write("    <th align=""right""><input type=""text"" name=""normreduction"" value=""100"" size=""3"" maxlength=""3"" onChange=""if(InputValidator(document.forms.norm.normreduction, 1, 100, "", 3)){return(true);};"">"&Str.Item("strReducedNNorm")&"</th>" & vbCr)
   If not Isnull(meanUtilization) and meanUtilization<>0 then
	 minNormReduction = Clng(100*arrInput(1,1)*meanUtilization/((arrInput(0,1)+(arrInput(1,1))*meanUtilization)))
   else
	 minNormReduction = 0
   end if
   Response.Write("    <th align=""right""><input type=""text"" name=""normreduction"" id=""normreduction"" value="""&normPercent&""" size=""2"" maxlength=""3"" onChange=""if(this.value>100||isNaN(this.value)){this.value=100;};if(this.value<"&minNormReduction&"){alert('der kan ikke reduceres til mindre end "&minNormReduction&" %');this.value=100;};normreduction(this.value);"">"&Str.Item("str%ofNNorm")&"</th>" & vbCr)
   Response.Write("    <td><input type=""button"" value=""OK""/></td>" & vbCr)
   Response.Write("  </tr>" & vbCr)

   for r = 0 to NUMBER_ROW_SURPLUS-1
'			if r <  NUMBER_ROW_SURPLUS-1 then
'        Response.Write("  <tr>" & vbCr)
'			else
			if r = 10 or r = 3 or r = 4 then
        Response.Write("  <tr><td colspan=""7"" style=""border-bottom: solid navy 1px;height: 1px;font-size: 0px;padding: 0px;>"">&nbsp;</td>" & vbCr)
        Response.Write("  </tr>" & vbCr)
        Response.Write("  <tr>" & vbCr)
			else
        Response.Write("  <tr>" & vbCr)
			end if
      Response.Write("    <td align=""left"">"&arrSurplus(r,0)&"</td>" & vbCr)
      Response.Write("    <td>&nbsp;</td>" & vbCr)
         if  not IsNull(arrSurplus(r,1)) then
'	         Response.Write("    <td align=""right"">"&FormatNumber(arrSurplus(r,1),0,-1,0,0)&"</td>" & vbCr)
'       	         Response.Write("    <td>&nbsp;</td>" & vbCr)
		   	     if totalArea <> 0 then
			           Response.Write("    <td align=""right"">"&FormatNumber(arrSurplus(r,1),1,-1,0,0)&"</td>" & vbCr)
			           Response.Write("    <td>&nbsp;</td>" & vbCr)
			           if r <>10 then'<  NUMBER_ROW_SURPLUS-1 then
			           Response.Write("    <td align=""right""><input type=""text"" id=""i"&Cint(40+r-6)&""" class=""noborder"" size=""5"" value="""&FormatNumber(arrSurplus(r,2),1,-1,0,0)&"""/></td>" & vbCr)
			           Response.Write("    <td>&nbsp;</td>" & vbCr)
			           Response.Write("    <td align=""right""><input type=""text"" id=""j"&Cint(40+r-6)&""" class=""noborder"" size=""5"" value="""&FormatNumber(arrSurplus(r,3),1,-1,0,0)&"""/></td>" & vbCr)
			           else
			           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
			           Response.Write("    <td>&nbsp;</td>" & vbCr)
			           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
			           end if
				     else
			           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
			           Response.Write("    <td>&nbsp;</td>" & vbCr)
			           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
             end if
         else
	         Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
	         Response.Write("    <td>&nbsp;</td>" & vbCr)
                 Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
                 Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
                 Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
                 Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
                 Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
         end if
if r=10 then				 
   Response.Write("  </tr><td></td>" & vbCr)
   Response.Write("  <tr>" & vbCr)
end if
   Response.Write("  </tr>" & vbCr)
   next   

   Response.Write("</table>" & vbCr)

end sub
'**************************************************************************************************
sub CreateTableTotal()

   Response.Write("<table class=""data"" align=""center"" border=""0"">" & vbCr) 
   Response.Write("  <tr>" & vbCr)   
   Response.Write("    <td class=""data"">&nbsp;</td>" & vbCr)
   Response.Write("    <td class=""data"">&nbsp;</td>" & vbCr)
	 Response.Write("      <td class=""data"" align=""right"">"&Str.Item("strkg N/ha")&"</td>" & vbCr)
	 Response.Write("      <td class=""data"" align=""right"">"&Str.Item("strAdjusted")&"</td>" & vbCr)
	 Response.Write("      <td class=""data"" align=""right"">"&Str.Item("strReducedNNorm")&"</td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   Response.Write("    <tr>" & vbCr)
'      if surplus >= 0 then 
'         Response.Write("      <td align=""right"">"&FormatNumber(surplus,0,-1,0,0)&"</td>" & vbCr)
'         Response.Write("      <td class=""data"">"&Str.Item("strkg N")&"</td>" & vbCr)
   	     if totalArea <> 0 then
   					Response.Write("    <td class=""data"">"&Str.Item("strBedriftNoverskud")&":</td>" & vbCr)
   					Response.Write("    <td class=""data"">&nbsp;</td>" & vbCr)
	           Response.Write("    <td align=""right"">"&FormatNumber(surplus,1,-1,0,0)&"</td>" & vbCr)
	           Response.Write("    <td align=""right""><input type=""text"" id=""g23"" class=""noborder"" size=""5"" value="""&FormatNumber(surplusCorr,1,-1,0,0)&"""/></td>" & vbCr)
	           Response.Write("    <td align=""right""><input type=""text"" id=""h23"" class=""noborder"" size=""5"" value="""&FormatNumber(surplusNcorr,1,-1,0,0)&"""/></td>" & vbCr)
		     else
   					Response.Write("    <td class=""data"">&nbsp;</td>" & vbCr)
   					Response.Write("    <td class=""data"">&nbsp;</td>" & vbCr)
	           Response.Write("    <td align=""right"">-</td>" & vbCr)
	           Response.Write("    <td align=""right"">-</td>" & vbCr)
	           Response.Write("    <td align=""right"">-</td>" & vbCr)
'         Response.Write("      <td align=""right"">"&FormatNumber(surplus/totalArea,1)&"</td>" & vbCr)
'	 Response.Write("      <td class=""data"">"&Str.Item("strkg N/ha")&"</td>" & vbCr)

'      else
'         Response.Write("      <td align=""right"">-999,9</td>" & vbCr)
'         Response.Write("      <td class=""data"">"&Str.Item("strkg N")&"</td>" & vbCr)
'         Response.Write("      <td align=""right"">-999,9</td>" & vbCr)
'	 Response.Write("      <td class=""data"">"&Str.Item("strKGNPERHA")&"</td>" & vbCr)
      end if

   Response.Write("    </tr>" & vbCr)
   Response.Write("  <tr>" & vbCr)   
   Response.Write("    <td class=""data"">"&Str.Item("strAREA")&" "&Str.Item("strCOLON")& "</td>" & vbCr)
'   Response.Write("    <td class=""data"">&nbsp;</td>" & vbCr)
   Response.Write("    <td align=""right"">"&FormatNumber(totalArea,0,-1,0,0)&"</td>" & vbCr)
   Response.Write("    <td class=""data"">"&Str.Item("strHA")&"</td>" & vbCr)
   Response.Write("    <td class=""data"">&nbsp;</td>" & vbCr)
   Response.Write("    <td class=""data"">&nbsp;</td>" & vbCr)
   Response.Write("  </tr>" & vbCr)
'if session("Language") <> "uk"  then
   Response.Write("  <tr>" & vbCr)   
   Response.Write("    <td class=""data"">"&Str.Item("strHERD")&" "&Str.Item("strCOLON")&"</td>" & vbCr)
'   Response.Write("    <td class=""data"">&nbsp;</td>" & vbCr)
   Response.Write("    <td align=""right"">"&FormatNumber(animalUnit,0,-1,0,0)&"</td>" & vbCr)
   Response.Write("    <td class=""data"">"&Str.Item("strLU")&"</td>" & vbCr)
'   	     if totalArea <> 0 then
'	           Response.Write("    <td align=""right"">"&FormatNumber(animalUnit/totalarea,1)&"</td>" & vbCr)
'		     else
'	           Response.Write("    <td align=""right"">-</td>" & vbCr)
'             end if
'   Response.Write("    <td align=""right"">"&FormatNumber(animalUnit/totalarea,1)&"</td>" & vbCr)
'   Response.Write("    <td class=""data"">DE/ha</td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
'end if
   Response.Write("    <tr>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
   Response.Write("</table>" & vbCr)
end sub
'**************************************************************************************************
sub InitTableInput()
dim boughtFertilizer, pigs, BoughtPigs, a,b,c,d
'PrintArrayInTable2 arrRotation
 '  boughtFertilizer = GetBoughtFertilizer(arrRotation)
 ReDim arrParameter(3)
   arrParameter(0) = CLng(Session("farmNumber"))
   arrParameter(1) = CInt(Session("scenarioNumber"))
   arrParameter(2) = 1
   arrParameter(3) = 415
   a = CallSPReturn ("GetPigHerdTotal",arrParameter)
'   a=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 1, 415) - 
   arrParameter(2) = 3
   arrParameter(3) = 414
   b = CallSPReturn ("GetPigHerdTotal",arrParameter)
'   b=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 3, 414) + 
   arrParameter(2) = 3
   arrParameter(3) = 415
   c = CallSPReturn ("GetPigHerdTotal",arrParameter)
'   c=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 3, 415) - 
   arrParameter(2) = 2
   arrParameter(3) = 414
   d = CallSPReturn ("GetPigHerdTotal",arrParameter)
'   d=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 2, 414)
   pigs =a-b+c-d 

   if pigs < 0 then 
   BoughtPigs=-pigs
   else 
   BoughtPigs=0
   end if
   arrInput(0,0)  = ""&Str.Item("strMINFERT")&"" 
   ReDim preserve arrParameter(1)
   arrInput(0,1)  = CallSPReturn ("GetFertilizer",arrParameter)/totalArea
'   arrInput(0,1)  = GetFertilizer(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   arrInput(1,0)  = ""&Str.Item("strMANUREBUY")&""
   arrInput(1,1)  = TotalBoughtManure/totalArea
   arrInput(0,2)  = round(arrInput(0,1),1)-(((meanUtilization)*round(arrInput(1,1),1)+round(arrInput(0,1),1))-((meanUtilization)*round(arrInput(1,1),1)+round(arrInput(0,1),1))*(normPercent/100))
   reducedFertilizer = round(arrInput(0,1),1)-round(arrInput(0,2),1)
   arrInput(1,2)  = TotalBoughtManure/totalArea
   arrInput(2,0)  = ""&Str.Item("strFEEDBUY")&""
   arrInput(2,1)  = boughtFeed/totalArea
   arrInput(2,2)  = boughtFeed/totalArea
   arrInput(3,0)  = ""&Str.Item("strSTRAWBUY")&""
   arrInput(3,1)  = BoughtBeddingN/totalArea
   arrInput(3,2)  = BoughtBeddingN/totalArea
   arrInput(4,0)  = ""&Str.Item("strANIMBUY")&""
   arrInput(4,1)  = BoughtPigs/totalArea
   arrInput(4,2)  = BoughtPigs/totalArea
   arrInput(5,0)  = ""&Str.Item("strSEED")&""
   arrInput(5,1)  = CallSPReturn ("GetTotalNInSeed",arrParameter)/totalArea
   arrInput(5,2)  = arrInput(5,1)
'   arrInput(5,1)  = GetTotalNInSeed(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   arrInput(6,0)  = ""&Str.Item("strNFIX")&""
   arrInput(6,1)  = GetFixation(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation)/totalArea
   arrInput(6,2)  = arrInput(6,1)
   arrInput(7,0)  = ""&Str.Item("strATMOSDEP")&""
   arrInput(7,1)  = 15'*totalArea
   arrInput(7,2)  = arrInput(7,1)
   arrInput(8,0)  = ""
   arrInput(8,1)  = null
   arrInput(8,2)  = null
   arrInput(9,0)  = ""&Str.Item("strTOTINPUT")&""
'   Response.write "<br> Test: " & arrInput(2,1)
   arrInput(9,1)  = arrInput(0,1)+arrInput(1,1)+arrInput(2,1)+arrInput(3,1)+arrInput(4,1)+arrInput(5,1)+arrInput(6,1)+arrInput(7,1)
   arrInput(9,2)  = arrInput(9,1)-(reducedFertilizer)
'   Response.write "<br> Test2: " & arrInput(9,1)
end sub
'**************************************************************************************************
sub InitTableOutput()
dim pigs, SoldPigs, nMilk, arrMilk, dictMilk, a, b, c, d
 
ReDim arrParameter(3)
   arrParameter(0) = CLng(Session("farmNumber"))
   arrParameter(1) = CInt(Session("scenarioNumber"))
   arrParameter(2) = 1
   arrParameter(3) = 415
   a = CallSPReturn ("GetPigHerdTotal",arrParameter)
'	 response.write "<br>a: "&a
'   a=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 1, 415) - 
   arrParameter(2) = 3
   arrParameter(3) = 414
   b = CallSPReturn ("GetPigHerdTotal",arrParameter)
'	 response.write "<br>b: "&b
'   b=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 3, 414) + 
   arrParameter(2) = 3
   arrParameter(3) = 415
   c = CallSPReturn ("GetPigHerdTotal",arrParameter)
'	 response.write "<br>c: "&c
'   c=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 3, 415) - 
   arrParameter(2) = 2
   arrParameter(3) = 414
   d = CallSPReturn ("GetPigHerdTotal",arrParameter)
'	 response.write "<br>d: "&d
'   d=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 2, 414)
   pigs =a-b+c-d
'	 response.write "<br>pigs: "&pigs
	  
 
 
 
 
' pigs = GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 1, 415) - GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 3, 414) + GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 3, 415) - GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 2, 414)
 
 
 
   set dictMilk = CreateObject("Scripting.Dictionary")
   nMilk = CallSP ("CalcTotalMilkkg " & CLng(Session("farmNumber")) & "," & CInt(Session("scenarioNumber")), arrMilk, dictMilk)
   nMilk = arrMilk(0, dictMilk.item("SumMilkkg"))
 
' nMilk = CalcTotalMilkkg(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrMilk) 
   If IsNull(arrMilk(0, dictMilk.item("SumMilkkg"))) Then 
   arrMilk(0, dictMilk.item("SumMilkkg"))=0
   End If
   arrMilk(0, dictMilk.item("SumMilkkg"))=arrMilk(0, dictMilk.item("SumMilkkg"))*0.00518
   if pigs > 0 then 
   SoldPigs=pigs
   else 
   SoldPigs=0
   end if
   arrOutput(0,0)  = ""&Str.Item("strCASHCROP")&""
   arrOutput(0,1)  = GetSoldCrop/totalArea
   arrOutput(1,0)  = ""&Str.Item("strANIMSOLD")&""
   arrOutput(1,1)  = SoldPigs/totalArea
   arrOutput(2,0)  = ""&Str.Item("strMEAT")&""
   ReDim preserve arrParameter(2)
   arrParameter(2) = PARAMETER_GROWTH_N
   arrOutput(2,1)  = CallSPReturn ("GetFarmTotal",arrParameter)/totalArea
'   arrOutput(2,1)  = GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), PARAMETER_GROWTH_N)
   arrOutput(3,0)  = ""&Str.Item("strMILK")&""
   arrOutput(3,1)  = arrMilk(0, dictMilk.item("SumMilkkg"))/totalArea
   arrOutput(4,0)  = ""&Str.Item("strMANURESOLD")&""
   arrOutput(4,1)  = TotalSoldManure/totalArea ' GetSoldManureTotal(8)
   arrOutput(5,0)  = ""&Str.Item("strFEEDSOLD")&""
   arrOutput(5,1)  = soldFeed/totalArea
   arrOutput(6,0)  = ""&Str.Item("strSoldStraw")&""
   arrOutput(6,1)  = SoldBeddingN/totalArea
   arrOutput(7,0)  = ""&Str.Item("strSoldPigMeat")&""

   ReDim  preserve arrParameter(3)
   arrParameter(0) = CLng(Session("farmNumber"))
   arrParameter(1) = CInt(Session("scenarioNumber"))
   arrParameter(2) = 2
   arrParameter(3) = 415
'   arrParameter(3) = 407
   a = CallSPReturn ("GetPigHerdTotal",arrParameter)
   ReDim  preserve arrParameter(2)
   arrParameter(2) = 413
   b = CallSPReturn ("GetFarmTotal",arrParameter)
   arrOutput(7,1)  = (clng(a)+Clng(b))/Clng(totalArea)
'	 response.write "<br>arrOutput(7,1): "&arrOutput(7,1)
'	 response.write "<br>totalArea: "&totalArea
'	 response.write "<br>a+b/totalArea: "&a+b/totalArea

'   arrOutput(7,1)  = GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 2, 415)+GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 413)
   arrOutput(8,0)  = ""&Str.Item("strPlantUptake")&""
   arrOutput(8,2)  = arrOutput(0,1)+arrOutput(5,1)+arrOutput(6,1)'null

   arrOutput(9,0) = ""&Str.Item("strOUTPUTALL")&""
   arrOutput(9,1) = arrOutput(0,1)+arrOutput(1,1)+arrOutput(2,1)+arrOutput(3,1)+arrOutput(4,1)+arrOutput(5,1)+arrOutput(6,1)+arrOutput(7,1)
end sub
'**************************************************************************************************
sub InitTableSurplus()
   dim aSoilChange, aBalance, NLeaching
   dim nNLES,meanNLES_N,meanNLES_mg,arrNLES, remainder,deltaD,deltaU

   surplus = arrInput(9,1)- arrOutput(9,1)
		 
   ReDim  arrParameter(1)
   arrParameter(0) = CLng(Session("farmNumber"))
   arrParameter(1) = CInt(Session("scenarioNumber"))
   
   arrSurplus(0,0)  = ""&Str.Item("strNH3HOUSE")&""
   arrSurplus(0,1)  = CallSPReturn ("GetNitrogenStableLossTotal",arrParameter)/totalArea
   arrSurplus(0,2)  = arrSurplus(0,1)
   arrSurplus(0,3)  = arrSurplus(0,1)
'   arrSurplus(0,1)  = GetNitrogenStableLossTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   arrSurplus(1,0)  = ""&Str.Item("strNH3STORE")&""
   arrSurplus(1,1)  = CallSPReturn ("GetNitrogenStorageLossTotal",arrParameter)/totalArea
   arrSurplus(1,2)  = arrSurplus(1,1)
   arrSurplus(1,3)  = arrSurplus(1,1)
'   arrSurplus(1,1)  = GetNitrogenStorageLossTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   arrSurplus(2,0)  = ""&Str.Item("strDENITRIF")&", " & Str.Item("storage")&""
   arrSurplus(2,1)  = CallSPReturn ("GetNitrogenStorageDenitrificationTotal",arrParameter)/totalArea
   arrSurplus(2,2)  = arrSurplus(2,1)
   arrSurplus(2,3)  = arrSurplus(2,1)
   arrSurplus(3,0)  = ""&Str.Item("strFieldSurplus")&""
   arrSurplus(3,1)  = surplus-arrSurplus(0,1)-arrSurplus(1,1)-arrSurplus(2,1)
   arrSurplus(3,2)  = arrSurplus(3,1)
'   arrSurplus(2,1) = GetNitrogenStorageDenitrificationTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   arrSurplus(4,0)  = ""&Str.Item("strNH3GRAZING")&""
   arrSurplus(4,1)  = LOSS_MANURE_N_FIELD * CallSPReturn ("fnGetBoughtGrazingN ",arrParameter)/totalArea
  ReDim  preserve arrParameter(2)
   arrParameter(2)  = PARAMETER_MANURE_N_FIELD
   arrSurplus(4,1)  = arrSurplus(4,1) + LOSS_MANURE_N_FIELD * CallSPReturn ("GetFarmTotal",arrParameter)/totalArea
   arrSurplus(4,2)  = arrSurplus(4,1)
   arrSurplus(4,3)  = arrSurplus(4,1)
'   arrSurplus(3,1)  = 0.07*GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), PARAMETER_MANURE_N_FIELD)
  ReDim  preserve arrParameter(1)
'  Response.write "<br> arrParameter "& arrParameter(0) & " " & arrParameter(1)
   arrSurplus(5,0)  = ""&Str.Item("strNH3SPREAD")&""
   arrSurplus(5,1)  = CallSPReturn ("GetTotalLossManure",arrParameter)/totalArea
   arrSurplus(5,2)  = arrSurplus(5,1)
   arrSurplus(5,3)  = arrSurplus(5,1)
'  Response.write "<br> arrSurplus "& arrSurplus(4,1)
'   arrSurplus(4,1)  = GetTotalLossManure(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   arrSurplus(6,0) = ""&Str.Item("strNH3MIN")&""
   arrSurplus(6,1) = 0.03 * CallSPReturn ("GetFertilizer",arrParameter)/totalArea
   arrSurplus(6,2) = arrSurplus(6,1)
   arrSurplus(6,3) = round(arrSurplus(6,2),1)-reducedFertilizer*RED_NORM_CORRECTION_NH3EVAP
'   arrSurplus(5,1) = 0.03*GetFertilizer(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   arrSurplus(7,0) = ""&Str.Item("strDENITRIF")&", " & Str.Item("strsoil")&""
   arrSurplus(7,1) = (Denitrification(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),CInt(Session("farmType")),arrRotation))/totalArea'*totalArea'der skal ikke ganges med arealet her
  ' arrSurplus(6,0) = ""&Str.Item("strDENITRIF")&" " & Str.Item("storage")&""
  ' arrSurplus(6,1) = GetNitrogenStorageDenitrificationTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
'  Response.write "<br> Denitrification "& Denitrification(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),CInt(Session("farmType")),arrRotation)
  aBalance = surplus - arrSurplus(0,1) - arrSurplus(1,1) - arrSurplus(2,1) - arrSurplus(4,1) - arrSurplus(5,1) - arrSurplus(6,1) - arrSurplus(7,1)

  aSoilChange = GetSoilChange(arrRotation,totalArea)/totalArea
  NLeaching = NitrogenLeaching(aBalance, aSoilChange, totalArea)
  nNLES = CalcN_Les(meanNLES_N,meanNLES_mg,meanRunOff,arrNLES)/totalArea
	remainder = NLeaching-meanNLES_N'*totalArea
'	deltaD
'   Response.write "<br> remainder "& remainder
'   Response.write "<br> NLeaching "& NLeaching
'   Response.write "<br> meanNLES_N "& meanNLES_N
'   Response.write "<br> aBalance "& aBalance

   arrSurplus(8,0) = ""&Str.Item("strSOILCHANGE")&""
      if not IsNull(aSoilChange) then
         arrSurplus(8,1) = aSoilChange
      else 
         arrSurplus(8,1) = null
      end if
'  arrSurplus(8,0) = ""&Str.Item("strNLEACH")&""
'      if not IsNull(NLeaching) then
'         arrSurplus(8,1) = NLeaching
'      else 
'         arrSurplus(8,1) = null
'      end if
  arrSurplus(9,0) = Str.Item("strNLEACH")& " ("&Str.Item("strkg N/ha")&")"
      if not IsNull(meanNLES_N) then
         arrSurplus(9,1) = meanNLES_N'*totalArea
      else 
         arrSurplus(9,1) = null
      end if
  arrSurplus(10,0) = ""&Str.Item("strRemainder")&""
  arrSurplus(11,0) = ""&Str.Item("strNLEACH")& " ("&Str.Item("strMgNO3/l")&")"
'      if not IsNull(meanNLES_N*totalArea-NLeaching) then
'         arrSurplus(9,1) = meanNLES_N*totalArea-NLeaching
      if not IsNull(remainder) and meanNLES_N<>0 then
			'den første måde at fordele rest på
'         arrSurplus(10,1) = remainder
''				 arrSurplus(6,2) = arrSurplus(6,1)-(arrSurplus(6,1)/(arrSurplus(6,1)+aSoilChange)*remainder)
'				 arrSurplus(7,2) = arrSurplus(7,1)+(Cdbl(arrSurplus(7,1))/(Cdbl(arrSurplus(7,1))+Cdbl(meanNLES_N*totalArea))*Cdbl(remainder))
'				 arrSurplus(7,3) = arrSurplus(7,2)
'				 arrSurplus(8,2) = aSoilChange+(meanNLES_N*totalArea/(arrSurplus(7,1)+meanNLES_N*totalArea)*remainder*0.5)
'				 arrSurplus(8,3) = arrSurplus(8,2)
'				 arrSurplus(9,2) = (meanNLES_N*totalArea)+(meanNLES_N*totalArea/(arrSurplus(7,1)+meanNLES_N*totalArea)*remainder*0.5)
'				 arrSurplus(9,3) = arrSurplus(9,2)
       'den nye måde at fordele rest på
         arrSurplus(10,1) = remainder
         arrSurplus(11,1) = meanNLES_mg'*totalArea
				 arrOutput(8,2) = arrOutput(8,2)+ remainder*SURPLUSCORRECTION_HARVEST
				 arrOutput(8,3) = round(arrOutput(8,2),1)-reducedFertilizer*RED_NORM_CORRECTION_HARVEST
				 surplusCorr = surplus-remainder*SURPLUSCORRECTION_HARVEST
         arrSurplus(3,2)  = surplusCorr-arrSurplus(0,1)-arrSurplus(1,1)-arrSurplus(2,1)
				 arrOutput(9,2) = arrOutput(8,2)+arrOutput(1,1)+arrOutput(2,1)+arrOutput(3,1)+arrOutput(4,1)+arrOutput(7,1)
				 arrOutput(9,3) = round(arrOutput(9,2),1)-(round(arrOutput(8,2),1)-round(arrOutput(8,3),1))
'				 arrSurplus(6,2) = arrSurplus(6,1)-(arrSurplus(6,1)/(arrSurplus(6,1)+aSoilChange)*remainder)
				 arrSurplus(7,2) = arrSurplus(7,1)+ remainder*SURPLUSCORRECTION_DENITR
				 arrSurplus(7,3) = round(arrSurplus(7,2),1)-reducedFertilizer*RED_NORM_CORRECTION_DENITR
				if arrSurplus(7,3)< 0 then arrSurplus(7,3)= 0 
				 arrSurplus(8,2) = (arrSurplus(8,1)+ remainder*SURPLUSCORRECTION_SOIL)
				 arrSurplus(8,3) = round(arrSurplus(8,2),1)-reducedFertilizer*RED_NORM_CORRECTION_SOIL
				 arrSurplus(9,2) = arrSurplus(9,1)+ remainder*SURPLUSCORRECTION_NLES
				 arrSurplus(11,2) = 100*arrSurplus(9,2)*4.43/meanRunOff
				 arrSurplus(9,3) = round(arrSurplus(9,2),1)-reducedFertilizer*RED_NORM_CORRECTION_NLES
				 arrSurplus(11,3) = 100*round(arrSurplus(9,3),1)*4.43/meanRunOff
      else 
         arrSurplus(10,1) = null
      end if
			surplusNcorr = round(arrInput(9,2),1)-round(arrOutput(9,3),1)
      arrSurplus(3,3)  = surplusNcorr-arrSurplus(0,1)-arrSurplus(1,1)-arrSurplus(2,1)
end sub
'**************************************************************************************************
sub CreateUpdateFormResult()
   Response.Write("<form name=""update"" action=""Dummy.asp"" method=""post"">" & vbCr)
   Response.Write("<input type=""hidden"" name=""saveUpdates"" value=""false"">" & vbCr)
   Response.Write("</form>" & vbCr)
end sub
'**************************************************************************************************
  Sub PrintArrayInTable2(anArray)
    Dim r,c
  Response.Write "<br><table border>" & Chr(13)
    For r = 0 to UBound(anArray,2)
      Response.Write "<tr>" & Chr(13)
      For c = 0 to UBound(anArray,1)
        Response.Write "<td>" & CStr(anArray(c,r)) & "</td>" & Chr(13)
      Next
      Response.Write "</tr>" & Chr(13)
    Next
    Response.Write "</table>" & Chr(13)
  End Sub

   Sub PrintArrayInTable(anArray)
    Dim r,c
  Response.Write "<br><table border>" & Chr(13)
      Response.Write "<tr>" & Chr(13)
      For c = 0 to UBound(anArray)
        Response.Write "<td>" & CStr(anArray(c)) & "</td>" & Chr(13)
      Next
      Response.Write "</tr>" & Chr(13)
    Response.Write "</table>" & Chr(13)
  End Sub
'**************************************************************************************************
sub DefineNormReductionScript()
   Response.Write("<script language=""JavaScript"" type=""text/javascript"">" & vbCr)
   Response.Write("<!--" & vbCr)
   
	 %>
  function normreduction(percent)
  {
  var d7=ReadVariable(document.getElementById('d7'));
  var c10=ReadVariable(document.getElementById('c10'));
  var c11=ReadVariable(document.getElementById('c11'));
  var d10=Round(c10-(((d7/100)*c11+c10)-((d7/100)*c11+c10)*(parseFloat(percent)/100)),1);
  var c19=ReadVariable(document.getElementById('c19'));
  var d19=Round(c19-(c10-d10),1);
  var i18=ReadVariable(document.getElementById('i18'));
  var p33=ReadVariable(document.getElementById('p33'));
  var j18=Round(i18-(c10-d10)*p33,1) ;
  var i19=ReadVariable(document.getElementById('i19'));
  var j19=Round(i19-(i18-j18),1) ;
  var h23=Round(d19-j19,1);
  var j34=ReadVariable(document.getElementById('j34'));
  var j35=ReadVariable(document.getElementById('j35'));
  var j36=ReadVariable(document.getElementById('j36'));
  var j37=Round(h23-j34-j35-j36,1);
  var i40=ReadVariable(document.getElementById('i40'));
  var i41=ReadVariable(document.getElementById('i41'));
  var i42=ReadVariable(document.getElementById('i42'));
  var i43=ReadVariable(document.getElementById('i43'));
  var p34=ReadVariable(document.getElementById('p34'));
  var p35=ReadVariable(document.getElementById('p35'));
  var p36=ReadVariable(document.getElementById('p36'));
  var p37=ReadVariable(document.getElementById('p37'));
  var j40=Round((i40-(c10-d10)*p36),1);
  var j41=Round((i41-(c10-d10)*p37),1);
	if (j41<0){j41=0};
  var j42=Round((i42-(c10-d10)*p35),1);
  var j43=(i43-(c10-d10)*p34);
	var runoff=ReadVariable(document.getElementById('runoff'));
	var j45=Round(100*j43*4.43/runoff,1)
  

  document.getElementById('d10').value=correctDecimal(d10,",");
  document.getElementById('d19').value=correctDecimal(d19,",");
  document.getElementById('j18').value=correctDecimal(j18,",");
  document.getElementById('j19').value=correctDecimal(j19,",");
  document.getElementById('h23').value=correctDecimal(h23,",");
  document.getElementById('j37').value=correctDecimal(j37,",");
  document.getElementById('j40').value=correctDecimal(j40,",");
  document.getElementById('j41').value=correctDecimal(j41,",");
  document.getElementById('j42').value=correctDecimal(j42,",");
  document.getElementById('j43').value=correctDecimal(Round(j43,1),",");
  document.getElementById('j45').value=correctDecimal(j45,",");
  }

function correctDecimal(argvalue,decimal)
{
  if (decimal == ",")
    return replacechar(''+argvalue, ".", ",")
  if (decimal == ".")
    return replacechar(''+argvalue, ",", ".")
}
function replacechar(argvalue, oldch, newch)
{
  var newstr = "";
  for (var i = 0;  i < argvalue.length;  i++)
  {
    ch = argvalue.charAt(i);
    if (ch == oldch)
      newstr = newstr + newch;
    else
      newstr = newstr + ch;
  }
  return (newstr);
}

function ReadVariable(obj)
{
  var val = correctDecimal(obj.value, '.');
  if (isNoGood(val))
     val = '';
  else
  {
     val = parseFloat(val);
     obj.value = correctDecimal(obj.value, ',');
  }
  return val;
}
function isNoGood(val)
{
   var s = '' + val;
   return ((s=='') || (isNaN(s)));
}

function Round(val, prec)
{
  // Round(105.45, 0)  = 105
  // Round(105.45, 1)  = 105.5
  // Round(105.45, 2)  = 105.45
  // Round(105.45, -1) = 110
  // Round(105.45, -2) = 100
  var fac = Math.pow(10,prec)
  return Math.round(val*fac)/fac
}

<%
   
   Response.Write("//-->" & vbCr)
   Response.Write("</script>" & vbCr)
end sub
'**************************************************************************************************
%>
