<%
'**************************************************************************************************
'**************************************************************************************************
sub CreateTabFarm()
   dim nFarm, arrFarm
   dim nScenario, arrScenario, aScenario
   dim nSoilType, arrSoilType, aSoilType
   dim nIrrigationType, arrIrrigationType, aIrrigationType
   dim nDeliveryType, arrDeliveryType, aDeliveryType, i
   dim nFarmDelivery, arrFarmDelivery, dictFarmDelivery, p
   dim nFarmType, arrFarmType
   dim nYieldLevel, arrYieldLevel, aYieldLevel
   dim nStorageType, arrStorageType 
   dim nBoughtManure, arrBoughtManure , bm 
   dim nSoldManure, arrSoldManure , sm
   dim dictFarm, dictScenario, dictSoilType,dictIrrigation, dictFarmType, dictStorageType, dictBoughtManure, dictSoldManure
   dim dictCropYear, arrCropYear, nCropYear, aCropYear
   dim dictAgroEcologicalZone, arrAgroEcologicalZone, nAgroEcologicalZone, aAgroEcologicalZone
   dim manureVersion
time4=now   
   InitFarmNDB
   CreateLanguageDictionary
   set dictFarm = CreateObject("Scripting.Dictionary")
   nFarm = CallSPStr ("GetFarmData " & CLng(Session("farmNumber")), arrFarm, dictFarm)	   
   set dictScenario = CreateObject("Scripting.Dictionary")
   nScenario = CallSPStr ("GetSelectListScenario " & CLng(Session("farmNumber")), arrScenario, dictScenario)	   
   set dictSoilType = CreateObject("Scripting.Dictionary")
   nSoilType = CallSPStr ("GetSelectListSoilType", arrSoilType, dictSoilType)	   
   set dictFarmType = CreateObject("Scripting.Dictionary")
   nFarmType = CallSPStr ("GetSelectListFarmType", arrFarmType, dictFarmType)	   
   set dictStorageType = CreateObject("Scripting.Dictionary")
   nStorageType = CallSPStr ("GetOrganicStorageTypesList", arrStorageType, dictStorageType)	   
'   nStorageType = CallSPStr ("getAllStorageTypesList", arrStorageType, dictStorageType)	   
   set dictCropYear = CreateObject("Scripting.Dictionary")
   nCropYear = CallSPStr ("GetSelectListCropYear", arrCropYear, dictCropYear)	   
   set dictAgroEcologicalZone = CreateObject("Scripting.Dictionary")
   nAgroEcologicalZone = CallSPStr ("GetAgroEcologicalZoneList", arrAgroEcologicalZone, dictAgroEcologicalZone)	   
   aAgroEcologicalZone = arrFarm(0,dictFarm.item("AgroEcologicalZone"))
   Session("AgroEcologicalZone")=aAgroEcologicalZone
   manureVersion = arrFarm(0,dictFarm.item("ManureVersion"))
   Session("version")=manureVersion
'dim x
'for each x in Session.Contents
'  response.write(x & "   -   " &Session(x)&"<br />")
'next

'   response.write "Session('AgroEcologicalZone') "&Session("AgroEcologicalZone")
'   nFarm        = GetFarmData(CLng(Session("farmNumber")),arrFarm)
'   nScenario    = GetSelectListScenario(CLng(Session("farmNumber")),arrScenario)
'   nSoilType    = GetSelectListSoilType(arrSoilType)
'   nFarmType    = GetSelectListFarmType(arrFarmType)
'   nStorageType = GetStorageTypeListNoGrazing(arrStorageType)
   
   redim arrIrrigationType(1,1)
   aScenario 				= arrFarm(0,dictFarm.item("SelectedScenario"))
   aCropYear				= arrFarm(0,dictFarm.item("CropYear"))
   aSoilType 				= arrFarm(0,dictFarm.item("SoilType"))
   aIrrigationType 			= arrFarm(0,dictFarm.item("Irrigation"))
   
	  if cbool(aIrrigationType)=True then
	     aIrrigationType=1
	  else
	     aIrrigationType=0
	  end if
'   nIrrigationType 			= 2
'   arrIrrigationType(0,0)	= 0
'   arrIrrigationType(1,0)	= str.Item("strRAINFED")
'   arrIrrigationType(0,1)	= 1
'   arrIrrigationType(1,1)	= str.Item("strIRRIGATED")
   set dictIrrigation = CreateObject("Scripting.Dictionary")
   nIrrigationType 			=  CallSPStr ("GetSelectListIrrigation", arrIrrigationType, dictIrrigation)	
   redim arrDeliveryType(1,2)
   aDeliveryType 			= Cint(arrFarm(0,dictFarm.item("Delivery")))

   nDeliveryType 			= 3
   arrDeliveryType(0,0)	    = 1
   arrDeliveryType(1,0)	    = str.Item("strDELIVERYINCORP")
   arrDeliveryType(0,1)	    = 2
   arrDeliveryType(1,1)	    = str.Item("strDELIVERYTUBE")
   arrDeliveryType(0,2)	    = 3
   arrDeliveryType(1,2)	    = str.Item("strBredspredning af fast husdyrgødning")
   

   set dictBoughtManure = CreateObject("Scripting.Dictionary")
   nBoughtManure = CallSPStr ("GetBoughtManureList " & Session("farmNumber") & "," & aScenario, arrBoughtManure, dictBoughtManure)	   
'   nBoughtManure = GetBoughtManureList(Session("farmNumber"),aScenario,arrBoughtManure)
   set dictSoldManure = CreateObject("Scripting.Dictionary")
   nSoldManure = CallSPStr ("GetSoldManureList " & Session("farmNumber") & "," & aScenario, arrSoldManure, dictSoldManure)	   
'   nSoldManure   = GetSoldManureList(Session("farmNumber"),aScenario,arrSoldManure)
   set dictFarmDelivery = CreateObject("Scripting.Dictionary")
   nFarmDelivery = CallSP("GetFarmDeliveryData "&CLng(Session("farmNumber")),arrFarmDelivery,dictFarmDelivery)
time20=now
   CloseFarmNDB


'   Response.write("<a href=""documentation/AnimalChange news.pdf"" target=""new""><img src=""images/info_rev.gif"" border=""no""/></a>" & vbCr)
   Response.Write("  <table class=""data"" border=""0"" style=""float: left;"">" & vbCr)

   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th align=""left"">"&str.Item("strFARMER")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
  Response.Write("      <td rowspan=""4"">" & vbCr)
Response.write("<h5>If local parameterfiles are present, the model will use these.<br/>" & vbCr)     
Response.write("For upload, the files can contain only one or two agroecological zones:<br/>If they are too big, they will not be uploaded.<br/>" & vbCr)     
Response.write("The uploaded files have to be named the same way as the default files.<br/>When local parameterfiles are removed. The model will use default-files<br/>" & vbCr)     
Response.write("<br/></h5>" & vbCr)     
   Response.Write("<form method=""post"" enctype=""multipart/form-data"" action=""uploadownparameters.asp"">" & vbCr)
   Response.Write("<table border=""0"">" & vbCr)
   Response.Write("<tr><td><b>Select a parameter-file or a feedstuff-file to upload:</b><br><input type=file size=""50"" name=""FILE1""></td></tr>" & vbCr)
   Response.Write("<tr><td><input type=""submit"" value=""Upload!""></td></tr>" & vbCr)
   Response.Write("</table>" & vbCr)
   Response.Write("</form>" & vbCr)
dim fs,fo,f,fldr, msg, files
fldr=cstr("c:/upload/AnimalChange/") & Session("FarmNumber")
  set fs=Server.CreateObject("Scripting.FileSystemObject")
   msg="You have no uploaded local files"
   If (fs.FolderExists(fldr)) Then
   msg = ""
      set fo = fs.GetFolder(fldr)
	    for each f in fo.files
		msg=msg  & f.name & "<br/>"
		files=true
	    next
		if files=true then
		msg ="<b>You have uploaded following parameter-files:</b><br/>" & msg
		end if

   Response.Write(msg & "<br/>" & vbCr)		
   Response.Write("<form action=""ClearLocalFiles.asp"" method=""post"">" & vbCr)
   if files=true then
   Response.Write(" <input type=""submit"" value=""Remove local parameter-files"">" & vbCr)
   end if
   Response.Write("</form>" & vbCr)
  end if		
   Response.Write("</td>" & vbCr)
   Response.Write("<script language=""javascript"">window.parent.frames[0].location.reload()</script>" & vbCr)
   Response.Write("<form name=""formFarm"" action=""FarmUpdate.asp"" method=""post"" id=""dynamichide"" onsubmit="""">" & vbCr)
   ' farmer information ***************************************************************************
   Response.Write("  <input type=""hidden"" name=""DeleteBoughtManureID"" id=""DeleteBoughtManureID"" value="""" />" & vbCr)
   Response.Write("  <input type=""hidden"" name=""DeleteSoldManureID"" id=""DeleteSoldManureID"" value="""" />" & vbCr)
   Response.Write("  <input type=""hidden"" name=""aDeleteScenarioID"" id=""aDeleteScenarioID"" value="""" />" & vbCr)

   Response.Write("    </tr>" & vbCr)   

   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th class=""data"" align=""left"">"&str.Item("strNAME")&"</th>" & vbCr)
   Response.Write("      <td class=""data"" align=""left""><input type=""text"" name=""inputName"" value="""&arrFarm(0,dictFarm.item("FarmOwnerName"))&""" maxlength=""180"" size=""50"" onChange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true';""/></td>" & vbCr)
'   Response.Write("      <td class=""data"" align=""left"" rowspan=""5""><textarea rows=""10"" cols=""30"" name=""inputDescription"" value="""&arrFarm(0,dictFarm.item("FarmDescription"))&""" onChange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true'""/>"&arrFarm(0,dictFarm.item("FarmDescription"))&"</textarea>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th class=""data"" align=""left"">"&str.Item("strADDRESS")&"</th>" & vbCr)
'   Response.Write("      <td class=""data"" align=""left""><input type=""text"" name=""inputAddress"" value="""&arrFarm(0,dictFarm.item("FarmOwnerAddress"))&""" maxlength=""500"" size=""100""  onChange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true'""/></td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left""><textarea rows=""3"" cols=""38"" name=""inputAddress"" onChange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true';"">"&arrFarm(0,dictFarm.item("FarmOwnerAddress"))&"</textarea></td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th class=""data"" align=""left"">"&str.Item("strFARMDESCRIPTION")&"</th>" & vbCr)
   Response.Write("      <input type=""hidden"" name=""inputZip"" value=""""/><input type=""hidden"" name=""inputTown"" value=""""/>" & vbCr)
   Response.Write("      <input type=""hidden"" name=""FarmNumber"" value="""&Session("farmNumber")&"""/><input type=""hidden"" name=""ScenarioID"" value="""&aScenario&"""/>" & vbCr)
'   Response.Write("    </tr>" & vbCr)
'   Response.Write("    <tr>" & vbCr)   
'   Response.Write("      <td class=""data"" align=""left"">&nbsp; </td>" & vbCr)
'   Response.Write("      <td class=""data"" align=""left"">&nbsp; </td>" & vbCr)
'   Response.Write("    </tr>" & vbCr)
'   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <td class=""data"" align=""left"">" & vbCr)
   Response.Write("<textarea rows=""10"" cols=""38"" name=""inputDescription"" value="""&arrFarm(0,dictFarm.item("FarmDescription"))&""" onChange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true';""/>"&arrFarm(0,dictFarm.item("FarmDescription"))&"</textarea>" & vbCr)
   Response.Write("      </td>" & vbCr)
'   Response.Write("    </tr>" & vbCr)
   
   Response.Write("  </table>" & vbCr)
'   Response.Write("<div style=""font-weight: bold;position: relative;top: 5px;"">Farm description</div><br/>" & vbCr)    
'   Response.Write("<textarea rows=""10"" cols=""30"" name=""inputDescription"" value="""&arrFarm(0,dictFarm.item("FarmDescription"))&""" onChange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true'""/>"&arrFarm(0,dictFarm.item("FarmDescription"))&"</textarea>" & vbCr)

   ' soiltype information *************************************************************************
   Response.Write("  <br/>" & vbCr) 
   Response.Write("  <table class=""data"" border=""0"" cellpadding=""5"" style=""clear: both;"">" & vbCr)
   
   Response.Write("    <tr>" & vbCr)   
'   Response.Write("      <th align=""left"">"&Str.Item("strSOILTYPE")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">"&Str.Item("strIRRIGATION")&"<br>("&str.Item("strsandjord")&")</th>" & vbCr)
'   Response.Write("      <th align=""left"">"&Str.Item("strYIELDLEVEL")&"<br>"&str.Item("strofnorm")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strFARMTYPE")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">"&Str.Item("strFARMTYPE")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strAgroEcologicalZone")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strATMOSDEP")&"</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   
   Response.Write("    <tr>" & vbCr)   
'   Response.Write("      <td class=""data"" align=""left"">")
'   Response.Write("<select name=""selectScenario"">")   
'   CreateSelectList aScenario,nScenario,arrScenario
'   Response.Write("</select>")   
'   Response.Write("</td>" & vbCr)

'   Response.Write("      <td class=""data"" align=""left"">")
'   Response.Write("<select name=""selectSoilType"" onChange=""if ((this.options[this.selectedIndex].value=='1')||(this.options[this.selectedIndex].value=='2')||(this.options[this.selectedIndex].value=='3')||(this.options[this.selectedIndex].value=='4')){document.forms.formFarm.selectIrrigation.disabled=false;}else{document.forms.formFarm.selectIrrigation.options[0].selected=true;document.forms.formFarm.selectIrrigation.disabled=true;};this.style.color='#ff0000';document.forms.update.saveUpdates.value='true'"">")   
'   CreateSelectList_SoilType aSoilType,nSoilType,arrSoilType
'   Response.Write("</select>")   
'   Response.Write("</td>" & vbCr)
'   Response.Write("      <td class=""data"" align=""left"">")
'   select case aSoilType
'   case 1,2,3,4
'     Response.Write("<select name=""selectIrrigation""")   
'   case else
'     Response.Write("<select name=""selectIrrigation"" disabled")   
'   end select
'   Response.Write(" onChange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true'"">")   
''   CreateSelectList aIrrigationType, nIrrigationType, arrIrrigationType
'   CreateSelectList_Language aIrrigationType, nIrrigationType, arrIrrigationType
'   Response.Write("</select>")   
'   Response.Write("</td>" & vbCr)
'   Response.Write("      <td class=""data"" align=""left"">")
'   Response.Write("<select name=""selectYieldLevel"">")   
'   CreateSelectList aYieldLevel,nYieldLevel,arrYieldLevel
'   Response.Write("</select>")
   Response.write("<input type=""hidden"" name=""selectSoilType"" value="""&aSoilType&"""/>" & vbCr)  
   Response.Write("<input type = ""hidden"" name=""selectYieldLevel"" value="""&arrFarm(0,dictFarm.item("YieldLevel"))&""" />")   
'   Response.Write(arrFarm(0,dictFarm.item("YieldLevel")))   
'   Response.Write("%")   
'   Response.Write("</td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")
   Response.Write("<select name=""selectFarmType"" onchange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true';"">")   
   CreateSelectList_Language Cint(Session("farmType")),nFarmType,arrFarmType
   Response.Write("</select>")   
   Response.Write("</td>" & vbCr)
   Response.Write("      <td class=""data"" align=""right"">")
'   Response.Write("<select name=""deliveryTypeID"">")   
'   CreateSelectList Cint(aDeliveryType),nDeliveryType,arrDeliveryType
'   Response.Write("</select>")

'   Response.Write("<input type=""hidden"" name=""deliveryTypeID"" value="""" />") 

   Response.Write("<select name=""selectAgroZone"" onChange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true';""")
   if Session("LockAgroZone")=1 then
   Response.Write(" disabled=""disabled"" title=""The zone can not be changed when cropping sequences are registered""") 
   end if  
   Response.Write(">")   
   CreateSelectList_Language Cint(aAgroEcologicalZone),nAgroEcologicalZone,arrAgroEcologicalZone
   Response.Write("</select>")
   

'   p=0
'   for i = 0 to nDeliveryType-1
'     Response.Write(arrDeliveryType(1,i)&"<input type=""checkbox"" name=""deliveryTypeID"" value="""&arrDeliveryType(0,i)&"""")
'	 if Ubound(arrFarmDelivery)<>-1 then
'	     if Cint(arrFarmDelivery(p,dictFarmDelivery.item("DeliveryID")))=Cint(arrDeliveryType(0,i)) then
'		   Response.write(" checked")
'		   p=p+1
'		 end if
'	 end if  
'	 Response.Write(" onChange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true'"" ><br />")
'   next
  
   Response.Write("</td>" & vbCr)
   Response.Write("<td>" & vbCr)
   Response.Write("<input type=""text"" name=""atmosphericDeposition"" value="""&arrFarm(0,dictFarm.item("AtmosphericNDeposition"))&""" size=""5"" onChange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true';""/>&nbsp;&nbsp;"&str.Item("strKg_ N_ha_year")&"" & vbCr)
   Response.Write("</td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
'   Response.Write("    <tr>" & vbCr)
'   Response.Write("      <td class=""data"" align=""left"">")
'   Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formFarm.action='FarmUpdate.asp';document.forms.formFarm.submit();"">"&Str.Item("strSAVE")&"</button>")
'   Response.Write("</td>" & vbCr)
'   Response.Write("    </tr>" & vbCr)

' scenario select box *************************************************************************

   Response.Write("    <tr>" & vbCr)
   Response.Write("      <td colspan=""10""><hr/></td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strSCENARIO")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">"&Str.Item("strSCENARIO_CROPYEAR")&"</th>" & vbCr)
   Response.Write("      <th align=""left""></th>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")
   Response.Write("       <select name=""selectScenario"" onChange=""document.forms.formFarm.action='FarmSelectedScenarioUpdate.asp';document.forms.formFarm.submit();"">")   
'   Response.Write("       <select name=""selectScenario"">")   
   CreateSelectList_ aScenario,nScenario,arrScenario
   Response.Write("       </select>")
   Response.Write("       &nbsp;<button style=""width: 0px; height: 0px; font-size: 0px; border: 0px;"" onClick=""javascript:veil();document.forms.formFarm.action='FarmUpdate.asp';document.forms.formFarm.submit();""></button><button style=""height: 20px; font-size: 10px"" onClick=""FarmNPopupWin=window.open('FarmScenarioNameUpdate.asp','FarmNPopupWin','toolbar=0,scrollbars=no,left=50,top=385,width=300,height=80');"">"&str.Item("strRENAME")&"</button>" & vbCr)  
   Response.Write("      </td>" & vbCr)
   Response.Write("      <td class=""data"">")
'   Response.Write("<select name=""selectCropYear"" onChange=""document.forms.formFarm.action='FarmScenarioCropYearUpdate.asp';document.forms.formFarm.submit();"">")   
'   CreateSelectList_  aCropYear,nCropYear,arrCropYear
'   Response.Write("</select>")   
'	 if clng(Session("UserID"))= 2 or clng(Session("UserID"))= 9 then
'   end if
   Response.Write("      </td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   Response.Write("  </table>" & vbCr)

'' Information on bought manure *************************************************************************
'
'   Response.Write("  <table>" & vbCr) 
'   Response.Write("    <tr>" & vbCr)
'   Response.Write("      <th align=""left"">"&Str.Item("strBoughtManure")&"</th>" & vbCr)
'   Response.Write("      <th align=""left""></th>" & vbCr)
'   Response.Write("      <th align=""left"">"&Str.Item("strMaxSaleOfManure")&"</th>" & vbCr)
''   Response.Write("      <th align=""left"">Max. salg af husdyrgødning</th>" & vbCr)
'   Response.Write("    </tr>" & vbCr)
'
''   Response.Write("    <tr>" & vbCr)
''   Response.Write("    <td>" & vbCr) 
''
''   Response.Write("  <table>" & vbCr)
'   Response.Write("    <tr>" & vbCr)
'   Response.Write("      <td><!--<button>--></td>" & vbCr) 
''   Response.Write("    <td><button style=""width: 40px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formFarm.action='FarmBoughtManureNew.asp';document.forms.formFarm.submit();"">"&Str.Item("strNEW")&"</button></td>" & vbCr)
'   Response.Write("      <td></td>" & vbCr) 
'   Response.Write("    <td><button style=""width: 40px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.update.saveUpdates.value='false';document.forms.formFarm.action='FarmSoldManureNew.asp';document.forms.formFarm.submit();"">"&Str.Item("strNEW")&"</button></td>" & vbCr)
'   Response.Write("    </tr>" & vbCr)
'   Response.Write("    <tr>" & vbCr)
'   Response.Write("      <td class=""data"" align=""left""><table>")
''for-løkke der gennemløber antal indkøbte husdyrgødningstyper
'   for bm = 0 to Ubound(arrBoughtManure)
''if arrBoughtManure(bm,dictBoughtManure.item("BoughtManureType"))>11 then
'   Response.Write("<tr><td><input type=""text"" value="""&Str.Item("str"&arrBoughtManure(bm,dictBoughtManure.item("StorageName")))&""" size=""49"" disabled/>")    'Str.Item("str"&arrItem(i,1))
'   Response.Write("<input type=""hidden"" name=""selectBoughtManure_"&bm&""" id=""selectBoughtManure_"&bm&""" value="""&arrBoughtManure(bm,dictBoughtManure.item("BoughtManureType"))&"""/></td>")    
'''else
''   Response.Write("       <select name=""selectBoughtManure_"&bm&"""   onChange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true'"">")   
'''  dropdown med lagertyper: kvæggylle,svinegylle,dybstrøelse o.s.v.
''   CreateSelectList_Language arrBoughtManure(bm,dictBoughtManure.item("BoughtManureType")),nStorageType,arrStorageType'dropdown med lagertyper: kvæggylle,svinegylle,dybstrøelse o.s.v.
''   Response.Write("       </select>")   
'''end if
'   Response.Write("       <td><input name=""BoughtAmount_"&bm&""" id=""BoughtAmount"" type=""text"" size=""4"" value="""&arrBoughtManure(bm,dictBoughtManure.item("BoughtManureAmount"))&""" onChange=""if (isFinite(this.value.replace(',','.'))) {this.style.color='#ff0000';document.forms.update.saveUpdates.value='true'}"" />Kg N")
'   Response.Write("       </td><td><input name=""BoughtUtilization_"&bm&""" id=""BoughtUtilization"" type=""text"" size=""4"" value="""&replace(cstr(arrBoughtManure(bm,dictBoughtManure.item("Utilization_Degree"))),".",",")&""" onChange=""if (isFinite(this.value.replace(',','.'))) {this.value=this.value.replace('.',',');this.style.color='#ff0000';document.forms.update.saveUpdates.value='true'}"" />%")
'   Response.Write("       </td><td><input name=""BoughtConversion_"&bm&""" id=""BoughtConversion"" type=""text"" size=""4"" value="""&replace(cstr(arrBoughtManure(bm,dictBoughtManure.item("ConversionFactor"))),".",","  )&""" onChange=""if (isFinite(this.value.replace(',','.'))) {if (document.getElementById('selectBoughtManure_"&bm&"').value=='3') {this.value='"&replace(cstr(arrBoughtManure(bm,dictBoughtManure.item("ConversionFactor"))),".",","  )&"'} else {this.value=this.value.replace('.',',');this.style.color='#ff0000';document.forms.update.saveUpdates.value='true';}}"" />C/N")
'   Response.Write("       </td><td><button style=""width: 50px; height: 20px; font-size: 10px"" onClick=""document.forms.formFarm.DeleteBoughtManureID.value='"&arrBoughtManure(bm,dictBoughtManure.item("BoughtManureID"))&"';document.forms.formFarm.action='FarmBoughtManureDelete.asp';document.forms.formFarm.submit();"">"&Str.Item("strDelete")&"</button>")
'   Response.Write("      </td></tr>" & vbCr)
'   Response.Write("  <input type=""hidden"" name=""BMID_"&bm&""" id=""BMID_"&bm&""" value="""&arrBoughtManure(bm,dictBoughtManure.item("BoughtManureID"))&""" />" & vbCr)
'	 next
''slut for-løkke  
'   Response.Write("  <input type=""hidden"" name=""nBM"" id=""nBM"" value="""&bm-1&""" />" & vbCr)
'   Response.Write("       <tr><td><select name=""selectBoughtManure"" id=""selectNewBoughtManure"" style=""width: 320px;"">")   
''  dropdown med lagertyper: kvæggylle,svinegylle,dybstrøelse o.s.v.
'   CreateSelectList_Language 1,nStorageType,arrStorageType'dropdown med lagertyper: kvæggylle,svinegylle,dybstrøelse o.s.v.
'   Response.Write("       </select></td><td><button style=""width: 40px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formFarm.action='FarmBoughtManureNew.asp';document.forms.formFarm.submit();"">"&Str.Item("strNEW")&"</button></td></tr>")   
'
'   Response.Write("  </table>" & vbCr) 
'   Response.Write("    </td>" & vbCr)
'   Response.Write("      <td></td>" & vbCr) 
'   Response.Write("      <td>" & vbCr) 
''   Response.Write("      </td>" & vbCr) 
''   Response.Write("      <td>" & vbCr) 

' Information on bought manure *************************************************************************

   Response.Write("  <table>" & vbCr) 
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strBoughtManure")&"</th>" & vbCr)
   Response.Write("      <th align=""left""></th>" & vbCr)
'   Response.Write("      <th align=""left"">"&Str.Item("strMaxSaleOfManure")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
'   Response.Write("      <th align=""left"">Max. salg af husdyrgødning</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)

   Response.Write("    <tr>" & vbCr)
   Response.Write("    <td>" & vbCr) 

   Response.Write("  <table>" & vbCr)
   Response.Write("    <tr>" & vbCr)
   Response.Write("    <td><button style=""width: 40px; height: 20px; font-size: 10px"" onClick=""javascript:veil();document.forms.formFarm.action='FarmBoughtManureNew.asp';document.forms.formFarm.submit();"">"&Str.Item("strNEW")&"</button></td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
'for-løkke der gennemløber antal indkøbte husdyrgødningstyper
   for bm = 0 to Ubound(arrBoughtManure)
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")
'if arrBoughtManure(bm,dictBoughtManure.item("BoughtManureType"))>11 then
'   Response.Write("<span>"&arrBoughtManure(bm,dictBoughtManure.item("StorageName"))&"&nbsp;&nbsp;&nbsp;</span>")    
'else
   Response.Write("       <select name=""selectBoughtManure_"&bm&"""   onChange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true'"">")   
'  dropdown med lagertyper: kvæggylle,svinegylle,dybstrøelse o.s.v.
   CreateSelectList_Language arrBoughtManure(bm,dictBoughtManure.item("BoughtManureType")),nStorageType,arrStorageType'dropdown med lagertyper: kvæggylle,svinegylle,dybstrøelse o.s.v.
   Response.Write("       </select>")   
'end if
   Response.Write("       &nbsp;<input name=""BoughtAmount_"&bm&""" id=""BoughtAmount"" type=""hidden"" size=""4"" value=""0"" onChange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true'"" />")
'   Response.Write("       &nbsp;<button style=""width: 40px; height: 20px; font-size: 10px"" onClick=""document.forms.formFarm.DeleteBoughtManureID.value='"&arrBoughtManure(2,bm)&"';document.forms.formFarm.action='FarmBoughtManureDelete.asp';document.forms.formFarm.submit();"">Slet</button>")
   Response.Write("       &nbsp;<button style=""width: 50px; height: 20px; font-size: 10px"" onClick=""veil();document.forms.formFarm.DeleteBoughtManureID.value='"&arrBoughtManure(bm,dictBoughtManure.item("BoughtManureID"))&"';document.forms.formFarm.action='FarmBoughtManureDelete.asp';document.forms.formFarm.submit();"">"&Str.Item("strDelete")&"</button>")
   Response.Write("      </td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   Response.Write("  <input type=""hidden"" name=""BMID_"&bm&""" id=""BMID_"&bm&""" value="""&arrBoughtManure(bm,dictBoughtManure.item("BoughtManureID"))&""" />" & vbCr)
	 next
'slut for-løkke  
   Response.Write("  <input type=""hidden"" name=""nBM"" id=""nBM"" value="""&bm-1&""" />" & vbCr)

   Response.Write("  </table>" & vbCr) 
   Response.Write("      </td>" & vbCr) 
   Response.Write("      <td>" & vbCr) 
   Response.Write("      </td>" & vbCr) 
   Response.Write("      <td>" & vbCr) 


' Information on sold manure *************************************************************************
   
'   Response.Write("  <table>" & vbCr) 
'   Response.Write("    <tr>" & vbCr)
'   Response.Write("    <td><button style=""width: 40px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formFarm.action='FarmSoldManureNew.asp';document.forms.formFarm.submit();"">"&Str.Item("strNEW")&"</button></td>" & vbCr)
'   Response.Write("    </tr>" & vbCr)
'for-løkke der gennemløber antal solgte husdyrgødningstyper
   for sm = 0 to Ubound(arrSoldManure)
'   Response.Write("    <tr>" & vbCr)
'   Response.Write("      <td class=""data"" align=""left"">")
'   Response.Write("       <select name=""selectSoldManure_"&sm&""" onChange=""this.style.color='#ff0000';"">") 'dropdown med lagertyper: kvæggylle,svinegylle,dybstrøelse o.s.v.  
'   CreateSelectList_Language arrSoldManure(sm,dictSoldManure.item("SoldManureType")),nStorageType,arrStorageType
'   Response.Write("       </select>")   
'  
'   Response.Write("       &nbsp;<input name=""SoldAmount_"&sm&""" id=""SoldAmount"" type=""text"" size=""4"" value="""&arrSoldManure(sm,dictSoldManure.item("SoldManureAmount"))&""" onChange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true'"" />Kg N")
'   Response.Write("       &nbsp;<button style=""width: 50px; height: 20px; font-size: 10px"" onClick=""document.forms.formFarm.DeleteSoldManureID.value='"&arrSoldManure(sm,dictSoldManure.item("SoldManureID"))&"';document.forms.formFarm.action='FarmSoldManureDelete.asp';document.forms.formFarm.submit();"">"&Str.Item("strDelete")&"</button>")
'  
'   Response.Write("      </td>" & vbCr)
'   Response.Write("    </tr>" & vbCr)
   Response.Write("  <input type=""hidden"" name=""SMID_"&sm&""" id=""SMID_"&sm&""" value="""&arrSoldManure(sm,dictSoldManure.item("SoldManureID"))&""" />" & vbCr)
	 next
''slut for-løkke  
   Response.Write("  <input type=""hidden"" name=""nSM"" id=""nSM"" value="""&sm-1&""" />" & vbCr)

'   Response.Write("  </table>" & vbCr) 
'   Response.Write("      </td>" & vbCr) 
'   Response.Write("    </tr>" & vbCr)
'
   Response.Write("  </table>" & vbCr) 
   ' buttons **********************************************************************************************
   Response.Write("  <br/>" & vbCr) 
   Response.Write("  <table class=""data"" border=""0"">" & vbCr)
   
   Response.Write("    <tr>" & vbCr)   

  '  Knapper til nyt scenario og delete scenario ********************************************
  
   Response.Write("      <td class=""data"" align=""left"">")


  Response.Write("<button style=""width: 120px; height: 20px; font-size: 10px"" onClick=""veil();document.forms.formFarm.action='FarmScenarioNew.asp'"" >"&Str.Item("strNEWSCENARIO")&"</button>")
   
   
   'Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:alert('"&Str.Item("strvirkerikke")&"')"">"&Str.Item("strNEWSCENARIO")&"</button>")

   Response.Write("      </td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")
   Response.Write("       <input type=""hidden"" name=""CopyScenario"" value="""" />" & vbCr)
   Response.Write("       <button style=""width: 120px;height: 20px; font-size: 10px"" onClick=""javascript:veil();with(document.forms.formFarm){action='FarmScenarioNew.asp';CopyScenario.value='CopyScenario';submit();}"">"&Str.Item("strCopyScenario")&"</button>" & vbCr)
   Response.Write("      </td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")

   
   
   Response.Write("<button style=""width: 120px; height: 20px; font-size: 10px"" onClick=""veil();document.forms.formFarm.action='FarmScenarioDelete.asp';document.forms.formFarm.submit();"">"&Str.Item("strDELETESCEN")&"</button>")
   
'   Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:alert('"&Str.Item("strvirkerikke")&"')"">"&Str.Item("strDELETESCEN")&"</button>")
'   Response.Write("</td>" & vbCr)
'   Response.Write("      <td class=""data"" align=""left"">")
'   Response.Write("<button style=""width: 200px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formFarm.action='ScenarioNew.asp';document.forms.formFarm.submit();"">Ny ejendom / scenarie</button>")
   Response.Write("</td>" & vbCr)
   Response.Write("    </tr>" & vbCr)  

      Response.Write("    <tr>" & vbCr)
   Response.Write("      <td colspan=""10""><hr/></td>" & vbCr)
   Response.Write("    </tr>" & vbCr)

   
      Response.Write("    <tr>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")
   Response.Write("<button style=""width: 120px; height: 20px; font-size: 10px"" onClick=""javascript:veil();document.forms.formFarm.action='FarmUpdate.asp';document.forms.formFarm.submit();"">"&Str.Item("strSAVE")&"</button>")
   Response.Write("</td>" & vbCr)
   Response.Write("    </tr>" & vbCr)

   Response.Write("    <tr>" & vbCr)
   Response.Write("      <td colspan = ""3"" class=""data"" align=""left"">")
   Response.Write("   <table>" & vbCr)
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")
'   Response.Write("<button style=""width: 120px; height: 20px; font-size: 10px"" onClick=""javascript:veil();document.forms.formFarm.action='RunModelService.asp?version=2';document.forms.formFarm.submit();"">Run model</button>")
   Response.Write("<button name=""adaptation"" value=""1"" style=""width: 150px; height: 20px; font-size: 10px"" onClick=""javascript:veil();document.forms.formFarm.action='RunModelService.asp?version=1';document.forms.formFarm.submit();"">"&Str.Item("strRunModel")&" "&Str.Item("strAdaptation")&"</button>")
   Response.Write("<button name=""projection"" value=""1"" style=""width: 150px; height: 20px; font-size: 10px"" onClick=""javascript:veil();document.forms.formFarm.action='RunModelService.asp?version=1';document.forms.formFarm.submit();"">"&Str.Item("strRunModel")&" "&Str.Item("strProjection")&"</button>")
   Response.Write("<button style=""width: 120px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formFarm.target='_parent';document.forms.formFarm.action='TestModelService.asp';document.forms.formFarm.submit();"">Create xml</button>")
   if request("message")<>"" then
   response.write "<span style=""color: red;"">&nbsp;"&request("message")&"</span>"
   end if
   Response.Write("</td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   Response.Write("   <table>" & vbCr)
   Response.Write("</td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   Response.Write("    <tr>" & vbCr)
   Response.Write("<td><a href=""/upload/AnimalChange/download/FarmAC.zip"">download current version of the model</a></td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   Response.Write("    <tr>" & vbCr)
   Response.Write("<td><a href=""/upload/AnimalChange/download/inputFiles.zip"">current version of parameter-files</a>" & vbCr)
   Response.Write("</td>" & vbCr)
   Response.Write("    </tr>" & vbCr)

   Response.Write("  </table>" & vbCr) 
'   if request("message")<>"" then
'   response.write "&nbsp;"&request("message")
'   end if
   Response.Write("</form>" & vbCr)
	 CreateUpdateFormFarm()
	 time5=now

    %>   <div id="google_translate_element" style="position: relative; right: 10px; float: right;"></div><script type="text/javascript">
                                                  function googleTranslateElementInit() {
                                                      new google.translate.TranslateElement({ pageLanguage: 'en', layout: google.translate.TranslateElement.InlineLayout.SIMPLE }, 'google_translate_element');
                                                  }
</script><script type="text/javascript" src="//translate.google.com/translate_a/element.js?cb=googleTranslateElementInit"></script><%
'dim x
'for each x in Session.Contents
'  Response.Write(x & "=" & Session.Contents(x) & "<br />")
'next
     end sub
   ' **********************************************************************************************
'sub CreateSelectList_SoilType(aType,nItem,arrItem)
'   dim i
'
'   for i = 0 to nItem-1
'      if arrItem(i,0) = aType then
'         Response.Write("<option value="""&arrItem(i,0)&""" selected>"&Str.Item("str"&arrItem(i,1))&"  "&arrItem(i,2)&"</option>")
'      else  
'         Response.Write("<option value="""&arrItem(i,0)&""">"&Str.Item("str"&arrItem(i,1))&"  "&arrItem(i,2)&"</option>")
'      end if 
'   next   
'end sub
'**************************************************************************************************
'sub CreateUpdateFormFarm()
'   Response.Write("<form name=""update"" action=""javascript:document.forms.formFarm.submit();"" method=""post"">" & vbCr)
'   Response.Write(" <input type=""hidden"" name=""saveUpdates"" value=""false"">" & vbCr)
'   Response.Write("</form>" & vbCr)
'end sub
'**************************************************************************************************
sub CreateUpdateFormFarm()
   Response.Write("<form name=""update"" action=""javascript:document.forms.formFarm.submit();"" method=""post"">" & vbCr)
   Response.Write(" <input type=""hidden"" name=""saveUpdates"" id=""saveUpdates"" value=""false"">" & vbCr)
'   Response.Write(" <input type=""text"" name=""saveUpdates"" id=""saveUpdates"" value=""false"" disabled>" & vbCr)
   Response.Write("</form>" & vbCr)
end sub
'**************************************************************************************************
%>
