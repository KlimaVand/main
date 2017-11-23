<%
'**************************************************************************************************
' Author: Margit Jorgensen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
%>
<!-- #include file="includes/RotationModel2.asp" -->
<!-- #include file="includes/FunctionField.asp" -->

<%
Dim tid0,tid1,tid2,tid3,tid4

Dim arrManureFertilizerDelivery,dictDelivery,MeanUtilization, totalManureNContent, totalManureShareOfNNeed
Dim arrA  ' array of ManureTypes
Dim arrB  ' array of fields(FieldPlanRotations)
Dim arrFertOut, arrFieldOut,scale,warning
Dim dummy1, dummy2, NPercent, nDelivery

version = Session("version")
''**************************************************************************************************
sub CreateTabManure()
'Dim tid0, tid1, tid2, tid3 
tid0=Timer
   CalculateManure version,NPercent,scale,arrA,arrB,warning, dummy1, dummy2
'tid1=Timer   
   CreateLanguageDictionary
 response.write "<br/>" & Str.Item("strDistributionVersion") & ": "& version

   Response.Write("<br>" & vbCr) 
'Tabel som bestemmer indkøbt handelsgødning.   
   Response.Write("<h3>"&Str.Item("strBFertilizer")&"</h3><br/>" & vbCr) 

   CreateTabelNNeedPercent NPercent
 
   Response.Write("<br>" & vbCr) 

   Response.Write("<h3>"&Str.Item("strDISTMANURE")&"</h3><br/>" & vbCr) 

   
   if totalManureShareOfNNeed <>0 then
     Response.Write("  <table border=""0"" cellpadding=""3"" cellspacing=""0"">" & vbCr)
	  
     set dictDelivery = CreateObject("Scripting.Dictionary")
     nDelivery = CallSP ("GetManureFertilizerDelivery " & CLng(Session("farmNumber")) & "," & CInt(Session("scenarioNumber")) ,arrManureFertilizerDelivery,dictDelivery)
'	  GetManureFertilizerDelivery CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), arrManureFertilizerDelivery
'      CloseFarmNDB
	  CreateTableManureDelivery
' tid2=Timer   
'     CreateN_Les_Table
' tid3=Timer   
'     CloseFarmNDB
	 if totalManureNContent=0 then
	  response.write("<tr><td><h3>"&Str.Item("strNOPOO")&"</h3></td></tr></table>" & vbCr) 
     end if
   else
    response.write("<tr><td><br/><h3>"&Str.Item("strNOROT")&"</h3></td></tr>" & vbCr)
   end if

   Response.Write("<br>" & vbCr) 
   Response.Write("<br>" & vbCr) 
   if scale >-1 then 
     Response.Write("&nbsp;&nbsp; " & FormatNumber(scale*100,1,-1,0,0)& Str.Item("strHGopfyldelses%") & vbCr) 
   end if

   if warning = 1 then 
     Response.Write("<br>" & vbCr) 
     Response.Write("<br>&nbsp;&nbsp; " & Str.Item("strTvungetSalg af HG") & vbCr) 
   end if

	

   Response.Write("<br/>" & vbCr)   
   Response.Write("<br/>" & vbCr)   
'response.write "<br>Tid Function  = "&   tid1-tid0
'response.write "<br>Tid Tabel  = "&   tid2-tid1
'response.write "<br>Tid NLess  = "&   tid3-tid2
'response.write "<br>Tid ialt  = "&   tid4-tid0

end sub
''**************************************************************************************************

sub CreateTabelNNeedPercent(NPercent)
Dim Fertilizer, arr
     redim arr(1)

	 arr(0) = CLng(Session("farmNumber"))
	 arr(1) = CInt(Session("scenarioNumber"))

     Fertilizer = CallSPReturn ("GetFertilizer",arr)

   Response.Write("<form name=""formManureFertilizer"" action=""ManureFertilizerUpdate.asp"" method=""post"">" & vbCr)
   Response.Write("  <table class=""data"" border=""0"">" & vbCr)

   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th align=""left"">"&Str.Item("strNPercent")& ": </th>" & vbCr)
   Response.Write("      <th align=""left""><input type = ""text"" size=""2"" maxlength=""3"" name=""selectNPercent"" value="""&NPercent&""", 3);"" />")   
   Response.Write("%")   
   Response.Write("</th>" & vbCr)

   Response.Write("      <th class=""data"" align=""left"">")
   Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formManureFertilizer.action='ManureFertilizerUpdate.asp';document.forms.formManureFertilizer.submit();"">"&Str.Item("strSAVE")&"</button>")
   Response.Write("</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   Response.Write("    <tr>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th align=""left"">"&Str.Item("strBoughtFertilizer")& " " &  FormatNumber(Fertilizer,0,-1,0,0) &" kg</th>" & vbCr)

   Response.Write("    </tr>" & vbCr)

   Response.Write("  </table>" & vbCr)

end sub
'**************************************************************************************************
sub CreateTableManureDelivery()
dim f,a,b 
Dim totalNDelivered, totalNLoss

      Response.Write("    <tr>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strROTATION")&"<br/></th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strCARRYOVER")&"<br/></th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strCROP")&"</th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strTYPEDIST")&"<br/>&nbsp;&nbsp;"&Str.Item("strfertilizer")&"<br/></th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strSPREADER")&"<br/>&nbsp;&nbsp;"&Str.Item("strMETHOD")&"<br/></th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strKGNNeed")&"<br/>&nbsp;&nbsp;"&Str.Item("strFulfilled")&"<br/>&nbsp;&nbsp;"&Str.Item("strKGHA")&"</th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strSPREAD")&"<br/>&nbsp;&nbsp;"&Str.Item("strKGHA")&"</th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strSPREADLOSS")&"<br/>&nbsp;&nbsp;"&Str.Item("strKGHA")&"</th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strAREA")&"<br/>&nbsp;&nbsp;"&Str.Item("strHA")&"</th>" & vbCr)
      Response.Write("    </tr>" & vbCr)
      
	  
      for a = 0 to Ubound(arrManureFertilizerDelivery)
      Response.Write("    <tr>" & vbCr)

		       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrManureFertilizerDelivery(a,dictDelivery.item("RotationName"))&"</td>" & vbCr)
		       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Str.Item(""&"str"&arrManureFertilizerDelivery(a,dictDelivery.item("PreCropName"))&"")&"</td>" & vbCr)
		       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Str.Item(""&"str"&arrManureFertilizerDelivery(a,dictDelivery.item("CropName"))&"")&"</td>" & vbCr)
		       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Str.Item(""&"str"&arrManureFertilizerDelivery(a,dictDelivery.item("StorageName"))&"")&"</td>" & vbCr)
		       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Str.Item(""&"str"&arrManureFertilizerDelivery(a,dictDelivery.item("DeliveryTypeDescription"))&"")&"</td>" & vbCr)
		       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&FormatNumber(arrManureFertilizerDelivery(a,dictDelivery.item("Kg_N_Utilized"))/arrManureFertilizerDelivery(a,dictDelivery.item("Area")),1,-1,0,0)&"</td>"& vbCr)
		       totalNDelivered=CDbl(totalNDelivered)+CDbl(arrManureFertilizerDelivery(a,dictDelivery.item("Kg_N_Delivered")))
		       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&FormatNumber(arrManureFertilizerDelivery(a,dictDelivery.item("Kg_N_Delivered"))/arrManureFertilizerDelivery(a,dictDelivery.item("Area")),1,-1,0,0)&"</td>" & vbCr)
               totalNLoss=CDbl(totalNLoss)+CDbl(arrManureFertilizerDelivery(a,dictDelivery.item("N_Loss")))
		       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&FormatNumber(arrManureFertilizerDelivery(a,dictDelivery.item("N_Loss"))/arrManureFertilizerDelivery(a,dictDelivery.item("Area")),1,-1,0,0)&"</td>" & vbCr)
'		       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&FormatNumber(arrManureFertilizerDelivery(a,dictDelivery.item("Area")),0,-1,0,0)&"</td>" & vbCr)
		       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrManureFertilizerDelivery(a,dictDelivery.item("Area"))&"</td>" & vbCr)
       
       Response.Write("    </tr>" & vbCr)
       next
      Response.Write("    <tr>" & vbCr)
      Response.Write("      <td colspan=""10""><hr/></td>" & vbCr)
      Response.Write("    </tr>" & vbCr)
      Response.Write("    <tr>" & vbCr)
'      Response.Write("      <td align=""left"" colspan=""4""><h4>Sædskifte navn: "&arrRotation(1,r)&"</td>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;<br/></th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
   '   Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
      Response.Write("      <th align=""left"" colspan=2>&nbsp;&nbsp;"&Str.Item("strIalt")&":</th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&FormatNumber(totalNDelivered,0,-1,0,0)&"</th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&FormatNumber(totalNLoss,0,-1,0,0)&"</th>" & vbCr)
      Response.Write("    </tr>" & vbCr)
   
   Response.Write("  </table>" & vbCr)
   Response.Write("</form>" & vbCr)
	CreateUpdateFormManure
   
end sub
'**************************************************************************************************
sub CreateUpdateFormManure()
   Response.Write("<form name=""update"" action=""javascript:document.forms.formManureFertilizer.submit();"" method=""post"">" & vbCr)
   Response.Write("<input type=""hidden"" name=""saveUpdates"" value=""false"" />" & vbCr)
   Response.Write("</form>" & vbCr)
end sub
'**************************************************************************************************
sub CreateN_Les_Table()
dim arrRotation,aRotationID,nRotation,dictRotation,r
dim arrFieldPlanRotation,aFieldPlanRotation,nFieldPlanRotation,dictFieldPlanRotation,f
dim NLes,arrParameter,RunOff,NLESmg,aSecondCrop,aPreCropSecondCrop,Area,TotalNLES_N,meanNLES_N,totalArea
dim totalRunOff,meanRunOff,meanNLES_mg
dim rotationRunOff,rotationMeanRunOff, rotationMeanNLES_mg, rotationMeanNLES_N, rotationNLES_N, rotationArea
dim fixation,secondCropfixation,CropCoeff,PreCropCoeff

redim arrParameter(1)
   arrParameter(0)=CLng(Session("farmNumber"))
   arrParameter(1)=CInt(Session("scenarioNumber"))
   totalArea = CallSPReturn ("GetTotalArea",arrParameter)
	 if totalArea = 0 then totalArea = 1
   Response.Write("  <table class=""data"" border=""0"">" & vbCr)
   set dictRotation = CreateObject("Scripting.Dictionary")
   nRotation = CallSP("GetRotationList "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber")),arrRotation,dictRotation)
redim preserve arrParameter(3)
for r = 0 to nRotation-1
		rotationRunOff = 0
	  rotationMeanRunOff = 0
		rotationMeanNLES_N = 0
		rotationNLES_N = 0
	  rotationMeanNLES_mg = 0
		rotationArea = 0
    aRotationID = arrRotation(r,dictRotation.Item("RotationID"))
    arrParameter(2)=aRotationID
    Response.Write("      <th align=""left"">"&arrRotation(r,dictRotation.Item("RotationName"))&"<br/></th>" & vbCr)
    Response.Write("    <tr>" & vbCr)
    Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strCARRYOVER")&"</th>" & vbCr)
'    Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strAfterCrop")&"</th>" & vbCr)
    Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strCROP")&"</th>" & vbCr)
    Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strAfterCrop")&"</th>" & vbCr)
    Response.Write("      <th align=""left"">&nbsp;&nbsp;N-LES<br />&nbsp;&nbsp;Kg.N/Ha</th>" & vbCr)
    Response.Write("      <th align=""left"">&nbsp;&nbsp;N-LES<br />&nbsp;&nbsp;mg NO<sub>3</sub>/l</th>" & vbCr)
    Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strRunoff")&"<br />&nbsp;&nbsp;mm</th>" & vbCr)
    Response.Write("      <th align=""left"">&nbsp;&nbsp;Areal<br />&nbsp;&nbsp;Ha</th>" & vbCr)
if Session("usertype")=50 then
    Response.Write("      <th align=""left"">&nbsp;&nbsp;Fix. crop + aftercrop</th>" & vbCr)
    Response.Write("      <th align=""left"">&nbsp;&nbsp;Cropcoeff</th>" & vbCr)
    Response.Write("      <th align=""left"">&nbsp;&nbsp;PreCropCoeff</th>" & vbCr)
end if
select case Session("userid")
 case "2","4","7","19","20","25","464646","474747","9"
    Response.Write("      <th align=""left"">&nbsp;&nbsp;Fix. crop + aftercrop</th>" & vbCr)
    Response.Write("      <th align=""left"">&nbsp;&nbsp;Cropcoeff</th>" & vbCr)
    Response.Write("      <th align=""left"">&nbsp;&nbsp;PreCropCoeff</th>" & vbCr)
end select
    Response.Write("    </tr>" & vbCr)
		set dictFieldPlanRotation = CreateObject("Scripting.Dictionary")
'		nFieldPlanRotation = CallSP("GetFieldPlanRotationList "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber"))&","&aRotationID,arrFieldPlanRotation,dictFieldPlanRotation)
'		for f=0 to nFieldPlanRotation-1
'			aSecondCrop = arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID"))
'			aPreCropSecondCrop = arrFieldPlanRotation(f,dictFieldPlanRotation("PreCropSecondCrop"))
'			aFieldPlanRotation = arrFieldPlanRotation(f,dictFieldPlanRotation.Item("FieldPlanRotationID"))
'			arrParameter(3)=aFieldPlanRotation
'			NLes=CallSPReturn("CalcN_LesPrFieldPlanRotation ",arrParameter)
'			fixation=CallSPReturn("fnGetCropFixationPrFieldPlanRotation ",arrParameter)
'			secondCropfixation=CallSPReturn("fnGetSecondCropFixationPrFieldPlanRotation ",arrParameter)
'			CropCoeff=CallSPReturn("fnGetCropCoeff ",arrParameter)
'			PreCropCoeff=CallSPReturn("fnGetPreCropCoeff ",arrParameter)
'			Area=Cdbl(arrFieldPlanRotation(f,dictFieldPlanRotation.Item("Area")))
'			rotationArea = rotationArea + Area
'			TotalNLES_N=TotalNLES_N+(NLes*Area)
'			rotationNLES_N = rotationNLES_N+(NLes*Area)
'			RunOff=CallSPReturn("GetRunOff ",arrParameter)
'			totalRunOff=totalRunOff+(RunOff*Area)
		nFieldPlanRotation = CallSP("GetFieldPlanRotationListRunoffNLes "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber"))&","&aRotationID,arrFieldPlanRotation,dictFieldPlanRotation)
		for f=0 to nFieldPlanRotation-1
			aSecondCrop = arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID"))
			aPreCropSecondCrop = arrFieldPlanRotation(f,dictFieldPlanRotation("PreCropSecondCrop"))
			aFieldPlanRotation = arrFieldPlanRotation(f,dictFieldPlanRotation.Item("FieldPlanRotationID"))
			arrParameter(3)=aFieldPlanRotation
'			fixation=CallSPReturn("fnGetCropFixationPrFieldPlanRotation ",arrParameter)
			fixation=arrFieldPlanRotation(f,dictFieldPlanRotation("Fixation"))
'			secondCropfixation=CallSPReturn("fnGetSecondCropFixationPrFieldPlanRotation ",arrParameter)
			secondCropfixation=arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropFixation"))
'			CropCoeff=CallSPReturn("fnGetCropCoeff ",arrParameter)
			CropCoeff=arrFieldPlanRotation(f,dictFieldPlanRotation("CropCoeff"))
'			PreCropCoeff=CallSPReturn("fnGetPreCropCoeff ",arrParameter)
			PreCropCoeff=arrFieldPlanRotation(f,dictFieldPlanRotation("PreCropCoeff"))
'			NLes=CallSPReturn("CalcN_LesPrFieldPlanRotation ",arrParameter)
			NLes=arrFieldPlanRotation(f,dictFieldPlanRotation("N_Les"))
			Area=Cdbl(arrFieldPlanRotation(f,dictFieldPlanRotation("Area")))
			rotationArea = rotationArea + Area
			TotalNLES_N=TotalNLES_N+(NLes*Area)
			rotationNLES_N = rotationNLES_N+(NLes*Area)
'			RunOff=CallSPReturn("GetRunOff ",arrParameter)
			RunOff=arrFieldPlanRotation(f,dictFieldPlanRotation("Runoff"))
			totalRunOff=totalRunOff+(RunOff*Area)
			rotationRunOff = rotationRunOff+(RunOff*Area)
			if RunOff<>0 then 
			  NLESmg=100*NLes*4.43/RunOff'NLES (KgN/Ha)-> 1000000*NLES (mg/Ha). RunOff (liter/squareMeter)-> 10000*RunOff (liter/Ha)
			else
			  NLESmg=100*NLes*4.43/1
			end if
			Response.Write("    <tr>" & vbCr)
			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Str.Item(""&"str"&arrFieldPlanRotation(f,dictFieldPlanRotation.item("PreCropOriginalName"))&"")) 
'			if CInt(aPreCropSecondCrop)>CInt(0) And  Not(CInt(aPreCropSecondCrop)=CInt(15)) And Not(CInt(aPreCropSecondCrop)=CInt(21)) And Not(CInt(aPreCropSecondCrop)=CInt(3)) And Not(CInt(aPreCropSecondCrop)=CInt(4)) And Not(CInt(aPreCropSecondCrop)=CInt(5)) And Not(CInt(aPreCropSecondCrop)=CInt(23)) then
'				 if CInt(aPreCropSecondCrop)=CInt(8) then
'				   Response.write(" "&Str.Item("strRapeUndersown"))
'				 else
'			     Response.write(" "&Str.Item("strUnderSown"))
'				 end if
'			end if
			if Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("PreCrop_AfterCrop")))<>0 then
			       Response.write("<br>->"&Str.Item(""&"str"&arrFieldPlanRotation(f,dictFieldPlanRotation("PreCrop_AfterCrop_Name"))&""))
		  end if
			Response.Write("</td>" & vbCr)
'			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrFieldPlanRotation(f,dictFieldPlanRotation.item("PreCrop_AfterCrop_Name"))&"</td>" & vbCr)
			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Str.Item(""&"str"&arrFieldPlanRotation(f,dictFieldPlanRotation.item("CropName"))&""))
'			if CInt(aSecondCrop)>CInt(0) And  Not(CInt(aSecondCrop)=CInt(15)) And Not(CInt(aSecondCrop)=CInt(21)) And Not(CInt(aSecondCrop)=CInt(3)) And Not(CInt(aSecondCrop)=CInt(4)) And Not(CInt(aSecondCrop)=CInt(5)) And Not(CInt(aSecondCrop)=CInt(23)) then
'				 if CInt(aSecondCrop)=CInt(8) then
'				   Response.write(" "&Str.Item("strRapeUndersown"))
'				 else
'			     Response.write(" "&Str.Item("strUnderSown"))
'				 end if
'			end if
			Response.Write("</td>" & vbCr)
'			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrFieldPlanRotation(f,dictFieldPlanRotation.item("Crop_AfterCrop_Name"))&"</td>" & vbCr)
			 if Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("Crop_AfterCrop")))<>0 then
			      Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Str.Item(""&"str"&arrFieldPlanRotation(f,dictFieldPlanRotation("Crop_AfterCrop_Name"))&"")&"</td>" & vbCr)
		     else
			    if Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))= 12 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=13 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=14 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=18 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=19 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=29 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=32 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=33 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=34 then
			     if Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("Crop")))then
			      Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Str.Item("str"&arrFieldPlanRotation(f,dictFieldPlanRotation("CropName")))&"</td>" & vbCr)
			     else
			      Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Str.Item("strUnderSown")&"</td>" & vbCr)
					 end if
				  else
				   if Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))= 3 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=4 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=5 or Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))=23 then
				     Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Str.Item("strVinterKorn")&"</td>" & vbCr)
				   else
				    if Cint(arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID")))= 8 then
					  Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Str.Item("strVinterraps")&"</td>" & vbCr)
					else
'					  Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrFieldPlanRotation(f,dictFieldPlanRotation("Crop_AfterCrop_Name"))&"</td>" & vbCr)
					  Response.Write("      <td align=""left"">&nbsp;&nbsp;-</td>" & vbCr)
					end if 
				   end if
				end if
			 end if
			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Round(NLes,2)&"</td>" & vbCr)
			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Round(NLesmg,2)&"</td>" & vbCr)
			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Round(RunOff,2)&"</td>" & vbCr)
			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Area&"</td>" & vbCr)
if Session("usertype")=50 then
			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&fixation&" + "&secondCropfixation&"</td>" & vbCr)
			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&CropCoeff&"</td>" & vbCr)
			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&PreCropCoeff&"</td>" & vbCr)
end if
select case Session("userid")
 case "2","7","19","20","25","464646","474747","9"
			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&fixation&" + "&secondCropfixation&"</td>" & vbCr)
			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&CropCoeff&"</td>" & vbCr)
			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&PreCropCoeff&"</td>" & vbCr)
end select
			Response.Write("    </tr>" & vbCr)
	  next
'if rotationArea<>0 then
'		rotationMeanNLES_N = rotationNLES_N/rotationArea
'		rotationMeanRunOff = rotationRunOff/rotationArea
'		rotationMeanNLES_mg = 100*rotationMeanNLES_N*4.43/rotationMeanRunOff
'else
'		rotationMeanNLES_N = 0
'		rotationMeanRunOff = 0
'		rotationMeanNLES_mg = 0
'end if
'	   Response.Write("    </tr>" & vbCr)
'	   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
''	   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
'	   Response.Write("      <th align=""right"" colspan=""2"">&nbsp;&nbsp;"&Str.Item("strMeanOnRotation")&"</th>" & vbCr)
'	'   Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
'	   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Round(rotationMeanNLES_N,2)&"</th>" & vbCr)
'	   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Round(rotationMeanNLES_mg,2)&"</th>" & vbCr)
'	   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Round(rotationMeanRunOff,2)&"</th>" & vbCr)
'	   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Round(rotationArea,2)&"</th>" & vbCr)
'	   Response.Write("    </tr>" & vbCr)

'dim CorrNHa,CorrNl,a,l,arrCorrNHa,arrCorrNl,dictCorrNHa,dictCorrNl
'
'     set dictCorrNHa = CreateObject("Scripting.Dictionary")
'     set dictCorrNl = CreateObject("Scripting.Dictionary")
'     a = CallSP ("VVMGetCorrectedNHa " & CLng(Session("farmNumber")) & "," & CInt(Session("scenarioNumber")) & "," & aRotationID,arrCorrNHa,dictCorrNHa)
'     l = CallSP ("VVMGetCorrectedNl " & CLng(Session("farmNumber")) & "," & CInt(Session("scenarioNumber")) & "," & aRotationID,arrCorrNl,dictCorrNl)
'	   Response.Write("    </tr>" & vbCr)
'	   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
''	   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
'	   Response.Write("      <th align=""right"" colspan=""2"">&nbsp;</th>" & vbCr)
'	'   Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
'	   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Round(arrCorrNHa(0,dictCorrNHa("ResultParameterValue")),2)&"</th>" & vbCr)
'	   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Round(arrCorrNl(0,dictCorrNl("ResultParameterValue")),2)&"</th>" & vbCr)
'	   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
'	   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
'	   Response.Write("    </tr>" & vbCr)
'     set dictCorrNHa = nothing
'     set dictCorrNl = nothing
next
if totalArea<>0 then
   meanNLES_N = TotalNLES_N/totalArea
	 meanRunOff = TotalRunOff/totalArea
	 meanNLES_mg = 100*meanNLES_N*4.43/meanRunOff
else
   meanNLES_N = 0
	 meanRunOff = 0
	 meanNLES_mg = 0
end if
   Response.Write("  <tr><td colspan=""8"" style=""border-bottom: solid navy 1px;height: 1px;font-size: 0px;padding: 0px;>"">&nbsp;</td>" & vbCr)
   Response.Write("  </tr>" & vbCr)
   Response.Write("    </tr>" & vbCr)
  Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"" colspan=""2"">&nbsp;&nbsp;"&Str.Item("strMeanOnFarm")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Round(meanNLES_N,2)&"</th>" & vbCr)
   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Round(meanNLES_mg,2)&"</th>" & vbCr)
   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Round(meanRunOff,2)&"</th>" & vbCr)
   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   Response.Write("  </table>" & vbCr)
end sub
'**************************************************************************************************
%>
