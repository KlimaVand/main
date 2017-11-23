<%
'**************************************************************************************************
' Author: Poul Lassen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
sub CreateTabFarm()
   dim nFarm, arrFarm
   dim nScenario, arrScenario, aScenario
   dim nSoilType, arrSoilType, aSoilType
   dim nIrrigationType, arrIrrigationType, aIrrigationType
   dim nDeliveryType, arrDeliveryType, aDeliveryType, i
   dim nFarmDelivery, arrFarmDelivery, dictFarmDelivery,p
   dim nFarmType, arrFarmType
   dim nYieldLevel, arrYieldLevel, aYieldLevel
   dim nStorageType, arrStorageType 
   dim nBoughtManure, arrBoughtManure ,bm 
   dim nSoldManure, arrSoldManure ,sm
   dim dictFarm, dictScenario, dictSoilType,dictIrrigation, dictFarmType, dictStorageType, dictBoughtManure, dictSoldManure
   dim dictCropYear, arrCropYear, nCropYear, aCropYear
   
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
'   nStorageType = CallSPStr ("GetStorageTypeListNoGrazing", arrStorageType, dictStorageType)	   
   nStorageType = CallSPStr ("getAllStorageTypesList", arrStorageType, dictStorageType)	   
   set dictCropYear = CreateObject("Scripting.Dictionary")
   nCropYear = CallSPStr ("GetSelectListCropYear", arrCropYear, dictCropYear)	   

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
   redim arrDeliveryType(1,1)
   aDeliveryType 			= Cint(arrFarm(0,dictFarm.item("Delivery")))

   nDeliveryType 			= 2
   arrDeliveryType(0,0)	    = 1
   arrDeliveryType(1,0)	    = str.Item("strDELIVERYINCORP")
   arrDeliveryType(0,1)	    = 2
   arrDeliveryType(1,1)	    = str.Item("strDELIVERYTUBE")

   set dictBoughtManure = CreateObject("Scripting.Dictionary")
   nBoughtManure = CallSPStr ("GetBoughtManureList " & Session("farmNumber") & "," & aScenario, arrBoughtManure, dictBoughtManure)	   
'   nBoughtManure = GetBoughtManureList(Session("farmNumber"),aScenario,arrBoughtManure)
   set dictSoldManure = CreateObject("Scripting.Dictionary")
   nSoldManure = CallSPStr ("GetSoldManureList " & Session("farmNumber") & "," & aScenario, arrSoldManure, dictSoldManure)	   
'   nSoldManure   = GetSoldManureList(Session("farmNumber"),aScenario,arrSoldManure)
   set dictFarmDelivery = CreateObject("Scripting.Dictionary")
   nFarmDelivery = CallSP("GetFarmDeliveryData "&CLng(Session("farmNumber")),arrFarmDelivery,dictFarmDelivery)

   CloseFarmNDB


   Response.Write("<form name=""formFarm"" action=""FarmUpdate.asp"" method=""post"">" & vbCr)
   ' farmer information ***************************************************************************
   Response.Write("  <input type=""hidden"" name=""DeleteBoughtManureID"" id=""DeleteBoughtManureID"" value="""" />" & vbCr)
   Response.Write("  <input type=""hidden"" name=""DeleteSoldManureID"" id=""DeleteSoldManureID"" value="""" />" & vbCr)
   Response.Write("  <input type=""hidden"" name=""aDeleteScenarioID"" id=""aDeleteScenarioID"" value="""" />" & vbCr)
   Response.Write("  <table class=""data"" border=""0"">" & vbCr)

   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th align=""left"">"&str.Item("strFARMER")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)   

   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <td class=""data"" align=""left"">"&str.Item("strNAME")&"</td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left""><input type=""text"" name=""inputName"" value="""&arrFarm(0,dictFarm.item("FarmOwnerName"))&""" maxlength=""50"" size=""50"" onChange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true'""/></td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <td class=""data"" align=""left"">"&str.Item("strADDRESS")&"</td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left""><input type=""text"" name=""inputAddress"" value="""&arrFarm(0,dictFarm.item("FarmOwnerAddress"))&""" maxlength=""50"" size=""50"" onChange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true'""/></td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <td class=""data"" align=""left"">"&str.Item("strPOSTNOANDTOWN")&"</td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left""><input type=""text"" name=""inputZip"" value="""&arrFarm(0,dictFarm.item("FarmOwnerZipCode"))&""" maxlength=""4"" size=""5"" onChange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true'""/>&nbsp;&nbsp;<input type=""text"" name=""inputTown"" value="""&arrFarm(0,dictFarm.item("FarmOwnerTown"))&""" maxlength=""50"" size=""40"" onChange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true'""/></td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   
   Response.Write("  </table>" & vbCr) 
   ' soiltype information *************************************************************************
   Response.Write("  <br/>" & vbCr) 
   Response.Write("  <table class=""data"" border=""0"" cellpadding=""5"">" & vbCr)
   
   Response.Write("    <tr>" & vbCr)   
'   Response.Write("      <th align=""left"">"&Str.Item("strSOILTYPE")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">"&Str.Item("strIRRIGATION")&"<br>("&str.Item("strsandjord")&")</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strYIELDLEVEL")&"<br>"&str.Item("strofnorm")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strFARMTYPE")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">"&Str.Item("strFARMTYPE")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strDELIVERYMACHINE")&"</th>" & vbCr)
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
   Response.Write("      <td class=""data"" align=""left"">")
'   Response.Write("<select name=""selectYieldLevel"">")   
'   CreateSelectList aYieldLevel,nYieldLevel,arrYieldLevel
'   Response.Write("</select>")   
   Response.Write("<input type = ""text"" size=""2"" maxlength=""3"" name=""selectYieldLevel"" value="""&arrFarm(0,dictFarm.item("YieldLevel"))&""" onChange=""InputValidator(this, 0, 999, '"&Str.Item("strYIELDLEVEL")&"', 3);this.style.color='#ff0000';document.forms.update.saveUpdates.value='true'"" />")   
   Response.Write("%")   
   Response.Write("</td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")
   Response.Write("<select name=""selectFarmType"">")   
   CreateSelectList_Language Cint(Session("farmType")),nFarmType,arrFarmType
   Response.Write("</select>")   
   Response.Write("</td>" & vbCr)
   Response.Write("      <td class=""data"" align=""right"">")
'   Response.Write("<select name=""deliveryTypeID"">")   
'   CreateSelectList Cint(aDeliveryType),nDeliveryType,arrDeliveryType
'   Response.Write("</select>")

'   Response.Write("<input type=""hidden"" name=""deliveryTypeID"" value="""" />") 

   p=0
   for i = 0 to nDeliveryType-1
     Response.Write(arrDeliveryType(1,i)&"<input type=""checkbox"" name=""deliveryTypeID"" value="""&arrDeliveryType(0,i)&"""")
	 if Ubound(arrFarmDelivery)<>-1 then
	     if Cint(arrFarmDelivery(p,dictFarmDelivery.item("DeliveryID")))=Cint(arrDeliveryType(0,i)) then
		   Response.write(" checked")
		   p=p+1
		 end if
	 end if  
	 Response.Write(" onChange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true'"" ><br />")
   next
  
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
   Response.Write("      <th align=""left"">"&Str.Item("strSCENARIO_CROPYEAR")&"</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")
   Response.Write("       <select name=""selectScenario"" onChange=""document.forms.formFarm.action='FarmSelectedScenarioUpdate.asp';document.forms.formFarm.submit();"">")   
'   Response.Write("       <select name=""selectScenario"">")   
   CreateSelectList_ aScenario,nScenario,arrScenario
   Response.Write("       </select>")
   Response.Write("       &nbsp;<button style=""height: 20px; font-size: 10px"" onClick=""FarmNPopupWin=window.open('FarmScenarioNameUpdate.asp','FarmNPopupWin','toolbar=0,scrollbars=no,left=50,top=385,width=300,height=80');"">"&str.Item("strRENAME")&"</button>" & vbCr)  
   Response.Write("      </td>" & vbCr)
   Response.Write("      <td class=""data"">")
   Response.Write("<select name=""selectCropYear"" onChange=""document.forms.formFarm.action='FarmScenarioCropYearUpdate.asp';document.forms.formFarm.submit();"">")   
   CreateSelectList_  aCropYear,nCropYear,arrCropYear
   Response.Write("</select>")   
'	 if clng(Session("UserID"))= 2 or clng(Session("UserID"))= 9 then
'   end if
   Response.Write("      </td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   Response.Write("  </table>" & vbCr)

' Information on bought manure *************************************************************************

   Response.Write("  <table>" & vbCr) 
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strBoughtManure")&"</th>" & vbCr)
   Response.Write("      <th align=""left""></th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strMaxSaleOfManure")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">Max. salg af husdyrgødning</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)

   Response.Write("    <tr>" & vbCr)
   Response.Write("    <td>" & vbCr) 

   Response.Write("  <table>" & vbCr)
   Response.Write("    <tr>" & vbCr)
   Response.Write("    <td><button style=""width: 40px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formFarm.action='FarmBoughtManureNew.asp';document.forms.formFarm.submit();"">"&Str.Item("strNEW")&"</button></td>" & vbCr)
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
   Response.Write("       &nbsp;<input name=""BoughtAmount_"&bm&""" id=""BoughtAmount"" type=""text"" size=""4"" value="""&arrBoughtManure(bm,dictBoughtManure.item("BoughtManureAmount"))&""" onChange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true'"" />Kg N")
'   Response.Write("       &nbsp;<button style=""width: 40px; height: 20px; font-size: 10px"" onClick=""document.forms.formFarm.DeleteBoughtManureID.value='"&arrBoughtManure(2,bm)&"';document.forms.formFarm.action='FarmBoughtManureDelete.asp';document.forms.formFarm.submit();"">Slet</button>")
   Response.Write("       &nbsp;<button style=""width: 50px; height: 20px; font-size: 10px"" onClick=""document.forms.formFarm.DeleteBoughtManureID.value='"&arrBoughtManure(bm,dictBoughtManure.item("BoughtManureID"))&"';document.forms.formFarm.action='FarmBoughtManureDelete.asp';document.forms.formFarm.submit();"">"&Str.Item("strDelete")&"</button>")
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
   
   Response.Write("  <table>" & vbCr) 
   Response.Write("    <tr>" & vbCr)
   Response.Write("    <td><button style=""width: 40px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formFarm.action='FarmSoldManureNew.asp';document.forms.formFarm.submit();"">"&Str.Item("strNEW")&"</button></td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
'for-løkke der gennemløber antal solgte husdyrgødningstyper
   for sm = 0 to Ubound(arrSoldManure)
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")
   Response.Write("       <select name=""selectSoldManure_"&sm&""" onChange=""this.style.color='#ff0000';"">") 'dropdown med lagertyper: kvæggylle,svinegylle,dybstrøelse o.s.v.  
   CreateSelectList_Language arrSoldManure(sm,dictSoldManure.item("SoldManureType")),nStorageType,arrStorageType
   Response.Write("       </select>")   
  
   Response.Write("       &nbsp;<input name=""SoldAmount_"&sm&""" id=""SoldAmount"" type=""text"" size=""4"" value="""&arrSoldManure(sm,dictSoldManure.item("SoldManureAmount"))&""" onChange=""this.style.color='#ff0000';document.forms.update.saveUpdates.value='true'"" />Kg N")
   Response.Write("       &nbsp;<button style=""width: 50px; height: 20px; font-size: 10px"" onClick=""document.forms.formFarm.DeleteSoldManureID.value='"&arrSoldManure(sm,dictSoldManure.item("SoldManureID"))&"';document.forms.formFarm.action='FarmSoldManureDelete.asp';document.forms.formFarm.submit();"">"&Str.Item("strDelete")&"</button>")
  
   Response.Write("      </td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   Response.Write("  <input type=""hidden"" name=""SMID_"&sm&""" id=""SMID_"&sm&""" value="""&arrSoldManure(sm,dictSoldManure.item("SoldManureID"))&""" />" & vbCr)
	 next
'slut for-løkke  
   Response.Write("  <input type=""hidden"" name=""nSM"" id=""nSM"" value="""&sm-1&""" />" & vbCr)

   Response.Write("  </table>" & vbCr) 
   Response.Write("      </td>" & vbCr) 
   Response.Write("    </tr>" & vbCr)

   Response.Write("  </table>" & vbCr) 
   ' buttons **********************************************************************************************
   Response.Write("  <br/>" & vbCr) 
   Response.Write("  <table class=""data"" border=""0"">" & vbCr)
   
   Response.Write("    <tr>" & vbCr)   

  '  Knapper til nyt scenario og delete scenario ********************************************
  
   Response.Write("      <td class=""data"" align=""left"">")


  Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""document.forms.formFarm.action='FarmScenarioNew.asp';document.forms.formFarm.submit();"">"&Str.Item("strNEWSCENARIO")&"</button>")
   
   
   'Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:alert('"&Str.Item("strvirkerikke")&"')"">"&Str.Item("strNEWSCENARIO")&"</button>")

   Response.Write("      </td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")
   Response.Write("       <input type=""hidden"" name=""CopyScenario"" value="""" />" & vbCr)
   Response.Write("       <button style=""height: 20px; font-size: 10px"" onClick=""javascript:with(document.forms.formFarm){action='FarmScenarioNew.asp';CopyScenario.value='CopyScenario';submit();}"">"&Str.Item("strCopyScenario")&"</button>" & vbCr)
   Response.Write("      </td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")

   
   
   Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""document.forms.formFarm.action='FarmScenarioDelete.asp';document.forms.formFarm.submit();"">"&Str.Item("strDELETESCEN")&"</button>")
   
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
   Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formFarm.action='FarmUpdate.asp';document.forms.formFarm.submit();"">"&Str.Item("strSAVE")&"</button>")
   Response.Write("</td>" & vbCr)
   Response.Write("    </tr>" & vbCr)

   Response.Write("  </table>" & vbCr) 
   Response.Write("</form>" & vbCr)
	 CreateUpdateFormFarm()
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
sub CreateUpdateFormFarm()
   Response.Write("<form name=""update"" action=""javascript:document.forms.formFarm.submit();"" method=""post"">" & vbCr)
   Response.Write(" <input type=""hidden"" name=""saveUpdates"" value=""false"">" & vbCr)
   Response.Write("</form>" & vbCr)
end sub
'**************************************************************************************************
%>
