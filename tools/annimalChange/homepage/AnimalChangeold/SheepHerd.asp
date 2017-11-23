<%
'**************************************************************************************************
' Author: Margit Jorgensen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
sub CreateTabSheep()



   dim i,c,n,nCol
   dim nSheepHerd, arrSheepHerd, aSheepHerdID, dictSheepHerd
   dim nSheepType, arrSheepType, aSheepTypeID, dictSheepType
   dim nStableType, arrStableType, aStableTypeID, dictStableType
   dim nFeedStuff, arrFeedStuff, aFeedStuffID, dictFeedStuff, nf, aFeedstuffAmount
   dim nSheepHerdFeedplanList, arrSheepHerdFeedplanList, aSheepHerdFeedplanID, dictSheepHerdFeedplanList, fp, strFeedplan, strAmount
   
   InitFarmNDB
   CreateLanguageDictionary
   
   
   set dictSheepHerd = CreateObject("Scripting.Dictionary")
   nSheepHerd = CallSP ("GetSheepHerdList " & Session("farmNumber")& "," & Session("scenarioNumber") , arrSheepHerd, dictSheepHerd)
   
'   nCattleHerd = GetSheepHerdList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrSheepHerd)
   set dictSheepType = CreateObject("Scripting.Dictionary")
   nSheepType = CallSP ("GetSelectListSheepType ", arrSheepType, dictSheepType)

   set dictFeedStuff = CreateObject("Scripting.Dictionary")
   nFeedStuff = CallSP ("GetFeedStuffList" , arrFeedStuff, dictFeedStuff)
   
  

'   DefineChangeTextScript
'   DefineFeedstuffListJScript_16
'   
'   Response.Write("<form name=""formSheepHerd"" action=""DummySheepHerd.asp"" method=""post"">" & vbCr)
'   Response.Write("  <table class=""data"" border=""0"">" & vbCr)
'   ' column headings
'   Response.Write("    <tr>" & vbCr)   
'   Response.Write("      <th align=""left"">"&Str.Item("strHusdyrhold")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">"&Str.Item("strANANIMAL")&"</th>" & vbCr)
''   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strANANIMAL")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">"&Str.Item("strMAXFRAC")&"<br>"&Str.Item("strGRAZING")&"</th>" & vbCr)
''   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strMAXFRAC")&"<br>&nbsp;&nbsp;"&Str.Item("strGRAZING")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">"&Str.Item("strPRODLEVEL")&"</th>" & vbCr)
''   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strPRODLEVEL")&"</th>" & vbCr)
''   Response.Write("      <th align=""left"">"&Str.Item("strEFFICIENCY")&"</th>" & vbCr)
''   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strEFFICIENCY")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">"&Str.Item("strHOUSETYPE")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">"&Str.Item("strFeedstuffPlan")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
'   Response.Write("    </tr>" & vbCr)
'   
'   for i = 0 to nSheepHerd-1
'   set dictSheepHerdFeedplanList = CreateObject("Scripting.Dictionary")
'   nSheepHerdFeedplanList = CallSP ("GetSheepHerdFeedplanList " & Session("farmNumber")& "," & Session("scenarioNumber") & "," & arrSheepHerd(i,dictSheepHerd.Item("SheepHerdID")), arrCattleHerdFeedplanList, dictCattleHerdFeedplanList)
'response.write "nSheepHerdFeedplanList: "&nSheepHerdFeedplanList
'response.write " Ubound(arrCattleHerdFeedplanList,1): "&ubound(arrCattleHerdFeedplanList,1)
'      aSheepHerdID = arrSheepHerd(i,dictSheepHerd.Item("SheepType"))
'      set dictStableType = CreateObject("Scripting.Dictionary")
'      nStableType = CallSP ("GetSelectListStableType " & aSheepHerdID, arrStableType, dictStableType)
''      nStableType   = GetSelectListStableType(aSheepHerdID,arrStableType)
'      aStableTypeID = arrSheepHerd(i,dictSheepHerd.Item("Stable"))
'      Response.Write("    <tr>" & vbCr)
'      ' column 1 = cattle type
'      Response.Write("      <td class=""data"" align=""left"">")
'      Response.Write("<input type=""hidden"" name=""hiddenCattleHerdID_"&i&""" value="""&arrSheepHerd(i,dictSheepHerd.Item("SheepHerdID"))&""">")
'      Response.Write("<select name=""selectCattleType_"&i&""" onChange=""ChangeText("&i&",this.options[this.selectedIndex].value);document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';"">")   
'      CreateSelectList_Language aSheepHerdID,nCattleType,arrCattleType
'      Response.Write("</select>")   
'      Response.Write("</td>" & vbCr)
'      ' column 2 = animals pr. year (årsdyr)   
'      Response.Write("      <td class=""data"" align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputAnimalYear_"&i&""" value="""&arrSheepHerd(i,dictSheepHerd.Item("AnimalYear"))&""" maxlength=""5"" size=""2"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/></td>" & vbCr)
'      ' column 3 = grazing
'      Response.Write("      <td class=""data"" align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputGrazing_"&i&""" value="""&arrSheepHerd(i,dictSheepHerd.Item("Grazing"))&""" maxlength=""5"" size=""2"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>&nbsp;"&Str.Item("strFEprdyr")&"</td>" & vbCr)
'      ' column 4 = production level
'  	  if aSheepHerdID=7 then
'        Response.Write("      <td class=""data"" align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputProductionLevel_"&i&""" id=""inputProductionLevel_"&i&""" value="""&arrSheepHerd(i,dictSheepHerd.Item("ProductionLevel"))&""" maxlength=""5"" size=""2"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>&nbsp;<input type=""text"" id=""ProductionLevelUnit_"&i&""" name=""ProductionLevelUnit_"&i&""" style=""border: 0px; background-color: #C8E1F3; color: green;"" value="""&Trim(Str.Item("str"&arrSheepHerd(i,dictSheepHerd.Item("ProductionLevelUnit"))))&""" size=""4""></input>&nbsp;&nbsp;<input class="""" type=""text"" name=""inputProductionLevel2_"&i&""" id=""inputProductionLevel2_"&i&""" value="""&arrSheepHerd(i,dictSheepHerd.Item("ProductionLevel2"))&""" maxlength=""5"" size=""2"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>&nbsp;<input class=""show"" type=""text"" id=""ProductionLevelUnit2_"&i&""" name=""ProductionLevelUnit2_"&i&""" value="""&Trim(Str.Item("strkg kalv"))&""" size=""4""></input></td>" & vbCr)
'			else	
'        Response.Write("      <td class=""data"" align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputProductionLevel_"&i&""" id=""inputProductionLevel_"&i&""" value="""&arrSheepHerd(i,dictSheepHerd.Item("ProductionLevel"))&""" maxlength=""5"" size=""2"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>&nbsp;<input type=""text"" id=""ProductionLevelUnit_"&i&""" name=""ProductionLevelUnit_"&i&""" style=""border: 0px; background-color: #C8E1F3; color: green;"" value="""&Trim(Str.Item("str"&arrSheepHerd(i,dictSheepHerd.Item("ProductionLevelUnit"))))&""" size=""4""></input>&nbsp;&nbsp;<input class=""hide"" type=""text"" name=""inputProductionLevel2_"&i&""" id=""inputProductionLevel2_"&i&""" value="""&arrSheepHerd(i,dictSheepHerd.Item("ProductionLevel2"))&""" maxlength=""5"" size=""2"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>&nbsp;<input class=""hide"" type=""text"" id=""ProductionLevelUnit2_"&i&""" name=""ProductionLevelUnit2_"&i&""" value="""&Trim(Str.Item("strkg kalv"))&""" size=""4""></input></td>" & vbCr)
'      end if
'      ' column 5 = effectivity   
''      Response.Write("      <td class=""data"" align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputEffectivity_"&i&""" value="""&arrSheepHerd(i,dictSheepHerd.Item("Effectivity"))&""" maxlength=""3"" size=""2"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>&nbsp;%</td>" & vbCr)
'      ' column 6 = stable type   
'      Response.Write("      <td class=""data"" align=""left"">")   
'      Response.Write("&nbsp;&nbsp;<select name=""selectStableType_"&i&""" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';"">")   
'      CreateSelectList_Language aStableTypeID,nStableType,arrStableType
'      Response.Write("</select>")   
'      Response.Write("</td>" & vbCr)
'      Response.Write("<td>" & vbCr)
'   Response.Write("<table border=""0"" cellspacing=""0"" width=""100%"">" & vbCr)
'   Response.Write("<tbody id=""tbodyFeedstuff_"&i&""" name=""tbodyFeedstuff_"&i&""">" & vbCr)
'if nSheepHerdFeedplanList=0 then
'   Response.Write("<tr>" & vbCr)
'   Response.Write("<td>" & vbCr)
'	       Response.Write("<select id=""selFeedstuff_"&i&""" name=""selFeedstuff_"&i&""" onchange=""checkForNewLine("&i&");setFeedstuffString("&i&");"" style=""position:relative;top:-3Px;"">" & vbCr)
'		   Response.write("  <option value=""-1"">"&Str.item("strSELECT_FEEDSTUFF")&"</option>" & vbCr)
'		   for nf = 0 to nFeedstuff-1
'		   if ubound(arrCattleHerdFeedplanList)<>-1 then 
'		     if nf < nSheepHerdFeedplanList then
'			  aFeedstuffID = arrCattleHerdFeedplanList(nf,0)
'			  aFeedstuffAmount = arrCattleHerdFeedplanList(nf,1)
'			 end if
'		   else
'		     aFeedstuffID = -1
'			 aFeedstuffAmount = 0
'		   end if
'		      if aFeedstuffID = Cstr(arrFeedstuff(nf,0)) then
'		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""" selected>"&arrFeedstuff(nf,1)&"</option>" & vbCr)
'		      else
'		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""">"&arrFeedstuff(nf,1)&"</option>" & vbCr)
'		      end if
'		   next
'		   Response.Write("</select>" & vbCr)
'   Response.Write("<input id=""feedstuffAmount_"&i&""" name=""feedstuffAmount_"&i&""" type=""text"" style=""width: 35px;position:relative;top:-3Px;"" value="""&aFeedstuffAmount&""" onchange=""setFeedstuffString("&i&");""/> Kg" & vbCr)
'   Response.Write("<img style=""position:relative;top:3Px;"" src=""images/del.gif"" onclick=""delRow(this,"&i&");"" onMouseover=""mouseOverGif(this)""  onMouseout=""mouseOutGif(this)""/>&nbsp;&nbsp;" & vbCr)
'   Response.Write("</td>" & vbCr)
'   Response.Write("</tr>" & vbCr)
'else
'for fp = 0 to nCattleHerdFeedplanList-1  
'   Response.Write("<tr>" & vbCr)
'   Response.Write("<td>" & vbCr)
'	       Response.Write("<select id=""selFeedstuff_"&i&""" name=""selFeedstuff_"&i&""" onchange=""checkForNewLine("&i&");setFeedstuffString("&i&");"" style=""position:relative;top:-3Px;"">" & vbCr)
'		   Response.write("  <option value=""-1"">"&Str.item("strSELECT_FEEDSTUFF")&"</option>" & vbCr)
'		   for nf = 0 to nFeedstuff-1
'		   if ubound(arrCattleHerdFeedplanList)<>-1 then 
'		     if fp < nSheepHerdFeedplanList then
'			  aFeedstuffID = arrCattleHerdFeedplanList(fp,0)
'			  aFeedstuffAmount = arrCattleHerdFeedplanList(fp,1)
'			 end if
'		   else
'		     aFeedstuffID = -1
'			 aFeedstuffAmount = 0
'		   end if
'		      if aFeedstuffID = Cstr(arrFeedstuff(nf,0)) then
'		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""" selected>"&arrFeedstuff(nf,1)&"</option>" & vbCr)
'		      else
'		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""">"&arrFeedstuff(nf,1)&"</option>" & vbCr)
'		      end if
'		   next
'		   Response.Write("</select>" & vbCr)
'   Response.Write("<input id=""feedstuffAmount_"&i&""" name=""feedstuffAmount_"&i&""" type=""text"" style=""width: 35px;position:relative;top:-3Px;"" value="""&aFeedstuffAmount&""" onchange=""setFeedstuffString("&i&");""/> Kg" & vbCr)
'   Response.Write("<img style=""position:relative;top:3Px;"" src=""images/del.gif"" onclick=""delRow(this,"&i&");"" onMouseover=""mouseOverGif(this)""  onMouseout=""mouseOutGif(this)""/>&nbsp;&nbsp;" & vbCr)
'   Response.Write("</td>" & vbCr)
'   Response.Write("</tr>" & vbCr)
'next
'   Response.Write("<tr>" & vbCr)
'   Response.Write("<td>" & vbCr)
'	       Response.Write("<select id=""selFeedstuff_"&i&""" name=""selFeedstuff_"&i&""" onchange=""checkForNewLine("&i&");setFeedstuffString("&i&");"" style=""position:relative;top:-3Px;"">" & vbCr)
'		   Response.write("  <option value=""-1"">"&Str.item("strSELECT_FEEDSTUFF")&"</option>" & vbCr)
'		   for nf = 0 to nFeedstuff-1
'		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""">"&arrFeedstuff(nf,1)&"</option>" & vbCr)
'		   next
'		   Response.Write("</select>" & vbCr)
'   Response.Write("<input id=""feedstuffAmount_"&i&""" name=""feedstuffAmount_"&i&""" type=""text"" style=""width: 35px;position:relative;top:-3Px;"" value=""0"" onchange=""setFeedstuffString("&i&");""/> Kg" & vbCr)
'   Response.Write("<img style=""position:relative;top:3Px;"" src=""images/del.gif"" onclick=""delRow(this,"&i&");"" onMouseover=""mouseOverGif(this)""  onMouseout=""mouseOutGif(this)""/>&nbsp;&nbsp;" & vbCr)
'   Response.Write("</td>" & vbCr)
'   Response.Write("</tr>" & vbCr)
'end if
'   
'   
'   Response.Write("</tbody>" & vbCr)
'   Response.Write("</table>" & vbCr)
'if nSheepHerdFeedplanList>0 then 
'for fp = 0 to nCattleHerdFeedplanList-1
'  strFeedplan = strFeedplan & arrFeedstuff(fp,0)& ","
'  strAmount = strAmount & arrCattleHerdFeedplanList(fp,1)& ","
'next
'else
'  strFeedplan = ""
'  strAmount = ""
'end if  
'      Response.Write("<input type=""hidden"" name=""hiddenFeedstuffList_"&i&"""  id=""hiddenFeedstuffList_"&i&""" value="""&strFeedplan&""">")
'      Response.Write("<input type=""hidden"" name=""hiddenAmountList_"&i&"""  id=""hiddenAmountList_"&i&""" value="""&strAmount&""">")
'      Response.Write("</td>" & vbCr)
'
'      ' column 7 = delete buttons   
'      Response.Write("      <td class=""data"" align=""left"">")   
'      Response.Write("<button style=""width: 50px; height: 20px; font-size: 10px"" onClick=""javascript:if(confirm('"&Str.Item("strBekræft")&"')) {document.forms.formCattleHerd.action='CattleHerdDelete.asp?SheepHerdID="&arrSheepHerd(i,dictSheepHerd.Item("SheepHerdID"))&"';document.forms.formCattleHerd.submit();}"">"&Str.Item("strDelete")&"</button>")
'      Response.Write("</td>" & vbCr)   
'      Response.Write("    </tr>" & vbCr)
'   Response.Write("    <tr>" & vbCr)
'   ' row for buttons
'   Response.Write("      <td class=""data"" colspan=""7""><hr/></td>" & vbCr)   
'   Response.Write("    </tr>" & vbCr)
'      set dictStableType = Nothing
'	  set dictCattleHerdFeedplanList = nothing
'   next
''   Response.Write("    <tr>" & vbCr)
''   ' row for buttons
''   Response.Write("      <td class=""data"" colspan=""7""><hr/></td>" & vbCr)   
''   Response.Write("    </tr>" & vbCr)
'   Response.Write("    <tr>" & vbCr)
'   Response.Write("      <td class=""data"" colspan=""7"">")   
'   Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formCattleHerd.action='CattleHerdUpdate.asp';document.forms.formCattleHerd.submit();"">"&Str.Item("strSAVE")&"</button>")
'   Response.Write("&nbsp;")   
'   Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formCattleHerd.action='CattleHerdNew.asp';document.forms.formCattleHerd.submit();"">"&Str.Item("strNEW")&"</button>")
'   Response.Write("<input type=""hidden"" name=""hiddenNCattleHerd"" value="""&nSheepHerd&""">" & vbCr)
'   Response.Write("</td>" & vbCr)   
'   Response.Write("    </tr>" & vbCr)
'   
'   Response.Write("    <tr>" & vbCr)
'   Response.Write("  </table>" & vbCr) 
'   Response.Write("</form>" & vbCr)
'   CreateUpdateFormCattle
'   if nSheepHerd > 0 then
'      CreateSheepHerdTotal
'   end if
'   CloseFarmNDB
end sub
''**************************************************************************************************
'sub CreateSheepHerdTotal()
'   dim p,t,c,f
'   dim nParameter, arrParameter, dictParameter
'   dim nCattleType, arrCattleType, aSheepHerdID, dictCattleType
'   dim nCattleHerdTotal, arrCattleHerdTotal, dictCattleHerdTotal
'   dim nFarmTotal, arrFarmTotal, dictFarmTotal
'
'   set dictParameter = CreateObject("Scripting.Dictionary")
'   nParameter = CallSP ("GetParameterList 3", arrParameter, dictParameter)
'
''   nParameter  = GetParameterList(3,arrParameter)
'
' '  nCattleType = GetCattleHerdCattleTypeList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrCattleType)
'
'   set dictCattleType = CreateObject("Scripting.Dictionary")
'   nCattleType = CallSP ("GetCattleHerdCattleTypeList " & Session("farmNumber")& "," & Session("scenarioNumber"), arrCattleType, dictCattleType)
'
''   nFarmTotal  = GetFarmTotalList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),3,arrFarmTotal)
'  set dictFarmTotal = CreateObject("Scripting.Dictionary")
'   nFarmTotal = CallSP ("GetFarmTotalList " & Session("farmNumber")& "," & Session("scenarioNumber") & ",3" , arrFarmTotal, dictFarmTotal)
'
'   ' rows for totals
'   Response.Write("  <br/>" & vbCr) 
'   Response.Write("  <h4>&nbsp;"&Str.Item("strbehovprodgødning")&"</h4>" & vbCr)
'         
'   Response.Write("  <table class=""data"" border=""0"" cellspacing=""0"" cellpadding=""0"">" & vbCr)
'   ' row for header
'   Response.Write("    <tr>" & vbCr)
'   Response.Write("      <th class=""data"" align=""left"" nowrap>&nbsp;&nbsp;"&Str.Item("strHusdyrhold")&"</th>" & vbCr) 
'   for p = 0 to nParameter-1 
'      Response.Write("      <th class=""data"" align=""right"">&nbsp;&nbsp;"& Str.Item("str"&arrParameter(p,dictParameter.Item("ParameterName")))&"<br/>("& Str.Item("str"&arrParameter(p,dictParameter.Item("ParameterUnit")))&")</th>" & vbCr)
'   next  
'   Response.Write("    </tr>" & vbCr)
'   ' rows for feeding requirements
' 
'   for t = 0 to nCattleType-1 
'      aSheepHerdID = arrCattleType(t,dictCattleType.Item("SheepType"))
'    set dictCattleHerdTotal = CreateObject("Scripting.Dictionary")
'      nCattleHerdTotal = CallSP ("GetCattleHerdTotalList " & Session("farmNumber")& "," & Session("scenarioNumber") & "," & aSheepHerdID , arrCattleHerdTotal, dictCattleHerdTotal)
''      nCattleHerdTotal = GetCattleHerdTotalList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),aSheepHerdID,arrCattleHerdTotal)
'      Response.Write("    <tr>" & vbCr)
'      Response.Write("      <td nowrap>&nbsp;&nbsp;"&arrCattleType(t,dictCattleType.Item("SheepType"))&" - "&Str.Item("str"&arrCattleType(t,dictCattleType.Item("CattleTypeName")))&"</td>" & vbCr)  
'      for c = 0 to nCattleHerdTotal-1 
''         Response.Write("      <td class=""data"" align=""right"">"&p&"</td>" & vbCr)
'         if arrCattleHerdTotal(c,dictCattleHerdTotal.Item("ParameterValue")) <> 0 then
'            Response.Write("      <td align=""right"">"&FormatNumber(arrCattleHerdTotal(c,dictCattleHerdTotal.Item("ParameterValue")),1,-1,0,0)&"</td>" & vbCr)
'         else
'            Response.Write("      <td align=""right"">&nbsp;</td>" & vbCr)
'         end if
'      next  
'      Response.Write("    </tr>" & vbCr)
'   set dictCattleHerdTotal = Nothing
'   next
'
'   Response.Write("    <tr>" & vbCr)
'   Response.Write("      <td colspan="""&nParameter+1&"""><hr/></td>" & vbCr)   
'   Response.Write("    </tr>" & vbCr)
'   ' row for total
'   Response.Write("    <tr>" & vbCr)
'   Response.Write("      <th class=""data"" align=""left"" nowrap>&nbsp;&nbsp;"&Str.Item("strTOTALALLGROUPS")&"</th>" & vbCr)
'   for f = 0 to nFarmTotal-1 
'      Response.Write("      <th class=""data"" align=""right"">"&FormatNumber(arrFarmTotal(f,dictFarmTotal.Item("ParameterValue")),1,-1,0,0)&"</th>" & vbCr)   
'   next      
'   Response.Write("    </tr>" & vbCr)
'   Response.Write("  </table>" & vbCr)
'end sub
''**************************************************************************************************
'sub DefineChangeTextScript()
'   dim nCattleType, arrCattleType,dictCattleType, j
''   CreateLanguageDictionary
''   nCattleType = GetCattleTypeList(arrCattleType)
'   set dictCattleType = CreateObject("Scripting.Dictionary")
'   nCattleType = CallSP ("GetCattleTypeList " , arrCattleType, dictCattleType)
'
'   Response.Write("<script language=""JavaScript"" type=""text/javascript"">" & vbCr)
'   Response.Write("<!--" & vbCr)
'   
'   Response.Write("  function ChangeText(i,t)" & vbCr)
'   Response.Write("  {" & vbCr)
'   Response.Write("     switch (t) {" & vbCr)
'   for j=0 to nCattleType-1
'	   Response.Write("     case '"&arrCattleType(j,dictCattleType.Item("CattleTypeID"))&"' :       eval(""document.forms.formCattleHerd.inputProductionLevel_""+i+"".value="&Trim(arrCattleType(j,dictCattleType.Item("ProductionLevelDefault")))&""");" & vbCr)
'	 if Cint(arrCattleType(j,dictCattleType.Item("CattleTypeID")))=7 then
'	   Response.Write(" eval(""document.forms.formCattleHerd.inputProductionLevel2_""+i+"".value="&Trim(arrCattleType(j,dictCattleType.Item("ProductionLevel2Default")))&""");" & vbCr)
'	   Response.Write(" eval(""document.forms.formCattleHerd.inputProductionLevel2_""+i+"".className='';"");" & vbCr)
'	   Response.Write(" eval(""document.forms.formCattleHerd.ProductionLevelUnit2_""+i+"".className='show';"");" & vbCr)
'	 else
'	   Response.Write(" eval(""document.forms.formCattleHerd.inputProductionLevel2_""+i+"".value="&Trim(arrCattleType(j,dictCattleType.Item("ProductionLevel2Default")))&""");" & vbCr)
'	   Response.Write(" eval(""document.forms.formCattleHerd.inputProductionLevel2_""+i+"".className='hide';"");" & vbCr)
'	   Response.Write(" eval(""document.forms.formCattleHerd.ProductionLevelUnit2_""+i+"".className='hide';"");" & vbCr)
'	 end if
'	   Response.Write(" eval(""document.forms.formCattleHerd.inputGrazing_""+i+"".value="&Trim(arrCattleType(j,dictCattleType.Item("GrazingDefault")))&""");" & vbCr)
'	   Response.Write(" eval(""document.forms.formCattleHerd.ProductionLevelUnit_""+i+"".value='"&Trim(Cstr(Str.Item("str"&arrCattleType(j,dictCattleType.Item("ProductionLevelUnit"))&"")))&"'"");" & vbCr)
'	   Response.Write("                      break;" & vbCr)
'   next
'   Response.Write("                }" & vbCr)
'   Response.Write("  }" & vbCr)
'   
'   Response.Write("//-->" & vbCr)
'   Response.Write("</script>" & vbCr)
'end sub
''**************************************************************************************************
sub DefineFeedstuffListJScript_16()
%>
<script language="javascript">

   function checkForNewLine(i)
   {
   	var oTBody = document.getElementById('tbodyFeedstuff_'+i+'')//;alert('tbodyFeedstuff_'+i+'');
   	var oLastRow = oTBody.lastChild;
   	var newLine = false;
        var oInput = document.getElementById ("oInput");
   	while (oLastRow.tagName != "TR")
   	{
   		oLastRow = oLastRow.previousSibling;
   	}	
   	for (var i = 0; i < oLastRow.childNodes.length; i++)
   	{
   		var oCell = oLastRow.childNodes[i];
   		for (var j = 0; j < oCell.childNodes.length; j++)
   		{
   			if (oCell.childNodes[j].tagName == "SELECT") 
   			{
   				var oCombo = oCell.childNodes[j];
   			}
   			if (oCell.childNodes[j].tagName == "INPUT")
   			{
   				oInput = oCell.childNodes[j];
   			}
   		}
   	}
   	if ((oCombo.selectedIndex != 0) || (oInput.value.length > 0))
   	{
   		//Dette svarer til at der indsættes en ny række i tabellen, når brugeren begynder at indtaste data i den nederste række
   		newLine = true;
   	}
   
   	if (newLine)
   	{
   		appendNewSelect(oTBody);
   	}
   }
 
   function appendNewSelect(element) 
   {	
   	var oTBody = element
   	var oLastRow = oTBody.lastChild;
   	//nedenstående loop er nødvendigt fordi IE og Netscape opfatter lastChild forskelligt. IE: direkte efterkommere. Netscape: sidste efterkommer (ikke nødvendigvis direkte)
   	while (oLastRow.tagName != "TR")
   	{
   		oLastRow = oLastRow.previousSibling;	
   	}
   	var oNewRow = oLastRow.cloneNode(true);
   	oTBody.appendChild(oNewRow);
   }

   function getSectionRowIndex(oElement)
   {
   	var oParent = oElement.parentNode;
   	while (oParent.tagName != "TR")
   	{
   		oParent = oParent.parentNode;
   	}
   	return oParent.sectionRowIndex;
   }
 
   function delRow(oElement,i)
   {
   	var oTBody = document.getElementById ('tbodyFeedstuff_'+i+'');
   	var actTBodyRowIndex = getSectionRowIndex(oElement);
   	if (actTBodyRowIndex == 0) //første række i body - efter overskriften
   	{
   		var oRow = oTBody.firstChild;
   		while (oRow.tagName != "TR")
   		{
   			oRow = oRow.nextSibling;
   		}
   		var oRowChild = oRow.firstChild;
   		if (oRowChild.hasChildNodes())
   		//IE
   		{
   			oRowChild.firstChild.selectedIndex = 0;
   		}
   		else
   		{
   			while (!oRowChild.hasChildNodes())		
   			{
   				oRowChild = oRowChild.nextSibling;
   			}
   			oRowChild = oRowChild.firstChild;
   			while (oRowChild.tagName != "SELECT")		
   			{
   				oRowChild = oRowChild.nextSibling;
   			}
   			oRowChild.selectedIndex = 0;
   		}
   	}
   	else
   	{
   		oTBody.deleteRow(actTBodyRowIndex);
		setFeedstuffString(i)
   	}
   }
 
   function getFeedstuff(i){
   	var cFeedstuff = document.getElementsByName('selFeedstuff_'+i+'');
   	var strFeedstuff = "";
   	for (var i = 0; i < cFeedstuff.length; i++)
   	{
   		if (parseInt(cFeedstuff[i].value,10) > 0)
   		{
   				strFeedstuff += cFeedstuff[i].value +", ";
   		}
   	}
   	return strFeedstuff;
   }

   function getAmount(i){
   	var cFeedstuff = document.getElementsByName('feedstuffAmount_'+i+'');
   	var strFeedstuff = "";
   	for (var i = 0; i < cFeedstuff.length; i++)
   	{
   		if (parseInt(cFeedstuff[i].value,10) > 0)
   		{
   				strFeedstuff += cFeedstuff[i].value +", ";
   		}
   	}
   	return strFeedstuff;
   }
 
   function mouseOverGif(oImg)
   {
   	oImg.setAttribute("src","Images/del1.gif");
   }
   
   function mouseOutGif(oImg)
   {
   	oImg.setAttribute("src","Images/del.gif");
   }
 
   function setFeedstuffString(i){
   	document.getElementById('hiddenFeedstuffList_'+i+'').value = getFeedstuff(i);
   	document.getElementById('hiddenAmountList_'+i+'').value = getAmount(i);
   }
</script>
<%
end sub
''**************************************************************************************************
'sub CreateUpdateFormCattle()
'   Response.Write("<form name=""update"" action=""javascript:document.forms.formSheepHerd.action='CattleHerdUpdate.asp';document.forms.formSheepHerd.submit();"" method=""post"">" & vbCr)
'   Response.Write("<input type=""hidden"" name=""saveUpdates"" value=""false"">" & vbCr)
'   Response.Write("</form>" & vbCr)
'end sub
'**************************************************************************************************
%>
