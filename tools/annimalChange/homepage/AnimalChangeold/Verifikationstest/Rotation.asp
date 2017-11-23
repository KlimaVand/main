<%
'**************************************************************************************************
' Author: Poul Lassen, Danish Institute of Agricultural Sciences
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
   
   vinterKornList = "3,4,5,23"
   vinterRape = "8"
   toAarsGraesList = "12,18,19,32,33,34"
   udlaegList = "12,13,14,18,19,29,32,33,34"
   
   InitFarmNDB
   CreateLanguageDictionary
   set dictRotation = CreateObject("Scripting.Dictionary")
   nRotation = CallSP("GetRotationList "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber")),arrRotation,dictRotation)
   set dictSecondCropYield = CreateObject("Scripting.Dictionary")
   nSecondCropYield = CallSP("GetSecondCropYield "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber")),arrSecondCropYield,dictSecondCropYield)
'dim a, i, j
'a = dictSecondCropYield.Keys
'For j=0 To UBound(arrSecondCropYield)
'for i = 0 to dictSecondCropYield.Count -1
'  response.write a(i)&" -- "
'  response.write arrSecondCropYield(j,dictSecondCropYield.Item(a(i)))&"<br/>"
'Next
'  response.write "<br> ----------------<br>"
'Next
'  response.write "<br> " & UBound(arrSecondCropYield)
   index=0
   Response.Write("  <table class=""data"" border=""0"" cellpadding=""3"" cellspacing=""0"">" & vbCr)
   Response.Write("<form name=""formRotation"" action=""DummyRotation.asp"" method=""post"">" & vbCr)
   for r = 0 to nRotation-1
      aRotationID = arrRotation(r,dictRotation.Item("RotationID"))
      set dictFieldPlanRotation = CreateObject("Scripting.Dictionary")
      nFieldPlanRotation = CallSP("GetFieldPlanRotationList "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber"))&","&aRotationID,arrFieldPlanRotation,dictFieldPlanRotation)
      if nFieldPlanRotation <> 0 then
	      Response.Write("    <tr>" & vbCr)
	      Response.Write("      <td align=""left"" colspan=""4""><h4>"&arrRotation(r,dictRotation.Item("RotationName"))&"</td>" & vbCr)
	      Response.Write("    </tr>" & vbCr)
	      ' column headings
	      Response.Write("    <tr>" & vbCr)   
	      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strCARRYOVER")&"</th>" & vbCr)
'	      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strAfterCrop")&"</th>" & vbCr)
		  Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strCROP")&"</th>" & vbCr)
	      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strAfterCrop")&"</th>" & vbCr)
	      
		  Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strAREA")&"<br><br><br><br>&nbsp;&nbsp;"&Str.Item("strHA")&"</th>" & vbCr)
	      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strMAINSTRAW")&"<br>&nbsp;&nbsp;"&Str.Item("strCATCHCROP")&"<br>&nbsp;&nbsp;"&Str.Item("strYIELD")&"<br>&nbsp;&nbsp;"&Str.Item("strTOTAL")&"<br>&nbsp;&nbsp;"&Str.Item("strFE")&"</th>" & vbCr)
	      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strYIELD")&"<br>&nbsp;&nbsp;<br><br>&nbsp;&nbsp;"&Str.Item("strTOTAL")&"<br>&nbsp;&nbsp;"&Str.Item("strFE")&"</th>" & vbCr)
	      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strMAIN")&"<br>&nbsp;&nbsp;"&Str.Item("strCATCHCROP")&"<br>&nbsp;&nbsp;"&Str.Item("strNDEMAND")&"<br><br>&nbsp;&nbsp;"&Str.Item("strKGHA")&"</th>" & vbCr)
	      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strANIMAL")&"<br>&nbsp;&nbsp;"&Str.Item("strEXCRETION")&"</th>" & vbCr)
	      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strMAXFRAC")&"<br>&nbsp;&nbsp;"&Str.Item("strGRAZING")&"<br><br><br>&nbsp;&nbsp;(%)</th>" & vbCr)
	      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strPROP")&"<br>&nbsp;&nbsp;"&Str.Item("strSOLD")&"<br><br>&nbsp;&nbsp;(%)</th>" & vbCr)
	      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strSTRAW")&"<br>&nbsp;&nbsp;"&Str.Item("strUSE")&"</th>" & vbCr)
	      Response.Write("    </tr>" & vbCr)
	
	      for f = 0 to nFieldPlanRotation-1
	         aStrawUseID = arrFieldPlanRotation(f,dictFieldPlanRotation("StrawUseType"))
			 aSecondCrop = arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID"))
			 aPreCropSecondCrop = arrFieldPlanRotation(f,dictFieldPlanRotation("PreCropSecondCrop"))
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
'	         aStrawUseID = arrFieldPlanRotation(19,f)
	         Response.Write("    <tr>" & vbCr)   
	         ' column 20 = previous crop original name
	         Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Str.Item("str"&arrFieldPlanRotation(f,dictFieldPlanRotation("PreCropOriginalName"))))
'' 			 if CInt(aPreCropSecondCrop)>CInt(0) And  Not(CInt(aPreCropSecondCrop)=CInt(15)) And Not(CInt(aPreCropSecondCrop)=CInt(21)) And Not(CInt(aPreCropSecondCrop)=CInt(3)) And Not(CInt(aPreCropSecondCrop)=CInt(4)) And Not(CInt(aPreCropSecondCrop)=CInt(5)) And Not(CInt(aPreCropSecondCrop)=CInt(23)) then
''				 if CInt(aPreCropSecondCrop)=CInt(8) then
''				   Response.write(" "&Str.Item("strRapeUndersown"))
''				 else
''			     Response.write(" "&Str.Item("strUnderSown"))
''				 end if
''			 end if
' 			 select case cStr(aPreCropSecondCrop)
'				 case vinterRape
'				   Response.write(" "&Str.Item("strRapeUndersown"))
'				 case 12,13,14,18,19,29,32,33,34
'			       Response.write(" "&Str.Item("strUnderSown"))
'			 end select
			 if Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("PreCrop_AfterCrop")))<>0 then
			       Response.write("<br>->"&Str.Item("str"&arrFieldPlanRotation(f,dictFieldPlanRotation("PreCrop_AfterCrop_Name"))))
		     end if
	'		 Response.Write("</td>")
			 Response.Write("<input type=""hidden"" name=""hiddenFieldPlanRotationID_"&aRotationID&"_"&f&""" value="""&arrFieldPlanRotation(f,dictFieldPlanRotation("FieldPlanRotationID"))&"""><input type=""hidden"" name=""hiddenPreviousCropID_"&aRotationID&"_"&f&""" value="""&(arrFieldPlanRotation(f,dictFieldPlanRotation("PreviousCrop")))&"""><input type=""hidden"" name=""hiddenPreCropOriginalID_"&aRotationID&"_"&f&""" value="""&Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("PreCropOriginalID")))&""">" & vbCr)
	         ' column 2 = Efterafgrøde PreCrop
'			 Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Str.Item("str"&arrFieldPlanRotation(f,dictFieldPlanRotation("PreCrop_AfterCrop_Name"))))
			 Response.Write("<input type=""hidden"" name=""hiddenPreCrop_AfterCropID_"&aRotationID&"_"&f&""" value="""&arrFieldPlanRotation(f,dictFieldPlanRotation("PreCrop_AfterCrop"))&"""></td>" & vbCr)
	         ' column 2 = crop
	         Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Str.Item("str"&arrFieldPlanRotation(f,dictFieldPlanRotation("CropName"))))
''			 if CInt(aSecondCrop)>CInt(0) And  Not(CInt(aSecondCrop)=CInt(15)) And Not(CInt(aSecondCrop)=CInt(21)) And Not(CInt(aSecondCrop)=CInt(3)) And Not(CInt(aSecondCrop)=CInt(4)) And Not(CInt(aSecondCrop)=CInt(5)) And Not(CInt(aSecondCrop)=CInt(23)) then
''				 if CInt(aSecondCrop)=CInt(8) then
''				   Response.write(" "&Str.Item("strRapeUndersown"))
''				 else
''			     Response.write(" "&Str.Item("strUnderSown"))
''				 end if
''			 end if
' 			 select case cStr(aSecondCrop)
'				 case vinterRape
'				   Response.write(" "&Str.Item("strRapeUndersown"))
'				 case 12,13,14,18,19,29,32,33,34
'			       Response.write(" "&Str.Item("strUnderSown"))
'			 end select
'			 Response.Write("</td>")
			 Response.Write("<input type=""hidden"" name=""hiddenCropID_"&aRotationID&"_"&f&""" value="""&arrFieldPlanRotation(f,dictFieldPlanRotation("Crop"))&"""></td>" & vbCr)
	         ' column 2 = Efterafgrøde Crop
			 if Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("Crop_AfterCrop")))<>0 then
			      Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Str.Item("str"&arrFieldPlanRotation(f,dictFieldPlanRotation("Crop_AfterCrop_Name"))))
		     else
			    if Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))= 12 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=13 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=14 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=18 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=19 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=29 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=32 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=33 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=34 then
			     if Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("Crop")))then
			      Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Str.Item("str"&arrFieldPlanRotation(f,dictFieldPlanRotation("CropName"))))
			     else
			      Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Str.Item("strUnderSown"))
					 end if
				  else
				   if Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))= 3 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=4 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=5 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=23 then
				     Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Str.Item("strVinterKorn"))
				   else
				    if Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))= 8 then
					  Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Str.Item("strVinterraps"))
					else
'					  Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrFieldPlanRotation(f,dictFieldPlanRotation("Crop_AfterCrop_Name")))
					  Response.Write("      <td align=""left"">&nbsp;&nbsp;-")
					end if 
				   end if
				end if
			 end if
'			 Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrFieldPlanRotation(f,dictFieldPlanRotation("Crop_AfterCrop_Name")))
			 Response.Write("<input type=""hidden"" name=""hiddenCrop_AfterCropID_"&aRotationID&"_"&f&""" value="""&arrFieldPlanRotation(f,dictFieldPlanRotation("Crop_AfterCrop"))&"""><input type=""hidden"" name=""hiddenPrePreCropID_"&aRotationID&"_"&f&""" value="""&arrFieldPlanRotation(f,dictFieldPlanRotation("PrePreCropID"))&"""></td>" & vbCr)
	         ' column 15 = area
	         Response.Write("      <td align=""right"">&nbsp;&nbsp;"&arrFieldPlanRotation(f,dictFieldPlanRotation("Area"))&"<input type=""hidden"" name=""hiddenArea_"&aRotationID&"_"&f&""" value="""&arrFieldPlanRotation(f,dictFieldPlanRotation("Area"))&"""></td>" & vbCr)
	         ' column 23 to 25 = cernal-, straw-, secondcrop-yield
	         Response.Write("      <td align=""left"">&nbsp;&nbsp;"&FormatNumber(arrFieldPlanRotation(f,dictFieldPlanRotation("CernalYield")),0,-1,0,0)&" , "&FormatNumber(arrFieldPlanRotation(f,dictFieldPlanRotation("StrawYield")),0,-1,0,0)&" , "&FormatNumber(SecondCropFE,0,-1,0,0)&"</td>" & vbCr)

	         ' column 21 = total yield
	         Response.Write("      <td align=""right"">&nbsp;&nbsp;"&FormatNumber(arrFieldPlanRotation(f,dictFieldPlanRotation("TotalYield"))+SecondCropFE,0,-1,0,0)&"</td>" & vbCr)
	         ' column 22 = Nitrogen need
	         Response.Write("      <td align=""right"">&nbsp;&nbsp;"&arrFieldPlanRotation(f,dictFieldPlanRotation("NitrogenNeed"))&" , "&FormatNumber(SecondCropNNeed,0,-1,0,0)&"</td>" & vbCr)
	         ' column 16 = organic fertilizer
			  ' column 3 = UseManure
			 if Cbool(arrFieldPlanRotation(f,dictFieldPlanRotation("UseManure"))) = True then
		         if Cbool(arrFieldPlanRotation(f,dictFieldPlanRotation("OrganicFertilizer"))) = True then    
		            Response.Write("      <td align=""center""><input type=""checkbox"" name=""checkboxOrganicFertilizer_"&aRotationID&"_"&f&""" checked onChange=""document.forms.formRotation.action='RotationUpdate.asp?RotationID="&aRotationID&"';document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""></td>" & vbCr)
		         else   
		            Response.Write("      <td align=""center""><input type=""checkbox"" name=""checkboxOrganicFertilizer_"&aRotationID&"_"&f&""" onChange=""document.forms.formRotation.action='RotationUpdate.asp?RotationID="&aRotationID&"';document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""></td>" & vbCr)
		         end if
			 else
		            Response.Write("      <td align=""center""><input type=""hidden"" name=""checkboxOrganicFertilizer_"&aRotationID&"_"&f&""" value=""false"">&nbsp;</td>" & vbCr)
			 end if 
	         ' column 17 = grazing part
			  ' column 4 = UseGrazing
			 if  Cbool(arrFieldPlanRotation(f,dictFieldPlanRotation("UseGrazing"))) = True then
		         Response.Write("      <td align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputGrazingPart_"&aRotationID&"_"&f&""" value="""&arrFieldPlanRotation(f,dictFieldPlanRotation("GrazingPart"))&""" maxlength=""5"" size=""5"" onChange=""document.forms.formRotation.action='RotationUpdate.asp?RotationID="&aRotationID&"';document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/></td>" & vbCr)
	         else
		         Response.Write("      <td align=""left""><input type=""Hidden"" name=""inputGrazingPart_"&aRotationID&"_"&f&""" value=""0""/>&nbsp;</td>" & vbCr)
			 end if
			 ' column 18 = sale part
			  ' column 5 = UseSale
			 if  Cbool(arrFieldPlanRotation(f,dictFieldPlanRotation("UseSale"))) = True then 
		         Response.Write("      <td align=""left"">&nbsp;&nbsp;<input type=""text"" name=""inputSalePart_"&aRotationID&"_"&f&""" value="""&arrFieldPlanRotation(f,dictFieldPlanRotation("SalePart"))&""" maxlength=""5"" size=""5"" onChange=""document.forms.formRotation.action='RotationUpdate.asp?RotationID="&aRotationID&"';document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';""/></td>" & vbCr)
	         else
		         Response.Write("      <td align=""left""><input type=""hidden"" name=""inputSalePart_"&aRotationID&"_"&f&""" value=""0""/>&nbsp;</td>" & vbCr)
			 end if
			 ' column 19 = straw use type
			  ' column 6 = UseStraw
	         if arrFieldPlanRotation(f,dictFieldPlanRotation("UseStraw")) <> 0 then
					 
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
		         Response.Write("      <td align=""left""><input type=""hidden"" name=""SecondCrop_"&aRotationID&"_"&f&""" value="""&arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID"))&"""/><input type=""hidden"" name=""PreCropSecondCrop_"&aRotationID&"_"&f&""" value="""&arrFieldPlanRotation(f,dictFieldPlanRotation("PreCropSecondCrop"))&"""/>&nbsp;</td>")   
	         Response.Write(vbCr&"    </tr>" & vbCr)
	
	
	      next
			 
	
	      Response.Write("    <tr>" & vbCr)
	      Response.Write("      <td colspan=""9"">")   
	      Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:if (document.forms.update.saveUpdates.value=='true') {document.forms.formRotation.action='RotationUpdate.asp?RotationID="&aRotationID&"';document.forms.formRotation.submit();}"">"&Str.Item("strSAVEROT")&"</button>")
	      Response.Write("<input type=""hidden"" name=""hiddenNFieldPlanRotation_"&aRotationID&""" value="""&nFieldPlanRotation&""">")
	      Response.Write("</td>" & vbCr)   
	      Response.Write("    </tr>" & vbCr)
	   else
	     response.write ("<tr><td colspan=""10""><h3>"&Str.Item("strROTATION")&""&Str.Item("strCOLON")&""&aRotationID&" "&Str.Item("strCALCFAIL")&""&Str.Item("strCOLON")&"<br/>"&Str.Item("strREASON1")&"</h3></td></tr>" & vbCr)
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
end sub
'**************************************************************************************************
sub CreateUpdateFormRotation()
   Response.Write("<form name=""update"" action=""javascript:document.forms.formRotation.submit();"" method=""post"">" & vbCr)
   Response.Write("<input type=""hidden"" name=""saveUpdates"" value=""false"">" & vbCr)
   Response.Write("</form>" & vbCr)
end sub
'**************************************************************************************************
%>
