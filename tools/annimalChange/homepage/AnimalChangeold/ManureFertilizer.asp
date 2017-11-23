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

Dim nManureFertilizerDelivery,arrManureFertilizerDelivery,dictManureFertilizerDelivery,MeanUtilization, totalManureNContent, totalManureShareOfNNeed
Dim arrA  ' array of ManureTypes
Dim arrB  ' array of fields(FieldPlanRotations)
Dim arrFertOut, arrFieldOut,scale,warning
Dim dummy1, dummy2, NPercent, nFieldPlanRotation
Dim arrFieldPlanRotation, dictFieldPlanRotation
Dim nFarmDelivery, dictFarmDelivery, arrFarmDelivery
Dim nFarmDeliverys, dictFarmDeliverys, arrFarmDeliverys
dim dictManureNContent, arrManureNContent, nManureNContent
dim dictManureNModelled, arrManureNModelled, nManureNModelled
dim monthString, aMonth

version = Session("version")
''**************************************************************************************************
sub CreateTabManure()
dim r
'   CalculateManure version,NPercent,scale,arrA,arrB,warning, dummy1, dummy2
time14=now
InitFarmNDB
  
   CreateLanguageDictionary
   Response.Write("<script language=""javascript"">window.parent.frames[1].location.reload()</script>" & vbCr)
   DefineManureListJScript_27
' response.write "<br/>" & Str.Item("strDistributionVersion") & ": "& version

   Response.Write("<br>" & vbCr) 
'Tabel som bestemmer indkøbt handelsgødning.   
   Response.Write("<h3>"&Str.Item("strNContentAbStorage")&"</h3><br/>" & vbCr) 
   set dictManureNModelled = CreateObject("Scripting.Dictionary")
   nManureNModelled = CallSP ("GetManureBalance " & Session("farmNumber") & "," & Session("scenarioNumber"), arrManureNModelled, dictManureNModelled)

     Response.Write("  <table border=""0"" cellpadding=""5"" cellspacing=""3"">" & vbCr)
      Response.Write("    <tr>" & vbCr)
      Response.Write("     <th align=""left"">"&Str.Item("strMANURETYPE")&"</th>" & vbCr)
'      Response.Write("     <td></td><th align=""left"">"&Str.Item("strKGNBARE")&"</th>" & vbCr)
   Response.Write("      <td></td><th align=""left"" colspan=""2"">"&Str.Item("strFIELDPROD")&"</th>" & vbCr)
   Response.Write("      <th align=""left"" colspan=""2"">"&Str.Item("strBOUGHT")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"" colspan=""2"">"&Str.Item("strSOLD")&"</th>" & vbCr)
   Response.Write("      <th align=""left"" colspan=""2"">"&Str.Item("strHOUSINGUSE")&"</th>" & vbCr)
      Response.Write("    </tr>" & vbCr)
   for r = 0 to nManureNModelled-1
   if (arrManureNModelled(r,dictManureNModelled.Item("AmountProduced"))<> 0 or arrManureNModelled(r,dictManureNModelled.Item("Bought"))<> 0 )then
      Response.Write("    <tr>" & vbCr)  
      Response.Write("      <th align=""left"">"&arrManureNModelled(r,dictManureNModelled.Item("StorageName"))&"</th>" & vbCr)
      Response.Write("      <td></td><td align=""left"">"&FormatNumber(arrManureNModelled(r,dictManureNModelled.Item("AmountProduced")),1)&"</td>" & vbCr)
      Response.Write("      <td></td><td align=""left"">"&FormatNumber(arrManureNModelled(r,dictManureNModelled.Item("Bought")),1)&"</td>" & vbCr)
'      Response.Write("      <td></td><td align=""left"">"&FormatNumber(arrManureNModelled(r,dictManureNModelled.Item("Sold")),1)&"</td>" & vbCr)
      Response.Write("      <td></td><td align=""left"">"&FormatNumber(arrManureNModelled(r,dictManureNModelled.Item("Amount")),1)&"</td>" & vbCr)
      Response.Write("    </tr>" & vbCr)
   end if 
   next   
      Response.Write("    <tr><td></td>" & vbCr)
      Response.Write("    </tr>" & vbCr)
     Response.Write("  </table>" & vbCr)
 
   Response.Write("<br>" & vbCr) 

   Response.Write("<h3>"&Str.Item("strDISTMANURE")&"</h3><br/>" & vbCr) 

'response.write   totalManureShareOfNNeed 
'   if totalManureShareOfNNeed <>0 then
   Response.Write("<form name=""formManureFertilizer"" action=""ManureFertilizerUpdate.asp"" method=""post"" accept-charset=""iso-8859-1"" id=""dynamichide"" style=""display: block;"">" & vbCr)
     Response.Write("  <table border=""0"" cellpadding=""3"" cellspacing=""0"">" & vbCr)
	  
     set dictFieldPlanRotation = CreateObject("Scripting.Dictionary")
	 
     nFieldPlanRotation = CallSP ("GetFieldplanRotationsForManure " & CLng(Session("farmNumber")) & "," & CInt(Session("scenarioNumber")) ,arrFieldPlanRotation,dictFieldPlanRotation)

   set dictFarmDelivery = CreateObject("Scripting.Dictionary")
   nFarmDelivery = CallSP("GetSelectListDeliveryType ",arrFarmDelivery,dictFarmDelivery)
   set dictManureNContent = CreateObject("Scripting.Dictionary")
'   nManureNContent = CallSP ("GetModelledManure " & Session("farmNumber") & "," & Session("scenarioNumber"), arrManureNContent, dictManureNContent)
   nManureNContent = CallSP ("GetManure_N_Content " & Session("farmNumber") & "," & Session("scenarioNumber"), arrManureNContent, dictManureNContent)
	  CreateTableManureDelivery
'	 if totalManureNContent=0 then
'	  response.write("<br/><h3>"&Str.Item("strNOPOO")&"</h3>" & vbCr) 
'     end if
'   else
'    response.write("<tr><td><br/><h3>"&Str.Item("strNOROT")&"</h3></td></tr>" & vbCr)
'   end if

'   Response.Write("<br>" & vbCr) 
'   Response.Write("<br>" & vbCr) 
'   if scale >-1 then 
'     Response.Write("&nbsp;&nbsp; " & FormatNumber(scale*100,1,-1,0,0)& Str.Item("strHGopfyldelses%") & vbCr) 
'   end if
'
'   if warning = 1 then 
'     Response.Write("<br>" & vbCr) 
'     Response.Write("<br>&nbsp;&nbsp; " & Str.Item("strTvungetSalg af HG") & vbCr) 
'   end if
'
	

   Response.Write("<br/>" & vbCr)   
   Response.Write("<br/>" & vbCr)   
CloseFarmNDB
time15=now
end sub
''**************************************************************************************************
sub CreateTableManureDelivery()
dim f,a,b,i,nm, nf, r
dim totalNDelivered, totalNLoss
dim aDeliveryID, aDeliveryAmount
dim aStorageID, fpID
dim manureString,manureString1, deliveryString, amountString


      Response.Write("    <tr>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strFIELD")&"<br/></th>" & vbCr)
'      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strCARRYOVER")&"<br/></th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strCROP")&"</th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strTYPEDIST")&"<br/>&nbsp;&nbsp;"&Str.Item("strfertilizer")&"<br/></th>" & vbCr)
      Response.Write("      <th align=""left""><span style=""position:relative;left:80px;"">"&Str.Item("strSPREADER")&"<br/>"&Str.Item("strMETHOD")&"</span></th>" & vbCr)
      Response.Write("      <th align=""left"">"&Str.Item("strKGNBARE")&"/"&Str.Item("strHA")&"</th>" & vbCr)
'      Response.Write("      <th align=""right"">"&Str.Item("strKGNBARE")&" "&Str.Item("strialtlille")&"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<br/>/"&Str.Item("strHA")&"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
'      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strKGNNeed")&"<br/>&nbsp;&nbsp;"&Str.Item("strFulfilled")&"<br/>&nbsp;&nbsp;"&Str.Item("strKGHA")&"</th>" & vbCr)
'      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strSPREAD")&"<br/>&nbsp;&nbsp;"&Str.Item("strKGHA")&"</th>" & vbCr)
'      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strSPREADLOSS")&"<br/>&nbsp;&nbsp;"&Str.Item("strKGHA")&"</th>" & vbCr)
'      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strAREA")&"<br/>&nbsp;&nbsp;"&Str.Item("strHA")&"</th>" & vbCr)
      Response.Write("    </tr>" & vbCr)
      
	  
      for a = 0 to Ubound(arrFieldPlanRotation)
	  fpID = arrFieldPlanRotation(a,dictFieldPlanRotation.item("FieldPlanRotationID"))
      Response.Write("    <tr>" & vbCr)

		       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrFieldPlanRotation(a,dictFieldPlanRotation.item("RotationName"))&"</td>" & vbCr)
		       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Str.Item(""&"str"&arrFieldPlanRotation(a,dictFieldPlanRotation.item("CropName"))&"")&"</td>" & vbCr)
		       Response.Write("      <td align=""left"" colspan=""4"">" & vbCr)
			   '''
   Response.Write("<table border=""0"" cellspacing=""0"" width=""100%"" style=""position:relative; top: -8px;"">" & vbCr)
   Response.Write("<tbody id=""tbodyManure_"&a&""" name=""tbodyManure_"&a&""">" & vbCr)
   set dictManureFertilizerDelivery = CreateObject("Scripting.Dictionary")
   nManureFertilizerDelivery = CallSP ("GetManureFertilizerDelivery " & CLng(Session("farmNumber")) & "," & CInt(Session("scenarioNumber")) & "," & arrFieldPlanRotation(a,dictFieldPlanRotation.item("RotationID")) & "," & arrFieldPlanRotation(a,dictFieldPlanRotation.item("FieldPlanRotationID")) ,arrManureFertilizerDelivery,dictManureFertilizerDelivery)''ændres, så den kaldes for hver fieldplanrotationID
   manureString = ""
   manureString1 = ""
   deliveryString = ""
   amountString = ""
   monthString = ""
for i = 0 to nManureFertilizerDelivery-1
   Response.Write("<tr>" & vbCr)
   Response.Write("<td>" & vbCr)
	       Response.Write("<select id=""selManure_"&a&""" name=""selManure_"&a&""" onchange=""checkForNewLine("&a&");setManureString("&a&");"" style=""position:relative;top:-3Px;background-color: white;"" onkeydown=""if(event.keyCode==13) {veil();document.forms.formManureFertilizer.action='ManureFertilizerUpdate.asp';document.forms.formManureFertilizer.submit();};"">" & vbCr)
		   Response.write("  <option value=""-1"">"&Str.item("strSelectManureFertilizer")&"</option>" & vbCr)
		   for nf = 0 to nManureNContent-1
           if nManureFertilizerDelivery=0 then
			     aStorageID = -1
           else
		   	   if ubound(arrManureNContent)<>-1 then 
			     if nf < nManureNContent then
				  aStorageID = arrManureFertilizerDelivery(i,5)
   set dictFarmDeliverys = CreateObject("Scripting.Dictionary")
   nFarmDeliverys = CallSP("GetSelectListDeliveryTypePrStorageType "&aStorageID,arrFarmDeliverys,dictFarmDeliverys)
				 end if
			   else
			     aStorageID = -1
				 manureString = manureString & ","
				 amountString = amountString & ";"
			   end if
           end if
		      if Cstr(aStorageID) = Cstr(arrManureNContent(nf,0)) then
		         Response.write("  <option value="""&arrManureNContent(nf,0)&""" selected>"&Str.Item("str"&arrManureNContent(nf,1)&"")&"</option>" & vbCr)
		      else
		         Response.write("  <option value="""&arrManureNContent(nf,0)&""">"&Str.Item("str"&arrManureNContent(nf,1)&"")&"</option>" & vbCr)
		      end if
		   next
'		   Response.write("  <option value=""9""")
'		   if cint(arrManureFertilizerDelivery(i,5)) = 9 then
'		   Response.write (" selected")
'		   end if
'		   Response.write(">"&Str.item("strMINFERT")&"</option>" & vbCr)
		   Response.write("  <option value=""31""")
		   if cint(arrManureFertilizerDelivery(i,5)) = 31 then
		   Response.write (" selected")
		   end if
		   Response.write(">"&Str.item("strAmmonium nitrate")&"</option>" & vbCr)
		   Response.write("  <option value=""32""")
		   if cint(arrManureFertilizerDelivery(i,5)) = 32 then
		   Response.write (" selected")
		   end if
		   Response.write(">"&Str.item("strAmmonium sulphate")&"</option>" & vbCr)
		   Response.write("  <option value=""33""")
		   if cint(arrManureFertilizerDelivery(i,5)) = 33 then
		   Response.write (" selected")
		   end if
		   Response.write(">"&Str.item("strCalcium ammonium nitrate")&"</option>" & vbCr)
		   Response.write("  <option value=""34""")
		   if cint(arrManureFertilizerDelivery(i,5)) = 34 then
		   Response.write (" selected")
		   end if
		   Response.write(">"&Str.item("strCalcium nitrate")&"</option>" & vbCr)
		   Response.write("  <option value=""35""")
		   if cint(arrManureFertilizerDelivery(i,5)) = 35 then
		   Response.write (" selected")
		   end if
		   Response.write(">"&Str.item("strDiammonium phosphate")&"</option>" & vbCr)
		   Response.write("  <option value=""36""")
		   if cint(arrManureFertilizerDelivery(i,5)) = 36 then
		   Response.write (" selected")
		   end if
		   Response.write(">"&Str.item("strMonoammonium phosphate")&"</option>" & vbCr)
		   Response.write("  <option value=""37""")
		   if cint(arrManureFertilizerDelivery(i,5)) = 37 then
		   Response.write (" selected")
		   end if
		   Response.write(">"&Str.item("strSulphur-coated urea")&"</option>" & vbCr)
		   Response.write("  <option value=""38""")
		   if cint(arrManureFertilizerDelivery(i,5)) = 38 then
		   Response.write (" selected")
		   end if
		   Response.write(">"&Str.item("strUrea ammonium nitrate")&"</option>" & vbCr)
		   Response.write("  <option value=""39""")
		   if cint(arrManureFertilizerDelivery(i,5)) = 39 then
		   Response.write (" selected")
		   end if
		   Response.write(">"&Str.item("strUrea ammonium sulphate")&"</option>" & vbCr)
		   Response.write("  <option value=""40""")
		   if cint(arrManureFertilizerDelivery(i,5)) = 40 then
		   Response.write (" selected")
		   end if
		   Response.write(">"&Str.item("strUrea")&"</option>" & vbCr)
		   Response.write("  <option value=""41""")
		   if cint(arrManureFertilizerDelivery(i,5)) = 41 then
		   Response.write (" selected")
		   end if
		   Response.write(">"&Str.item("strNitrateInhibitor")&"</option>" & vbCr)
		   Response.write("  <option value=""42""")
		   if cint(arrManureFertilizerDelivery(i,5)) = 42 then
		   Response.write (" selected")
		   end if
		   Response.write(">"&Str.item("strNPK-fertiliser")&"</option>" & vbCr)
		   
		   Response.Write("</select>" & vbCr)
	       Response.Write("<select id=""selDelivery_"&a&""" name=""selDelivery_"&a&""" onchange=""setDeliveryString("&a&");"" style=""width: 180px;position:relative;top:-3Px;background-color: white;"" onkeydown=""if(event.keyCode==13) {veil();document.forms.formManureFertilizer.action='ManureFertilizerUpdate.asp';document.forms.formManureFertilizer.submit();};"">" & vbCr)
		   Response.write("  <option value=""-1"">"&Str.item("strSelectDeliveryMethod")&"</option>" & vbCr)
if nManureFertilizerDelivery>0 then
		   for nm = 0 to nFarmDelivery-1
		   if nManureFertilizerDelivery>0 then 
			  aDeliveryID = arrManureFertilizerDelivery(i,7)
			  aDeliveryAmount = arrManureFertilizerDelivery(i,9)'/arrManureFertilizerDelivery(i,11)
			  aMonth = arrManureFertilizerDelivery(i,13)
		   else
		     aDeliveryID = -1
			 aDeliveryAmount = 0
			 aMonth = -1
		   end if
		      if aDeliveryID = Cstr(arrFarmDelivery(nm,0)) then
		         Response.write("  <option value="""&arrFarmDelivery(nm,0)&""" selected>"&Str.Item("str"&arrFarmDelivery(nm,1)&"")&"</option>" & vbCr)
		      else
		         Response.write("  <option value="""&arrFarmDelivery(nm,0)&""">"&Str.Item("str"&arrFarmDelivery(nm,1)&"")&"</option>" & vbCr)
		      end if
		   next
else
		   for nm = 0 to nFarmDelivery-1
		   if nManureFertilizerDelivery>0 then 
			  aDeliveryID = arrManureFertilizerDelivery(i,7)
			  aDeliveryAmount = arrManureFertilizerDelivery(i,9)'/arrManureFertilizerDelivery(i,11)
			  aMonth = arrManureFertilizerDelivery(i,13)
		   else
		     aDeliveryID = -1
			 aDeliveryAmount = 0
			 aMonth = -1
		   end if
		      if aDeliveryID = Cstr(arrFarmDelivery(nm,0)) then
		         Response.write("  <option value="""&arrFarmDelivery(nm,0)&""" selected>"&Str.Item("str"&arrFarmDelivery(nm,1)&"")&"</option>" & vbCr)
		      else
		         Response.write("  <option value="""&arrFarmDelivery(nm,0)&""">"&Str.Item("str"&arrFarmDelivery(nm,1)&"")&"</option>" & vbCr)
		      end if
		   next
end if		   
		   if nManureFertilizerDelivery>0 then
			  manureString1 = manureString1 & arrManureFertilizerDelivery(i,5) & ","
			  deliveryString = deliveryString & arrManureFertilizerDelivery(i,7)& ","
			  amountString = amountString & arrManureFertilizerDelivery(i,9)& ";"'/arrManureFertilizerDelivery(i,11)& ";"
			  monthString = monthString & arrManureFertilizerDelivery(i,13)& ","
		   else
		     manureString1 = manureString1 & ","
			 deliveryString = deliveryString & ","
			 amountString = amountString & ";"
			 monthString = monthString & ","
		   end if
		   Response.Write("</select>" & vbCr)
   Response.Write("<input id=""Amount_"&a&""" name=""Amount_"&a&""" type=""text"" style=""width: 55px;position:relative;top:-3Px;background-color: white;"" value="""&aDeliveryAmount&""" onchange=""setAmountString("&a&");"" onkeydown=""if(event.keyCode==13) {veil();document.forms.formManureFertilizer.action='ManureFertilizerUpdate.asp';document.forms.formManureFertilizer.submit();};""/>" & vbCr)
CreateMonthSelect a,"white",aMonth
   Response.Write("<img style=""position:relative;top:3Px;"" src=""images/del.gif"" onclick=""delRow(this,"&a&");"" onMouseover=""mouseOverGif(this)""  onMouseout=""mouseOutGif(this)""/>" & vbCr)
   Response.Write("</td>" & vbCr)
   Response.Write("</tr>" & vbCr)
   set dictFarmDeliverys = nothing
next
   Response.Write("<tr>" & vbCr)
   Response.Write("<td>" & vbCr)
	       Response.Write("<select id=""selManure_"&a&""" name=""selManure_"&a&""" onchange=""checkForNewLine("&a&");setManureString("&a&");"" style=""position:relative;top:-3Px;background-color: lightyellow;"">" & vbCr)
		   Response.write("  <option value=""-1"">"&Str.item("strSelectManureFertilizer")&"</option>" & vbCr)
		   for nf = 0 to nManureNContent-1
		         Response.write("  <option value="""&arrManureNContent(nf,0)&""">"&Str.Item("str"&arrManureNContent(nf,1)&"")&"</option>" & vbCr)
		   next
'		   Response.write("  <option value=""9""")
'		   Response.write(">"&Str.item("strMINFERT")&"</option>" & vbCr)
		   Response.write("  <option value=""31""")
		   Response.write(">"&Str.item("strAmmonium nitrate")&"</option>" & vbCr)
		   Response.write("  <option value=""32""")
		   Response.write(">"&Str.item("strAmmonium sulphate")&"</option>" & vbCr)
		   Response.write("  <option value=""33""")
		   Response.write(">"&Str.item("strCalcium ammonium nitrate")&"</option>" & vbCr)
		   Response.write("  <option value=""34""")
		   Response.write(">"&Str.item("strCalcium nitrate")&"</option>" & vbCr)
		   Response.write("  <option value=""35""")
		   Response.write(">"&Str.item("strDiammonium phosphate")&"</option>" & vbCr)
		   Response.write("  <option value=""36""")
		   Response.write(">"&Str.item("strMonoammonium phosphate")&"</option>" & vbCr)
		   Response.write("  <option value=""37""")
		   Response.write(">"&Str.item("strSulphur-coated urea")&"</option>" & vbCr)
		   Response.write("  <option value=""38""")
		   Response.write(">"&Str.item("strUrea ammonium nitrate")&"</option>" & vbCr)
		   Response.write("  <option value=""39""")
		   Response.write(">"&Str.item("strUrea ammonium sulphate")&"</option>" & vbCr)
		   Response.write("  <option value=""40""")
		   Response.write(">"&Str.item("strNitrateInhibitor")&"</option>" & vbCr)
		   Response.write("  <option value=""41""")
		   Response.write(">"&Str.item("strUrea")&"</option>" & vbCr)
		   Response.write("  <option value=""42""")
		   Response.write(">"&Str.item("strNPK-fertiliser")&"</option>" & vbCr)
		   Response.Write("</select>" & vbCr)
	       Response.Write("<select id=""selDelivery_"&a&""" name=""selDelivery_"&a&""" onchange=""setDeliveryString("&a&");"" style=""width: 180px;position:relative;top:-3Px;background-color: lightyellow;"">" & vbCr)
		   Response.write("  <option value=""-1"">"&Str.item("strSelectDeliveryMethod")&"</option>" & vbCr)

		   for nm = 0 to nFarmDelivery-1
		         Response.write("  <option value="""&arrFarmDelivery(nm,0)&""">"&Str.Item("str"&arrFarmDelivery(nm,1)&"")&"</option>" & vbCr)
		   next
		   Response.Write("</select>" & vbCr)
   Response.Write("<input id=""Amount_"&a&""" name=""Amount_"&a&""" type=""text"" style=""width: 55px;position:relative;top:-3Px;background-color: lightyellow;"" value=""0"" onchange=""setAmountString("&a&");""/>" & vbCr)
CreateMonthSelect a,"lightyellow",-1
   Response.Write("<img style=""position:relative;top:3Px;"" src=""images/del.gif"" onclick=""delRow(this,"&a&");"" onMouseover=""mouseOverGif(this)""  onMouseout=""mouseOutGif(this)""/>" & vbCr)
   Response.Write("</td>" & vbCr)
   Response.Write("</tr>" & vbCr)
   Response.Write("</tbody>" & vbCr)
   Response.Write("</table>" & vbCr)
   '''
		       Response.Write("      </td>" & vbCr)
'if  nManureFertilizerDelivery<>0 then
'		       Response.Write("      <td align=""left"">")
'			   for i = 0 to nManureFertilizerDelivery-1
'			     Response.Write("&nbsp;&nbsp;<span id=""util_"&a&"_"&i&""">"&FormatNumber(arrManureFertilizerDelivery(i,dictManureFertilizerDelivery.item("Kg_N_Utilized"))/arrManureFertilizerDelivery(i,dictManureFertilizerDelivery.item("Area")),1,-1,0,0)&"</span><br/><br/>")
'			   next
'			   Response.Write("</td>"& vbCr)
'		       Response.Write("      <td align=""left"">")
'			   for i = 0 to nManureFertilizerDelivery-1
'			     Response.Write("&nbsp;&nbsp;<span id=""deliv_"&a&"_"&i&""">"&FormatNumber(arrManureFertilizerDelivery(i,dictManureFertilizerDelivery.item("Kg_N_Delivered"))/arrManureFertilizerDelivery(i,dictManureFertilizerDelivery.item("Area")),1,-1,0,0)&"</span><br/><br/>")
'			   next
'			   Response.Write("</td>"& vbCr)
'		       Response.Write("      <td align=""left"">")
'			   for i = 0 to nManureFertilizerDelivery-1
'			     Response.Write("&nbsp;&nbsp;<span id=""loss_"&a&"_"&i&""">"&FormatNumber(arrManureFertilizerDelivery(i,dictManureFertilizerDelivery.item("N_Loss"))/arrManureFertilizerDelivery(i,dictManureFertilizerDelivery.item("Area")),1,-1,0,0)&"</span><br/><br/>")
'			   next
'			   Response.Write("</td>"& vbCr)
'		       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrFieldPlanRotation(a,dictFieldPlanRotation.item("Area"))&"</td>" & vbCr)
'else
'		       Response.Write("      <td align=""left"">&nbsp;&nbsp;</td>"& vbCr)
'		       Response.Write("      <td align=""left"">&nbsp;&nbsp;</td>"& vbCr)
'		       Response.Write("      <td align=""left"">&nbsp;&nbsp;</td>"& vbCr)
'		       Response.Write("      <td align=""left"">&nbsp;&nbsp;</td>" & vbCr)
'end if
       
      Response.Write("    </tr>" & vbCr)
      Response.Write("<tr><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td><button style=""width: 50px; height: 20px; font-size: 10px"" onClick=""{veil();document.forms.formManureFertilizer.action='ManureFertilizerUpdate.asp';document.forms.formManureFertilizer.submit();}"">"&Str.Item("strSAVE")&"</button></td></tr>")
      Response.Write("    <tr>" & vbCr)
      Response.Write("      <td colspan=""10""><hr/></td>" & vbCr)
      Response.Write("    </tr>" & vbCr)
   Response.Write("<input type=""hidden"" name=""hiddenManureList_"&a&""" id=""hiddenManureList_"&a&""" value="""&manureString1&""" />" & vbCr)
   Response.Write("<input type=""hidden"" name=""hiddenDeliveryList_"&a&""" id=""hiddenDeliveryList_"&a&""" value="""&deliveryString&""" />" & vbCr)
   Response.Write("<input type=""hidden"" name=""hiddenAmountList_"&a&""" id=""hiddenAmountList_"&a&""" value="""&amountString&""" />" & vbCr)
   Response.Write("<input type=""hidden"" name=""hiddenMonthList_"&a&""" id=""hiddenMonthList_"&a&""" value="""&monthString&""" />" & vbCr)
   set dictManureFertilizerDelivery = nothing
next

''      Response.Write("    <tr>" & vbCr)
''      Response.Write("      <td colspan=""10""><hr/></td>" & vbCr)
''      Response.Write("    </tr>" & vbCr)
'      Response.Write("    <tr>" & vbCr)
''      Response.Write("      <td align=""left"" colspan=""4""><h4>Sædskifte navn: "&arrRotation(1,r)&"</td>" & vbCr)
'      Response.Write("      <th align=""left"">&nbsp;&nbsp;<br/></th>" & vbCr)
'      Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
'      Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
'      Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
'      Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
'      Response.Write("      <th align=""left"" colspan=2>&nbsp;&nbsp;"&Str.Item("strIalt")&":</th>" & vbCr)
'      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&FormatNumber(totalNDelivered,0,-1,0,0)&"</th>" & vbCr)
'      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&FormatNumber(totalNLoss,0,-1,0,0)&"</th>" & vbCr)
      Response.Write("    </tr>" & vbCr)
'      Response.Write("<tr><td><button style=""width: 50px; height: 20px; font-size: 10px"" onClick=""{veil();document.forms.formManureFertilizer.action='ManureFertilizerUpdate.asp';document.forms.formManureFertilizer.submit();}"">"&Str.Item("strSAVE")&"</button></td></tr>")
   Response.Write("  </table>" & vbCr)
   Response.Write("<input type=""hidden"" name=""hiddenNFieldPlanRotation"" value="""&nFieldPlanRotation&"""/>" & vbCr)
   Response.Write("</form>" & vbCr)
   CreateUpdateFormManure
   
end sub
'**************************************************************************************************
sub CreateMonthSelect (a,color,value)

	       Response.Write("<select id=""selMonth_"&a&""" name=""selMonth_"&a&""" style=""position:relative;top:-3Px;background-color: "&color&";"" onchange=""setMonthString("&a&");"" onkeydown=""veil();document.forms.formManureFertilizer.action='ManureFertilizerUpdate.asp';document.forms.formManureFertilizer.submit();"">" & vbCr)
		   Response.write("  <option value=""-1""")
		   Response.write(">"&Str.item("strSelectDeliveryMonth")&"</option>" & vbCr)
		   Response.write("  <option value=""1""")
		   if value = 1 then response.write " selected"
		   Response.write(">"&Str.Item("strJanuary")&"</option>" & vbCr)
		   Response.write("  <option value=""2""")
		   if value = 2 then response.write " selected"
		   Response.write(">"&Str.Item("strFebruary")&"</option>" & vbCr)
		   Response.write("  <option value=""3""")
		   if value = 3 then response.write " selected"
		   Response.write(">"&Str.Item("strMarch")&"</option>" & vbCr)
		   Response.write("  <option value=""4""")
		   if value = 4 then response.write " selected"
		   Response.write(">"&Str.Item("strApril")&"</option>" & vbCr)
		   Response.write("  <option value=""5""")
		   if value = 5 then response.write " selected"
		   Response.write(">"&Str.Item("strMay")&"</option>" & vbCr)
		   Response.write("  <option value=""6""")
		   if value = 6 then response.write " selected"
		   Response.write(">"&Str.Item("strJune")&"</option>" & vbCr)
		   Response.write("  <option value=""7""")
		   if value = 7 then response.write " selected"
		   Response.write(">"&Str.Item("strJuly")&"</option>" & vbCr)
		   Response.write("  <option value=""8""")
		   if value = 8 then response.write " selected"
		   Response.write(">"&Str.Item("strAugust")&"</option>" & vbCr)
		   Response.write("  <option value=""9""")
		   if value = 9 then response.write " selected"
		   Response.write(">"&Str.Item("strSeptember")&"</option>" & vbCr)
		   Response.write("  <option value=""10""")
		   if value = 10 then response.write " selected"
		   Response.write(">"&Str.Item("strOctober")&"</option>" & vbCr)
		   Response.write("  <option value=""11""")
		   if value = 11 then response.write " selected"
		   Response.write(">"&Str.Item("strNovember")&"</option>" & vbCr)
		   Response.write("  <option value=""12""")
		   if value = 12 then response.write " selected"
		   Response.write(">"&Str.Item("strDecember")&"</option>" & vbCr)
		   Response.Write("</select>" & vbCr)
		   
'	       Response.Write("<select id=""selMonth_"&a&""" name=""selMonth_"&a&""" onchange=""setFeedstuffString("&a&");blank("&a&","&i&");"" style=""position:relative;top:-3Px;background-color: lightyellow;"">" & vbCr)
'		   Response.write("  <option value=""-1"">"&Str.item("strSelectDeliveryMonth")&"</option>" & vbCr)
'		   for nm = 0 to nFarmDelivery-1
'		         Response.write("  <option value="""&arrFarmDelivery(nm,0)&""">"&Str.Item("str"&arrFarmDelivery(nm,1)&"")&"</option>" & vbCr)
'		   next
'		   Response.Write("</select>" & vbCr)

end sub
'**************************************************************************************************
sub CreateUpdateFormManure()
   Response.Write("<form name=""update"" action=""javascript:document.forms.formManureFertilizer.submit();"" method=""post"">" & vbCr)
   Response.Write("<input type=""hidden"" name=""saveUpdates"" value=""false"" />" & vbCr)
   Response.Write("</form>" & vbCr)
end sub
'**************************************************************************************************
sub DefineManureListJScript_27()
%>
<script language="javascript">

   function checkForNewLine(a)
   {
   	var oTBody = document.getElementById('tbodyManure_'+a+'');
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
 
   function delRow(oElement,a)
   {
   	var oTBody = document.getElementById ('tbodyManure_'+a+'');
   	var actTBodyRowIndex = getSectionRowIndex(oElement);
/*    	if (actTBodyRowIndex == 0) //første række i body - efter overskriften
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
   	{*/
   		oTBody.deleteRow(actTBodyRowIndex);
//   	}
		setManureString(a);
		setDeliveryString(a);
		setAmountString(a);
		setMonthString(a);
   }
 
   function getManure(a){
   	var cManure = document.getElementsByName('selManure_'+a+'');
   	var strManure = "";
   	for (var i = 0; i < cManure.length; i++)
   	{
   		if (parseInt(cManure[i].value,10) > 0)
   		{
   				strManure += cManure[i].value +", ";
   		}
   	}
   	return strManure;
   }

   function getDelivery(a){
   	var cDelivery = document.getElementsByName('selDelivery_'+a+'');
   	var strDelivery = "";
   	for (var i = 0; i < cDelivery.length; i++)
   	{
   		if (parseInt(cDelivery[i].value,10) > 0)
   		{
   				strDelivery += cDelivery[i].value +", ";
   		}
   	}
   	return strDelivery;
   }

   function getAmount(a){
   	var cManure = document.getElementsByName('selManure_'+a+'');
   	var strManure = "";
   	var cAmount = document.getElementsByName('Amount_'+a+'');
   	var strAmount = "";
   	for (var i = 0; i < cManure.length; i++)
   	{
   		if (parseInt(cManure[i].value,10) > 0)
   		{
   				strAmount += cAmount[i].value +"; ";
   		}
   	}
   	return strAmount;
   }

   function getMonths(a){
   	var cMonth = document.getElementsByName('selMonth_'+a+'');
   	var strMonth = "";
   	for (var i = 0; i < cMonth.length; i++)
   	{
   		if (parseInt(cMonth[i].value,10) > 0)
   		{
   				strMonth += cMonth[i].value +", ";
   		}
   	}
   	return strMonth;
   }

   function blank(a,i){
   	var util = document.getElementById('util_'+a+'_'+i+'');
   	var deliv = document.getElementById('deliv_'+a+'_'+i+'');
   	var loss = document.getElementById('loss_'+a+'_'+i+'');
	util.innerHTML='';
	deliv.innerHTML='';
	loss.innerHTML='';
   }

 
   function mouseOverGif(oImg)
   {
   	oImg.setAttribute("src","Images/del1.gif");
   }
   
   function mouseOutGif(oImg)
   {
   	oImg.setAttribute("src","Images/del.gif");
   }
 
   function setManureString(a){
   	document.getElementById('hiddenManureList_'+a+'').value = getManure(a);
   }
   function setDeliveryString(a){
   	document.getElementById('hiddenDeliveryList_'+a+'').value = getDelivery(a);
   }
   function setAmountString(a){
   	document.getElementById('hiddenAmountList_'+a+'').value = getAmount(a);
   }
   function setMonthString(a){
   	document.getElementById('hiddenMonthList_'+a+'').value = getMonths(a);
   }
</script>
<%
end sub
'**************************************************************************************************
%>
