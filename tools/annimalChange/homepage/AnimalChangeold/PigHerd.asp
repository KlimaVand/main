<%
'**************************************************************************************************
' Author: Poul Lassen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************

sub CreateTabPig()
   dim i, nPigHerd, nPigType, arrPigType, arrPigHerd, aPigTypeID, nStableType1, nStableType2, aStableType1ID, aStableType2ID, arrStableType1, arrStableType2, dictStableType, arrPigTypeNotSows, dictPigHerd, dictPigType
   dim nFeedStuff, arrFeedStuff, aFeedStuffID, aFeedstuffAmount, dictFeedStuff, nf,pf
   dim strFeedplan1,strFeedplan2,strAmount1,strAmount2
   dim nPigHerdFeedplanList, arrPigHerdFeedplanList, aPigHerdFeedplanID, dictPigHerdFeedplanList, fp, strFeedplan, strAmount,f
   dim nManureStorage, arrManureStorage, aManureStorageID, dictManureStorage, ms
   dim nPigHerdManuReStorageList, arrPigHerdManureStorageList, aPigHerdManureStorageID, dictPigHerdManureStorageList

 '  Response.Write("<br/><center><h3>Det er ikke muligt at arbejde med svin endnu</h3></center>" & vbCr)
time12=now
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
   set dictFeedStuff = CreateObject("Scripting.Dictionary")
   nFeedStuff = CallSP ("GetFeedStuffList" , arrFeedStuff, dictFeedStuff)
   
   DefineChangeTextScriptPig
   DefineFeedstuffListJScript_8

   Response.Write("<form name=""formPigHerd"" action=""DummyPigHerd.asp"" method=""post"" id=""dynamichide"" style=""display: block;"">" & vbCr) 

   Response.Write("  <table class=""data"" border=""0"" style=""width: 1800px;"">" & vbCr)
   ' column headings
   Response.Write("    <tr>" & vbCr)   
      Response.Write("      <td colspan=""7""><h4>"&Str.Item("strSøer")&"</h4></td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
   
   
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th align=""left"" style=""width: 130px;"">"&Str.Item("strAntal årssøer")&"&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"" style=""width: 190px;"">"&Str.Item("strAntal fravænnede grise")&"&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"" style=""width: 182px;"">"&Str.Item("strVægt ved fravænning")&"&nbsp;</th>" & vbCr)
'   Response.Write("      <th align=""left"" style=""width: 40px;"">&nbsp;</th>" & vbCr)
'   Response.Write("      <th align=""left"" width=""10%"">"&Str.Item("strProtein indhold")&"&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"" style=""width: 625px;"">"&Str.Item("strFeedstuffPlan")&"&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strStaldtype") & " - " & Str.Item("strdrægtige søer")&"&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strStaldtype") & " - " & Str.Item("strdiegivende søer")&"&nbsp;</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   
   for i = 0 to nPigHerd-1
'response.write "nPigHerdManureStorageList : "&nPigHerdManureStorageList
'response.write "arrPigHerd(i,dictPigHerd.Item('PigHerdID')) : "&arrPigHerd(i,dictPigHerd.Item("PigHerdID"))
   f= arrPigHerd(i,dictPigHerd.item("PigHerdID"))
   set dictPigHerdFeedplanList = CreateObject("Scripting.Dictionary")
   nPigHerdFeedplanList = CallSP ("GetPigHerdFeedplanList " & Session("farmNumber")& "," & Session("scenarioNumber") & "," & arrPigHerd(i,dictPigHerd.Item("PigHerdID")), arrPigHerdFeedplanList, dictPigHerdFeedplanList)
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
      Response.Write("      <input type=""hidden"" name=""inputFeedLevel_"&i&""" value="""&arrPigHerd(i,dictPigHerd.item("FeedLevel"))&""" />" & vbCr)
      ' column 5 = Protein niveau   
      Response.Write("      <input type=""hidden"" name=""inputProteinLevel_"&i&""" value="""&arrPigHerd(i,dictPigHerd.item("ProteinLevel"))&"""/>" & vbCr)
     ' column 6 = stable type   
      Response.Write("<td>" & vbCr)
   Response.Write("<table border=""0"" cellspacing=""0"" width=""100%"">" & vbCr)
   Response.Write("<tbody id=""tbodyFeedstuff_"&f&""" name=""tbodyFeedstuff_"&f&""">" & vbCr)
if nPigHerdFeedplanList=0 then
   Response.Write("<tr>" & vbCr)
   Response.Write("<td>" & vbCr)
	       Response.Write("<select id=""selFeedstuff_"&f&""" name=""selFeedstuff_"&f&""" onchange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';checkForNewLine("&f&");setFeedstuffString("&f&");"" style=""position:relative;top:-3Px;background-color: lightyellow;"">" & vbCr)
		   Response.write("  <option value=""-1"">"&Str.item("strSELECT_FEEDSTUFF")&"</option>" & vbCr)
		   for nf = 0 to nFeedstuff-1
		      if aFeedstuffID = Cstr(arrFeedstuff(nf,0)) then
		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""" selected>"&arrFeedstuff(nf,1)&"</option>" & vbCr)
		      else
		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""">"&arrFeedstuff(nf,1)&"</option>" & vbCr)
		      end if
		   next
		   Response.Write("</select>" & vbCr)
   Response.Write("<input type=""text"" name=""feedstuffAmount_"&f&""" id=""feedstuffAmount_"&f&""" style=""width: 35px;position:relative;top:-3Px;background-color: lightyellow;"" onchange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';this.value.replace(',','.');setFeedstuffString("&f&");"" value=""0""> "&Str.Item("strKgDM/animal/day")&"" & vbCr)
   Response.Write("<img style=""position:relative;top:3Px;"" src=""images/del.gif"" onclick=""delRow(this,"&f&");"" onMouseover=""mouseOverGif(this)""  onMouseout=""mouseOutGif(this)""/>&nbsp;&nbsp;" & vbCr)
   Response.Write("</td>" & vbCr)
   Response.Write("</tr>" & vbCr)
else
for fp = 0 to nPigHerdFeedplanList-1 
  strFeedplan = strFeedplan & arrPigHerdFeedplanList(fp,0)& ","
  strAmount = strAmount & replace(arrPigHerdFeedplanList(fp,1),",",".")& ","
   Response.Write("<tr>" & vbCr)
   Response.Write("<td>" & vbCr)
	       Response.Write("<select id=""selFeedstuff_"&f&""" name=""selFeedstuff_"&f&""" onchange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';checkForNewLine("&f&");setFeedstuffString("&f&");"" style=""position:relative;top:-3Px;background-color: white;"">" & vbCr)
		   Response.write("  <option value=""-1"">"&Str.item("strSELECT_FEEDSTUFF")&"</option>" & vbCr)
		   for nf = 0 to nFeedstuff-1
		   if ubound(arrPigHerdFeedplanList)<>-1 then 
		     if fp < nPigHerdFeedplanList then
			  aFeedstuffID = arrPigHerdFeedplanList(fp,0)
			  aFeedstuffAmount = replace(arrPigHerdFeedplanList(fp,1),",",".")
			 end if
		   else
		     aFeedstuffID = -1
			 aFeedstuffAmount = 0
		   end if
		      if aFeedstuffID = Cstr(arrFeedstuff(nf,0)) then
		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""" selected>"&arrFeedstuff(nf,1)&"</option>" & vbCr)
		      else
		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""">"&arrFeedstuff(nf,1)&"</option>" & vbCr)
		      end if
		   next
		   Response.Write("</select>" & vbCr)
   Response.Write("<input id=""feedstuffAmount_"&f&""" name=""feedstuffAmount_"&f&""" type=""text"" style=""width: 35px;position:relative;top:-3Px;background-color: white;"" value="""&aFeedstuffAmount&""" onchange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';this.value=this.value.replace(',','.');setFeedstuffString("&f&");""/> "&Str.Item("strKgDM/animal/day")&"" & vbCr)
   Response.Write("<img style=""position:relative;top:3Px;"" src=""images/del.gif"" onclick=""delRow(this,"&f&");"" onMouseover=""mouseOverGif(this)""  onMouseout=""mouseOutGif(this)""/>&nbsp;&nbsp;" & vbCr)
   Response.Write("</td>" & vbCr)
   Response.Write("</tr>" & vbCr)
   aFeedstuffID=-1
next
   Response.Write("<tr>" & vbCr)
   Response.Write("<td>" & vbCr)
	       Response.Write("<select id=""selFeedstuff_"&f&""" name=""selFeedstuff_"&f&""" onchange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';checkForNewLine("&f&");setFeedstuffString("&f&");"" style=""position:relative;top:-3Px;background-color: lightyellow;"">" & vbCr)
		   Response.write("  <option value=""-1"">"&Str.item("strSELECT_FEEDSTUFF")&"</option>" & vbCr)
		   for nf = 0 to nFeedstuff-1
		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""">"&arrFeedstuff(nf,1)&"</option>" & vbCr)
		   next
		   Response.Write("</select>" & vbCr)
   Response.Write("<input type=""text"" name=""feedstuffAmount_"&f&""" id=""feedstuffAmount_"&f&""" style=""width: 35px;position:relative;top:-3Px;background-color: lightyellow;"" onchange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';this.value=this.value.replace(',','.');setFeedstuffString("&f&");"" value=""0""> "&Str.Item("strKgDM/animal/day")&"" & vbCr)
   Response.Write("<img style=""position:relative;top:3Px;"" src=""images/del.gif"" onclick=""delRow(this,"&f&");"" onMouseover=""mouseOverGif(this)""  onMouseout=""mouseOutGif(this)""/>&nbsp;&nbsp;" & vbCr)
   Response.Write("</td>" & vbCr)
   Response.Write("</tr>" & vbCr)
end if
   Response.Write("</tbody>" & vbCr)
   Response.Write("</table>" & vbCr)
      Response.Write("<input type=""hidden"" name=""hiddenFeedstuffList_"&f&"""  id=""hiddenFeedstuffList_"&f&""" value="""&strFeedplan&""">")
	  strFeedplan = ""
      Response.Write("<input type=""hidden"" name=""hiddenAmountList_"&f&"""  id=""hiddenAmountList_"&f&""" value="""&strAmount&""">")
      Response.Write("</td>" & vbCr)
      Response.Write("      <td class=""data"" align=""left"" style=""vertical-align: top;"">")   
      Response.Write("<select style=""width: 220px;"" name=""selectStableType1_"&i&""" onChange=""javascript:document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';"">")   
      CreateSelectList_Language aStableType1ID,nStableType1,arrStableType1
      Response.Write("</select><br/><br/>")   
      response.write "<input type=""hidden"" id=""storageChanged1_"&i&""" name=""storageChanged1_"&i&""" value=""0""/>"
   set dictPigHerdManureStorageList = CreateObject("Scripting.Dictionary")
   nPigHerdManureStorageList = CallSP ("GetPigHerdManureStorageList " & Session("farmNumber")& "," & Session("scenarioNumber") & "," & arrPigHerd(i,dictPigHerd.Item("PigHerdID"))& ",1", arrPigHerdManureStorageList, dictPigHerdManureStorageList)
     set dictStorageType = CreateObject("Scripting.Dictionary")
     nStorageType = CallSP ("GetStorageTypesPrPigStable " & aPigTypeID & "," & aStableType1ID, arrStorageType, dictStorageType)
'response.write "nStorageType : "&nStorageType
     Response.Write("<input type=""hidden"" name=""nManureStorage1_"&i&"""  id=""nManureStorage1_"&i&""" value="""&nStorageType&"""/>")
      response.write "<b>"&Str.Item("strManureStorageType")&"</b><br/>"
	 for ms = 0 to ubound(arrStorageType)  
   set dictManureStorage = CreateObject("Scripting.Dictionary")
   nManureStorage = CallSP ("GetSelectListPigManureStorageType " & aPigTypeID & "," & aStableType1ID &","&arrStorageType(ms,0), arrManureStorage, dictManureStorage)
   aPigHerdManureStorageID = arrManureStorage(0,0)
	if nPigHerdManureStorageList>0  then
		if ms<nPigHerdManureStorageList then
		 aPigHerdManureStorageID = arrPigHerdManureStorageList(ms,1)
		end if
	end if
'      response.write Str.Item("strManureStorageType")
      Response.Write("<select style=""width: 220px;"" name=""selectManureStorageType1_"&i&""" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';"">")   
      CreateSelectList_ aPigHerdManureStorageID,nManureStorage,arrManureStorage
      Response.Write("</select>")
	  if ms < ubound(arrStorageType) then response.write "<br/><br/>"
      set dictManureStorage = nothing
	 next   
      Response.Write("</td>" & vbCr)
      Response.Write("      <td class=""data"" align=""left"" style=""vertical-align: top;"">")   
       Response.Write("<select style=""width: 220px;"" name=""selectStableType2_"&i&""" onChange=""javascript:document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';"">")   
      CreateSelectList_Language aStableType2ID,nStableType2,arrStableType2
      Response.Write("</select><br/><br/>")   
      response.write "<input type=""hidden"" id=""storageChanged2_"&i&""" name=""storageChanged2_"&i&""" value=""0""/>"
   set dictPigHerdManureStorageList = nothing
   set dictPigHerdManureStorageList = CreateObject("Scripting.Dictionary")
   nPigHerdManureStorageList = CallSP ("GetPigHerdManureStorageList " & Session("farmNumber")& "," & Session("scenarioNumber") & "," & arrPigHerd(i,dictPigHerd.Item("PigHerdID"))& ",2", arrPigHerdManureStorageList, dictPigHerdManureStorageList)
     set dictStorageType = CreateObject("Scripting.Dictionary")
     nStorageType = CallSP ("GetStorageTypesPrPigStable " & aPigTypeID & "," & aStableType2ID, arrStorageType, dictStorageType)
     Response.Write("<input type=""hidden"" name=""nManureStorage2_"&i&"""  id=""nManureStorage2_"&i&""" value="""&nStorageType&"""/>")
'     Response.Write("      <td class=""data"" align=""left"">")
      response.write "<b>"&Str.Item("strManureStorageType")&"</b><br/>"
	 for ms = 0 to ubound(arrStorageType)  
   set dictManureStorage = CreateObject("Scripting.Dictionary")
   nManureStorage = CallSP ("GetSelectListPigManureStorageType " & aPigTypeID & "," & aStableType2ID &","&arrStorageType(ms,0), arrManureStorage, dictManureStorage)
   aPigHerdManureStorageID = arrManureStorage(0,0)
	if nPigHerdManureStorageList>0  then
		if ms<nPigHerdManureStorageList then
		 aPigHerdManureStorageID = arrPigHerdManureStorageList(ms,1)
		end if
	end if
      Response.Write("<select style=""width: 220px;"" name=""selectManureStorageType2_"&i&""" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';"">")   
      CreateSelectList_ aPigHerdManureStorageID,nManureStorage,arrManureStorage
      Response.Write("</select>")
	  if ms < ubound(arrStorageType) then response.write "<br/><br/>"
      set dictManureStorage = nothing
	 next   

     Response.Write("</td>" & vbCr)
      ' column 7 = delete buttons   
      Response.Write("      <td class=""data"" align=""left"">")   
      Response.Write("<button style=""width: 50px; height: 20px; font-size: 10px"" onClick=""javascript:if(confirm('"&Str.Item("strBekræft")&"')) {veil();document.forms.formPigHerd.action='PigHerdDelete.asp?PigHerdID="& arrPigHerd(i,dictPigHerd.item("PigHerdID"))&"';document.forms.formPigHerd.submit();}"">"&Str.Item("strDelete")&"</button>")
      Response.Write("</td>" & vbCr)   
      Response.Write("    </tr>" & vbCr)
   Response.Write("    <tr>" & vbCr)

   Response.Write("      <td class=""data"" colspan=""7""><hr/></td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
   set dictStableType = Nothing
   end if
   set dictPigHerdFeedplanList = nothing
   set dictPigHerdManureStorageList = nothing
   strFeedplan = ""
   strAmount = ""
  next
'    row for buttons
'if aPigTypeID=1 then
      Response.Write("    <tr>" & vbCr)
   Response.Write("      <td class=""data"" colspan=""7"">&nbsp;&nbsp;")   
   Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:veil();document.forms.formPigHerd.action='PigHerdNew.asp';document.forms.formPigHerd.hiddenPigType.value='1';document.forms.formPigHerd.submit();"">"&Str.Item("strNEW")&"</button>")
   Response.Write("</td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
'end if
   Response.Write("    <tr>" & vbCr)

   Response.Write("      <td class=""data"" colspan=""7""><hr/></td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
   Response.Write("  </table>" & vbCr) 
  '    Tabel for reste
  

   Response.Write("  <table class=""data"" border=""0"" width=""100%"">" & vbCr)
   ' column headings

        Response.Write("    <tr>" & vbCr)   
      Response.Write("      <td colspan=""7""><h4>"&Str.Item("strSmågrise") & " " & Str.Item("strog") & " " &  Str.Item("strSlagtesvin") &"</h4></td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
   
   
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th align=""left"" style=""width: 140px;"">Type&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"" style=""width: 140px;"">"&Str.Item("strANANIMAL") & "&nbsp;</th>" & vbCr)
'   Response.Write("      <th align=""left"" style=""width: 110px;"">"&Str.Item("strStartvægt") & "&nbsp;</th>" & vbCr)
'   Response.Write("      <th align=""left"" style=""width: 110px;"">"&Str.Item("strSlutvægt") & "&nbsp;</th>" & vbCr)
'   Response.Write("      <th align=""left"" width=""10%"">"&Str.Item("strFodermængde pr. produceret dyr") & "&nbsp;</th>" & vbCr)
'   Response.Write("      <th align=""left"" width=""15%"">"&Str.Item("strProtein indhold") & "&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"" style=""width: 625px;"">"&Str.Item("strFeedstuffPlan")&"&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strStaldtype") & "&nbsp;</th>" & vbCr)
'   Response.Write("      <th align=""left"" width=""10%"">&nbsp;&nbsp;</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)

for i = 0 to nPigHerd-1
   set dictPigHerdManureStorageList = CreateObject("Scripting.Dictionary")
   nPigHerdManureStorageList = CallSP ("GetCattleHerdManureStorageList " & Session("farmNumber")& "," & Session("scenarioNumber") & "," & arrPigHerd(i,dictPigHerd.Item("PigHerdID")), arrPigHerdManureStorageList, dictPigHerdManureStorageList)
   f= arrPigHerd(i,dictPigHerd.item("PigHerdID"))
       aPigTypeID = arrPigHerd(i,dictPigHerd.item("PigTypeID"))
   set dictPigHerdFeedplanList = CreateObject("Scripting.Dictionary")
   nPigHerdFeedplanList = CallSP ("GetPigHerdFeedplanList " & Session("farmNumber")& "," & Session("scenarioNumber") & "," & arrPigHerd(i,dictPigHerd.Item("PigHerdID")), arrPigHerdFeedplanList, dictPigHerdFeedplanList)
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
      Response.Write("      <input type=""hidden"" name=""inputProductionVar1_"&i&""" value="""&arrPigHerd(i,dictPigHerd.item("ProductionVar1"))&""" maxlength=""5"" size=""5"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>"& vbCr)
      Response.Write("      <input type=""hidden"" name=""inputProductionVar2_"&i&""" value="""&arrPigHerd(i,dictPigHerd.item("ProductionVar2"))&""" maxlength=""5"" size=""5"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/><input type=""hidden"" name=""inputProductionVar3_"&i&""" value="""&arrPigHerd(i,dictPigHerd.item("ProductionVar3"))&"""/>" & vbCr)
      ' column 4 = Foder mængde
      Response.Write("      <input type=""hidden"" name=""inputFeedLevel_"&i&""" value="""&arrPigHerd(i,dictPigHerd.item("FeedLevel"))&"""/>" & vbCr)
      ' column 5 = Protein niveau   
      Response.Write("      <input type=""hidden"" name=""inputProteinLevel_"&i&""" value="""&arrPigHerd(i,dictPigHerd.item("ProteinLevel"))&"""/>" & vbCr)
     ' column 6 = stable type   
     Response.Write("<td>" & vbCr)
   Response.Write("<table id=""dynamichide"" style=""display: block;"" border=""0"" cellspacing=""0"" width=""100%"">" & vbCr)
   Response.Write("<tbody id=""tbodyFeedstuff_"&f&""" name=""tbodyFeedstuff_"&f&""">" & vbCr)
if nPigHerdFeedplanList=0 then
   Response.Write("<tr>" & vbCr)
   Response.Write("<td>" & vbCr)
	       Response.Write("<select id=""selFeedstuff_"&f&""" name=""selFeedstuff_"&f&""" onchange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';checkForNewLine("&f&");setFeedstuffString("&f&");"" style=""position:relative;top:-3Px;background-color: lightyellow;"">" & vbCr)
		   Response.write("  <option value=""-1"" selected>"&Str.item("strSELECT_FEEDSTUFF")&"</option>" & vbCr)
		   for nf = 0 to nFeedstuff-1
'		      if aFeedstuffID = Cstr(arrFeedstuff(nf,0)) then
'		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""" selected>"&arrFeedstuff(nf,1)&"</option>" & vbCr)
'		      else
		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""">"&arrFeedstuff(nf,1)&"</option>" & vbCr)
'		      end if
		   next
		   Response.Write("</select>" & vbCr)
   Response.Write("<input type=""text"" name=""feedstuffAmount_"&f&""" id=""feedstuffAmount_"&f&""" style=""width: 35px;position:relative;top:-3Px;background-color: lightyellow;"" onchange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';this.value=this.value.replace(',','.');setFeedstuffString("&f&");"" value=""0""> "&Str.Item("strKgDM/animal/day")&"" & vbCr)
   Response.Write("<img style=""position:relative;top:3Px;"" src=""images/del.gif"" onclick=""delRow(this,"&f&");"" onMouseover=""mouseOverGif(this)""  onMouseout=""mouseOutGif(this)""/>&nbsp;&nbsp;" & vbCr)
   Response.Write("</td>" & vbCr)
   Response.Write("</tr>" & vbCr)
else
for fp = 0 to nPigHerdFeedplanList-1 
  strFeedplan = strFeedplan & arrPigHerdFeedplanList(fp,0)& ","
  strAmount = strAmount & replace(arrPigHerdFeedplanList(fp,1),",",".")& ","
   Response.Write("<tr>" & vbCr)
   Response.Write("<td>" & vbCr)
	       Response.Write("<select id=""selFeedstuff_"&f&""" name=""selFeedstuff_"&f&""" onchange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';checkForNewLine("&f&");setFeedstuffString("&f&");"" style=""position:relative;top:-3Px;background-color: white;"">" & vbCr)
		   Response.write("  <option value=""-1"">"&Str.item("strSELECT_FEEDSTUFF")&"</option>" & vbCr)
		   for nf = 0 to nFeedstuff-1
		   if ubound(arrPigHerdFeedplanList)<>-1 then 
		     if fp < nPigHerdFeedplanList then
			  aFeedstuffID = arrPigHerdFeedplanList(fp,0)
			  aFeedstuffAmount = replace(arrPigHerdFeedplanList(fp,1),",",".")
			 end if
		   else
		     aFeedstuffID = -1
			 aFeedstuffAmount = 0
		   end if
		      if aFeedstuffID = Cstr(arrFeedstuff(nf,0)) then
		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""" selected>"&arrFeedstuff(nf,1)&"</option>" & vbCr)
		      else
		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""">"&arrFeedstuff(nf,1)&"</option>" & vbCr)
		      end if
		   next
		   Response.Write("</select>" & vbCr)
   Response.Write("<input id=""feedstuffAmount_"&f&""" name=""feedstuffAmount_"&f&""" type=""text"" style=""width: 35px;position:relative;top:-3Px;background-color: white;"" value="""&aFeedstuffAmount&""" onchange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';this.value=this.value.replace(',','.');setFeedstuffString("&f&");""/> "&Str.Item("strKgDM/animal/day")&"" & vbCr)
   Response.Write("<img style=""position:relative;top:3Px;"" src=""images/del.gif"" onclick=""delRow(this,"&f&");"" onMouseover=""mouseOverGif(this)""  onMouseout=""mouseOutGif(this)""/>&nbsp;&nbsp;" & vbCr)
   Response.Write("</td>" & vbCr)
   Response.Write("</tr>" & vbCr)
   aFeedstuffID=-1
next
   Response.Write("<tr>" & vbCr)
   Response.Write("<td>" & vbCr)
	       Response.Write("<select id=""selFeedstuff_"&f&""" name=""selFeedstuff_"&f&""" onchange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';checkForNewLine("&f&");setFeedstuffString("&f&");"" style=""position:relative;top:-3Px;background-color: lightyellow;"">" & vbCr)
		   Response.write("  <option value=""-1"">"&Str.item("strSELECT_FEEDSTUFF")&"</option>" & vbCr)
		   for nf = 0 to nFeedstuff-1
		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""">"&arrFeedstuff(nf,1)&"</option>" & vbCr)
		   next
		   Response.Write("</select>" & vbCr)
   Response.Write("<input type=""text"" name=""feedstuffAmount_"&f&""" id=""feedstuffAmount_"&f&""" style=""width: 35px;position:relative;top:-3Px;background-color: lightyellow;"" onchange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';this.value=this.value.replace(',','.');setFeedstuffString("&f&");"" value=""0""> "&Str.Item("strKgDM/animal/day")&"" & vbCr)
   Response.Write("<img style=""position:relative;top:3Px;"" src=""images/del.gif"" onclick=""delRow(this,"&f&");"" onMouseover=""mouseOverGif(this)""  onMouseout=""mouseOutGif(this)""/>&nbsp;&nbsp;" & vbCr)
   Response.Write("</td>" & vbCr)
   Response.Write("</tr>" & vbCr)
end if
   Response.Write("</tbody>" & vbCr)
   Response.Write("</table>" & vbCr)
      Response.Write("<input type=""hidden"" name=""hiddenFeedstuffList_"&f&"""  id=""hiddenFeedstuffList_"&f&""" value="""&strFeedplan&""">")
	  strFeedplan = ""
      Response.Write("<input type=""hidden"" name=""hiddenAmountList_"&f&"""  id=""hiddenAmountList_"&f&""" value="""&strAmount&""">")
      Response.Write("</td>" & vbCr)
      Response.Write("      <td class=""data"" align=""left"" style=""vertical-align: top;"">")   
      Response.Write("<select style=""width: 220px;"" name=""selectStableType1_"&i&""" onChange=""javascript:this.style.color='#ff0000';"">")   
      CreateSelectList_Language aStableType1ID,nStableType1,arrStableType1
      Response.Write("</select><br/><br/>")   
      response.write "<input type=""hidden"" id=""storageChanged1_"&i&""" name=""storageChanged1_"&i&""" value=""0""/>"
   set dictPigHerdManureStorageList = CreateObject("Scripting.Dictionary")
   nPigHerdManureStorageList = CallSP ("GetPigHerdManureStorageList " & Session("farmNumber")& "," & Session("scenarioNumber") & "," & arrPigHerd(i,dictPigHerd.Item("PigHerdID"))& ",1", arrPigHerdManureStorageList, dictPigHerdManureStorageList)
     set dictStorageType = CreateObject("Scripting.Dictionary")
     nStorageType = CallSP ("GetStorageTypesPrPigStable " & aPigTypeID & "," & aStableType1ID, arrStorageType, dictStorageType)
'response.write "nStorageType : "&nStorageType
     Response.Write("<input type=""hidden"" name=""nManureStorage1_"&i&"""  id=""nManureStorage1_"&i&""" value="""&nStorageType&"""/>")
      response.write "<b>"&Str.Item("strManureStorageType")&"</b><br/>"
	 for ms = 0 to ubound(arrStorageType)  
   set dictManureStorage = CreateObject("Scripting.Dictionary")
   nManureStorage = CallSP ("GetSelectListPigManureStorageType " & aPigTypeID & "," & aStableType1ID &","&arrStorageType(ms,0), arrManureStorage, dictManureStorage)
	  aPigHerdManureStorageID = arrManureStorage(0,0)
	if nPigHerdManureStorageList>0  then
		if ms<nPigHerdManureStorageList then
		 aPigHerdManureStorageID = arrPigHerdManureStorageList(ms,1)
		end if
	end if
'      response.write Str.Item("strManureStorageType")
      Response.Write("<select style=""width: 220px;"" name=""selectManureStorageType1_"&i&""" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';"">")   
      CreateSelectList_ aPigHerdManureStorageID,nManureStorage,arrManureStorage
      Response.Write("</select>")
	  if ms < ubound(arrStorageType) then response.write "<br/><br/>"
      set dictManureStorage = nothing
	 next   
      Response.Write("</td>" & vbCr)
       ' column 7 = delete buttons   
      Response.Write("      <td class=""data"" align=""left"">")   
      Response.Write("<button style=""width: 50px; height: 20px; font-size: 10px"" onClick=""javascript:if(confirm('"&Str.Item("strBekræft")&"')) {veil();document.forms.formPigHerd.action='PigHerdDelete.asp?PigHerdID="& arrPigHerd(i,dictPigHerd.item("PigHerdID"))&"';document.forms.formPigHerd.submit();}"">"&Str.Item("strDelete")&"</button>")
      Response.Write("</td>" & vbCr)   
      Response.Write("    </tr>" & vbCr)
   Response.Write("    <tr>" & vbCr)

   Response.Write("      <td class=""data"" colspan=""7""><hr/></td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
   set dictStableType = Nothing
  end if
  set dictPigHerdFeedplanList = nothing
  set dictPigHerdManureStorageList = nothing
  strFeedplan = ""
  strAmount = ""
  next
'    row for buttons
      Response.Write("    <tr>" & vbCr)
   Response.Write("      <td class=""data"" colspan=""7"">")   
   Response.Write("&nbsp;&nbsp;<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:veil();document.forms.formPigHerd.action='PigHerdNew.asp';document.forms.formPigHerd.submit();"">"&Str.Item("strNEW")&"</button>")
   Response.Write("</td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)

   
   Response.Write("    <tr>" & vbCr)

   Response.Write("      <td class=""data"" colspan=""8""><hr/></td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <td class=""data"" colspan=""7"">")   
   Response.Write("&nbsp;&nbsp;<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:veil();document.forms.formPigHerd.action='PigHerdUpdate.asp';document.forms.formPigHerd.submit();"">"&Str.Item("strSAVE")&"</button>")
   Response.Write("<input type=""hidden"" name=""hiddenNPigHerd"" value="""&nPigHerd&"""/>" & vbCr)
   Response.Write("<input type=""hidden"" name=""hiddenPigType"" id=""hiddenPigType"" value=""2""/>" & vbCr)
   Response.Write("</td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
   
   Response.Write("  </table>" & vbCr) 
   Response.Write("</form>" & vbCr)
   CreateUpdateFormPig
   if nPigHerd > 0 then
'      CreatePigHerdTotal
   end if
   CloseFarmNDB
   set dictPigType = nothing
time13=now
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
sub DefineFeedstuffListJScript_8()
%>
<script language="javascript">

   function checkForNewLine(a)
   {
   	var oTBody = document.getElementById('tbodyFeedstuff_'+a+'')//;alert('tbodyFeedstuff_'+i+'');
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
 
   function getFeedstuff(a){
   	var cFeedstuff = document.getElementsByName('selFeedstuff_'+a+'');
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

   function getAmount(a){
   	var cFeedstuff = document.getElementsByName('feedstuffAmount_'+a+'');
   	var strFeedstuff = "";
   	for (var i = 0; i < cFeedstuff.length; i++)
   	{
   		if (parseInt(cFeedstuff[i].value,10) >= 0)
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
