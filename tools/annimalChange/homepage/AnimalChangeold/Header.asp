<%
'**************************************************************************************************
' Author: Poul Lassen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
option explicit
%>
<!-- #include file="includes/General.asp" -->
<!-- #include file="includes/dbInterface.asp" -->
<!-- #include file="includes/LanguageDictionary.asp" -->
<%
Private Const TITLE = "Farm AC"
Private Const SUBTITLE = ""

BeginHeader TITLE,"GHG Emissions","Tool for estimating GHG emissions due to change in farm management"
CreateStyleLink "styles/stylesheet.css"
EndHeader
BeginBody
InitFarmNDB
CreateLanguageDictionary
CloseFarmNDB
CreatePage
EndBody
'**************************************************************************************************
sub CreatePage()

        
   Response.Write("<form name=""formHeader"" action=""DummyHeader.asp"" method=""post"">" & vbCr)
   Response.Write("<table width=""100%"" border=""0"">" & vbCr)
   Response.Write("<tr>" & vbCr)
   if Session("view")="normal" then
   Response.Write("<td width=""1000"" align=""left""><h5>"&Str.Item("strFARM")&": "&session("farmNumber")) 
'   if Session("nFarm")>1 then
     Response.Write(" <a href=""SelectFarm.asp"" target=""_top"">"&Str.Item("strSELECT_FARM")&"</a>&nbsp;<a href=""Logout.asp"" target=""_top"">"&Str.Item("strLOGOUT")&"</a>")
'   Response.Write("<br/>"&session("farmOwner")&"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;("&session("zipCode")&")<br/>"&Str.Item("strSCENARIO")&": "&session("scenarioName")&"</h5></td>" & vbCr)
   Response.Write("<br/>"&session("farmOwner")&"<br/>"&session("scenarioName")&"</h5></td>" & vbCr)
'   Response.Write("<td width=""40%"" align=""center""><h1>"&TITLE&"</h1><h3>"&SUBTITLE&"</h3></td>" & vbCr)
   Response.Write("<td  align=""left"" ""nowrap""><h1>"&TITLE&"</h1><h5 style=""color: red;"">"&Str.Item("strREMARK")&"</h5></td>" & vbCr)
   Response.Write("<td  align=""right"" ""nowrap""><h5>"&FormatDateTime(Now,vbShortDate)&"<br/>"&session("user")&"</h5></td>" & vbCr)   '<br/>Timeout: "&Session.Timeout&" minutter</h5></td>" & vbCr)
   else
     Response.Write(" <a href=""Logout.asp"" target=""_top"">"&Str.Item("strBACK")&"</a>")
   end if
   Response.Write("</tr>" & vbCr)
   Response.Write("</table>" & vbCr)
   Response.Write("</form>" & vbCr)
end sub
'**************************************************************************************************
%>