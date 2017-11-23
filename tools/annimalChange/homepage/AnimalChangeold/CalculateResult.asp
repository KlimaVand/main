<%
'**************************************************************************************************
' Author: Margit Jorgensen
' Description: 
'**************************************************************************************************
'<!-- #include file="includes/NonPublicInc.asp" -->
option explicit
%>
<!-- #include file="includes/adovbs.asp" -->
<!-- #include file="includes/dbInterface.asp" -->
<!-- #include file="includes/General.asp" -->
<!-- #include file="includes/LanguageDictionary.asp" -->
<!-- #include file="includes/RotationModel2.asp" -->
<%
'<!-- #include file="includes/FunctionResult.asp" -->
%>
<!-- #include file="includes/FunctionBalance.asp" -->
<!-- #include file="includes/FunctionSoil.asp" -->
<!-- #include file="includes/FunctionField.asp" -->
<%
Private Const NUMBER_COL  = 3
Private Const NUMBER_ROW  = 10
Private Const NUMBER_COL_SURPLUS  = 4
Private Const NUMBER_ROW_SURPLUS  = 12
Private Const SURPLUSCORRECTION_HARVEST  = 0.45
Private Const SURPLUSCORRECTION_SOIL     = 0.10
Private Const SURPLUSCORRECTION_DENITR   = 0.10
Private Const SURPLUSCORRECTION_NLES     = 0.35

Private Const RED_NORM_CORRECTION_HARVEST  = 0.45'p33
Private Const RED_NORM_CORRECTION_NLES     = 0.35'p34
Private Const RED_NORM_CORRECTION_SOIL     = 0.11'p35
Private Const RED_NORM_CORRECTION_NH3EVAP  = 0.01'p36
Private Const RED_NORM_CORRECTION_DENITR   = 0.08'p37

Private Const LOSS_MANURE_N_FIELD      = 0.07

dim arrOutput
dim arrInput
dim arrSurplus
dim surplus, surplusCorr, surplusNcorr
dim arrFeedResult
dim totalArea, animalUnit
dim nRotation, arrRotation
dim boughtFeed, soldFeed, BoughtBeddingN, SoldBeddingN
dim TotalBoughtManure, TotalSoldManure, HighEnergyConc,meanRunOff
dim minNormReduction
dim normPercent, reducedFertilizer
dim aUserLoginName,aUserPassword,dictUser,arrUser,nUser,returnURL,NPercent,scale,arrA,arrB,aUserID,dictFarm
dim warning,totalManureNContent,MeanUtilization,totalManureShareOfNNeed,dictRotation,arrFarm,nFarm,x
Dim tid0, tid1, tid2, tid3, tid4, version

version = request("version")
if version = "" then version = 1

Server.ScriptTimeout=500
normPercent = request("normpercent")
if normPercent = "" then normPercent = 100
normPercent=Cdbl(replace(normPercent,",","."))
response.write normPercent

aUserLoginName = request("loginname")
if aUserLoginName="" Then
  aUserLoginName="VVM"
end if
aUserPassword = request("password")


InitFarmNDB
    set dictUser = CreateObject("Scripting.Dictionary")
    nUser = CallSP ("GetUserByLogin 'VVM','"&aUserPassword&"'", arrUser, dictUser)
'response.write "<br> nUser "& nUser 
'	nUser = GetUserByLogin("zzz", aUserID, arrUser)
	if CInt(nUser) = CInt(1) then
	   aUserID = arrUser(0,dictUser.item("UserID"))
	
       set dictFarm = CreateObject("Scripting.Dictionary")
       nFarm = CallSPStr ("GetFarmList '" & aUserID&"'", arrFarm, dictFarm)	   
'	   nFarm = GetFarmList(aUserID,arrFarm)
	end if
if CInt(nUser) = CInt(1) then
'if version = "" then version = arrUser(0,dictUser.item("ManureVersion")) 
   Session("hasLogin")       = "yes"
   Session("userID")         = arrUser(0,dictUser.item("UserID"))
   Session("nFarm")          = nFarm
   Session("user")           = arrUser(0,dictUser.item("UserName"))
   Session("userType")       = arrUser(0, dictUser.item("UserType"))'new Session-variable 14-06-06
   Session("farmNumber")     = arrFarm(0,dictFarm.item("FarmNumber"))
   Session("farmOwner")      = arrFarm(0,dictFarm.item("FarmOwnerName"))
   Session("scenarioNumber") = request("scenarioID")'arrFarm(0,dictFarm.item("SelectedScenario"))
   Session("scenarioName")   = arrFarm(0,dictFarm.item("ScenarioName"))
   Session("soilType")       = arrFarm(0,dictFarm.item("SoilType"))
   Session("irrigation")     = arrFarm(0,dictFarm.item("Irrigation"))
   if Session("irrigation") = "true" then 
      Session("irrigation") = 1
   else
      Session("irrigation") = 0
   end if
   Session("farmType")       = arrFarm(0,dictFarm.item("FarmType"))
   Session("yieldLevel")     = arrFarm(0,dictFarm.item("YieldLevel"))
   Session("tabNumber")      = 1
'	 Session.TimeOut					 = 40
else
   Session("hasLogin")       = "no"
end if
'if version = "" then version = 1
if Session("hasLogin") = "yes" then
   Session("loginTry") = "yes"
   returnURL = "CalculateResult.asp"
else
   Session("loginTry") = "no"
   returnURL = "default.asp"
end if
   ReDim arrParameter(1)
   arrParameter(0) = CLng(Session("farmNumber"))
   arrParameter(1) = CInt(Session("scenarioNumber"))
'   x = CallSPReturn ("VVM_AdjustTooMuchBoughtGrazingManure",arrParameter)
   CalculateManure version, NPercent, scale,arrA,arrB,warning, TotalBoughtManure, TotalSoldManure
'   InitFarmNDB
   CreateLanguageDictionary
   reDim arrInput(NUMBER_ROW-1,NUMBER_COL-1)
   reDim arrOutput(NUMBER_ROW-1,NUMBER_COL)
   reDim arrSurplus(NUMBER_ROW_SURPLUS-1,NUMBER_COL_SURPLUS-1)
   reDim arrFeedResult(NUMBER_FEED_ROW,NUMBER_FEED_COL-1)


   totalArea = CallSPReturn ("GetTotalArea",arrParameter)
   '***********************Dyreenheder for kvæg og grise vha stored procedure.

   Dim arranimal, dictanimal, nanimal
   Set dictanimal = CreateObject("Scripting.Dictionary")
   nanimal = CallSP("GetAnimalUnit "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber")), arranimal, dictanimal)
   animalUnit = arranimal(0,dictanimal.item("DE"))
   Set dictanimal = Nothing 
'   response.write "Test af animalUnit " & animalUnit
   
   
   Set dictRotation = CreateObject("Scripting.Dictionary")
   nRotation = CallSP("GetRotationList "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber")), arrRotation, dictRotation)

'   nRotation  = GetRotationList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation)
'   PrintArrayInTable2 arrRotation
'   DefineNormReductionScript

   InitTableFeed arrFeedResult, boughtFeed, soldFeed, HighEnergyConc, BoughtBeddingN, SoldBeddingN
   InitTableInput 'CLng(Session("farmNumber")),CInt(Session("scenarioNumber"))
   InitTableOutput 'CLng(Session("farmNumber")),CInt(Session("scenarioNumber"))
   InitTableSurplus 'CLng(Session("farmNumber")),CInt(Session("scenarioNumber"))
   
   CloseFarmNDB
'end sub
'**************************************************************************************************
sub CreateTableResult()
   CreateTableInputOutput
   CreateTableTotal
   CreateTableSurplus
end sub
'**************************************************************************************************
sub CreateTableInputOutput()
response.write "<br/>"&Str.Item("strUtilizedN")&":<input type=""text"" id=""d7"" class=""noborder"" size=""5"" value="""&FormatNumber(100*MeanUtilization,1,-1,0,0)&"""/>"
   Response.Write("<form name=""formFarm"" action=""DummyFarm.asp"" method=""post"">" & vbCr)
   Response.Write("<table class=""data"" border=""0"" width=""100%"">" & vbCr)
   CreateHeaderInputOutput
   CreateDataInputOutput
   Response.Write("</table>" & vbCr)
   Response.Write("</form>" & vbCr)
end sub
'**************************************************************************************************
sub CreateHeaderInputOutput()
   Response.Write("  <tr>" & vbCr)   
   Response.Write("    <td width=""21%"">&nbsp;</td>" & vbCr)
   Response.Write("    <td width=""7%"">&nbsp;</td>" & vbCr)
   Response.Write("    <td width=""7%"">&nbsp;</td>" & vbCr)
   Response.Write("    <td width=""4%"">&nbsp;</td>" & vbCr)
   Response.Write("    <td width=""22%"" align=""center""><h1>"&Str.Item("strFARMSCALE")&"</h1></td>" & vbCr)
   Response.Write("    <td width=""4%"">&nbsp;</td>" & vbCr)
   Response.Write("    <td width=""6%"">&nbsp;</td>" & vbCr)
   Response.Write("    <td width=""7%"">&nbsp;</td>" & vbCr)
   Response.Write("    <td width=""6%"">&nbsp;</td>" & vbCr)
   Response.Write("    <td width=""16%"">&nbsp;</td>" & vbCr)
   Response.Write("  </tr>" & vbCr)
   
'   Response.Write("  <tr>" & vbCr)   
'   Response.Write("    <td colspan=""9"" align=""center""><h1>"&Str.Item("strFARMSCALE")&"</h1></td>" & vbCr)
'   Response.Write("  </tr>" & vbCr)
      
   Response.Write("  <tr>" & vbCr)   
   Response.Write("    <td colspan=""4"" align=""center""><h3>"&Str.item("strINPUTKGN")&  " </h3></td>" & vbCr)
'   Response.Write("    <td rowspan="""&rMax+1&""" valign=""middle"" align=""center""><a href=""url""><img src=""images/farm.gif"" width=""200"" height=""128"" border=""0"" alt=""text""></a></td>" & vbCr)
   Response.Write("    <td rowspan="""&NUMBER_ROW+2&""" valign=""middle"" align=""center""><br/><br/><br/><br/><img src=""images/farm.gif"" width=""200"" height=""128"" border=""0"" alt=""text""></td>" & vbCr)
   Response.Write("    <td colspan=""4"" align=""center""><h3>"&Str.Item("strOUTPUTKGN")&"</h3></td>" & vbCr)
   Response.Write("  </tr>" & vbCr)
   
   Response.Write("  <tr>" & vbCr)   
   Response.Write("    <th>&nbsp;</th>" & vbCr)
   Response.Write("    <td align=""right"">"&Str.Item("strkg N/ha")&"</th>" & vbCr)
   Response.Write("    <td align=""right""><i>"&Str.Item("strReducedNNorm")&"</i></th>" & vbCr)
   Response.Write("    <th>&nbsp;</th>" & vbCr)
   Response.Write("    <th>&nbsp;</th>" & vbCr)
   Response.Write("    <td align=""right"">"&Str.Item("strkg N/ha")&"</th>" & vbCr)
   Response.Write("    <td align=""right"">"&Str.Item("strAdjusted")&"</th>" & vbCr)
   Response.Write("    <td align=""right""><i>"&Str.Item("strReducedNNorm")&"</i></th>" & vbCr)
   Response.Write("    <th>&nbsp;</th>" & vbCr)
   Response.Write("  </tr>" & vbCr)
end sub
'**************************************************************************************************
sub CreateDataInputOutput()
   dim r
   
   for r = 0 to NUMBER_ROW-1
      Response.Write("  <tr>" & vbCr)
      Response.Write("    <td align=""left"">"&arrInput(r,0)&"</td>" & vbCr)
         if arrInput(r,1)>=0 then
		     if totalArea <> 0 then
					  if r<=1 or r=9 then
		           Response.Write("    <td align=""right""><input type=""text"" id=""c1"&r&""" class=""noborder"" size=""5"" value="""&FormatNumber(arrInput(r,1)/totalArea,1,-1,0,0)&"""/></td>" & vbCr)
		           Response.Write("    <td align=""right""><input type=""text"" id=""d1"&r&""" class=""noborder"" size=""5"" value="""&FormatNumber(arrInput(r,1)/totalArea,1,-1,0,0)&"""/></td>" & vbCr)
	             Response.Write("    <td align=""right"">-&gt;</td>" & vbCr)
					  else
		           Response.Write("    <td align=""right"">"&FormatNumber(arrInput(r,1)/totalArea,1,-1,0,0)&"</td>" & vbCr)
		           Response.Write("    <td align=""right"">"&FormatNumber(arrInput(r,1)/totalArea,1,-1,0,0)&"</td>" & vbCr)
	             Response.Write("    <td align=""right"">-&gt;</td>" & vbCr)
	          end if
	         else
		           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
		           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
		           Response.Write("    <td align=""right"">-&gt;</td>" & vbCr)
'	             Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
	         end if
'	           Response.Write("    <td align=""right"">"&FormatNumber(arrInput(r,1)/totalArea,1,-1,0,0)&"</td>" & vbCr)
'             Response.Write("    <td align=""right"">-&gt;</td>" & vbCr)
         else
	           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
	           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
	           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
'             Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
         end if
      if arrOutput(r,1)>=0 then
		    if totalArea <> 0 then
				  if r>=8 then
		           if r=9 then
								Response.Write("    <td align=""left"">-&gt;</td>" & vbCr)
'								Response.Write("    <td align=""left"">&nbsp;</td>" & vbCr)
								Response.Write("    <td align=""right"">"&FormatNumber(arrOutput(r,1)/totalArea,1,-1,0,0)&"</td>" & vbCr)
		           else 
								Response.Write("    <td align=""left"">&nbsp;</td>" & vbCr)
'								Response.Write("    <td align=""left"">&nbsp;</td>" & vbCr)
								Response.Write("    <td align=""left"">&nbsp;</td>" & vbCr)
		           end if
		            Response.Write("    <td align=""right""><input type =""text"" id=""i1"&r&""" class=""noborder"" size=""5"" value="""&FormatNumber(arrOutput(r,2)/totalArea,1,-1,0,0)&"""/></td>" & vbCr)
		            Response.Write("    <td align=""right""><input type =""text"" id=""j1"&r&""" class=""noborder"" size=""5"" value="""&FormatNumber(arrOutput(r,2)/totalArea,1,-1,0,0)&"""/></td>" & vbCr)
				  else
		            Response.Write("    <td align=""left"">-&gt;</td>" & vbCr)
		            Response.Write("    <td align=""right"">"&FormatNumber(arrOutput(r,1)/totalArea,1,-1,0,0)&"</td>" & vbCr)
		            Response.Write("    <td align=""left"">&nbsp;</td>" & vbCr)
		            Response.Write("    <td align=""left"">&nbsp;</td>" & vbCr)
		      end if
'		    if totalArea <> 0 then
'               Response.Write("    <td align=""right"">"&FormatNumber(arrOutput(r,1)/totalArea,1,-1,0,0)&"</td>" & vbCr)
'		    else
'               Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
'		    end if
         else
           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
	         Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
	         Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
         end if
        else
          Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
          Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
         Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
         Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
        end if
      Response.Write("    <td align=""right"">"&arrOutput(r,0)&"</td>" & vbCr)
	  Response.Write("  </tr>" & vbCr)
   next 
	  Response.Write("  <input type=""hidden"" id=""p33"" value="""&RED_NORM_CORRECTION_HARVEST&"""/>" & vbCr)   
	  Response.Write("  <input type=""hidden"" id=""p34"" value="""&RED_NORM_CORRECTION_NLES&"""/>" & vbCr) 
	  Response.Write("  <input type=""hidden"" id=""p35"" value="""&RED_NORM_CORRECTION_SOIL&"""/>" & vbCr)  
	  Response.Write("  <input type=""hidden"" id=""p36"" value="""&RED_NORM_CORRECTION_NH3EVAP&"""/>" & vbCr)   
	  Response.Write("  <input type=""hidden"" id=""p37"" value="""&RED_NORM_CORRECTION_DENITR&"""/>" & vbCr)  
	  Response.Write("  <input type=""hidden"" id=""runoff"" value="""&meanRunOff&"""/>" & vbCr)  
end sub
'**************************************************************************************************
sub CreateTableSurplus()
   dim r

   Response.Write("<table class=""data"" align=""center"" border=""0"">" & vbCr)
   Response.Write("  <tr>" & vbCr)   
   Response.Write("    <td>&nbsp;</td>" & vbCr)
   Response.Write("    <td>&nbsp;</td>" & vbCr)
   Response.Write("    <td>&nbsp;</td>" & vbCr)

   Response.Write("  </tr>" & vbCr)
   
   Response.Write("  <tr>" & vbCr)   
   Response.Write("    <td colspan=""3"" align=""center""><h3>"&Str.Item("strFordelingNoverskud")&"</h3></td>" & vbCr)
   Response.Write("    <td>&nbsp;</td>" & vbCr)
   Response.Write("    <td>&nbsp;</td>" & vbCr)
   Response.Write("    <td>&nbsp;</td>" & vbCr)
   Response.Write("    <td>&nbsp;</td>" & vbCr)
   Response.Write("  </tr>" & vbCr)
'   Response.Write("  <tr>" & vbCr)   
'   Response.Write("    <th>&nbsp;</th>" & vbCr)
'   Response.Write("    <td>&nbsp;</td>" & vbCr)
'   Response.Write("    <th align=""right"">"&Str.Item("strTOTAL")&"</th>" & vbCr)
'   Response.Write("    <td>&nbsp;</td>" & vbCr)
'   Response.Write("    <th align=""right"">"&Str.Item("strPERHA")&"</th>" & vbCr)
'   Response.Write("    <td>&nbsp;</td>" & vbCr)
'   Response.Write("    <th align=""right"">"&Str.Item("strPERHA")&"</th>" & vbCr)
'   Response.Write("  </tr>" & vbCr)
   Response.Write("  <tr>" & vbCr)   
   Response.Write("    <td>&nbsp;</th>" & vbCr)
   Response.Write("    <td>&nbsp;</td>" & vbCr)
   Response.Write("    <th align=""right"">"&Str.Item("strkg N/ha")&"</th>" & vbCr)
   Response.Write("    <td>&nbsp;</td>" & vbCr)
   Response.Write("    <th align=""right"">"&Str.Item("strAdjusted")&"</th>" & vbCr)
   Response.Write("    <td>&nbsp;</td>" & vbCr)
'   Response.Write("    <th align=""right""><form name=""norm""><input type=""text"" name=""normreduction"" value=""100"" size=""3"" maxlength=""3"" onChange=""if(InputValidator(document.forms.norm, 1, 100, "normreduction", 3)){return(true);};""></form>"&Str.Item("strReducedNNorm")&"</th>" & vbCr)
'   Response.Write("    <th align=""right""><input type=""text"" name=""normreduction"" value=""100"" size=""3"" maxlength=""3"" onChange=""if(InputValidator(document.forms.norm.normreduction, 1, 100, "", 3)){return(true);};"">"&Str.Item("strReducedNNorm")&"</th>" & vbCr)
   Response.Write("    <th align=""right""><input type=""text"" name=""normreduction"" id=""normreduction"" value=""100"" size=""2"" maxlength=""3"" onChange=""if(this.value>100||this.value<0||isNaN(this.value)){this.value=100;};normreduction(this.value);"">"&Str.Item("str%ofNNorm")&"</th>" & vbCr)
   Response.Write("    <td><input type=""button"" value=""OK""/></td>" & vbCr)
   Response.Write("  </tr>" & vbCr)

   for r = 0 to NUMBER_ROW_SURPLUS-1
'			if r <  NUMBER_ROW_SURPLUS-1 then
'        Response.Write("  <tr>" & vbCr)
'			else
			if r = 10 or r = 3 or r = 4 then
        Response.Write("  <tr><td colspan=""7"" style=""border-bottom: solid navy 1px;height: 1px;font-size: 0px;padding: 0px;>"">&nbsp;</td>" & vbCr)
        Response.Write("  </tr>" & vbCr)
        Response.Write("  <tr>" & vbCr)
			else
        Response.Write("  <tr>" & vbCr)
			end if
      Response.Write("    <td align=""left"">"&arrSurplus(r,0)&"</td>" & vbCr)
      Response.Write("    <td>&nbsp;</td>" & vbCr)
         if  not IsNull(arrSurplus(r,1)) then
'	         Response.Write("    <td align=""right"">"&FormatNumber(arrSurplus(r,1),0,-1,0,0)&"</td>" & vbCr)
'       	         Response.Write("    <td>&nbsp;</td>" & vbCr)
		   	     if totalArea <> 0 then
			           Response.Write("    <td align=""right"">"&FormatNumber(arrSurplus(r,1)/totalArea,1,-1,0,0)&"</td>" & vbCr)
			           Response.Write("    <td>&nbsp;</td>" & vbCr)
			           if r <>10 then'<  NUMBER_ROW_SURPLUS-1 then
			           Response.Write("    <td align=""right""><input type=""text"" id=""i"&Cint(40+r-6)&""" class=""noborder"" size=""5"" value="""&FormatNumber(arrSurplus(r,2)/totalArea,1,-1,0,0)&"""/></td>" & vbCr)
			           Response.Write("    <td>&nbsp;</td>" & vbCr)
			           Response.Write("    <td align=""right""><input type=""text"" id=""j"&Cint(40+r-6)&""" class=""noborder"" size=""5"" value="""&FormatNumber(arrSurplus(r,3)/totalArea,1,-1,0,0)&"""/></td>" & vbCr)
			           else
			           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
			           Response.Write("    <td>&nbsp;</td>" & vbCr)
			           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
			           end if
				     else
			           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
			           Response.Write("    <td>&nbsp;</td>" & vbCr)
			           Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
             end if
         else
	         Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
	         Response.Write("    <td>&nbsp;</td>" & vbCr)
                 Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
                 Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
                 Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
                 Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
                 Response.Write("    <td align=""right"">&nbsp;</td>" & vbCr)
         end if
if r=10 then				 
   Response.Write("  </tr><td></td>" & vbCr)
   Response.Write("  <tr>" & vbCr)
end if
   Response.Write("  </tr>" & vbCr)
   next   

   Response.Write("</table>" & vbCr)

end sub
'**************************************************************************************************
sub CreateTableTotal()

   Response.Write("<table class=""data"" align=""center"" border=""0"">" & vbCr) 
   Response.Write("  <tr>" & vbCr)   
   Response.Write("    <td class=""data"">&nbsp;</td>" & vbCr)
   Response.Write("    <td class=""data"">&nbsp;</td>" & vbCr)
	 Response.Write("      <td class=""data"" align=""right"">"&Str.Item("strkg N/ha")&"</td>" & vbCr)
	 Response.Write("      <td class=""data"" align=""right"">"&Str.Item("strAdjusted")&"</td>" & vbCr)
	 Response.Write("      <td class=""data"" align=""right"">"&Str.Item("strReducedNNorm")&"</td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   Response.Write("    <tr>" & vbCr)
'      if surplus >= 0 then 
'         Response.Write("      <td align=""right"">"&FormatNumber(surplus,0,-1,0,0)&"</td>" & vbCr)
'         Response.Write("      <td class=""data"">"&Str.Item("strkg N")&"</td>" & vbCr)
   	     if totalArea <> 0 then
   					Response.Write("    <td class=""data"">"&Str.Item("strBedriftNoverskud")&":</td>" & vbCr)
   					Response.Write("    <td class=""data"">&nbsp;</td>" & vbCr)
	           Response.Write("    <td align=""right"">"&FormatNumber(surplus/totalArea,1,-1,0,0)&"</td>" & vbCr)
	           Response.Write("    <td align=""right""><input type=""text"" id=""g23"" class=""noborder"" size=""5"" value="""&FormatNumber(surplusCorr/totalArea,1,-1,0,0)&"""/></td>" & vbCr)
	           Response.Write("    <td align=""right""><input type=""text"" id=""h23"" class=""noborder"" size=""5"" value="""&FormatNumber(surplusCorr/totalArea,1,-1,0,0)&"""/></td>" & vbCr)
		     else
   					Response.Write("    <td class=""data"">&nbsp;</td>" & vbCr)
   					Response.Write("    <td class=""data"">&nbsp;</td>" & vbCr)
	           Response.Write("    <td align=""right"">-</td>" & vbCr)
	           Response.Write("    <td align=""right"">-</td>" & vbCr)
	           Response.Write("    <td align=""right"">-</td>" & vbCr)
'         Response.Write("      <td align=""right"">"&FormatNumber(surplus/totalArea,1)&"</td>" & vbCr)
'	 Response.Write("      <td class=""data"">"&Str.Item("strkg N/ha")&"</td>" & vbCr)

'      else
'         Response.Write("      <td align=""right"">-999,9</td>" & vbCr)
'         Response.Write("      <td class=""data"">"&Str.Item("strkg N")&"</td>" & vbCr)
'         Response.Write("      <td align=""right"">-999,9</td>" & vbCr)
'	 Response.Write("      <td class=""data"">"&Str.Item("strKGNPERHA")&"</td>" & vbCr)
      end if

   Response.Write("    </tr>" & vbCr)
   Response.Write("  <tr>" & vbCr)   
   Response.Write("    <td class=""data"">"&Str.Item("strAREA")&" "&Str.Item("strCOLON")& "</td>" & vbCr)
'   Response.Write("    <td class=""data"">&nbsp;</td>" & vbCr)
   Response.Write("    <td align=""right"">"&FormatNumber(totalArea,0,-1,0,0)&"</td>" & vbCr)
   Response.Write("    <td class=""data"">"&Str.Item("strHA")&"</td>" & vbCr)
   Response.Write("    <td class=""data"">&nbsp;</td>" & vbCr)
   Response.Write("    <td class=""data"">&nbsp;</td>" & vbCr)
   Response.Write("  </tr>" & vbCr)
'if session("Language") <> "uk"  then
   Response.Write("  <tr>" & vbCr)   
   Response.Write("    <td class=""data"">"&Str.Item("strHERD")&" "&Str.Item("strCOLON")&"</td>" & vbCr)
'   Response.Write("    <td class=""data"">&nbsp;</td>" & vbCr)
   Response.Write("    <td align=""right"">"&FormatNumber(animalUnit,0,-1,0,0)&"</td>" & vbCr)
   Response.Write("    <td class=""data"">"&Str.Item("strLU")&"</td>" & vbCr)
'   	     if totalArea <> 0 then
'	           Response.Write("    <td align=""right"">"&FormatNumber(animalUnit/totalarea,1)&"</td>" & vbCr)
'		     else
'	           Response.Write("    <td align=""right"">-</td>" & vbCr)
'             end if
'   Response.Write("    <td align=""right"">"&FormatNumber(animalUnit/totalarea,1)&"</td>" & vbCr)
'   Response.Write("    <td class=""data"">DE/ha</td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
'end if
   Response.Write("    <tr>" & vbCr)   
   Response.Write("    </tr>" & vbCr)
   Response.Write("</table>" & vbCr)
end sub
'**************************************************************************************************
sub InitTableInput()
dim boughtFertilizer, pigs, BoughtPigs, a,b,c,d
'PrintArrayInTable2 arrRotation
 '  boughtFertilizer = GetBoughtFertilizer(arrRotation)
 ReDim arrParameter(3)
   arrParameter(0) = CLng(Session("farmNumber"))
   arrParameter(1) = CInt(Session("scenarioNumber"))
   arrParameter(2) = 1
   arrParameter(3) = 415
   a = CallSPReturn ("GetPigHerdTotal",arrParameter)
'   a=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 1, 415) - 
   arrParameter(2) = 3
   arrParameter(3) = 414
   b = CallSPReturn ("GetPigHerdTotal",arrParameter)
'   b=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 3, 414) + 
   arrParameter(2) = 3
   arrParameter(3) = 415
   c = CallSPReturn ("GetPigHerdTotal",arrParameter)
'   c=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 3, 415) - 
   arrParameter(2) = 2
   arrParameter(3) = 414
   d = CallSPReturn ("GetPigHerdTotal",arrParameter)
'   d=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 2, 414)
   pigs =a-b+c-d 

   if pigs < 0 then 
   BoughtPigs=-pigs
   else 
   BoughtPigs=0
   end if
   arrInput(0,0)  = ""&Str.Item("strMINFERT")&"" 
   ReDim preserve arrParameter(1)
   arrInput(0,1)  = CallSPReturn ("GetFertilizer",arrParameter)/totalArea
'   arrInput(0,1)  = GetFertilizer(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   arrInput(1,0)  = ""&Str.Item("strMANUREBUY")&""
   arrInput(1,1)  = TotalBoughtManure/totalArea
   If not Isnull(meanUtilization) and meanUtilization<>0 then
	 minNormReduction = Cint(100*arrInput(1,1)*meanUtilization/(arrInput(0,1)+(arrInput(1,1)*meanUtilization)))
   else
	 minNormReduction = 0
   end if
'	 response.write minNormReduction
	 if normPercent<minNormReduction then normPercent = minNormReduction
'	 response.write normPercent
   arrInput(0,2)  = round(arrInput(0,1),1)-(((meanUtilization)*round(arrInput(1,1),1)+round(arrInput(0,1),1))-((meanUtilization)*round(arrInput(1,1),1)+round(arrInput(0,1),1))*(normPercent/100))
   reducedFertilizer = round(arrInput(0,1),1)-round(arrInput(0,2),1)
   arrInput(1,2)  = TotalBoughtManure/totalArea
   arrInput(2,0)  = ""&Str.Item("strFEEDBUY")&""
   arrInput(2,1)  = boughtFeed/totalArea
   arrInput(2,2)  = boughtFeed/totalArea
   arrInput(3,0)  = ""&Str.Item("strSTRAWBUY")&""
   arrInput(3,1)  = BoughtBeddingN/totalArea
   arrInput(3,2)  = BoughtBeddingN/totalArea
   arrInput(4,0)  = ""&Str.Item("strANIMBUY")&""
   arrInput(4,1)  = BoughtPigs/totalArea
   arrInput(4,2)  = BoughtPigs/totalArea
   arrInput(5,0)  = ""&Str.Item("strSEED")&""
   arrInput(5,1)  = CallSPReturn ("GetTotalNInSeed",arrParameter)/totalArea
   arrInput(5,2)  = arrInput(5,1)
'   arrInput(5,1)  = GetTotalNInSeed(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   arrInput(6,0)  = ""&Str.Item("strNFIX")&""
   arrInput(6,1)  = GetFixation(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation)/totalArea
   arrInput(6,2)  = arrInput(6,1)
   arrInput(7,0)  = ""&Str.Item("strATMOSDEP")&""
   arrInput(7,1)  = 15'*totalArea
   arrInput(7,2)  = arrInput(7,1)
   arrInput(8,0)  = ""
   arrInput(8,1)  = null
   arrInput(8,2)  = null
   arrInput(9,0)  = ""&Str.Item("strTOTINPUT")&""
'   Response.write "<br> Test: " & arrInput(2,1)
   arrInput(9,1)  = arrInput(0,1)+arrInput(1,1)+arrInput(2,1)+arrInput(3,1)+arrInput(4,1)+arrInput(5,1)+arrInput(6,1)+arrInput(7,1)
   arrInput(9,2)  = arrInput(9,1)-(reducedFertilizer)
'   Response.write "<br> Test2: " & arrInput(9,1)
end sub
'**************************************************************************************************
sub InitTableOutput()
dim pigs, SoldPigs, nMilk, arrMilk, dictMilk, a, b, c, d
 
ReDim arrParameter(3)
   arrParameter(0) = CLng(Session("farmNumber"))
   arrParameter(1) = CInt(Session("scenarioNumber"))
   arrParameter(2) = 1
   arrParameter(3) = 415
   a = CallSPReturn ("GetPigHerdTotal",arrParameter)
'   a=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 1, 415) - 
   arrParameter(2) = 3
   arrParameter(3) = 414
   b = CallSPReturn ("GetPigHerdTotal",arrParameter)
'   b=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 3, 414) + 
   arrParameter(2) = 3
   arrParameter(3) = 415
   c = CallSPReturn ("GetPigHerdTotal",arrParameter)
'   c=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 3, 415) - 
   arrParameter(2) = 2
   arrParameter(3) = 414
   d = CallSPReturn ("GetPigHerdTotal",arrParameter)
'   d=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 2, 414)
   pigs =a-b+c-d 
 
 
 
 
' pigs = GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 1, 415) - GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 3, 414) + GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 3, 415) - GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 2, 414)
 
 
 
   set dictMilk = CreateObject("Scripting.Dictionary")
   nMilk = CallSP ("CalcTotalMilkkg " & CLng(Session("farmNumber")) & "," & CInt(Session("scenarioNumber")), arrMilk, dictMilk)
   nMilk = arrMilk(0, dictMilk.item("SumMilkkg"))
 
' nMilk = CalcTotalMilkkg(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrMilk) 
   If IsNull(arrMilk(0, dictMilk.item("SumMilkkg"))) Then 
   arrMilk(0, dictMilk.item("SumMilkkg"))=0
   End If
   arrMilk(0, dictMilk.item("SumMilkkg"))=arrMilk(0, dictMilk.item("SumMilkkg"))*0.00518
   if pigs > 0 then 
   SoldPigs=pigs
   else 
   SoldPigs=0
   end if
   arrOutput(0,0)  = ""&Str.Item("strCASHCROP")&""
   arrOutput(0,1)  = GetSoldCrop/totalArea
   arrOutput(1,0)  = ""&Str.Item("strANIMSOLD")&""
   arrOutput(1,1)  = SoldPigs/totalArea
   arrOutput(2,0)  = ""&Str.Item("strMEAT")&""
   ReDim preserve arrParameter(2)
   arrParameter(2) = PARAMETER_GROWTH_N
   arrOutput(2,1)  = CallSPReturn ("GetFarmTotal",arrParameter)/totalArea
'   arrOutput(2,1)  = GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), PARAMETER_GROWTH_N)
   arrOutput(3,0)  = ""&Str.Item("strMILK")&""
   arrOutput(3,1)  = arrMilk(0, dictMilk.item("SumMilkkg"))/totalArea
   arrOutput(4,0)  = ""&Str.Item("strMANURESOLD")&""
   arrOutput(4,1)  = TotalSoldManure/totalArea ' GetSoldManureTotal(8)
   arrOutput(5,0)  = ""&Str.Item("strFEEDSOLD")&""
   arrOutput(5,1)  = soldFeed/totalArea
   arrOutput(6,0)  = ""&Str.Item("strSoldStraw")&""
   arrOutput(6,1)  = SoldBeddingN/totalArea
   arrOutput(7,0)  = ""&Str.Item("strSoldPigMeat")&""

   ReDim  preserve arrParameter(3)
   arrParameter(0) = CLng(Session("farmNumber"))
   arrParameter(1) = CInt(Session("scenarioNumber"))
   arrParameter(2) = 2
   arrParameter(3) = 415
   a = CallSPReturn ("GetPigHerdTotal",arrParameter)
   ReDim  preserve arrParameter(2)
   arrParameter(2) = 413
   b = CallSPReturn ("GetFarmTotal",arrParameter)
   arrOutput(7,1)  = a+b/totalArea

'   arrOutput(7,1)  = GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 2, 415)+GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 413)
   arrOutput(8,0)  = ""&Str.Item("strPlantUptake")&""
   arrOutput(8,2)  = arrOutput(0,1)+arrOutput(5,1)+arrOutput(6,1)'null

   arrOutput(9,0) = ""&Str.Item("strOUTPUTALL")&""
   arrOutput(9,1) = arrOutput(0,1)+arrOutput(1,1)+arrOutput(2,1)+arrOutput(3,1)+arrOutput(4,1)+arrOutput(5,1)+arrOutput(6,1)+arrOutput(7,1)
end sub
'**************************************************************************************************
sub InitTableSurplus()
   dim aSoilChange, aBalance, NLeaching
   dim nNLES,meanNLES_N,meanNLES_mg,arrNLES, remainder,deltaD,deltaU
dim arrResult,y,x

redim arrResult(3,1)

   surplus = arrInput(9,1)- arrOutput(9,1)
		 
   ReDim  arrParameter(1)
   arrParameter(0) = CLng(Session("farmNumber"))
   arrParameter(1) = CInt(Session("scenarioNumber"))
   
   arrSurplus(0,0)  = ""&Str.Item("strNH3HOUSE")&""
   arrSurplus(0,1)  = CallSPReturn ("GetNitrogenStableLossTotal",arrParameter)/totalArea
   arrSurplus(0,2)  = arrSurplus(0,1)
   arrSurplus(0,3)  = arrSurplus(0,1)
'   arrSurplus(0,1)  = GetNitrogenStableLossTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   arrSurplus(1,0)  = ""&Str.Item("strNH3STORE")&""
   arrSurplus(1,1)  = CallSPReturn ("GetNitrogenStorageLossTotal",arrParameter)/totalArea
   arrSurplus(1,2)  = arrSurplus(1,1)
   arrSurplus(1,3)  = arrSurplus(1,1)
'   arrSurplus(1,1)  = GetNitrogenStorageLossTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   arrSurplus(2,0)  = ""&Str.Item("strDENITRIF")&", " & Str.Item("storage")&""
   arrSurplus(2,1)  = CallSPReturn ("GetNitrogenStorageDenitrificationTotal",arrParameter)/totalArea
   arrSurplus(2,2)  = arrSurplus(2,1)
   arrSurplus(2,3)  = arrSurplus(2,1)
   arrSurplus(3,0)  = ""&Str.Item("strFieldSurplus")&""
   arrSurplus(3,1)  = surplus-arrSurplus(0,1)-arrSurplus(1,1)-arrSurplus(2,1)
   arrSurplus(3,2)  = arrSurplus(3,1)
'   arrSurplus(2,1) = GetNitrogenStorageDenitrificationTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   arrSurplus(4,0)  = ""&Str.Item("strNH3GRAZING")&""
   arrSurplus(4,1)  = LOSS_MANURE_N_FIELD * CallSPReturn ("fnGetBoughtGrazingN ",arrParameter)/totalArea
  ReDim  preserve arrParameter(2)
   arrParameter(2)  = PARAMETER_MANURE_N_FIELD
   arrSurplus(4,1)  = arrSurplus(4,1) + LOSS_MANURE_N_FIELD * CallSPReturn ("GetFarmTotal",arrParameter)/totalArea
   arrSurplus(4,2)  = arrSurplus(4,1)
   arrSurplus(4,3)  = arrSurplus(4,1)
'   arrSurplus(3,1)  = 0.07*GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), PARAMETER_MANURE_N_FIELD)
  ReDim  preserve arrParameter(1)
'  Response.write "<br> arrParameter "& arrParameter(0) & " " & arrParameter(1)
   arrSurplus(5,0)  = ""&Str.Item("strNH3SPREAD")&""
   arrSurplus(5,1)  = CallSPReturn ("GetTotalLossManure",arrParameter)/totalArea
   arrSurplus(5,2)  = arrSurplus(5,1)
   arrSurplus(5,3)  = arrSurplus(5,1)
'  Response.write "<br> arrSurplus "& arrSurplus(4,1)
'   arrSurplus(4,1)  = GetTotalLossManure(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   arrSurplus(6,0) = ""&Str.Item("strNH3MIN")&""
   arrSurplus(6,1) = 0.03 * CallSPReturn ("GetFertilizer",arrParameter)/totalArea
   arrSurplus(6,2) = arrSurplus(6,1)
   arrSurplus(6,3) = round(arrSurplus(6,2),1)-reducedFertilizer*RED_NORM_CORRECTION_NH3EVAP
'   arrSurplus(5,1) = 0.03*GetFertilizer(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   arrSurplus(7,0) = ""&Str.Item("strDENITRIF")&", " & Str.Item("strsoil")&""
   arrSurplus(7,1) = (Denitrification(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),CInt(Session("farmType")),arrRotation))/totalArea'*totalArea'der skal ikke ganges med arealet her
  ' arrSurplus(6,0) = ""&Str.Item("strDENITRIF")&" " & Str.Item("storage")&""
  ' arrSurplus(6,1) = GetNitrogenStorageDenitrificationTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
'  Response.write "<br> Denitrification "& Denitrification(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),CInt(Session("farmType")),arrRotation)
  aBalance = surplus - arrSurplus(0,1) - arrSurplus(1,1) - arrSurplus(2,1) - arrSurplus(4,1) - arrSurplus(5,1) - arrSurplus(6,1) - arrSurplus(7,1)

  aSoilChange = GetSoilChange(arrRotation,totalArea)/totalArea
  NLeaching = NitrogenLeaching(aBalance, aSoilChange, totalArea)'/totalArea 
  nNLES = CalcN_Les(meanNLES_N,meanNLES_mg,meanRunOff,arrNLES)/totalArea
	remainder = NLeaching-meanNLES_N'*totalArea
'	deltaD
'   Response.write "<br> aSoilChange "& aSoilChange & " tot " & totalArea
'   Response.write "<br> NLeaching "& NLeaching

   arrSurplus(8,0) = ""&Str.Item("strSOILCHANGE")&""
      if not IsNull(aSoilChange) then
         arrSurplus(8,1) = aSoilChange
      else 
         arrSurplus(8,1) = null
      end if
'  arrSurplus(8,0) = ""&Str.Item("strNLEACH")&""
'      if not IsNull(NLeaching) then
'         arrSurplus(8,1) = NLeaching
'      else 
'         arrSurplus(8,1) = null
'      end if
  arrSurplus(9,0) = Str.Item("strNLEACH")& " ("&Str.Item("strkg N/ha")&")"
      if not IsNull(meanNLES_N) then
         arrSurplus(9,1) = meanNLES_N'*totalArea
      else 
         arrSurplus(9,1) = null
      end if
  arrSurplus(10,0) = ""&Str.Item("strRemainder")&""
  arrSurplus(11,0) = ""&Str.Item("strNLEACH")& " ("&Str.Item("strMgNO3/l")&")"
'      if not IsNull(meanNLES_N*totalArea-NLeaching) then
'         arrSurplus(9,1) = meanNLES_N*totalArea-NLeaching
      if not IsNull(remainder) and meanNLES_N<>0 then
			'den første måde at fordele rest på
'         arrSurplus(10,1) = remainder
''				 arrSurplus(6,2) = arrSurplus(6,1)-(arrSurplus(6,1)/(arrSurplus(6,1)+aSoilChange)*remainder)
'				 arrSurplus(7,2) = arrSurplus(7,1)+(Cdbl(arrSurplus(7,1))/(Cdbl(arrSurplus(7,1))+Cdbl(meanNLES_N*totalArea))*Cdbl(remainder))
'				 arrSurplus(7,3) = arrSurplus(7,2)
'				 arrSurplus(8,2) = aSoilChange+(meanNLES_N*totalArea/(arrSurplus(7,1)+meanNLES_N*totalArea)*remainder*0.5)
'				 arrSurplus(8,3) = arrSurplus(8,2)
'				 arrSurplus(9,2) = (meanNLES_N*totalArea)+(meanNLES_N*totalArea/(arrSurplus(7,1)+meanNLES_N*totalArea)*remainder*0.5)
'				 arrSurplus(9,3) = arrSurplus(9,2)
       'den nye måde at fordele rest på
         arrSurplus(10,1) = remainder
         arrSurplus(11,1) = meanNLES_mg'*totalArea
				 arrOutput(8,2) = arrOutput(8,2)+ remainder*SURPLUSCORRECTION_HARVEST
				 arrOutput(8,3) = round(arrOutput(8,2),1)-reducedFertilizer*RED_NORM_CORRECTION_HARVEST
				 surplusCorr = surplus-remainder*SURPLUSCORRECTION_HARVEST
         arrSurplus(3,2)  = surplusCorr-arrSurplus(0,1)-arrSurplus(1,1)-arrSurplus(2,1)
				 arrOutput(9,2) = arrOutput(8,2)+arrOutput(1,1)+arrOutput(2,1)+arrOutput(3,1)+arrOutput(4,1)+arrOutput(7,1)
				 arrOutput(9,3) = round(arrOutput(9,2),1)-(round(arrOutput(8,2),1)-round(arrOutput(8,3),1))
'				 arrSurplus(6,2) = arrSurplus(6,1)-(arrSurplus(6,1)/(arrSurplus(6,1)+aSoilChange)*remainder)
				 arrSurplus(7,2) = arrSurplus(7,1)+ remainder*SURPLUSCORRECTION_DENITR
				 arrSurplus(7,3) = round(arrSurplus(7,2),1)-reducedFertilizer*RED_NORM_CORRECTION_DENITR
				if arrSurplus(7,3)< 0 then arrSurplus(7,3)= 0 
				 arrSurplus(8,2) = (arrSurplus(8,1)+ remainder*SURPLUSCORRECTION_SOIL)
				 arrSurplus(8,3) = round(arrSurplus(8,2),1)-reducedFertilizer*RED_NORM_CORRECTION_SOIL
				 arrSurplus(9,2) = arrSurplus(9,1)+ remainder*SURPLUSCORRECTION_NLES
				 arrSurplus(11,2) = 100*arrSurplus(9,2)*4.43/meanRunOff
				 arrSurplus(9,3) = round(arrSurplus(9,2),1)-reducedFertilizer*RED_NORM_CORRECTION_NLES
				 arrSurplus(11,3) = 100*round(arrSurplus(9,3),1)*4.43/meanRunOff
      else 
         arrSurplus(10,1) = null
      end if
	  
      surplusNcorr = round(arrInput(9,2),1)-round(arrOutput(9,3),1)
      arrSurplus(3,3)  = surplusNcorr-arrSurplus(0,1)-arrSurplus(1,1)-arrSurplus(2,1)
	  
      arrResult(0,0)=60'NLes KgN/ha
      arrResult(1,0)=62'NLes mgNO3/l
      arrResult(2,0)=260'NLes KgN/ha corrected+normreduction
      arrResult(3,0)=262'NLes mgNO3/l corrected+normreduction
      arrResult(0,1)=round(arrSurplus(9,1),1)
      arrResult(1,1)=round(arrSurplus(11,1),1)
      arrResult(2,1)=round(arrSurplus(9,3),1)
      arrResult(3,1)=round(arrSurplus(11,3),1)
	  
   redim preserve arrParameter(3)
	 for y = 0 to ubound(arrResult)
	   arrParameter(2)=arrResult(y,0)
     if not isnull(arrResult(y,1)) and (arrResult(y,1)<>"") then
	    arrParameter(3)=arrResult(y,1)
		 else 
	    arrParameter(3)=0
		 end if 
     x = CallSPReturn ("InsertUpdateResult " , arrParameter)
	 next
	   arrParameter(2)=6'parameter for normpercent
	   arrParameter(3)=normpercent
     x = CallSPReturn ("InsertUpdateResult " , arrParameter)
	   arrParameter(2)=500'parameter for forhold mll.korrigeret NLes og NLes 
	   arrParameter(3)=arrSurplus(9,3)/(arrSurplus(9,1))'+arrSurplus(9,3))
     x = CallSPReturn ("InsertUpdateResult " , arrParameter)
     redim arrParameter(1)
       arrParameter(0) = CLng(Session("farmNumber"))
       arrParameter(1) = cint(version)
	 x = CallSPReturn ("UpdateManureVersion " , arrParameter)
end sub
'**************************************************************************************************
%>
