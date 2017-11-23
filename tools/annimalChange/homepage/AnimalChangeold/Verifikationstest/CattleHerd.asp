<%
'**************************************************************************************************
' Author: Poul Lassen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
sub CreateTabCattle()
   dim i,c,n,nCol
   dim nCattleHerd, arrCattleHerd, aCattleHerdID, dictCattleHerd
   dim nCattleType, arrCattleType, aCattleTypeID, dictCattleType
   dim nStableType, arrStableType, aStableTypeID, dictStableType
   
   InitFarmNDB
   CreateLanguageDictionary
   
   
   set dictCattleHerd = CreateObject("Scripting.Dictionary")
   nCattleHerd = CallSP ("GetCattleHerdList " & Session("farmNumber")& "," & Session("scenarioNumber") , arrCattleHerd, dictCattleHerd)
   
'   nCattleHerd = GetCattleHerdList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrCattleHerd)
    set dictCattleType = CreateObject("Scripting.Dictionary")
   nCattleType = CallSP ("GetSelectListCattleType ", arrCattleType, dictCattleType)

'   nCattleType = GetSelectListCattleType(arrCattleType)

   DefineChangeTextScript
   
   Response.Write("<form name=""formCattleHerd"" action=""DummyCattleHerd.asp"" method=""post"">" & vbCr)
   Response.Write("  <table class=""data"" border=""0"">" & vbCr)
   ' column headings
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th align=""left"">"&Str.Item("strHusdyrhold")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strANANIMAL")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strANANIMAL")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strMAXFRAC")&"<br>"&Str.Item("strGRAZING")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strMAXFRAC")&"<br>&nbsp;&nbsp;"&Str.Item("strGRAZING")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strPRODLEVEL")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strPRODLEVEL")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strEFFICIENCY")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strEFFICIENCY")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strHOUSETYPE")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strHOUSETYPE")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   
   for i = 0 to nCattleHerd-1
      aCattleTypeID = arrCattleHerd(i,dictCattleHerd.Item("CattleType"))
      set dictStableType = CreateObject("Scripting.Dictionary")
      nStableType = CallSP ("GetSelectListStableType " & aCattleTypeID, arrStableType, dictStableType)
'      nStableType   = GetSelectListStableType(aCattleTypeID,arrStableType)
      aStableTypeID = arrCattleHerd(i,dictCattleHerd.Item("Stable"))
      Response.Write("    <tr>" & vbCr)
      ' column 1 = cattle type
      Response.Write("      <td class=""data"" align=""left"">")
      Response.Write("<input type=""hidden"" name=""hiddenCattleHerdID_"&i&""" value="""&arrCattleHerd(i,dictCattleHerd.Item("CattleHerdID"))&""">")
      Response.Write("<select name=""selectCattleType_"&i&""" onChange=""ChangeText("&i&",this.options[this.selectedIndex].value);document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';"">")   
      CreateSelectList_Language aCattleTypeID,nCattleType,arrCattleType
      Response.Write("</select>")   
      Response.Write("</td>" & vbCr)
      ' column 2 = animals pr. year (årsdyr)   
      Response.Write("      <td class=""data"" align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputAnimalYear_"&i&""" value="""&arrCattleHerd(i,dictCattleHerd.Item("AnimalYear"))&""" maxlength=""5"" size=""2"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/></td>" & vbCr)
      ' column 3 = grazing
      Response.Write("      <td class=""data"" align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputGrazing_"&i&""" value="""&arrCattleHerd(i,dictCattleHerd.Item("Grazing"))&""" maxlength=""5"" size=""2"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>&nbsp;"&Str.Item("strFEprdyr")&"</td>" & vbCr)
      ' column 4 = production level
  	  if aCattleTypeID=7 then
        Response.Write("      <td class=""data"" align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputProductionLevel_"&i&""" id=""inputProductionLevel_"&i&""" value="""&arrCattleHerd(i,dictCattleHerd.Item("ProductionLevel"))&""" maxlength=""5"" size=""2"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>&nbsp;<input type=""text"" id=""ProductionLevelUnit_"&i&""" name=""ProductionLevelUnit_"&i&""" style=""border: 0px; background-color: #C8E1F3; color: navy;"" value="""&Trim(Str.Item("str"&arrCattleHerd(i,dictCattleHerd.Item("ProductionLevelUnit"))))&""" size=""10""></input>&nbsp;&nbsp;<input class="""" type=""text"" name=""inputProductionLevel2_"&i&""" id=""inputProductionLevel2_"&i&""" value="""&arrCattleHerd(i,dictCattleHerd.Item("ProductionLevel2"))&""" maxlength=""5"" size=""2"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>&nbsp;<input class=""show"" type=""text"" id=""ProductionLevelUnit2_"&i&""" name=""ProductionLevelUnit2_"&i&""" value="""&Trim(Str.Item("strkg kalv"))&""" size=""12""></input></td>" & vbCr)
			else	
        Response.Write("      <td class=""data"" align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputProductionLevel_"&i&""" id=""inputProductionLevel_"&i&""" value="""&arrCattleHerd(i,dictCattleHerd.Item("ProductionLevel"))&""" maxlength=""5"" size=""2"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>&nbsp;<input type=""text"" id=""ProductionLevelUnit_"&i&""" name=""ProductionLevelUnit_"&i&""" style=""border: 0px; background-color: #C8E1F3; color: navy;"" value="""&Trim(Str.Item("str"&arrCattleHerd(i,dictCattleHerd.Item("ProductionLevelUnit"))))&""" size=""10""></input>&nbsp;&nbsp;<input class=""hide"" type=""text"" name=""inputProductionLevel2_"&i&""" id=""inputProductionLevel2_"&i&""" value="""&arrCattleHerd(i,dictCattleHerd.Item("ProductionLevel2"))&""" maxlength=""5"" size=""2"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>&nbsp;<input class=""hide"" type=""text"" id=""ProductionLevelUnit2_"&i&""" name=""ProductionLevelUnit2_"&i&""" value="""&Trim(Str.Item("strkg kalv"))&""" size=""12""></input></td>" & vbCr)
      end if
      ' column 5 = effectivity   
      Response.Write("      <td class=""data"" align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputEffectivity_"&i&""" value="""&arrCattleHerd(i,dictCattleHerd.Item("Effectivity"))&""" maxlength=""3"" size=""2"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>&nbsp;%</td>" & vbCr)
      ' column 6 = stable type   
      Response.Write("      <td class=""data"" align=""left"">")   
      Response.Write("&nbsp;&nbsp;<select name=""selectStableType_"&i&""" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';"">")   
      CreateSelectList_Language aStableTypeID,nStableType,arrStableType
      Response.Write("</select>")   
      Response.Write("</td>" & vbCr)
      ' column 7 = delete buttons   
      Response.Write("      <td class=""data"" align=""left"">")   
      Response.Write("<button style=""width: 50px; height: 20px; font-size: 10px"" onClick=""javascript:if(confirm('"&Str.Item("strBekræft")&"')) {document.forms.formCattleHerd.action='CattleHerdDelete.asp?CattleHerdID="&arrCattleHerd(i,dictCattleHerd.Item("CattleHerdID"))&"';document.forms.formCattleHerd.submit();}"">"&Str.Item("strDelete")&"</button>")
      Response.Write("</td>" & vbCr)   
      Response.Write("    </tr>" & vbCr)
      set dictStableType = Nothing
   next
   Response.Write("    <tr>" & vbCr)
   ' row for buttons
   Response.Write("      <td class=""data"" colspan=""7""><hr/></td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <td class=""data"" colspan=""7"">")   
   Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formCattleHerd.action='CattleHerdUpdate.asp';document.forms.formCattleHerd.submit();"">"&Str.Item("strSAVE")&"</button>")
   Response.Write("&nbsp;")   
   Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formCattleHerd.action='CattleHerdNew.asp';document.forms.formCattleHerd.submit();"">"&Str.Item("strNEW")&"</button>")
   Response.Write("<input type=""hidden"" name=""hiddenNCattleHerd"" value="""&nCattleHerd&""">" & vbCr)
   Response.Write("</td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
   
   Response.Write("    <tr>" & vbCr)
   Response.Write("  </table>" & vbCr) 
   Response.Write("</form>" & vbCr)
   CreateUpdateFormCattle
   if nCattleHerd > 0 then
      CreateCattleHerdTotal
   end if
   CloseFarmNDB
end sub
'**************************************************************************************************
sub CreateCattleHerdTotal()
   dim p,t,c,f
   dim nParameter, arrParameter, dictParameter
   dim nCattleType, arrCattleType, aCattleTypeID, dictCattleType
   dim nCattleHerdTotal, arrCattleHerdTotal, dictCattleHerdTotal
   dim nFarmTotal, arrFarmTotal, dictFarmTotal

   set dictParameter = CreateObject("Scripting.Dictionary")
   nParameter = CallSP ("GetParameterList 3", arrParameter, dictParameter)

'   nParameter  = GetParameterList(3,arrParameter)

 '  nCattleType = GetCattleHerdCattleTypeList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrCattleType)

   set dictCattleType = CreateObject("Scripting.Dictionary")
   nCattleType = CallSP ("GetCattleHerdCattleTypeList " & Session("farmNumber")& "," & Session("scenarioNumber"), arrCattleType, dictCattleType)

'   nFarmTotal  = GetFarmTotalList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),3,arrFarmTotal)
  set dictFarmTotal = CreateObject("Scripting.Dictionary")
   nFarmTotal = CallSP ("GetFarmTotalList " & Session("farmNumber")& "," & Session("scenarioNumber") & ",3" , arrFarmTotal, dictFarmTotal)

   ' rows for totals
   Response.Write("  <br/>" & vbCr) 
   Response.Write("  <h4>&nbsp;"&Str.Item("strbehovprodgødning")&"</h4>" & vbCr)
         
   Response.Write("  <table class=""data"" border=""0"" cellspacing=""0"" cellpadding=""0"">" & vbCr)
   ' row for header
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <th class=""data"" align=""left"" nowrap>&nbsp;&nbsp;"&Str.Item("strHusdyrhold")&"</th>" & vbCr) 
   for p = 0 to nParameter-1 
      Response.Write("      <th class=""data"" align=""right"">&nbsp;&nbsp;"& Str.Item("str"&arrParameter(p,dictParameter.Item("ParameterName")))&"<br/>("& Str.Item("str"&arrParameter(p,dictParameter.Item("ParameterUnit")))&")</th>" & vbCr)
   next  
   Response.Write("    </tr>" & vbCr)
   ' rows for feeding requirements
 
   for t = 0 to nCattleType-1 
      aCattleTypeID = arrCattleType(t,dictCattleType.Item("CattleType"))
    set dictCattleHerdTotal = CreateObject("Scripting.Dictionary")
      nCattleHerdTotal = CallSP ("GetCattleHerdTotalList " & Session("farmNumber")& "," & Session("scenarioNumber") & "," & aCattleTypeID , arrCattleHerdTotal, dictCattleHerdTotal)
'      nCattleHerdTotal = GetCattleHerdTotalList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),aCattleTypeID,arrCattleHerdTotal)
      Response.Write("    <tr>" & vbCr)
      Response.Write("      <td nowrap>&nbsp;&nbsp;"&arrCattleType(t,dictCattleType.Item("CattleType"))&" - "&Str.Item("str"&arrCattleType(t,dictCattleType.Item("CattleTypeName")))&"</td>" & vbCr)  
      for c = 0 to nCattleHerdTotal-1 
'         Response.Write("      <td class=""data"" align=""right"">"&p&"</td>" & vbCr)
         if arrCattleHerdTotal(c,dictCattleHerdTotal.Item("ParameterValue")) <> 0 then
            Response.Write("      <td align=""right"">"&FormatNumber(arrCattleHerdTotal(c,dictCattleHerdTotal.Item("ParameterValue")),1,-1,0,0)&"</td>" & vbCr)
         else
            Response.Write("      <td align=""right"">&nbsp;</td>" & vbCr)
         end if
      next  
      Response.Write("    </tr>" & vbCr)
   set dictCattleHerdTotal = Nothing
   next

   Response.Write("    <tr>" & vbCr)
   Response.Write("      <td colspan="""&nParameter+1&"""><hr/></td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
   ' row for total
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <th class=""data"" align=""left"" nowrap>&nbsp;&nbsp;"&Str.Item("strTOTALALLGROUPS")&"</th>" & vbCr)
   for f = 0 to nFarmTotal-1 
      Response.Write("      <th class=""data"" align=""right"">"&FormatNumber(arrFarmTotal(f,dictFarmTotal.Item("ParameterValue")),1,-1,0,0)&"</th>" & vbCr)   
   next      
   Response.Write("    </tr>" & vbCr)
   Response.Write("  </table>" & vbCr)
end sub
'**************************************************************************************************
sub DefineChangeTextScript()
   dim nCattleType, arrCattleType,dictCattleType, j
'   CreateLanguageDictionary
'   nCattleType = GetCattleTypeList(arrCattleType)
   set dictCattleType = CreateObject("Scripting.Dictionary")
   nCattleType = CallSP ("GetCattleTypeList " , arrCattleType, dictCattleType)

   Response.Write("<script language=""JavaScript"" type=""text/javascript"">" & vbCr)
   Response.Write("<!--" & vbCr)
   
   Response.Write("  function ChangeText(i,t)" & vbCr)
   Response.Write("  {" & vbCr)
   Response.Write("     switch (t) {" & vbCr)
   for j=0 to nCattleType-1
	   Response.Write("     case '"&arrCattleType(j,dictCattleType.Item("CattleTypeID"))&"' :       eval(""document.forms.formCattleHerd.inputProductionLevel_""+i+"".value="&Trim(arrCattleType(j,dictCattleType.Item("ProductionLevelDefault")))&""");" & vbCr)
	 if Cint(arrCattleType(j,dictCattleType.Item("CattleTypeID")))=7 then
	   Response.Write(" eval(""document.forms.formCattleHerd.inputProductionLevel2_""+i+"".value="&Trim(arrCattleType(j,dictCattleType.Item("ProductionLevel2Default")))&""");" & vbCr)
	   Response.Write(" eval(""document.forms.formCattleHerd.inputProductionLevel2_""+i+"".className='';"");" & vbCr)
	   Response.Write(" eval(""document.forms.formCattleHerd.ProductionLevelUnit2_""+i+"".className='show';"");" & vbCr)
	 else
	   Response.Write(" eval(""document.forms.formCattleHerd.inputProductionLevel2_""+i+"".value="&Trim(arrCattleType(j,dictCattleType.Item("ProductionLevel2Default")))&""");" & vbCr)
	   Response.Write(" eval(""document.forms.formCattleHerd.inputProductionLevel2_""+i+"".className='hide';"");" & vbCr)
	   Response.Write(" eval(""document.forms.formCattleHerd.ProductionLevelUnit2_""+i+"".className='hide';"");" & vbCr)
	 end if
	   Response.Write(" eval(""document.forms.formCattleHerd.inputGrazing_""+i+"".value="&Trim(arrCattleType(j,dictCattleType.Item("GrazingDefault")))&""");" & vbCr)
	   Response.Write(" eval(""document.forms.formCattleHerd.ProductionLevelUnit_""+i+"".value='"&Trim(Cstr(Str.Item("str"&arrCattleType(j,dictCattleType.Item("ProductionLevelUnit"))&"")))&"'"");" & vbCr)
	   Response.Write("                      break;" & vbCr)
   next
   Response.Write("                }" & vbCr)
   Response.Write("  }" & vbCr)
   
   Response.Write("//-->" & vbCr)
   Response.Write("</script>" & vbCr)
end sub
'**************************************************************************************************
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
'	   Response.Write(" eval(""document.forms.formCattleHerd.inputProductionLevel2_""+i+"".value="&Trim(arrCattleType(j,dictCattleType.Item("ProductionLevel2Default")))&""");" & vbCr)
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
'**************************************************************************************************
sub CreateUpdateFormCattle()
   Response.Write("<form name=""update"" action=""javascript:document.forms.formCattleHerd.action='CattleHerdUpdate.asp';document.forms.formCattleHerd.submit();"" method=""post"">" & vbCr)
   Response.Write("<input type=""hidden"" name=""saveUpdates"" value=""false"">" & vbCr)
   Response.Write("</form>" & vbCr)
end sub
'**************************************************************************************************
%>
