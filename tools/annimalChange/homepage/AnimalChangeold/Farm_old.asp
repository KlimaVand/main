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
'   dim nDeliveryType, arrDeliveryType, aDeliveryType
   dim nFarmType, arrFarmType
   dim nYieldLevel, arrYieldLevel, aYieldLevel
   
   InitFarmNDB
   CreateLanguageDictionary
   nFarm     = GetFarmData(CLng(Session("farmNumber")),arrFarm)
   nScenario = GetSelectListScenario(CLng(Session("farmNumber")),arrScenario)
   nSoilType = GetSelectListSoilType(arrSoilType)
   nFarmType = GetSelectListFarmType(arrFarmType)
   CloseFarmNDB
   
   redim arrIrrigationType(1,1)
   aScenario 				= arrFarm(9,0)
   aSoilType 				= arrFarm(5,0)
   aIrrigationType 			= arrFarm(6,0)
	  if aIrrigationType="True" then
	     aIrrigationType=1
	  else
	     aIrrigationType=0
	  end if
   nIrrigationType 			= 2
   arrIrrigationType(0,0)	= 0
   arrIrrigationType(1,0)	= str.Item("strRAINFED")
   arrIrrigationType(0,1)	= 1
   arrIrrigationType(1,1)	= str.Item("strIRRIGATED")
   redim arrYieldLevel(1,20)
   aYieldLevel				= arrFarm(10,0)
   nYieldLevel 				= 21
   arrYieldLevel(0,0)	= 50
   arrYieldLevel(0,1)	= 55
   arrYieldLevel(0,2)	= 60
   arrYieldLevel(0,3)	= 65
   arrYieldLevel(0,4)	= 70
   arrYieldLevel(0,5)	= 75
   arrYieldLevel(0,6)	= 80
   arrYieldLevel(0,7)	= 85
   arrYieldLevel(0,8)	= 90
   arrYieldLevel(0,9)	= 95
   arrYieldLevel(0,10)	= 100
   arrYieldLevel(0,11)	= 105
   arrYieldLevel(0,12)	= 110
   arrYieldLevel(0,13)	= 115
   arrYieldLevel(0,14)	= 120
   arrYieldLevel(0,15)	= 125
   arrYieldLevel(0,16)	= 130
   arrYieldLevel(0,17)	= 135
   arrYieldLevel(0,18)	= 140
   arrYieldLevel(0,19)	= 145
   arrYieldLevel(0,20)	= 150
   arrYieldLevel(1,0)	= "50 %"
   arrYieldLevel(1,1)	= "55 %"
   arrYieldLevel(1,2)	= "60 %"
   arrYieldLevel(1,3)	= "65 %"
   arrYieldLevel(1,4)	= "70 %"
   arrYieldLevel(1,5)	= "75 %"
   arrYieldLevel(1,6)	= "80 %"
   arrYieldLevel(1,7)	= "85 %"
   arrYieldLevel(1,8)	= "90 %"
   arrYieldLevel(1,9)	= "95 %"
   arrYieldLevel(1,10)	= "100 %"
   arrYieldLevel(1,11)	= "105 %"
   arrYieldLevel(1,12)	= "110 %"
   arrYieldLevel(1,13)	= "115 %"
   arrYieldLevel(1,14)	= "120 %"
   arrYieldLevel(1,15)	= "125 %"
   arrYieldLevel(1,16)	= "130 %"
   arrYieldLevel(1,17)	= "135 %"
   arrYieldLevel(1,18)	= "140 %"
   arrYieldLevel(1,19)	= "145 %"
   arrYieldLevel(1,20)	= "150 %"

   Response.Write("<form name=""formFarm"" action=""DummyFarm.asp"" method=""post"">" & vbCr)
   ' farmer information ***************************************************************************
   Response.Write("  <table class=""data"" border=""0"">" & vbCr)

   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th align=""left"">"&str.Item("strFARMER")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)   

   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <td class=""data"" align=""left"">"&str.Item("strNAME")&"</td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left""><input type=""text"" name=""inputName"" value="""&arrFarm(1,0)&""" maxlength=""50"" size=""50""/></td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <td class=""data"" align=""left"">"&str.Item("strADDRESS")&"</td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left""><input type=""text"" name=""inputAddress"" value="""&arrFarm(2,0)&""" maxlength=""50"" size=""50""/></td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <td class=""data"" align=""left"">"&str.Item("strPOSTNOANDTOWN")&"</td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left""><input type=""text"" name=""inputZip"" value="""&arrFarm(3,0)&""" maxlength=""4"" size=""5""/>&nbsp;&nbsp;<input type=""text"" name=""inputTown"" value="""&arrFarm(4,0)&""" maxlength=""50"" size=""40""/></td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   
   Response.Write("  </table>" & vbCr) 
   ' scenario and soiltype information *************************************************************************
   Response.Write("  <br/>" & vbCr) 
   Response.Write("  <table class=""data"" border=""0"">" & vbCr)
   
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th align=""left"">"&Str.Item("strSCENARIOS")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strSOILTYPE")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strIRRIGATION")&"<br>("&str.Item("strsandjord")&")</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strYIELDLEVEL")&"<br>"&str.Item("strofnorm")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strFARMTYPE")&"</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <td class=""data"" align=""left"">")
   Response.Write("<select name=""selectScenario"">")   
   CreateSelectList aScenario,nScenario,arrScenario
   Response.Write("</select>")   
   Response.Write("</td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")
   Response.Write("<select name=""selectSoilType"" onChange=""if ((this.options[this.selectedIndex].value=='1')||(this.options[this.selectedIndex].value=='2')||(this.options[this.selectedIndex].value=='3')||(this.options[this.selectedIndex].value=='4')){document.forms.formFarm.selectIrrigation.disabled=false;}else{document.forms.formFarm.selectIrrigation.options[0].selected=true;document.forms.formFarm.selectIrrigation.disabled=true;}"">")   
   CreateSelectList aSoilType,nSoilType,arrSoilType
   Response.Write("</select>")   
   Response.Write("</td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")
   select case aSoilType
   case 1,2,3,4
     Response.Write("<select name=""selectIrrigation"">")   
   case else
     Response.Write("<select name=""selectIrrigation"" disabled>")   
   end select
   CreateSelectList aIrrigationType, nIrrigationType, arrIrrigationType
   Response.Write("</select>")   
   Response.Write("</td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")
   Response.Write("<select name=""selectYieldLevel"">")   
   CreateSelectList aYieldLevel,nYieldLevel,arrYieldLevel
   Response.Write("</select>")   
   Response.Write("</td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")
   Response.Write("<select name=""selectFarmType"">")   
   CreateSelectListLanguage Cint(Session("farmType")),nFarmType,arrFarmType
   Response.Write("</select>")   
   Response.Write("</td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
     
   Response.Write("  </table>" & vbCr) 
   ' buttons **********************************************************************************************
   Response.Write("  <br/>" & vbCr) 
   Response.Write("  <table class=""data"" border=""0"">" & vbCr)
   
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <td class=""data"" align=""left"">")
   Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formFarm.action='FarmUpdate.asp';document.forms.formFarm.submit();"">"&Str.Item("strSAVE")&"</button>")
   Response.Write("</td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")
   Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:alert('"&Str.Item("strvirkerikke")&"')"">"&Str.Item("strNEWSCENARIO")&"</button>")
   Response.Write("</td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")
   Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:alert('"&Str.Item("strvirkerikke")&"')"">"&Str.Item("strDELETESCEN")&"</button>")
'   Response.Write("</td>" & vbCr)
'   Response.Write("      <td class=""data"" align=""left"">")
'   Response.Write("<button style=""width: 200px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formFarm.action='ScenarioNew.asp';document.forms.formFarm.submit();"">Ny ejendom / scenarie</button>")
'   Response.Write("</td>" & vbCr)
   Response.Write("    </tr>" & vbCr)  

   Response.Write("  </table>" & vbCr) 
   ' **********************************************************************************************
   Response.Write("</form>" & vbCr)
   Response.Write("<form name=""update"" action=""javascript:"" method=""post"">" & vbCr)
   Response.Write(" <input type=""hidden"" name=""saveUpdates"" value=""false"">" & vbCr)
   Response.Write("</form>" & vbCr)
end sub
'**************************************************************************************************
%>
