<%
'**************************************************************************************************
' Author: Margit Jorgensen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
option explicit
%>
<!-- #include file="includes/General.asp" -->
<!-- #include file="includes/adovbs.asp" -->
<!-- #include file="includes/dbInterface.asp" -->
<!-- #include file="includes/LanguageDictionary.asp" -->
<%
BeginHeader "FarmN ","",""
CreateStyleLink "styles/stylesheet.css"
EndHeader
BeginBody
CreateTabFarm()
EndBody
'**************************************************************************************************
sub BeginBody()
   Response.Write("<body class=""data"">" & vbCr)
end sub
'**************************************************************************************************
sub CreateTabFarm()
   dim nFarm, arrFarm
   dim nScenario, arrScenario, aScenario
   dim nSoilType, arrSoilType, aSoilType
   dim nIrrigationType, arrIrrigationType, aIrrigationType
'   dim nDeliveryType, arrDeliveryType, aDeliveryType
   dim nFarmType, arrFarmType
   dim nYieldLevel, arrYieldLevel, aYieldLevel
	 dim nStorageType, arrStorageType 
	 dim nBoughtManure, arrBoughtManure ,bm 
	 dim nSoldManure, arrSoldManure ,sm
   
   InitFarmNDB
   CreateLanguageDictionary
   nFarm        = GetFarmData(Session("farmNumber"),arrFarm)
   nScenario    = GetSelectListScenario(Session("farmNumber"),arrScenario)
   nSoilType    = GetSelectListSoilType(arrSoilType)
   nFarmType    = GetSelectListFarmType(arrFarmType)
   nStorageType = GetStorageTypeListNoGrazing(arrStorageType)
   
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

   nBoughtManure = GetBoughtManureList(Session("farmNumber"),aScenario,arrBoughtManure)
   nSoldManure   = GetSoldManureList(Session("farmNumber"),aScenario,arrSoldManure)


'eksempel på generel hentning af data som i GartnerIT: data lægges i array, kolonnenavne lægges i dictionary
'i array'et er første dimension = rækker, sidste dimension = kolonner
'i dictionary'et er kolonnenavnet key og kolonnens index/nummer er tilhørende item 

'  Dim dictX,arrX,X
  Dim strSQL  
'  set dictX = CreateObject("Scripting.Dictionary")
'	X = CallSP ("GetBoughtManureList " & Session("farmNumber") & ","&aScenario, arrX, dictX)
'  response.write "<br/>" & arrX(0,dictX.Item("BoughtManureAmount"))
'  response.write "<br/>" & dictX.Item("BoughtManureAmount") & "<br/>"
'dim a,i
'a = dictX.Keys
'for i = 0 to UBound(a)'dictX.Count -1
'  response.write a(i)&" -- "
'  response.write arrX(0,dictX.Item(a(i)))&"<br/>"
'next

dim arrParameter,nID,aScenarioID,aScenarioName,arrID,returnValue
	
nID           = GetScenarioMaxID(Session("farmNumber"), arrID)
Response.write "<br> nID " & nID 
aScenarioID   = arrID(0,0)+1
Response.write "<br> aScenarioID " & aScenarioID
aScenarioName = "Scenario "&aScenarioID
	strSQL = "InsertUpdateScenario " 

	redim arrParameter(2)
	arrParameter(0)=Session("farmNumber")
	arrParameter(1)=aScenarioID
	arrParameter(2)=aScenarioName

returnValue=CallSPReturn(strSQL,arrParameter)
response.write returnValue

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
   ' soiltype information *************************************************************************
   Response.Write("  <br/>" & vbCr) 
   Response.Write("  <table class=""data"" border=""0"" cellpadding=""5"">" & vbCr)
   
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th align=""left"">"&Str.Item("strSOILTYPE")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strIRRIGATION")&"<br>("&str.Item("strsandjord")&")</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strYIELDLEVEL")&"<br>"&str.Item("strofnorm")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strFARMTYPE")&"</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   
   Response.Write("    <tr>" & vbCr)   
'   Response.Write("      <td class=""data"" align=""left"">")
'   Response.Write("<select name=""selectScenario"">")   
'   CreateSelectList aScenario,nScenario,arrScenario
'   Response.Write("</select>")   
'   Response.Write("</td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")
   Response.Write("<select name=""selectSoilType"" onChange=""if ((this.options[this.selectedIndex].value=='1')||(this.options[this.selectedIndex].value=='2')||(this.options[this.selectedIndex].value=='3')||(this.options[this.selectedIndex].value=='4')){document.forms.formFarm.selectIrrigation.disabled=false;}else{document.forms.formFarm.selectIrrigation.options[0].selected=true;document.forms.formFarm.selectIrrigation.disabled=true;}"">")   
   CreateSelectListLanguage aSoilType,nSoilType,arrSoilType
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
'   Response.Write("<select name=""selectYieldLevel"">")   
'   CreateSelectList aYieldLevel,nYieldLevel,arrYieldLevel
'   Response.Write("</select>")   
   Response.Write("<input type = ""text"" size=""2"" maxlength=""3"" name=""selectYieldLevel"" value="""&arrFarm(10,0)&""" onChange=""InputValidator(this, 0, 999, '"&Str.Item("strYIELDLEVEL")&"', 3);"" />")   
   Response.Write("%")   
   Response.Write("</td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")
   Response.Write("<select name=""selectFarmType"">")   
   CreateSelectListLanguage Cint(Session("farmType")),nFarmType,arrFarmType
   Response.Write("</select>")   
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
   Response.Write("      <th align=""left"">"&Str.Item("strSCENARIOS")&"</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")
   Response.Write("       <select name=""selectScenario"" onChange=""document.forms.formFarm.action='FarmSelectedScenarioUpdate.asp';document.forms.formFarm.submit();"">")   
'   Response.Write("       <select name=""selectScenario"">")   
   CreateSelectList aScenario,nScenario,arrScenario
   Response.Write("       </select>")   
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
   for bm = 0 to Ubound(arrBoughtManure,2)-1
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")
   Response.Write("       <select name=""selectBoughtManure_"&bm&"""  onChange=""this.style.color='#ff0000';"">")   
'  dropdown med lagertyper: kvæggylle,svinegylle,dybstrøelse o.s.v.
   CreateSelectListLanguage arrBoughtManure(4,bm),nStorageType,arrStorageType'dropdown med lagertyper: kvæggylle,svinegylle,dybstrøelse o.s.v.
   Response.Write("       </select>")   
   Response.Write("       &nbsp;<input name=""BoughtAmount_"&bm&""" id=""BoughtAmount"" type=""text"" size=""4"" value="""&arrBoughtManure(3,bm)&""" onChange=""this.style.color='#ff0000';"" />Kg")
'   Response.Write("       &nbsp;<button style=""width: 40px; height: 20px; font-size: 10px"" onClick=""document.forms.formFarm.DeleteBoughtManureID.value='"&arrBoughtManure(2,bm)&"';document.forms.formFarm.action='FarmBoughtManureDelete.asp';document.forms.formFarm.submit();"">Slet</button>")
   Response.Write("       &nbsp;<button style=""width: 50px; height: 20px; font-size: 10px"" onClick=""document.forms.formFarm.DeleteBoughtManureID.value='"&arrBoughtManure(2,bm)&"';document.forms.formFarm.action='FarmBoughtManureDelete.asp';document.forms.formFarm.submit();"">"&Str.Item("strDelete")&"</button>")
   Response.Write("      </td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   Response.Write("  <input type=""hidden"" name=""BMID_"&bm&""" id=""BMID_"&bm&""" value="""&arrBoughtManure(2,bm)&""" />" & vbCr)
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
   for sm = 0 to Ubound(arrSoldManure,2)-1
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")
   Response.Write("       <select name=""selectSoldManure_"&sm&""" onChange=""this.style.color='#ff0000';"">") 'dropdown med lagertyper: kvæggylle,svinegylle,dybstrøelse o.s.v.  
   CreateSelectListLanguage arrSoldManure(4,sm),nStorageType,arrStorageType
   Response.Write("       </select>")   
  
   Response.Write("       &nbsp;<input name=""SoldAmount_"&sm&""" id=""SoldAmount"" type=""text"" size=""4"" value="""&arrSoldManure(3,sm)&""" onChange=""this.style.color='#ff0000';"" />Kg")
   Response.Write("       &nbsp;<button style=""width: 50px; height: 20px; font-size: 10px"" onClick=""document.forms.formFarm.DeleteSoldManureID.value='"&arrSoldManure(2,sm)&"';document.forms.formFarm.action='FarmSoldManureDelete.asp';document.forms.formFarm.submit();"">"&Str.Item("strDelete")&"</button>")
  
   Response.Write("      </td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   Response.Write("  <input type=""hidden"" name=""SMID_"&sm&""" id=""SMID_"&sm&""" value="""&arrSoldManure(2,sm)&""" />" & vbCr)
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

   Response.Write("</td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left"">")

   
   
   Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""document.forms.formFarm.action='FarmScenarioDelete.asp';document.forms.formFarm.submit();"">"&Str.Item("strDELETESCEN")&"</button>")
   
'   Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:alert('"&Str.Item("strvirkerikke")&"')"">"&Str.Item("strDELETESCEN")&"</button>")
'   Response.Write("</td>" & vbCr)
'   Response.Write("      <td class=""data"" align=""left"">")
'   Response.Write("<button style=""width: 200px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formFarm.action='ScenarioNew.asp';document.forms.formFarm.submit();"">Ny ejendom / scenarie</button>")
'   Response.Write("</td>" & vbCr)
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

end sub
'**************************************************************************************************
%>
