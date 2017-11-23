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
<!-- #include file="includes/FunctionBalance.asp" -->
<!-- #include file="includes/RotationModel2.asp" -->
<%
dim disabledString, invisibleString, arrDisabled, arrInvisible
dim nShowTabList, arrShowTabList, dictShowTabList
dim dummy1, dummy2, dummy3, dummy4, HighEnergyConc, arrparameter
dim internalGrazingDM
dim showPopup,debug
dim varstr


Private Const TITLE = "Tab"

disabledString = request("disabledString")
invisibleString = request("invisibleString")
debug=request("debug")
if debug="" then debug="0"
showPopup = 0

   InitFarmNDB
   reDim arrFeedBalance(23,10)
   ReDim arrParameter(1)
   arrParameter(0) = CLng(Session("farmNumber"))
   arrParameter(1) = CInt(Session("scenarioNumber"))
'   InitTableFeed arrFeedBalance, dummy1, dummy2, HighEnergyConc, dummy3, dummy4,internalGrazingDM
'response.write "internalGrazingDM : "&internalGrazingDM
'		  if abs(internalGrazingDM)>5 then
'		  showPopup = 1
'		  end if
   
   set dictShowTabList = CreateObject("Scripting.Dictionary")
   nShowTabList = CallSP ("GetShowTabList " & CLng(Session("farmNumber"))& "," & Session("scenarioNumber") , arrShowTabList, dictShowTabList)
'   response.write "nShowTabList = "&nShowTabList
   redim arrDisabled(10)
if disabledString = "" then
   
   arrDisabled(0) = ""
   if Session("ShowResult")=False then
   arrDisabled(6) = 7
   else
   arrDisabled(6) = ""
   end if
   arrDisabled(7) = 8
   arrDisabled(8) = 9
   arrDisabled(9) = 10
   if nShowTabList<>0 then
   if cint(arrShowTabList(0,dictShowTabList.item("FieldPlanID")))= 0 then
          Session("LockAgroZone")=0
          arrDisabled(2) = 3
          arrDisabled(6) = 7
 	      arrDisabled(7) = 8
 	      arrDisabled(8) = 9
          arrDisabled(9) = 10
    if cstr(arrShowTabList(0,dictShowTabList.item("Landless")))= "False" then
	      arrDisabled(3) = 4
	      arrDisabled(4) = 5
	      arrDisabled(5) = 6
		  arrDisabled(6) = 7
	      arrDisabled(7) = 8
	      arrDisabled(8) = 9
          arrDisabled(9) = 10
	 end if
	 end if
   else
          Session("LockAgroZone")=1
   if nShowTabList<>0 then
        if arrShowTabList(0,dictShowTabList.item("CattleHerdID"))= 0 and arrShowTabList(0,dictShowTabList.item("SheepHerdID"))= 0 and arrShowTabList(0,dictShowTabList.item("PigHerdID"))= 0  and arrShowTabList(0,dictShowTabList.item("BoughtManureID"))= 0 then
'	      arrDisabled(6) = 7
	      arrDisabled(7) = 8
	      arrDisabled(8) = 9
          arrDisabled(9) = 10
		else
          arrDisabled(0) = ""
		  if abs(internalGrazingDM)>5 then
          arrDisabled(0) = ""
	      arrDisabled(7) = 8
	      arrDisabled(8) = 9
          arrDisabled(9) = 10
		'  showPopup = 1
'		  else
'		      if Session("ShowResult")<>true then 
'			  arrDisabled(6) = 8
'			  arrDisabled(7) = 9
'			  end if
		  end if
		end if
	end if	
   end if
		      if Cstr(Session("ShowResult"))="True" then 
              arrDisabled(0) = ""
			  arrDisabled(7) = ""
			  arrDisabled(8) = ""
              arrDisabled(9) = ""
			  end if
   
disabledString = join(arrDisabled,",")
'response.write disabledString
end if 
  
   CreateLanguageDictionary
   CloseFarmNDB


BeginHeader TITLE,"",""
CreateStyleLink "styles/stylesheet.css"
EndHeader
BeginBody
'response.write showPopup
if showPopup = 1 then
varstr="Warning: There is not balance between grazing amount \non Ruminants page and grazing on Yield page"
   if internalGrazingDM > 0 then
   varstr=varstr & "\n\nToo much grazing registered for Ruminants"   
   else
   varstr=varstr & "\n\nToo much grazing registered on Yield"
   end if   
response.write "<script language=""javascript"">alert('"&varstr&"')</script>"
end if
dim tab
tab = request("TB")
if cstr(tab) = "" then
tab = Session("tabNumber")
end if
if cstr(tab) = "" then
tab = 1
end if
CreatePage(cint(tab))
EndBody
'**************************************************************************************************
function FindString(tb)

select case cint(tb)
case 1
FindString = Str.Item("strFARM")
case 2
FindString = Str.Item("strFIELD")
case 3
FindString = Str.Item("strROTATION") 
case 4
FindString = Str.Item("strCATTLE")
'case 5
'FindString = Str.Item("strSHEEP")
case 5
FindString = Str.Item("strPIG") 
case 6
FindString = Str.Item("strMANURE") 
case 7
FindString = Str.Item("strBALANCE") 
case 8
FindString = Str.Item("strRESULT_N")
case 9
FindString = Str.Item("strRESULT_C")
case 10
FindString = Str.Item("strRESULT_GHG")
case 11
FindString = Str.Item("strDOCUMENTATION")'"Documentation"
case else
FindString = Str.Item("strFARM") 
end select

end function
'**************************************************************************************************
sub CreatePage(tb)
dim t
response.write "<div id=""tb_vis"" ><div id=""tb_pos"" >"
response.write "<table width=""100%"" cellpadding=""0"" cellspacing=""0"" border=""0"" ><tr><td align=""left"" ><table width=""100%"" cellpadding=""0"" cellspacing=""0"" border=""0"" ><tr>"
for t=1 to 11
if tb=t then
response.write "<td align=""left"" width=""5"" height=""21"" ><img src=""images\blue\tb_left_sel.gif"" width=""5"" height=""21"" border=""0""></td>"
'response.write "<td align=""left"" height=""21""  class=""TabView"" bgcolor=""#C8E1F3"" nowrap onclick=""javascript:window.parent.frames[2].document.getElementById('veil').style.display='block';if (window.parent.frames[2].document.getElementById('dynamichide')) {window.parent.frames[2].document.getElementById('dynamichide').style.display='none';};window.parent.frames[2].document.location='data.asp?TB="&t&"&debug="&debug&"';""""><a id=tb_"&t&" class=""TabView"" href=""javascript:window.parent.frames[2].document.getElementById('veil').style.display='block';if (window.parent.frames[2].document.getElementById('dynamichide')) {window.parent.frames[2].document.getElementById('dynamichide').style.display='none';};window.parent.frames[2].document.location='data.asp?TB="&t&"&debug="&debug&"';"" ><font color=""navy"" ><b>&nbsp;"
response.write "<td align=""left"" height=""21""  class=""TabView"" bgcolor=""#C8E1F3"" nowrap onclick=""window.top.location.reload();""""><a id=""tb_"&t&""" class=""TabView"" href=""start.asp"" target=""FarmNparent""><font color=""navy"" ><b>&nbsp;"
response.write FindString(t)&"&nbsp;</b></font></a></td>"
response.write "<td align=""left"" width=""5"" height=""21"" ><img src=""images\blue\tb_right_sel.gif"" width=""5"" height=""21"" border=""0""></td>"
else
'	if instr(invisibleString,cstr(t))=0 then
'	 if (instr(disabledString,cstr(t))<>0) then
     if arrDisabled(t-1)<>"" then
		response.write "<td align=""left"" width=""3"" height=""21"" ><img src=""images\grey/tb_left_sns.gif"" width=""3"" height=""21"" border=""0""></td>"
		response.write "<td align=""left"" height=""20"" nowrap ><table width=""100%"" cellpadding=""0"" cellspacing=""0"" border=""0"" style=""position:relative;top:1px;border-top: solid #f4f4f4 1px;"">"
		'response.write "<tr  height=""2""></tr>"
		response.write "<tr bgcolor=""#f4f4f4"" >"
		response.write "<td align=""left"" height=""20""  class=""TabView"" nowrap style=""color: #cacaca"">&nbsp;"
		response.write FindString(t)&"&nbsp;</td></tr></table></td>"
		response.write "<td align=""left"" width=""5"" height=""21"" ><img src=""images\grey/tb_right.gif"" width=""3"" height=""21"" border=""0""></td>"
	 else
		response.write "<td align=""left"" width=""3"" height=""21"" ><img src=""images\blue/tb_left_sns.gif"" width=""3"" height=""21"" border=""0""></td>"
		response.write "<td align=""left"" height=""20"" nowrap ><table width=""100%"" cellpadding=""0"" cellspacing=""0"" border=""0"" style=""position:relative;top:1px;border-top: solid #EEF5FA 1px;"">"
		'response.write "<tr  height=""2""></tr>"
		response.write "<tr bgcolor=""#EEF5FA"" >"
'		response.write "<td align=""left"" height=""20""  class=""TabView"" onclick=""javascript:window.parent.frames[2].document.getElementById('veil').style.display='block';document.location='tab1.asp?TB="&t&"&debug="&debug&"';if (window.parent.frames[2].document.getElementById('dynamichide')) {window.parent.frames[2].document.getElementById('dynamichide').style.display='none';};window.parent.frames[2].document.location='data.asp?TB="&t&"&debug="&debug&"';"" nowrap><a id=tb_"&t&" href=""javascript:this.style.cursor='pointer';"" class=""TabView"" >&nbsp;"
		response.write "<td align=""left"" height=""20""  class=""TabView"" onclick=""javascript:window.parent.frames[2].document.getElementById('veil').style.display='block';document.location='tab1.asp?TB="&t&"&debug="&debug&"';if (window.parent.frames[2].document.getElementById('dynamichide')) {window.parent.frames[2].document.getElementById('dynamichide').style.display='none';};window.parent.frames[2].document.location='data.asp?TB="&t&"&debug="&debug&"';"" nowrap><a id=tb_"&t&" href=""#"" onmouseover=""this.style.cursor='pointer';"" class=""TabView"" >&nbsp;"
		response.write FindString(t)&"&nbsp;</a></td></tr></table></td>"
		response.write "<td align=""left"" width=""5"" height=""21"" ><img src=""images\blue/tb_right.gif"" width=""3"" height=""21"" border=""0""></td>"
	 end if
'	end if
end if
next
response.write "</tr></table>"
response.write "<td align=""left"" height=""21"" valign=""bottom"" width=""100%"" >&nbsp;</td></tr></table>"
response.write "<input type=""hidden"" name=""tb_hid"" value=""on""/ >"
response.write "</div></div>"

end sub
'**************************************************************************************************
%>