<%
'**************************************************************************************************
' Author: Poul Lassen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
sub CreateTabField()
   dim r,f
   dim nRotation, arrRotation, aRotationID
   dim nFieldPlan, arrFieldPlan
   dim nSoilType, arrSoilType, aSoilTypeID
   dim nIrrigationType, arrIrrigationType, aIrrigationType
   dim nCrop, arrCrop, aCropID
   dim nAfterCrop, arrAfterCrop, aAfterCropID, canHaveAfterCrop
   dim nYieldLevel, aYieldLevel
   'arrYieldLevel, aYieldLevel
   
   InitFarmNDB
   CreateLanguageDictionary
   Dim dictRotation
   set dictRotation = CreateObject("Scripting.Dictionary")
   nRotation = CallSP ("GetRotationList " & Session("farmNumber")& "," & Session("scenarioNumber") , arrRotation, dictRotation)
'   nRotation = GetRotationList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation)
   Dim dictSoilType
   set dictSoilType = CreateObject("Scripting.Dictionary")
   nSoilType = CallSP ("GetSelectListSoilType" , arrSoilType, dictSoilType)
'   nSoilType = GetSelectListSoilType(arrSoilType)
   Dim dictCrop
   set dictCrop = CreateObject("Scripting.Dictionary")
   nCrop = CallSP ("GetSelectListCrop" , arrCrop, dictCrop)
'   nCrop     = GetSelectListCrop(arrCrop)
   Dim dictAfterCrop
   set dictAfterCrop = CreateObject("Scripting.Dictionary")
   nAfterCrop = CallSP ("GetSelectListAfterCrop" , arrAfterCrop, dictAfterCrop)

   redim arrIrrigationType(1,1)
   nIrrigationType 			= 2
   arrIrrigationType(0,0)	= 0
   arrIrrigationType(1,0)	= Str.Item("strRAINFED")
   arrIrrigationType(0,1)	= 1
   arrIrrigationType(1,1)	= Str.Item("strIRRIGATED")

   Response.Write("<form name=""formFieldPlan"" action=""DummyFieldPlan.asp"" method=""post"">" & vbCr)
   Response.Write("  <table class=""data"" border=""0"">" & vbCr)
  
   Dim dictFieldPlan

   for r = 0 to nRotation-1
      aRotationID = arrRotation(r,dictRotation.item("RotationID"))
      aSoilTypeID = arrRotation(r,dictRotation.item("SoilType"))
	  aIrrigationType = arrRotation(r, dictRotation.item("Irrigation"))
	  if aIrrigationType="True" then
	     aIrrigationType=1
	  else
	     aIrrigationType=0
	  end if
'	  response.write aIrrigationType
	  aYieldLevel = arrRotation(r,dictRotation.item("YieldLevel"))	  
      Response.Write("    <tr>" & vbCr)
      Response.Write("      <td align=""left"" colspan=""2""><h4>"&Str.Item("strRotationName")&": " & vbCr)
	  Response.Write("<input type=""text"" name=""RotationName_"&aRotationID&""" value="""&arrRotation(r,dictRotation.item("RotationName"))&""" maxlength=""50"" size=""30"" onChange=""document.forms.update.saveUpdates.value='true';document.forms.formFieldPlan.action='FieldPlanUpdate.asp?RotationID="&aRotationID&"';document.forms.update.action='javascript:document.forms.formFieldPlan.submit();';this.style.color='#ff0000';""/>" & vbCr)
	  Response.Write("</td>" & vbCr)
      Response.Write("      <td align=""left"" colspan=""2""><h4>"&Str.Item("strSOILTYPE")&" " & vbCr)
	  Response.Write("<select name=""SoilType_"&aRotationID&""" onChange=""if ((this.options[this.selectedIndex].value=='1')||(this.options[this.selectedIndex].value=='2')||(this.options[this.selectedIndex].value=='3')||(this.options[this.selectedIndex].value=='4')){document.forms.formFieldPlan.Irrigation_"&aRotationID&".disabled=false;}else{document.forms.formFieldPlan.Irrigation_"&aRotationID&".options[0].selected=true;document.forms.formFieldPlan.Irrigation_"&aRotationID&".disabled=true;};document.forms.update.saveUpdates.value='true';document.forms.formFieldPlan.action='FieldPlanUpdate.asp?RotationID="&aRotationID&"&Irrigation_"&aRotationID&"=0';document.forms.update.action='javascript:document.forms.formFieldPlan.submit();';this.style.color='#ff0000';"">" & vbCr)
      CreateSelectList_SoilType aSoilTypeID,nSoilType,arrSoilType
	  Response.Write("</td>" & vbCr)
      Response.Write("      <td align=""left""><h4>"&Str.Item("strIRRIGATION")&" " & vbCr)
   select case aSoilTypeID
   case 1,2,3,4
	  Response.Write("<select name=""Irrigation_"&aRotationID&""" onChange=""document.forms.update.saveUpdates.value='true';document.forms.formFieldPlan.action='FieldPlanUpdate.asp?RotationID="&aRotationID&"';document.forms.update.action='javascript:document.forms.formFieldPlan.submit();';this.style.color='#ff0000';"">" & vbCr)
   case else
	  Response.Write("<select name=""Irrigation_"&aRotationID&""" onChange=""document.forms.update.saveUpdates.value='true';document.forms.formFieldPlan.action='FieldPlanUpdate.asp?RotationID="&aRotationID&"';document.forms.update.action='javascript:document.forms.formFieldPlan.submit();';this.style.color='#ff0000';"" disabled>" & vbCr)
   end select
      CreateSelectList aIrrigationType,nIrrigationType,arrIrrigationType
	  Response.Write("</td>" & vbCr)
      Response.Write("      <td align=""left""><h4>"&Str.Item("strYIELDLEVEL")&": " & vbCr)
'	  Response.Write("<select name=""YieldLevel_"&aRotationID&""" onChange=""document.forms.update.saveUpdates.value='true';document.forms.formFieldPlan.action='FieldPlanUpdate.asp?RotationID="&aRotationID&"';document.forms.update.action='javascript:document.forms.formFieldPlan.submit();';this.style.color='#ff0000';"">" & vbCr)
'      CreateSelectList aYieldLevel,nYieldLevel,arrYieldLevel
      Response.Write("<input type = ""text"" size=""2"" maxlength=""3"" name=""YieldLevel_"&aRotationID&""" value="""&arrRotation(r,dictRotation.item("YieldLevel"))&""" onChange=""InputValidator(this, 0, 999, '"&Str.Item("strYIELDLEVEL")&"', 3);document.forms.update.saveUpdates.value='true';document.forms.formFieldPlan.action='FieldPlanUpdate.asp?RotationID="&aRotationID&"';document.forms.update.action='javascript:document.forms.formFieldPlan.submit();';this.style.color='#ff0000';"" />%")   
	  Response.Write("</td>" & vbCr)
	  Response.Write("    </tr>" & vbCr)
      ' column headings
      Response.Write("    <tr>" & vbCr)   
      Response.Write("      <th align=""left"">"&Str.Item("strCROP")&"</th>" & vbCr)
      Response.Write("      <th align=""left"">"&Str.Item("strAREAHA")&"</th>" & vbCr)
      Response.Write("      <th align=""left"">"&Str.Item("strAfterCrop")&"</th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
      Response.Write("    </tr>" & vbCr)
      
   set dictFieldPlan = CreateObject("Scripting.Dictionary")
   nFieldPlan = CallSP ("GetFieldPlanList " & CLng(Session("farmNumber")) & "," & CInt(Session("scenarioNumber")) & "," & aRotationID, arrFieldPlan, dictFieldPlan)
  '	  nFieldPlan = GetFieldPlanList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),aRotationID,arrFieldPlan)
dim checkTotalArea, checkTotalAfterCrop
      for f = 0 to nFieldPlan-1
         aCropID = arrFieldPlan(f,dictFieldPlan.item("Crop"))
				 aAfterCropID = arrFieldPlan(f,dictFieldPlan.item("AfterCropID"))
				 canHaveAfterCrop = cBool(arrFieldPlan(f,dictFieldPlan.item("Can_Have_AfterCrop")))
         Response.Write("    <tr>" & vbCr)
         ' column 1 = crop
         Response.Write("      <td class=""data"" align=""left"">")
         Response.Write("<input type=""hidden"" name=""hiddenFieldPlanID_"&aRotationID&"_"&f&""" value="""&arrFieldPlan(f,dictFieldPlan.item("FieldPlanID"))&""">")
         Response.Write("<input type=""hidden"" name=""hiddenCropID_"&aRotationID&"_"&f&""" value="""&arrFieldPlan(f,dictFieldPlan.item("Crop"))&""">")
         Response.Write("<select name=""selectCrop_"&aRotationID&"_"&f&""" onChange=""document.forms.update.saveUpdates.value='true';document.forms.formFieldPlan.action='FieldPlanUpdate.asp?RotationID="&aRotationID&"';document.forms.update.action='javascript:document.forms.formFieldPlan.submit();';this.style.color='#ff0000';"">")   
         CreateSelectList_Language aCropID,nCrop,arrCrop
         Response.Write("</select>")   
         Response.Write("</td>" & vbCr)
         ' column 2 = area
         Response.Write("      <td class=""data"" align=""left""><input type=""text"" name=""inputArea_"&aRotationID&"_"&f&""" value="""&arrFieldPlan(f,dictFieldPlan.item("Area"))&""" maxlength=""5"" size=""8"" onChange=""document.forms.update.saveUpdates.value='true';document.forms.formFieldPlan.action='FieldPlanUpdate.asp?RotationID="&aRotationID&"';document.forms.update.action='javascript:document.forms.formFieldPlan.submit();';this.style.color='#ff0000';""/></td>" & vbCr)
checkTotalArea = checkTotalArea + cdbl(arrFieldPlan(f,dictFieldPlan.item("Area")))
if aAfterCropID = 1 then checkTotalAfterCrop = checkTotalAfterCrop + cdbl(arrFieldPlan(f,dictFieldPlan.item("Area")))
         Response.Write("      <td class=""data"" align=""left"">")   
         ' column 3 = AfterCrop
				 if canHaveAfterCrop = true then   
           Response.Write("<select name=""selectAfterCrop_"&aRotationID&"_"&f&""" onChange=""document.forms.update.saveUpdates.value='true';document.forms.formFieldPlan.action='FieldPlanUpdate.asp?RotationID="&aRotationID&"';document.forms.update.action='javascript:document.forms.formFieldPlan.submit();';this.style.color='#ff0000';"">")   
           CreateSelectList_Language aAfterCropID,nAfterCrop,arrAfterCrop
           Response.Write("</select>")
				 else
           Response.Write("<input type=""hidden"" name=""selectAfterCrop_"&aRotationID&"_"&f&""" value=""0"">")   
				 end if         
         Response.Write("</td>" & vbCr)
         Response.Write("      <td class=""data"" align=""left"">")   
  
         ' column 4 = delete buttons   
         Response.Write("&nbsp;&nbsp;<button style=""width: 125px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.update.saveUpdates.value='true';if(confirm('"&Str.Item("strBekræftafgrøde")&"')) {document.forms.formFieldPlan.action='FieldPlanDelete.asp?RotationID="&aRotationID&"&FieldPlanID="&arrFieldPlan(f,dictFieldPlan.item("FieldPlanID"))&"';document.forms.formFieldPlan.submit();}"">"&Str.Item("strDELETECROP")&"</button>")
         Response.Write("</td>" & vbCr)   
         Response.Write("    </tr>" & vbCr)
      next
      set dictFieldPlan = Nothing
      Response.Write("    <tr>" & vbCr)
      Response.Write("      <td>"&checkTotalArea&"</td><td>"&checkTotalAfterCrop&"</td><td>"&checkTotalAfterCrop/checkTotalArea*100&"</td></tr>")   
      
      Response.Write("    <tr>" & vbCr)
      Response.Write("      <td class=""data"" colspan=""3"">")   
      Response.Write("<button style=""width: 125px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.update.saveUpdates.value='false';document.forms.formFieldPlan.action='FieldPlanNew.asp?RotationID="&aRotationID&"';document.forms.formFieldPlan.submit();"">"&Str.Item("strNEWCROP")&"</button>")
      Response.Write("&nbsp;")   
      Response.Write("<button style=""width: 125px; height: 20px; font-size: 10px"" onClick=""javascript:if (document.forms.update.saveUpdates.value=='true') {document.forms.formFieldPlan.action='FieldPlanUpdate.asp?RotationID="&aRotationID&"';document.forms.formFieldPlan.submit()};"">"&Str.Item("strSAVEROT")&"</button>")
      Response.Write("&nbsp;")   
      Response.Write("<button style=""width: 125px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.update.saveUpdates.value='false';if(confirm('"&Str.Item("strBekræftsædskifte")&"')) {document.forms.formFieldPlan.action='RotationDelete.asp?RotationID="&aRotationID&"';document.forms.formFieldPlan.submit();}"">"&Str.Item("strDELROT")&"</button>")
      Response.Write("<input type=""hidden"" name=""hiddenNFieldPlan_"&aRotationID&""" value="""&nFieldPlan&""">")
      Response.Write("</td>" & vbCr)   
      Response.Write("    </tr>" & vbCr)

      Response.Write("    <tr>" & vbCr)
      Response.Write("      <td colspan=""6""><hr/></td>" & vbCr)
      Response.Write("    </tr>" & vbCr)
   next
   
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <td colspan=""8"">")   
   Response.Write("<button style=""width: 200px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.update.saveUpdates.value='false';document.forms.formFieldPlan.action='RotationNew.asp';document.forms.formFieldPlan.submit();"">"&Str.Item("strNEWROT")&"</button>")
   Response.Write("</td>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
   CloseFarmNDB

   Response.Write("  </table>" & vbCr) 
   Response.Write("</form>" & vbCr)
   CreateUpdateFormFieldPlan
end sub
'**************************************************************************************************
sub CreateUpdateFormFieldPlan()
   Response.Write("<form name=""update"" action=""javascript:document.forms.formFieldPlan.submit();"" method=""post"">" & vbCr)
   Response.Write("<input type=""hidden"" name=""saveUpdates"" value=""false"">" & vbCr)
   Response.Write("</form>" & vbCr)
end sub
'**************************************************************************************************
%>
