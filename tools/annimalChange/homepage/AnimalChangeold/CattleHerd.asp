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
   dim nFeedStuff, arrFeedStuff, arrFeedStuffWithGrazing, aFeedStuffID, dictFeedStuff, nf, aFeedstuffAmount, g
   dim nCattleHerdFeedplanList, arrCattleHerdFeedplanList, aCattleHerdFeedplanID, dictCattleHerdFeedplanList, fp, strFeedplan, strAmount
   dim nProducedFeedStuff, arrGrazingFeedStuff, aGrazingFeedStuffID, dictGrazingFeedStuff, gf, pf
   dim nGrazingResidueFeedStuff, arrGrazingResidueFeedStuff, aGrazingResidueFeedStuffID, dictGrazingResidueFeedStuff
   dim arrParameter
   dim nStorageType, arrStorageType, aStorageTypeID, dictStorageType
   dim nManureStorage, arrManureStorage, aManureStorageID, dictManureStorage, ms
   dim nCattleHerdManuReStorageList, arrCattleHerdManureStorageList, aCattleHerdManureStorageID, dictCattleHerdManureStorageList, message
time10=now
   message= request("message")
'   response.write message    
   InitFarmNDB
   CreateLanguageDictionary
   
   
   set dictCattleHerd = CreateObject("Scripting.Dictionary")
   nCattleHerd = CallSP ("GetCattleHerdList " & Session("farmNumber")& "," & Session("scenarioNumber") , arrCattleHerd, dictCattleHerd)
   
'   nCattleHerd = GetCattleHerdList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrCattleHerd)
    set dictCattleType = CreateObject("Scripting.Dictionary")
   nCattleType = CallSP ("GetSelectListCattleType "& Session("AgroEcologicalZone"), arrCattleType, dictCattleType)
   set dictFeedStuff = CreateObject("Scripting.Dictionary")
   nFeedStuff = CallSP ("GetFeedStuffList" , arrFeedStuff, dictFeedStuff)
   
   set dictGrazingFeedStuff = CreateObject("Scripting.Dictionary")
   nProducedFeedStuff = CallSP ("GetProducedFeedstuffList " & Session("farmNumber")& "," & Session("scenarioNumber") , arrGrazingFeedStuff, dictGrazingFeedStuff)
   
   set dictGrazingResidueFeedStuff = CreateObject("Scripting.Dictionary")
   nGrazingResidueFeedStuff = CallSP ("GetProducedResidueGrazed " & Session("farmNumber")& "," & Session("scenarioNumber") , arrGrazingResidueFeedStuff, dictGrazingResidueFeedStuff)

 
' if nGrazingFeedStuff > 0 then
' redim arrFeedStuffWithGrazing(nFeedStuff+nGrazingFeedStuff,1)
' g=0
' for nf = 0 to nFeedStuff-1
'  for gf=0 to nGrazingFeedStuff-1
'    if cint(arrFeedstuff(nf,0))=cint(arrGrazingFeedStuff(gf,0)) then
'	arrFeedStuffWithGrazing(g,0)=arrFeedstuff(nf,0)
'	arrFeedStuffWithGrazing(g,1)=cstr(arrFeedstuff(nf,1))
'	g=g+1
'	arrFeedStuffWithGrazing(g,0)=cint(arrGrazingFeedStuff(gf,0)) + 1000
'	arrFeedStuffWithGrazing(g,1)=cstr(arrFeedstuff(nf,1))&" grazed"
'	else
'	if g < ubound(arrFeedStuffWithGrazing) and arrFeedStuffWithGrazing(g,0) = "" then
'	arrFeedStuffWithGrazing(g,0)=arrFeedstuff(nf,0)
'	arrFeedStuffWithGrazing(g,1)=cstr(arrFeedstuff(nf,1))
'	end if
'	end if
'  next
'	g=g+1
' next

' arrFeedstuff = arrFeedStuffWithGrazing
' nFeedStuff=g
' else
'   redim arrParameter(1)
'   arrParameter(0) = Session("farmNumber")
'   arrParameter(1) = Session("scenarioNumber")
'   nGrazingFeedStuff = CallSPReturn("DeleteGrazingFeedstuff" ,arrParameter)
' end if
   DefineChangeTextScript
   DefineFeedstuffListJScript_18
   Response.Write("<script language=""javascript"">window.parent.frames[1].location.reload()</script>" & vbCr)
   
   Response.Write("<form name=""formCattleHerd"" action=""CattleHerdUpdate.asp"" method=""post"" id=""dynamichide"" style=""display: block;"">" & vbCr)
   Response.Write("  <table class=""data"" border=""0"" style=""width: 1800px;"">" & vbCr)
   ' column headings
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th align=""left"" style=""width: 100px;"">"&Str.Item("strHusdyrhold")&"</th>" & vbCr)
   Response.Write("      <th align=""left"" style=""width: 100px;"">"&Str.Item("strANANIMAL")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strANANIMAL")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">"&Str.Item("strMAXFRAC")&"<br>"&Str.Item("strGRAZING")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strMAXFRAC")&"<br>&nbsp;&nbsp;"&Str.Item("strGRAZING")&"</th>" & vbCr)
   Response.Write("      <th align=""left"" style=""width: 270px;""></th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strPRODLEVEL")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">"&Str.Item("strEFFICIENCY")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strEFFICIENCY")&"</th>" & vbCr)
   Response.Write("      <th align=""left"" style=""width: 680px;"">"&Str.Item("strFeedstuffPlan")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strHOUSETYPE")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strManureStorageType")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   
   for i = 0 to nCattleHerd-1
   set dictCattleHerdFeedplanList = CreateObject("Scripting.Dictionary")
   set dictCattleHerdManureStorageList = CreateObject("Scripting.Dictionary")
   nCattleHerdFeedplanList = CallSP ("GetCattleHerdFeedplanList " & Session("farmNumber")& "," & Session("scenarioNumber") & "," & arrCattleHerd(i,dictCattleHerd.Item("CattleHerdID")), arrCattleHerdFeedplanList, dictCattleHerdFeedplanList)
'response.write "nCattleHerdFeedplanList: "&nCattleHerdFeedplanList
'response.write " Ubound(arrCattleHerdFeedplanList,1): "&ubound(arrCattleHerdFeedplanList,1)
   nCattleHerdManureStorageList = CallSP ("GetCattleHerdManureStorageList " & Session("farmNumber")& "," & Session("scenarioNumber") & "," & arrCattleHerd(i,dictCattleHerd.Item("CattleHerdID")), arrCattleHerdManureStorageList, dictCattleHerdManureStorageList)
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
      Response.Write("      <td class=""data"" align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputAnimalYear_"&i&""" value="""&arrCattleHerd(i,dictCattleHerd.Item("AnimalYear"))&""" maxlength=""5"" size=""2"" onChange=""document.forms.formCattleHerd.action='CattleHerdUpdate.asp';document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';"" onkeydown=""return false();"" onkeypress=""return false();"" /></td>" & vbCr)
      ' column 3 = grazing
      Response.Write("      <input type=""hidden"" name=""inputGrazing_"&i&""" value="""&arrCattleHerd(i,dictCattleHerd.Item("Grazing"))&"""/>" & vbCr)
      ' column 4 = production level
  	  if aCattleTypeID=7 then
        Response.Write("      <td class=""data"" align=""left""><input type=""hidden"" name=""inputProductionLevel_"&i&""" id=""inputProductionLevel_"&i&""" value="""&arrCattleHerd(i,dictCattleHerd.Item("ProductionLevel"))&"""/>&nbsp;<input type=""hidden"" id=""ProductionLevelUnit_"&i&""" name=""ProductionLevelUnit_"&i&""" value="""&Trim(Str.Item("str"&arrCattleHerd(i,dictCattleHerd.Item("ProductionLevelUnit"))))&"""></input>&nbsp;&nbsp;<input class="""" type=""hidden"" name=""inputProductionLevel2_"&i&""" id=""inputProductionLevel2_"&i&""" value="""&arrCattleHerd(i,dictCattleHerd.Item("ProductionLevel2"))&"""/>&nbsp;<input class=""show"" type=""hidden"" id=""ProductionLevelUnit2_"&i&""" name=""ProductionLevelUnit2_"&i&""" value="""&Trim(Str.Item("strkg kalv"))&"""></input></td>" & vbCr)
			else	
        Response.Write("      <td class=""data"" align=""left""><input type=""hidden"" name=""inputProductionLevel_"&i&""" id=""inputProductionLevel_"&i&""" value="""&arrCattleHerd(i,dictCattleHerd.Item("ProductionLevel"))&"""/>&nbsp;<input type=""hidden"" id=""ProductionLevelUnit_"&i&""" name=""ProductionLevelUnit_"&i&""" value="""&Trim(Str.Item("str"&arrCattleHerd(i,dictCattleHerd.Item("ProductionLevelUnit"))))&"""></input>&nbsp;&nbsp;<input class=""hide"" type=""hidden"" name=""inputProductionLevel2_"&i&""" id=""inputProductionLevel2_"&i&""" value="""&arrCattleHerd(i,dictCattleHerd.Item("ProductionLevel2"))&"""/>&nbsp;<input class=""hide"" type=""hidden"" id=""ProductionLevelUnit2_"&i&""" name=""ProductionLevelUnit2_"&i&""" value="""&Trim(Str.Item("strkg kalv"))&"""></input></td>" & vbCr)
      end if
      ' column 5 = effectivity   
'      Response.Write("      <td class=""data"" align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputEffectivity_"&i&""" value="""&arrCattleHerd(i,dictCattleHerd.Item("Effectivity"))&""" maxlength=""3"" size=""2"" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>&nbsp;%</td>" & vbCr)
      ' column 6 = stable type   
      Response.Write("<td>" & vbCr)
   Response.Write("<table border=""0"" cellspacing=""0"" width=""100%"">" & vbCr)
   Response.Write("<tbody id=""tbodyFeedstuff_"&i&""" name=""tbodyFeedstuff_"&i&""">" & vbCr)
if nCattleHerdFeedplanList=0 then
pf=0
   Response.Write("<tr>" & vbCr)
   Response.Write("<td>" & vbCr)
	       Response.Write("<select id=""selFeedstuff_"&i&""" name=""selFeedstuff_"&i&""" onchange=""checkForNewLine("&i&");setFeedstuffString("&i&");"" style=""position:relative;top:-3Px;background-color: lightyellow;"">" & vbCr)
		   Response.write("  <option value=""-1"">"&Str.item("strSELECT_FEEDSTUFF")&"</option>" & vbCr)
		   for nf = 0 to nFeedstuff-1
		      if nProducedFeedstuff <> 0 then
			     if Cint(arrGrazingFeedStuff(pf,0)) = Cint(arrFeedstuff(nf,0)) then
		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""" class=""back"">"&arrFeedstuff(nf,1)&"</option>" & vbCr)
				 if pf<nProducedFeedStuff-1 then pf=pf+1
				 else
			       Response.write("  <option value="""&arrFeedstuff(nf,0)&""">"&arrFeedstuff(nf,1)&"</option>" & vbCr)
			     end if
			  else
		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""">"&arrFeedstuff(nf,1)&"</option>" & vbCr)
			  end if
'		   if ubound(arrCattleHerdFeedplanList)<>-1 then 
'		     if nf < nCattleHerdFeedplanList then
'			  aFeedstuffID = arrCattleHerdFeedplanList(nf,0)
'			  aFeedstuffAmount = replace(arrCattleHerdFeedplanList(nf,1),",",".")
'			 end if
'		   else
'		     aFeedstuffID = -1
'			 aFeedstuffAmount = 0
'		   end if
'		      if aFeedstuffID = Cstr(arrFeedstuff(nf,0)) then
'			     if Cint(arrGrazingFeedStuff(pf,0)) = Cint(arrFeedstuff(nf,0)) then
'		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""" class=""back"" selected>"&arrFeedstuff(nf,1)&"</option>" & vbCr)
'				 if pf<nProducedFeedStuff-1 then pf=pf+1
'			     else
'		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""" selected>"&arrFeedstuff(nf,1)&"</option>" & vbCr)
'				 end if
'		      else
'			     if Cint(arrGrazingFeedStuff(pf,0)) = Cint(arrFeedstuff(nf,0)) then
'		         Response.write("  <option value="""&arrFeedstuff(nf,0)&"""  class=""back"">"&arrFeedstuff(nf,1)&"</option>" & vbCr)
'				 if pf<nProducedFeedStuff-1 then pf=pf+1
'			     else
'		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""">"&arrFeedstuff(nf,1)&"</option>" & vbCr)
'				 end if
'		      end if
		   next
		   Response.Write("</select>" & vbCr)
   Response.Write("<input id=""feedstuffAmount_"&i&""" name=""feedstuffAmount_"&i&""" type=""text"" style=""width: 35px;position:relative;top:-3Px;background-color: lightyellow;"" value="""&aFeedstuffAmount&""" onchange=""this.value=this.value.replace(',','.');setFeedstuffString("&i&");""/> "&Str.Item("strKgDM/animal/day")&"" & vbCr)
   Response.Write("<img style=""position:relative;top:3Px;"" src=""images/del.gif"" onclick=""delRow(this,"&i&");"" onMouseover=""mouseOverGif(this)""  onMouseout=""mouseOutGif(this)""/>&nbsp;&nbsp;" & vbCr)
   Response.Write("</td>" & vbCr)
'      Response.Write("      <input type=""hidden"" name=""inputGrazing_"&i&""" value="""&arrCattleHerd(i,dictCattleHerd.Item("Grazing"))&"""/>" & vbCr)
   Response.Write("</tr>" & vbCr)
else
for fp = 0 to nCattleHerdFeedplanList-1 
pf=0 
   Response.Write("<tr>" & vbCr)
   Response.Write("<td>" & vbCr)
	       Response.Write("<select id=""selFeedstuff_"&i&""" name=""selFeedstuff_"&i&""" onchange=""checkForNewLine("&i&");setFeedstuffString("&i&");"" style=""position:relative;top:-3Px;background-color: white;"">" & vbCr)
		   Response.write("  <option value=""-1"">"&Str.item("strSELECT_FEEDSTUFF")&"</option>" & vbCr)
		   for nf = 0 to nFeedstuff-1
		   if ubound(arrCattleHerdFeedplanList)<>-1 then 
		     if fp < nCattleHerdFeedplanList then
			  aFeedstuffID = arrCattleHerdFeedplanList(fp,0)
			  aFeedstuffAmount = replace(arrCattleHerdFeedplanList(fp,1),",",".")
			 end if
		   else
		     aFeedstuffID = -1
			 aFeedstuffAmount = 0
		   end if
		      if aFeedstuffID = Cstr(arrFeedstuff(nf,0)) then
		      if nProducedFeedstuff <> 0 then
			     if Cint(arrGrazingFeedStuff(pf,0)) = Cint(arrFeedstuff(nf,0)) then
		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""" class=""back"" selected>"&arrFeedstuff(nf,1)&"</option>" & vbCr)
				 if pf<nProducedFeedStuff-1 then pf=pf+1
				 else
			       Response.write("  <option value="""&arrFeedstuff(nf,0)&""" selected>"&arrFeedstuff(nf,1)&"</option>" & vbCr)
				 end if
			   else
		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""" selected>"&arrFeedstuff(nf,1)&"</option>" & vbCr)
			   end if
		      else
		      if nProducedFeedstuff <> 0 then
			     if Cint(arrGrazingFeedStuff(pf,0)) = Cint(arrFeedstuff(nf,0)) then
		         Response.write("  <option value="""&arrFeedstuff(nf,0)&"""  class=""back"">"&arrFeedstuff(nf,1)&"</option>" & vbCr)
				 if pf<nProducedFeedStuff-1 then pf=pf+1
				 else
			       Response.write("  <option value="""&arrFeedstuff(nf,0)&""">"&arrFeedstuff(nf,1)&"</option>" & vbCr)
				 end if
			   else
		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""">"&arrFeedstuff(nf,1)&"</option>" & vbCr)
			   end if
		      end if
		   next
		   Response.Write("</select>" & vbCr)
   Response.Write("<input id=""feedstuffAmount_"&i&""" name=""feedstuffAmount_"&i&""" type=""text"" style=""width: 35px;position:relative;top:-3Px;background-color: white;"" value="""&aFeedstuffAmount&""" onchange=""this.value=this.value.replace(',','.');setFeedstuffString("&i&");""/> "&Str.Item("strKgDM/animal/day")&"" & vbCr)
   Response.Write("<img style=""position:relative;top:3Px;"" src=""images/del.gif"" onclick=""delRow(this,"&i&");"" onMouseover=""mouseOverGif(this)""  onMouseout=""mouseOutGif(this)""/>&nbsp;&nbsp;" & vbCr)
   Response.Write("</td>" & vbCr)
   Response.Write("</tr>" & vbCr)
next
pf=0
   Response.Write("<tr>" & vbCr)
   Response.Write("<td>" & vbCr)
	       Response.Write("<select id=""selFeedstuff_"&i&""" name=""selFeedstuff_"&i&""" onchange=""checkForNewLine("&i&");setFeedstuffString("&i&");"" style=""position:relative;top:-3Px;background-color: lightyellow;"">" & vbCr)
		   Response.write("  <option value=""-1"">"&Str.item("strSELECT_FEEDSTUFF")&"</option>" & vbCr)
		   for nf = 0 to nFeedstuff-1
		        if nProducedFeedstuff <> 0 then
			     if Cint(arrGrazingFeedStuff(pf,0)) = Cint(arrFeedstuff(nf,0)) then
		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""" class=""back"">"&arrFeedstuff(nf,1)&"</option>" & vbCr)
				 if pf<nProducedFeedStuff-1 then pf=pf+1
				 else
			       Response.write("  <option value="""&arrFeedstuff(nf,0)&""">"&arrFeedstuff(nf,1)&"</option>" & vbCr)
				 end if
			    else
		         Response.write("  <option value="""&arrFeedstuff(nf,0)&""">"&arrFeedstuff(nf,1)&"</option>" & vbCr)
				end if
		   next
		   Response.Write("</select>" & vbCr)
   Response.Write("<input id=""feedstuffAmount_"&i&""" name=""feedstuffAmount_"&i&""" type=""text"" style=""width: 35px;position:relative;top:-3Px;background-color: lightyellow;"" value=""0"" onchange=""this.value=this.value.replace(',','.');setFeedstuffString("&i&");""/> "&Str.Item("strKgDM/animal/day")&"" & vbCr)
   Response.Write("<img style=""position:relative;top:3Px;"" src=""images/del.gif"" onclick=""delRow(this,"&i&");"" onMouseover=""mouseOverGif(this)""  onMouseout=""mouseOutGif(this)""/>&nbsp;&nbsp;" & vbCr)
   Response.Write("</td>" & vbCr)
   Response.Write("</tr>" & vbCr)
end if
   
   
   Response.Write("</tbody>" & vbCr)
   Response.Write("</table>" & vbCr)
if nCattleHerdFeedplanList>0 then 
for fp = 0 to nCattleHerdFeedplanList-1
  strFeedplan = strFeedplan & arrCattleHerdFeedplanList(fp,0)& ","
  strAmount = strAmount & replace(arrCattleHerdFeedplanList(fp,1),",",".")& ","
next
else
  strFeedplan = ""
  strAmount = ""
end if  
      Response.Write("<input type=""hidden"" name=""hiddenFeedstuffList_"&i&"""  id=""hiddenFeedstuffList_"&i&""" value="""&strFeedplan&"""/>")
      Response.Write("<input type=""hidden"" name=""hiddenAmountList_"&i&"""  id=""hiddenAmountList_"&i&""" value="""&strAmount&"""/>")
      Response.Write("</td>" & vbCr)
  strFeedplan = ""
  strAmount = ""

      Response.Write("      <td class=""data"" align=""left"">")   
      Response.Write("&nbsp;&nbsp;<select name=""selectStableType_"&i&""" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';document.forms.formCattleHerd.action='CattleHerdUpdate.asp';document.getElementById('storageChanged_"&i&"').value='1';document.forms.formCattleHerd.submit();"">")   
      CreateSelectList_Language aStableTypeID,nStableType,arrStableType
      Response.Write("</select>")   
      Response.Write("</td>" & vbCr)
     set dictStorageType = CreateObject("Scripting.Dictionary")
     nStorageType = CallSP ("GetStorageTypesPrStable " & aCattleTypeID & "," & aStableTypeID, arrStorageType, dictStorageType)
     Response.Write("<input type=""hidden"" name=""nManureStorage_"&i&"""  id=""nManureStorage_"&i&""" value="""&nStorageType&"""/>")
     Response.Write("      <td class=""data"" align=""left"">")
	 for ms = 0 to ubound(arrStorageType)  
   set dictManureStorage = CreateObject("Scripting.Dictionary")
   nManureStorage = CallSP ("GetSelectListManureStorageType " & aCattleTypeID & "," & aStableTypeID &","&arrStorageType(ms,0), arrManureStorage, dictManureStorage)
	if nCattleHerdManureStorageList>0  then
		if ms<nCattleHerdManureStorageList then
		 aCattleHerdManureStorageID = arrCattleHerdManureStorageList(ms,1)
		end if
	end if
      Response.Write("&nbsp;&nbsp;<select style=""width: 200px;"" name=""selectManureStorageType_"&i&""" onChange=""document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';"">")   
      CreateSelectList_ aCattleHerdManureStorageID,nManureStorage,arrManureStorage
      Response.Write("</select>")
	  if ms < ubound(arrStorageType) then response.write "<br/><br/>"
      set dictManureStorage = nothing
	 next   
      response.write "<input type=""hidden"" id=""storageChanged_"&i&""" name=""storageChanged_"&i&""" value=""0""/>"
      Response.Write("</td>" & vbCr)
      ' column 7 = delete buttons   
      Response.Write("      <td class=""data"" align=""left"">")   
      Response.Write("<button style=""width: 0px; height: 0px; font-size: 0px; border: 0px;"" onClick=""javascript:veil();document.forms.formCattleHerd.action='CattleHerdUpdate.asp';document.forms.formCattleHerd.submit();""></button><button style=""width: 50px; height: 20px; font-size: 10px"" onClick=""javascript:if(confirm('"&Str.Item("strBekræft")&"')) {veil();document.forms.formCattleHerd.action='CattleHerdDelete.asp?CattleHerdID="&arrCattleHerd(i,dictCattleHerd.Item("CattleHerdID"))&"';document.forms.formCattleHerd.submit();}"">"&Str.Item("strDelete")&"</button>")
      Response.Write("</td>" & vbCr)   
      Response.Write("    </tr>" & vbCr)
   Response.Write("    <tr>" & vbCr)
   ' row for buttons
   Response.Write("      <td class=""data"" colspan=""7""><hr/></td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
      set dictStableType = Nothing
	  set dictCattleHerdFeedplanList = nothing
	  set dictCattleHerdManureStorageList = nothing
   next
'   Response.Write("    <tr>" & vbCr)
'   ' row for buttons
'   Response.Write("      <td class=""data"" colspan=""7""><hr/></td>" & vbCr)   
'   Response.Write("    </tr>" & vbCr)
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <td class=""data"" colspan=""7"">")   
   Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:veil();document.forms.formCattleHerd.action='CattleHerdUpdate.asp';document.forms.formCattleHerd.submit();"">"&Str.Item("strSAVE")&"</button>")
   Response.Write("&nbsp;")   
   Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:veil();document.forms.formCattleHerd.action='CattleHerdNew.asp';document.forms.formCattleHerd.submit();"">"&Str.Item("strNEW")&"</button>")
   Response.Write("<input type=""hidden"" name=""hiddenNCattleHerd"" value="""&nCattleHerd&""">" & vbCr)
   Response.Write("&nbsp;&nbsp;&nbsp;"&message & vbCr)
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
   time11=now
end sub
'**************************************************************************************************
'sub CreateCattleHerdTotal()
'   dim p,t,c,f
'   dim nParameter, arrParameter, dictParameter
'   dim nCattleType, arrCattleType, aCattleTypeID, dictCattleType
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
'      aCattleTypeID = arrCattleType(t,dictCattleType.Item("CattleType"))
'    set dictCattleHerdTotal = CreateObject("Scripting.Dictionary")
'      nCattleHerdTotal = CallSP ("GetCattleHerdTotalList " & Session("farmNumber")& "," & Session("scenarioNumber") & "," & aCattleTypeID , arrCattleHerdTotal, dictCattleHerdTotal)
''      nCattleHerdTotal = GetCattleHerdTotalList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),aCattleTypeID,arrCattleHerdTotal)
'      Response.Write("    <tr>" & vbCr)
'      Response.Write("      <td nowrap>&nbsp;&nbsp;"&arrCattleType(t,dictCattleType.Item("CattleType"))&" - "&Str.Item("str"&arrCattleType(t,dictCattleType.Item("CattleTypeName")))&"</td>" & vbCr)  
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
'**************************************************************************************************
sub CreateCattleHerdTotal()
   dim p,t,c,f
   dim nParameter, arrParameter, dictParameter
   dim nCattleHerd, arrCattleHerd, aCattleHerdID, dictCattleHerd
   dim nCattleHerdTotal, arrCattleHerdTotal, dictCattleHerdTotal
   dim nFarmTotal, arrFarmTotal, dictFarmTotal
   dim nCattleType, arrCattleType, aCattleTypeID, dictCattleType
   dim nGrazingBalance, arrGrazingBalance, aGrazingProductID, dictGrazingBalance

   set dictParameter = CreateObject("Scripting.Dictionary")
   nParameter = CallSP ("GetParameterList 5", arrParameter, dictParameter)

   set dictGrazingBalance = CreateObject("Scripting.Dictionary")
   nGrazingBalance =  CallSP ("GetGrazingBalance " & Session("farmNumber")& "," & Session("scenarioNumber") , arrGrazingBalance, dictGrazingBalance)
'   nParameter  = GetParameterList(3,arrParameter)

'   nCattleType = GetCattleHerdCattleTypeList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrCattleType)
   set dictCattleHerd = CreateObject("Scripting.Dictionary")
   nCattleHerd = CallSP ("GetCattleHerdList " & Session("farmNumber")& "," & Session("scenarioNumber") , arrCattleHerd, dictCattleHerd)

   set dictCattleType = CreateObject("Scripting.Dictionary")
   nCattleType = CallSP ("GetCattleHerdCattleTypeList " & Session("farmNumber")& "," & Session("scenarioNumber"), arrCattleType, dictCattleType)

'   nFarmTotal  = GetFarmTotalList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),3,arrFarmTotal)
'  set dictFarmTotal = CreateObject("Scripting.Dictionary")
'   nFarmTotal = CallSP ("GetFarmTotalList " & Session("farmNumber")& "," & Session("scenarioNumber") & ",5" , arrFarmTotal, dictFarmTotal)

   ' rows for totals
   Response.Write("  <br/>" & vbCr) 
   Response.Write("  <h4>&nbsp;"&Str.Item("strbehovprodgødning")&"</h4>" & vbCr)
         
   Response.Write("  <table class=""data"" border=""0"" cellspacing=""15"" cellpadding=""0"">" & vbCr)
   ' row for header
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <th class=""data"" align=""left"">&nbsp;&nbsp;"&Str.Item("strHusdyrhold")&"</th>" & vbCr) 
   for p = 0 to nParameter-1 
      Response.Write("      <th class=""data"" align=""left"">"& arrParameter(p,dictParameter.Item("Description"))&"<br/>("& arrParameter(p,dictParameter.Item("ParameterUnit"))&")</th>" & vbCr)
   next  
   Response.Write("    </tr>" & vbCr)
   ' rows for feeding requirements
 
   for t = 0 to nCattleHerd-1 
      aCattleHerdID = arrCattleHerd(t,dictCattleHerd.Item("CattleHerdID"))
    set dictCattleHerdTotal = CreateObject("Scripting.Dictionary")
      nCattleHerdTotal = CallSP ("GetCattleHerdCalculationList " & Session("farmNumber")& "," & Session("scenarioNumber") & "," & aCattleHerdID , arrCattleHerdTotal, dictCattleHerdTotal)
'      nCattleHerdTotal = GetCattleHerdTotalList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),aCattleTypeID,arrCattleHerdTotal)
if nCattleHerdTotal<>0 then
      Response.Write("    <tr>" & vbCr)
      Response.Write("      <td>"&arrCattleHerd(t,dictCattleHerd.Item("CattleType"))&" - "&Str.Item("str"&arrCattleHerd(t,dictCattleHerd.Item("CattleTypeName")))&"</td>" & vbCr)  
'      Response.Write("      <td nowrap>&nbsp;&nbsp;"&arrCattleHerd(t,dictCattleHerd.Item("CattleType"))&" - "&Str.Item("str"&arrCattleHerd(t,dictCattleHerd.Item("CattleTypeName")))&"</td>" & vbCr)  
      for c = 0 to nCattleHerdTotal-1 
'         Response.Write("      <td class=""data"" align=""right"">"&p&"</td>" & vbCr)
         if arrCattleHerdTotal(c,dictCattleHerdTotal.Item("Value")) <> 0 then
            Response.Write("      <td align=""right"">"&FormatNumber(arrCattleHerdTotal(c,dictCattleHerdTotal.Item("Value")),1,-1,0,0)&"</td>" & vbCr)
         else
            Response.Write("      <td align=""right"">&nbsp;</td>" & vbCr)
         end if
      next  
      Response.Write("    </tr>" & vbCr)
end if
   set dictCattleHerdTotal = Nothing
   next

   Response.Write("    <tr>" & vbCr)
   Response.Write("      <td colspan="""&nParameter+1&"""><hr/></td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
   ' row for total
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <th class=""data"" align=""left"" nowrap>&nbsp;&nbsp;"&Str.Item("strGrazingBalance")&"<br/>&nbsp;&nbsp;(Kg DM)</th><th class=""data"" align=""right"" nowrap>"&Str.Item("strFIELDPROD")&"&nbsp;&nbsp;</th><th class=""data"" align=""right"" nowrap>"&Str.Item("strHOUSINGUSE")&"&nbsp;&nbsp;</th><th class=""data"" align=""right"" nowrap>"&Str.Item("strBALANCE")&"&nbsp;&nbsp;</th><th class=""data"" align=""right"" nowrap>"&Str.Item("strBALANCE")&" %&nbsp;&nbsp;</th>" & vbCr)
   for f = 0 to nGrazingBalance-1 
      Response.Write("      <tr><td class=""data"" align=""right"">"&arrGrazingBalance(f,dictGrazingBalance.Item("GrazingProductID"))&"</td>" & vbCr)   
      Response.Write("      <td class=""data"" align=""right"">"&arrGrazingBalance(f,dictGrazingBalance.Item("SumProduction"))&"</td>" & vbCr)   
      Response.Write("      <td class=""data"" align=""right"">"&arrGrazingBalance(f,dictGrazingBalance.Item("SumFeeding"))&"</td>" & vbCr)   
      Response.Write("      <td class=""data"" align=""right"">"&Cdbl(arrGrazingBalance(f,dictGrazingBalance.Item("SumProduction"))-arrGrazingBalance(f,dictGrazingBalance.Item("SumFeeding")))&"</td>" & vbCr) 
	  if Cdbl(arrGrazingBalance(f,dictGrazingBalance.Item("SumProduction")))>0 then  
      Response.Write("      <td class=""data"" align=""right"">"&FormatNumber(Cdbl(arrGrazingBalance(f,dictGrazingBalance.Item("SumProduction"))-arrGrazingBalance(f,dictGrazingBalance.Item("SumFeeding")))*100/Cdbl(arrGrazingBalance(f,dictGrazingBalance.Item("SumProduction"))),2)&"</td></tr>" & vbCr)   
      end if
   next 
'nGrazingBalance, arrGrazingBalance, aGrazingProductID, dictGrazingBalance     
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
	 if Cint(arrCattleType(j,dictCattleType.Item("CattleTypeID")))=7 or Cint(arrCattleType(j,dictCattleType.Item("CattleTypeID")))=11 then
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
sub DefineFeedstuffListJScript_18()
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
   		if (parseFloat(cFeedstuff[i].value,10) > 0)
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
sub CreateUpdateFormCattle()
   Response.Write("<form name=""update"" action=""javascript:document.forms.formCattleHerd.action='CattleHerdUpdate.asp';document.forms.formCattleHerd.submit();"" method=""post"">" & vbCr)
   Response.Write("<input type=""hidden"" name=""saveUpdates"" value=""false"">" & vbCr)
   Response.Write("</form>" & vbCr)
end sub
'**************************************************************************************************
%>
