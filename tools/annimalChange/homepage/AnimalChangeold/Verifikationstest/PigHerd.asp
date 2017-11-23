<%
'**************************************************************************************************
' Author: Poul Lassen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************

sub CreateTabPig()
   dim i, nPigHerd, nPigType, arrPigType, arrPigHerd, aPigTypeID, nStableType1, nStableType2, aStableType1ID, aStableType2ID, arrStableType1, arrStableType2, dictStableType, arrPigTypeNotSows, dictPigHerd, dictPigType

 '  Response.Write("<br/><center><h3>Det er ikke muligt at arbejde med svin endnu</h3></center>" & vbCr)

   InitFarmNDB
   CreateLanguageDictionary
   
   set dictPigHerd = CreateObject("Scripting.Dictionary")
   nPigHerd = CallSP ("GetPigHerdList " & CLng(Session("farmNumber"))& "," & CInt(Session("scenarioNumber")) , arrPigHerd, dictPigHerd)
   ' nPigHerd = GetPigHerdList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrPigHerd)

'   print2 arrPigHerd, dictPigHerd

   set dictPigType = CreateObject("Scripting.Dictionary")
   nPigType = CallSP ("GetSelectListPigType ", arrPigType, dictPigType)

   'nPigType = GetSelectListPigType(arrPigType)
   PigTypeNotSows arrPigType, arrPigTypeNotSows

'   print2 arrPigType, dictPigType
   DefineChangeTextScriptPig

   Response.Write("<form name=""formPigHerd"" action=""DummyPigHerd.asp"" method=""post"">" & vbCr) 

   Response.Write("  <table class=""data"" border=""0"" width=""100%"">" & vbCr)
   ' column headings
   Response.Write("    <tr>" & vbCr)   
      Response.Write("      <td colspan=""7""><h4>"&Str.Item("strSøer")&"</h4></td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
   
   
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th align=""left"">"&Str.Item("strAntal årssøer")&"&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strAntal fravænnede grise")&"&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strVægt ved fravænning")&"&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strFodermængde pr. årsso")&"&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strProtein indhold")&"&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strStaldtype") & " - " & Str.Item("strdrægtige søer")&"&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strStaldtype") & " - " & Str.Item("strdiegivende søer")&"&nbsp;</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   
   for i = 0 to nPigHerd-1
      aPigTypeID = arrPigHerd(i,dictPigHerd.item("PigTypeID"))
      if aPigTypeID = 1 then
      
      select case aPigTypeID
        case arrPigType(0,dictPigType.item("PigTypeID"))

		   set dictStableType = CreateObject("Scripting.Dictionary")
           nStableType1 = CallSP ("GetSelectListStableTypePig " & aPigTypeID& "," & 2 , arrStableType1, dictStableType)
		   set dictStableType = Nothing
		   set dictStableType = CreateObject("Scripting.Dictionary")
           nStableType2 = CallSP ("GetSelectListStableTypePig " & aPigTypeID& "," & 3 , arrStableType2, dictStableType)  
		   
'	 	   nStableType1   = GetSelectListStableTypePig(aPigTypeID,2,arrStableType1)
'          nStableType2   = GetSelectListStableTypePig(aPigTypeID,3,arrStableType2)
	case else
 		   set dictStableType = CreateObject("Scripting.Dictionary")
           nStableType1 = CallSP ("GetSelectListStableTypePig " & aPigTypeID& "," & 1 , arrStableType1, dictStableType)
		   set dictStableType = Nothing
		   set dictStableType = CreateObject("Scripting.Dictionary")
           nStableType2 = CallSP ("GetSelectListStableTypePig " & aPigTypeID& "," & 1 , arrStableType2, dictStableType)

'		   nStableType1   = GetSelectListStableTypePig(aPigTypeID,1,arrStableType1)
'          nStableType2   = GetSelectListStableTypePig(aPigTypeID,1,arrStableType2)
      end select 
      
      aStableType1ID = arrPigHerd(i,dictPigHerd.item("Stable1"))
      aStableType2ID = arrPigHerd(i,dictPigHerd.item("Stable2"))


      Response.Write("    <tr>" & vbCr)
      ' column 1 = pig type
   '   Response.Write("      <td class=""data"" align=""left"">")
  '    Response.Write("Søer")   
    '  Response.Write("</td>" & vbCr)
      ' column 2 = antal   
      Response.Write("      <td class=""data"" align=""left"">")
      Response.Write("<input type=""hidden"" name=""hiddenPigHerdID_"&i&""" value="""& arrPigHerd(i,dictPigHerd.item("PigHerdID"))&"""/>") 
      Response.Write("<input type=""hidden"" name=""selectPigType_"&i&""" value="""& aPigTypeID & """/>")   

      Response.Write("&nbsp;&nbsp;<input type=""text"" name=""inputAnimalYear_"&i&""" value="""& arrPigHerd(i,dictPigHerd.item("AnimalYear"))&""" maxlength=""5"" size=""5"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>&nbsp;"&Str.Item("str"&arrPigHerd(i,dictPigHerd.item("AnimalYearunitText")))&"&nbsp;</td>" & vbCr)  

      ' column 3 = produktionsniveau
      Response.Write("      <td class=""data"" align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputProductionVar1_"&i&""" value="""&arrPigHerd(i,dictPigHerd.item("ProductionVar1"))&""" maxlength=""5"" size=""5"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>&nbsp;"&Str.Item("str"&arrPigHerd(i,dictPigHerd.item("ProductionVar1UnitText")))&"<input type=""hidden"" name=""inputProductionVar3_"&i&""" value="""&arrPigHerd(i,dictPigHerd.item("ProductionVar3"))&"""/>&nbsp;</td>" & vbCr)
      Response.Write("      <td class=""data"" align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputProductionVar2_"&i&""" value="""&arrPigHerd(i,dictPigHerd.item("ProductionVar2"))&""" maxlength=""5"" size=""5"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>&nbsp;"&Str.Item("str"&arrPigHerd(i,dictPigHerd.item("ProductionVar2UnitText")))&"&nbsp;</td>" & vbCr)
      ' column 4 = Foder mængde
      Response.Write("      <td class=""data"" align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputFeedLevel_"&i&""" value="""&arrPigHerd(i,dictPigHerd.item("FeedLevel"))&""" maxlength=""5"" size=""5""/>&nbsp;"&Str.Item("str"&arrPigHerd(i,dictPigHerd.item("FeedLevelUnitText")))&"&nbsp;</td>" & vbCr)
      ' column 5 = Protein niveau   
      Response.Write("      <td class=""data"" align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputProteinLevel_"&i&""" value="""&arrPigHerd(i,dictPigHerd.item("ProteinLevel"))&""" maxlength=""5"" size=""5"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>&nbsp;"&Str.Item("str"&arrPigHerd(i,dictPigHerd.item("ProteinLevelUnitText")))&"&nbsp;</td>" & vbCr)
     ' column 6 = stable type   
      Response.Write("      <td class=""data"" align=""left"">")   
      Response.Write("&nbsp;&nbsp;<select name=""selectStableType1_"&i&""" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';"">")   
      CreateSelectList_Language aStableType1ID,nStableType1,arrStableType1
      Response.Write("</select>")   
      Response.Write("</td>" & vbCr)
      Response.Write("      <td class=""data"" align=""left"">")   
       Response.Write("&nbsp;&nbsp;<select name=""selectStableType2_"&i&""" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';"">")   
      CreateSelectList_Language aStableType2ID,nStableType2,arrStableType2
      Response.Write("</select>")   
     Response.Write("</td>" & vbCr)
      ' column 7 = delete buttons   
      Response.Write("      <td class=""data"" align=""left"">")   
      Response.Write("<button style=""width: 50px; height: 20px; font-size: 10px"" onClick=""javascript:if(confirm('"&Str.Item("strBekræft")&"')) {document.forms.formPigHerd.action='PigHerdDelete.asp?PigHerdID="& arrPigHerd(i,dictPigHerd.item("PigHerdID"))&"';document.forms.formPigHerd.submit();}"">"&Str.Item("strDelete")&"</button>")
      Response.Write("</td>" & vbCr)   
      Response.Write("    </tr>" & vbCr)
   set dictStableType = Nothing
   end if
   next
'    row for buttons
      Response.Write("    <tr>" & vbCr)
   Response.Write("      <td class=""data"" colspan=""7"">&nbsp;&nbsp;")   
   Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formPigHerd.action='PigHerdNew.asp';document.forms.formPigHerd.hiddenPigType.value='1';document.forms.formPigHerd.submit();"">"&Str.Item("strNEW")&"</button>")
   Response.Write("</td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)

   Response.Write("    <tr>" & vbCr)

   Response.Write("      <td class=""data"" colspan=""7""><hr/></td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
   Response.Write("  </table>" & vbCr) 
  '    Tabel for resten
   Response.Write("  <table class=""data"" border=""0"" width=""100%"">" & vbCr)
   ' column headings

        Response.Write("    <tr>" & vbCr)   
      Response.Write("      <td colspan=""7""><h4>"&Str.Item("strSmågrise") & " " & Str.Item("strog") & " " &  Str.Item("strSlagtesvin") &"</h4></td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
   
   
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th align=""left"">Type&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strAntal producerede dyr") & "&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strStartvægt") & "&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strSlutvægt") & "&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strFodermængde pr. produceret dyr") & "&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strProtein indhold") & "&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strStaldtype") & "&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)

for i = 0 to nPigHerd-1
      aPigTypeID = arrPigHerd(i,dictPigHerd.item("PigTypeID"))
'	  response.write "<br> aPigTypeID " & aPigTypeID
      if aPigTypeID > 1 then
      
 	  set dictStableType = CreateObject("Scripting.Dictionary")
      nStableType1 = CallSP ("GetSelectListStableTypePig " & aPigTypeID& "," & 1, arrStableType1, dictStableType)
'	   nStableType1   = GetSelectListStableTypePig(aPigTypeID,1,arrStableType1)
 
	  aStableType1ID = arrPigHerd(i,dictPigHerd.item("Stable1"))
      aStableType2ID = null

      Response.Write("    <tr>" & vbCr)
      ' column 1 = pig type
      Response.Write("      <td class=""data"" align=""left"">")
      Response.Write("<input type=""hidden"" name=""hiddenPigHerdID_"&i&""" value="""& arrPigHerd(i,dictPigHerd.item("PigHerdID"))&"""/>") 
      Response.Write("&nbsp;&nbsp;<select name=""selectPigType_"&i&""" onChange=""ChangeTextPig("&i&",this.options[this.selectedIndex].value);document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';"">")   
      CreateSelectListLanguage aPigTypeID,Ubound(arrPigTypeNotSows,2)+1,arrPigTypeNotSows
      Response.Write("</select>")   
      Response.Write("</td>" & vbCr)
      ' column 2 = antal   
      Response.Write("      <td class=""data"" align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputAnimalYear_"&i&""" value="""& arrPigHerd(i,dictPigHerd.item("AnimalYear"))&""" maxlength=""5"" size=""5"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>&nbsp;"&Str.Item("str"&arrPigHerd(i,dictPigHerd.item("AnimalYearunitText")))&"&nbsp;</td>" & vbCr)  

      ' column 3 = produktionsniveau
      Response.Write("      <td class=""data"" align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputProductionVar1_"&i&""" value="""&arrPigHerd(i,dictPigHerd.item("ProductionVar1"))&""" maxlength=""5"" size=""5"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>&nbsp;"&Str.Item("str"&arrPigHerd(i,dictPigHerd.item("ProductionVar1UnitText")))&"&nbsp;</td>" & vbCr)
      Response.Write("      <td class=""data"" align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputProductionVar2_"&i&""" value="""&arrPigHerd(i,dictPigHerd.item("ProductionVar2"))&""" maxlength=""5"" size=""5"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>&nbsp;"&Str.Item("str"&arrPigHerd(i,dictPigHerd.item("ProductionVar2UnitText")))&"&nbsp;<input type=""hidden"" name=""inputProductionVar3_"&i&""" value="""&arrPigHerd(i,dictPigHerd.item("ProductionVar3"))&"""/></td>" & vbCr)
      ' column 4 = Foder mængde
      Response.Write("      <td class=""data"" align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputFeedLevel_"&i&""" value="""&arrPigHerd(i,dictPigHerd.item("FeedLevel"))&""" maxlength=""5"" size=""5""/>&nbsp;"&Str.Item("str"&arrPigHerd(i,dictPigHerd.item("FeedLevelUnitText")))&"&nbsp;</td>" & vbCr)
      ' column 5 = Protein niveau   
      Response.Write("      <td class=""data"" align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputProteinLevel_"&i&""" value="""&arrPigHerd(i,dictPigHerd.item("ProteinLevel"))&""" maxlength=""5"" size=""5"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>&nbsp;"&Str.Item("str"&arrPigHerd(i,dictPigHerd.item("ProteinLevelUnitText")))&"&nbsp;</td>" & vbCr)
     ' column 6 = stable type   
      Response.Write("      <td class=""data"" align=""left"">")   
      Response.Write("&nbsp;&nbsp;<select name=""selectStableType1_"&i&""" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';"">")   
      CreateSelectList_Language aStableType1ID,nStableType1,arrStableType1
      Response.Write("</select>")   
      Response.Write("</td>" & vbCr)
      ' column 7 = delete buttons   
      Response.Write("      <td class=""data"" align=""left"">")   
      Response.Write("<button style=""width: 50px; height: 20px; font-size: 10px"" onClick=""javascript:if(confirm('"&Str.Item("strBekræft")&"')) {document.forms.formPigHerd.action='PigHerdDelete.asp?PigHerdID="& arrPigHerd(i,dictPigHerd.item("PigHerdID"))&"';document.forms.formPigHerd.submit();}"">"&Str.Item("strDelete")&"</button>")
      Response.Write("</td>" & vbCr)   
      Response.Write("    </tr>" & vbCr)
   set dictStableType = Nothing
  end if
   next
'    row for buttons
      Response.Write("    <tr>" & vbCr)
   Response.Write("      <td class=""data"" colspan=""7"">")   
   Response.Write("&nbsp;&nbsp;<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formPigHerd.action='PigHerdNew.asp';document.forms.formPigHerd.submit();"">"&Str.Item("strNEW")&"</button>")
   Response.Write("</td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)

   
   Response.Write("    <tr>" & vbCr)

   Response.Write("      <td class=""data"" colspan=""7""><hr/></td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <td class=""data"" colspan=""7"">")   
   Response.Write("&nbsp;&nbsp;<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formPigHerd.action='PigHerdUpdate.asp';document.forms.formPigHerd.submit();"">"&Str.Item("strSAVE")&"</button>")
   Response.Write("<input type=""hidden"" name=""hiddenNPigHerd"" value="""&nPigHerd&"""/>" & vbCr)
   Response.Write("<input type=""hidden"" name=""hiddenPigType"" id=""hiddenPigType"" value=""2""/>" & vbCr)
   Response.Write("</td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
   
   Response.Write("  </table>" & vbCr) 
   Response.Write("</form>" & vbCr)
   CreateUpdateFormPig
   if nPigHerd > 0 then
      CreatePigHerdTotal
   end if
   CloseFarmNDB
   set dictPigType = nothing

end sub
'**************************************************************************************************
sub CreatePigHerdTotal()
   dim p,t,c,f
   dim nParameter, arrParameter, dictParameter, aPigTypeID, nPigHerdTotal, arrPigHerdTotal, dictPigHerdTotal
   dim nFarmTotal, arrFarmTotal, dictFarmTotal, nPigType, arrPigType, dictPigType

   set dictParameter = CreateObject("Scripting.Dictionary")
   nPigType = CallSP ("GetParameterList " & 4 , arrParameter, dictParameter)
'   nParameter  = GetParameterList(4,arrParameter)
'   print2 arrParameter, dictParameter
   set dictPigType = CreateObject("Scripting.Dictionary")
   nPigType = CallSP ("GetPigHerdPigTypeList " & CLng(Session("farmNumber")) &","& CInt(Session("scenarioNumber")), arrPigType, dictPigType)
'   print2 arrPigType, dictPigType
'   nPigType = GetPigHerdPigTypeList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrPigType)
    set dictFarmTotal = CreateObject("Scripting.Dictionary")
   nFarmTotal = CallSP ("GetFarmTotalList " & CLng(Session("farmNumber")) & "," & CInt(Session("scenarioNumber")) & "," & 4 , arrFarmTotal, dictFarmTotal)
   'nFarmTotal  = GetFarmTotalList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),4,arrFarmTotal)
'   print2 arrFarmTotal, dictFarmTotal

   ' rows for totals
   Response.Write("  <br/>" & vbCr) 
   Response.Write("  <h4>&nbsp;"&Str.Item("strbehovprodgødning")&"</h4>" & vbCr)
         
   Response.Write("  <table class=""data"" border=""0"" cellspacing=""0"" cellpadding=""0""  width=""80%"">" & vbCr)
   ' row for header
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <th class=""data"" align=""left"" nowrap>&nbsp;&nbsp;"&Str.Item("strHusdyrhold")&"</th>" & vbCr) 
   for p = 0 to UBound(arrParameter) 
      Response.Write("      <th class=""data"" align=""right"">&nbsp;&nbsp;"&  Str.Item("str"&arrParameter(p,dictParameter.item("ParameterName")))&"<br/>("& Str.Item("str"&arrParameter(p,dictParameter.item("ParameterUnit")))&")</th>" & vbCr)
   next  
   Response.Write("    </tr>" & vbCr)
   ' rows for feeding requirements
   for t = 0 to UBound(arrPigtype) 
      aPigTypeID = arrPigType(t,dictPigType.item("PigType"))
      
      set dictPigHerdTotal = CreateObject("Scripting.Dictionary")
      nPigHerdTotal = CallSP ("GetPigHerdTotalList " & CLng(Session("farmNumber")) & "," & CInt(Session("scenarioNumber")) & "," & aPigTypeID , arrPigHerdTotal, dictPigHerdTotal)
'	  print2 arrPigHerdTotal, dictPigHerdTotal
' 	  nPigHerdTotal = GetPigHerdTotalList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),aPigTypeID,arrPigHerdTotal)
      Response.Write("    <tr>" & vbCr)
      Response.Write("      <td nowrap>&nbsp;&nbsp;"&arrPigType(t,dictPigType.item("PigType"))&" - "&Str.Item("str"&arrPigType(t,dictPigType.item("PigTypeName")))&"</td>" & vbCr)  
      for c = 0 to UBound(arrPigHerdTotal)
         if CLng(arrPigHerdTotal(c,dictPigHerdTotal.item("ParameterValue"))) <> CInt(0) then
            Response.Write("      <td align=""right"">"&FormatNumber(arrPigHerdTotal(c,dictPigHerdTotal.item("ParameterValue")),0,-1,0,0)&"</td>" & vbCr)
         else
            Response.Write("      <td align=""right"">&nbsp;</td>" & vbCr)
         end if
      next  
      Response.Write("    </tr>" & vbCr)
      set dictPigHerdTotal = nothing
   next

   Response.Write("    <tr>" & vbCr)
   Response.Write("      <td colspan="""&UBound(arrParameter)+2&"""><hr/></td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
   ' row for total
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <th class=""data"" align=""left"" nowrap>&nbsp;&nbsp;"&Str.Item("strTOTALALLGROUPS")&"</th>" & vbCr)
   for f = 0 to UBound(arrFarmTotal)
      Response.Write("      <th class=""data"" align=""right"">"&FormatNumber(arrFarmTotal(f,dictFarmTotal("ParameterValue")),0,-1,0,0)&"</th>" & vbCr)   
   next      
   Response.Write("    </tr>" & vbCr)
   Response.Write("  </table>" & vbCr)
end sub
'**************************************************************************************************
sub DefineChangeTextScriptPig()
   dim nPigType, arrPigType, dictPigType, j
   
   set dictPigType = CreateObject("Scripting.Dictionary")
   nPigType = CallSP ("GetPigTypeList ", arrPigType, dictPigType)
'   nPigType = GetPigTypeList(arrPigType)

   Response.Write("<script language=""JavaScript"" type=""text/javascript"">" & vbCr)
   Response.Write("<!--" & vbCr)
   
   Response.Write("  function ChangeTextPig(i,t)" & vbCr)
   Response.Write("  {" & vbCr)
   Response.Write("     switch (t) {" & vbCr)
   for j=0 to nPigType-1
	   Response.Write("     case '"&arrPigType(j,dictPigType.item("PigTypeID"))&"' :                         eval(""document.forms.formPigHerd.inputProductionVar1_""+i+"".value='"&Trim(arrPigType(j,dictPigType.item("ProductionVar1Default")))&"'"");" & vbCr)	   
	   Response.Write("                      eval(""document.forms.formPigHerd.inputProductionVar2_""+i+"".value='"&Trim(arrPigType(j,dictPigType.item("ProductionVar2Default")))&"'"");" & vbCr)	   
	   Response.Write("                      eval(""document.forms.formPigHerd.inputFeedLevel_""+i+"".value='"&Trim(arrPigType(j,dictPigType.item("FeedLevelDefault")))&"'"");" & vbCr)	   
	   Response.Write("                      eval(""document.forms.formPigHerd.inputProteinLevel_""+i+"".value='"&Trim(arrPigType(j,dictPigType.item("ProteinLevelDefault")))&"'"");" & vbCr)	   
	   Response.Write("                      break;" & vbCr)
   next
   Response.Write("                }" & vbCr)
   Response.Write("  }" & vbCr)
   
   Response.Write("//-->" & vbCr)
   Response.Write("</script>" & vbCr)
end sub
'**************************************************************************************************

sub CreateUpdateFormPig()
   Response.Write("<form name=""update"" action=""javascript:document.forms.formPigHerd.action='PigHerdUpdate.asp';document.forms.formPigHerd.submit();"" method=""post"">" & vbCr)
   Response.Write("<input type=""hidden"" name=""saveUpdates"" value=""false"">" & vbCr)
   Response.Write("</form>" & vbCr)
end sub
'**************************************************************************************************

sub PigTypeNotSows(arrPigType, arrPigTypeNotSows)
dim i,j
 redim arrPigTypeNotSows(1,0)
for i = 0 to Ubound(arrPigType)
 if CInt(arrPigType(i,0))<>CInt(1) then
  redim preserve arrPigTypeNotSows(1,j)
  arrPigTypeNotSows(0,j)=arrPigType(i,0)
  arrPigTypeNotSows(1,j)=arrPigType(i,1)
'  response.write "<br> Test "&arrPigTypeNotSows(0,j) & " " & arrPigTypeNotSows(1,j) 
  j=j+1
 end if
Next
end sub
'**************************************************************************************************

%>
