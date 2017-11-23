<%
'**************************************************************************************************
' Author: Margit Jorgensen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
sub CreateTabRotation()
   dim r,f,aSecondCrop, aPreCropSecondCrop
   dim nRotation, arrRotation, aRotationID, dictRotation
   dim nStrawUse, arrStrawUse, aStrawUseID, dictStrawUse
   dim nFieldPlanRotation, arrFieldPlanRotation, dictFieldPlanRotation
   Dim nSecondCropYield, arrSecondCropYield, dictSecondCropYield
   Dim index, SecondCropFE, SecondCropNNeed
   dim vinterKornList,vinterRape,toAarsGraesList,udlaegList
   dim hasStraw, hasSecondary
   
   vinterKornList = "3,4,5,23"
   vinterRape = "8"
   toAarsGraesList = "12,18,19,32,33,34"
   udlaegList = "12,13,14,18,19,29,32,33,34"
 time8=now  
   InitFarmNDB
   CreateLanguageDictionary
   Response.Write("<script language=""javascript"">window.parent.frames[1].location.reload()</script>" & vbCr)
   set dictRotation = CreateObject("Scripting.Dictionary")
   nRotation = CallSP("GetRotationList "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber")),arrRotation,dictRotation)
   set dictSecondCropYield = CreateObject("Scripting.Dictionary")
   nSecondCropYield = CallSP("GetSecondCropYield "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber")),arrSecondCropYield,dictSecondCropYield)
   index=0
   Response.Write("  <table class=""data"" border=""0"" cellpadding=""5"" cellspacing=""0"">" & vbCr)
   Response.Write("<form name=""formRotation"" action=""RotationUpdate.asp"" method=""post"">" & vbCr)
   for r = 0 to nRotation-1
      aRotationID = arrRotation(r,dictRotation.Item("RotationID"))
      set dictFieldPlanRotation = CreateObject("Scripting.Dictionary")
      nFieldPlanRotation = CallSP("GetFieldPlanRotationList "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber"))&","&aRotationID,arrFieldPlanRotation,dictFieldPlanRotation)
      if nFieldPlanRotation <> 0 then
	      Response.Write("    <tr>" & vbCr)
	      Response.Write("      <td align=""left"" colspan=""4""><h4>"&arrRotation(r,dictRotation.Item("RotationName"))&"</td>" & vbCr)
	      Response.Write("    </tr>" & vbCr)
'response.write 	"nFieldPlanRotation: "&nFieldPlanRotation	  
	      ' column headings
	      Response.Write("    <tr>" & vbCr)   
		  Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strCROP")&"</th>" & vbCr)
		  Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
	      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("str1product")&"<br/>"&Str.Item("strgrazed")&"&nbsp;&nbsp;"&Str.Item("strYIELD")&"<br/><br/>&nbsp;&nbsp;Kg DM/Ha</th>" & vbCr)
	      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("str2product")&"<br/>"&Str.Item("strgrazed")&"&nbsp;&nbsp;"&Str.Item("strYIELD")&"<br/><br/>&nbsp;&nbsp;Kg DM/Ha</th>" & vbCr)
'	      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strMAXFRAC")&"<br>&nbsp;&nbsp;"&Str.Item("strGRAZING")&"<br><br>&nbsp;&nbsp;(%)</th>" & vbCr)
	      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strSTRAW")&"<br>&nbsp;&nbsp;"&Str.Item("strUSE")&"</th>" & vbCr)
	      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("str1product")&"<br/>"&Str.Item("strPotential")&"&nbsp;&nbsp;"&Str.Item("strYIELD")&"<br/><br/>&nbsp;&nbsp;Kg DM/Ha</th>" & vbCr)
	      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("str2product")&"<br/>"&Str.Item("strPotential")&"&nbsp;&nbsp;"&Str.Item("strYIELD")&"<br/><br/>&nbsp;&nbsp;Kg DM/Ha</th>" & vbCr)
	      Response.Write("    </tr>" & vbCr)
	
	      for f = 0 to nFieldPlanRotation-1
	         aStrawUseID = arrFieldPlanRotation(f,dictFieldPlanRotation("StrawUseType"))
			 aSecondCrop = arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID"))
			 aPreCropSecondCrop = arrFieldPlanRotation(f,dictFieldPlanRotation("PreCropSecondCrop"))
			 hasSecondary = arrFieldPlanRotation(f,dictFieldPlanRotation("StrawFeedStuffID"))
	         hasStraw =  cBool(arrFieldPlanRotation(f,dictFieldPlanRotation("UseStraw")))
			 If index <= UBound(arrSecondCropYield) then 
			   If CInt(arrSecondCropYield(index,dictSecondCropYield.Item("RotationID"))) = CInt(arrFieldPlanRotation(f,dictFieldPlanRotation("RotationID"))) And CInt(arrSecondCropYield(index,dictSecondCropYield.Item("FieldPlanRotationID"))) = CInt(arrFieldPlanRotation(f,dictFieldPlanRotation("FieldPlanRotationID"))) Then
                 SecondCropFE=CDbl(arrSecondCropYield(index,dictSecondCropYield.Item("SecondCrop_FE")))
                 SecondCropNNeed=CDbl(arrSecondCropYield(index,dictSecondCropYield.Item("SecondCropNNeed")))
			     index=index+1
               Else 
               SecondCropFE=0
               SecondCropNNeed=0
			   End If
			 Else 
               SecondCropFE=0
               SecondCropNNeed=0
			 End If 
	         Response.Write("    <tr>" & vbCr)   
			 Response.Write("<input type=""hidden"" name=""hiddenFieldPlanRotationID_"&aRotationID&"_"&f&""" value="""&arrFieldPlanRotation(f,dictFieldPlanRotation("FieldPlanRotationID"))&"""><input type=""hidden"" name=""hiddenPreviousCropID_"&aRotationID&"_"&f&""" value="""&(arrFieldPlanRotation(f,dictFieldPlanRotation("PreviousCrop")))&"""><input type=""hidden"" name=""hiddenPreCropOriginalID_"&aRotationID&"_"&f&""" value="""&Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("PreCropOriginalID")))&""">" & vbCr)
	         ' column 2 = Efterafgrøde PreCrop
			 Response.Write("<input type=""hidden"" name=""hiddenPreCrop_AfterCropID_"&aRotationID&"_"&f&""" value="""&arrFieldPlanRotation(f,dictFieldPlanRotation("PreCrop_AfterCrop"))&""">"& vbCr)'</td>" & vbCr)
	         ' column 2 = crop
	         Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Str.Item("str"&arrFieldPlanRotation(f,dictFieldPlanRotation("CropName"))))
			 Response.Write("<input type=""hidden"" name=""hiddenCropID_"&aRotationID&"_"&f&""" value="""&arrFieldPlanRotation(f,dictFieldPlanRotation("Crop"))&"""></td>" & vbCr)
	         ' column 2 = Efterafgrøde Crop
             response.write "</td>" & vbCr
			 Response.Write("<input type=""hidden"" name=""hiddenCrop_AfterCropID_"&aRotationID&"_"&f&""" value="""&arrFieldPlanRotation(f,dictFieldPlanRotation("Crop_AfterCrop"))&"""><input type=""hidden"" name=""hiddenPrePreCropID_"&aRotationID&"_"&f&""" value="""&arrFieldPlanRotation(f,dictFieldPlanRotation("PrePreCropID"))&""">" & vbCr)
	         ' column 15 = area
	         Response.Write("      <td align=""right"">&nbsp;&nbsp;<input type=""hidden"" name=""hiddenArea_"&aRotationID&"_"&f&""" value="""&arrFieldPlanRotation(f,dictFieldPlanRotation("Area"))&"""></td>" & vbCr)
	         ' column 23 to 25 = cernal-, straw-, secondcrop-yield
	         Response.Write("      <td align=""left"">" & vbCr)
			 if arrFieldPlanRotation(f,dictFieldPlanRotation("FeedStuffID"))<>0 then
				 if ((arrFieldPlanRotation(f,dictFieldPlanRotation("FeedStuffID"))>1000))  then
			         Response.Write("<input type=""text"" name=""mainYield_"&aRotationID&"_"&f&""" size=""4"" value="""&FormatNumber(arrFieldPlanRotation(f,dictFieldPlanRotation("MainYield")),0,-1,0,0)&""" onChange=""document.forms.formRotation.action='RotationUpdate.asp?RotationID="&aRotationID&"';document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';"" />" & vbCr)'&nbsp;&nbsp;&nbsp;&nbsp;<span id=""ME_1_"&aRotationID&"_"&f&""">"&FormatNumber((arrFieldPlanRotation(f,dictFieldPlanRotation("PercentDrymatter"))*arrFieldPlanRotation(f,dictFieldPlanRotation("DigestibleEnergyPrKgDrymatter")))/100,0,-1,0,0)&"</span>" & vbCr)
				end if
			 else
		         Response.Write("<input type=""hidden"" name=""mainYield_"&aRotationID&"_"&f&""" size=""4"" value="""&FormatNumber(arrFieldPlanRotation(f,dictFieldPlanRotation("MainYield")),0,-1,0,0)&""" onChange=""document.forms.formRotation.action='RotationUpdate.asp?RotationID="&aRotationID&"';document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';"" />" & vbCr)'&nbsp;&nbsp;&nbsp;&nbsp;<span id=""ME_1_"&aRotationID&"_"&f&""">"&FormatNumber((arrFieldPlanRotation(f,dictFieldPlanRotation("PercentDrymatter"))*arrFieldPlanRotation(f,dictFieldPlanRotation("DigestibleEnergyPrKgDrymatter")))/100,0,-1,0,0)&"</span>" & vbCr)
			 end if
	         Response.Write("      </td>" & vbCr)
	         Response.Write("      <td align=""left"">" & vbCr)
			 if arrFieldPlanRotation(f,dictFieldPlanRotation("StrawFeedStuffID"))<>0 then
             if ((arrFieldPlanRotation(f,dictFieldPlanRotation("StrawFeedStuffID"))>1000)) or (arrFieldPlanRotation(f,dictFieldPlanRotation("StrawUseType"))=7) or (arrFieldPlanRotation(f,dictFieldPlanRotation("StrawUseType"))=8) or (arrFieldPlanRotation(f,dictFieldPlanRotation("StrawUseType"))=9) then
			         Response.Write("<input type=""text"" name=""strawYield_"&aRotationID&"_"&f&""" size=""4"" value="""&FormatNumber(arrFieldPlanRotation(f,dictFieldPlanRotation("StrawYield")),0,-1,0,0)&""" onChange=""document.forms.formRotation.action='RotationUpdate.asp?RotationID="&aRotationID&"';document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>" & vbCr)'&nbsp;&nbsp;&nbsp;&nbsp;<span id=""ME_3_"&aRotationID&"_"&f&""">"&FormatNumber((arrFieldPlanRotation(f,dictFieldPlanRotation("PercentDrymatter")))/100,0,-1,0,0)&"</span>" & vbCr)
             end if
		     else
		         Response.Write("<input type=""hidden"" name=""strawYield_"&aRotationID&"_"&f&""" size=""4"" value="""&FormatNumber(arrFieldPlanRotation(f,dictFieldPlanRotation("StrawYield")),0,-1,0,0)&""" onChange=""document.forms.formRotation.action='RotationUpdate.asp?RotationID="&aRotationID&"';document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/>" & vbCr)'&nbsp;&nbsp;&nbsp;&nbsp;<span id=""ME_3_"&aRotationID&"_"&f&""">"&FormatNumber((arrFieldPlanRotation(f,dictFieldPlanRotation("PercentDrymatter")))/100,0,-1,0,0)&"</span>" & vbCr)
			 end if
	         Response.Write("      </td>" & vbCr)
	         ' column 21 = total yield
	         ' column 17 = grazing part
			  ' column 4 = UseGrazing
'			 if  Cbool(arrFieldPlanRotation(f,dictFieldPlanRotation("UseGrazing"))) = True then
'		         Response.Write("      <td align=""center"">&nbsp;&nbsp;<input type=""text"" name=""inputGrazingPart_"&aRotationID&"_"&f&""" value="""&arrFieldPlanRotation(f,dictFieldPlanRotation("GrazingPart"))&""" maxlength=""5"" size=""5"" onChange=""if (isFinite(this.value)&&this.value!='') {document.forms.formRotation.action='RotationUpdate.asp?RotationID="&aRotationID&"';document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';} else {this.value='0'}""/></td>" & vbCr)
'	         else
'			 end if
			 ' column 19 = straw use type
			  ' column 6 = UseStraw
	         if (arrFieldPlanRotation(f,dictFieldPlanRotation("UseStraw")) <> 0) and (arrFieldPlanRotation(f,dictFieldPlanRotation("StrawFeedStuffID"))<>0) then
					 
             set dictStrawUse = CreateObject("Scripting.Dictionary")
	           nStrawUse = CallSP ("GetSelectListStrawUseBoxType " & arrFieldPlanRotation(f,dictFieldPlanRotation("UseStraw")) , arrStrawUse, dictStrawUse)
				     Response.Write("      <td align=""left"">&nbsp;&nbsp;")
		         Response.Write("<select name=""selectStrawUseType_"&aRotationID&"_"&f&""" onChange=""document.forms.formRotation.action='RotationUpdate.asp?RotationID="&aRotationID&"';document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';"">")   
		         CreateSelectList_Language aStrawUseID,nStrawUse,arrStrawUse
			       set dictStrawUse = nothing
		         Response.Write("</select>")   
		         Response.Write("</td>" & vbCr)
			 else
		         Response.Write("      <td align=""left""><input type=""hidden"" name=""selectStrawUseType_"&aRotationID&"_"&f&""" value=""0""/>&nbsp;</td>")   
			 end if
		         Response.Write("      <input type=""hidden"" name=""SecondCrop_"&aRotationID&"_"&f&""" value="""&arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID"))&"""/><input type=""hidden"" name=""PreCropSecondCrop_"&aRotationID&"_"&f&""" value="""&arrFieldPlanRotation(f,dictFieldPlanRotation("PreCropSecondCrop"))&"""/>")   
			 if arrFieldPlanRotation(f,dictFieldPlanRotation("FeedStuffID"))<>0 then
		         Response.Write("      <td><input type=""text"" size=""4"" name=""inputGrazingPart_"&aRotationID&"_"&f&""" value="""&arrFieldPlanRotation(f,dictFieldPlanRotation("GrazingPart"))&"""/></td>" & vbCr)
			 else
		         Response.Write("      <td></td>" & vbCr)
			 end if
             if arrFieldPlanRotation(f,dictFieldPlanRotation("StrawFeedStuffID")) <> 0 then
		         Response.Write("      <td><input type=""text"" size=""4"" name=""checkboxOrganicFertilizer_"&aRotationID&"_"&f&""" value="""&arrFieldPlanRotation(f,dictFieldPlanRotation("OrganicFertilizer"))&"""/></td>" & vbCr)
			 end if
			 'end if
	         Response.Write(vbCr&"    </tr>" & vbCr)
	
	
	      next
time20=now			 
	
	      Response.Write("    <tr>" & vbCr)
	      Response.Write("      <td colspan=""9"">")   
	      Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:if (document.forms.update.saveUpdates.value=='true') {veil();document.forms.formRotation.action='RotationUpdate.asp?RotationID="&aRotationID&"';document.forms.formRotation.submit();}"">"&Str.Item("strSAVE")&"</button>")
	      Response.Write("<input type=""hidden"" name=""hiddenNFieldPlanRotation_"&aRotationID&""" value="""&nFieldPlanRotation&""">")
	      Response.Write("</td>" & vbCr)   
	      Response.Write("    </tr>" & vbCr)
	   else
'	     response.write ("<tr><td colspan=""10""><h3>"&Str.Item("strROTATION")&""&Str.Item("strCOLON")&""&aRotationID&" "&Str.Item("strCALCFAIL")&""&Str.Item("strCOLON")&"<br/>"&Str.Item("strREASON1")&"</h3></td></tr>" & vbCr)
	     response.write ("<tr><td colspan=""10""><h3>"&Str.Item("strROTATION")&""&Str.Item("strCOLON")&""&aRotationID&" "&Str.Item("strCALCFAIL")&"</h3></td></tr>" & vbCr)
response.write 	"nFieldPlanRotation: "&nFieldPlanRotation	  
	   end if
      Response.Write("    <tr>" & vbCr)
      Response.Write("      <td colspan=""10""><hr/></td>" & vbCr)
      Response.Write("    </tr>" & vbCr)
      set dictFieldPlanRotation = nothing
   next
   CloseFarmNDB
   set dictRotation = nothing
   
   Response.Write("</form>" & vbCr)
   Response.Write("  </table>" & vbCr)
   CreateUpdateFormRotation
   time9=now
end sub
'**************************************************************************************************
sub CreateUpdateFormRotation()
   Response.Write("<form name=""update"" action=""javascript:document.forms.formRotation.submit();"" method=""post"">" & vbCr)
   Response.Write("<input type=""hidden"" name=""saveUpdates"" value=""true"">" & vbCr)
   Response.Write("</form>" & vbCr)
end sub
'**************************************************************************************************
%>
