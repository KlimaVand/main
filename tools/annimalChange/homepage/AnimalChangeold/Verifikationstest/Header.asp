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
Private Const TITLE = "Farm N"
Private Const SUBTITLE = ""

BeginHeader TITLE,"VVM,Værktøj,Nitrogen","Værktøj til Screening og VVM godkendelser"
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
   Response.Write("<td width=""30%"" align=""left""><h5>"&Str.Item("strFARM")&": "&session("farmNumber")) 
'   if Session("nFarm")>1 then
   if Session("userType")>=100 then
     Response.Write(" <a href=""SelectFarm.asp"" target=""_top"">"&Str.Item("strSELECT_FARM")&"</a>")
   end if
'   Response.Write("<br/>"&session("farmOwner")&"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;("&session("zipCode")&")<br/>"&Str.Item("strSCENARIO")&": "&session("scenarioName")&"</h5></td>" & vbCr)
   Response.Write("<br/>"&session("farmOwner")&"<br/>"&session("scenarioName")&"</h5></td>" & vbCr)
'   Response.Write("<td width=""40%"" align=""center""><h1>"&TITLE&"</h1><h3>"&SUBTITLE&"</h3></td>" & vbCr)
   Response.Write("<td width=""50%"" align=""center""><h1>"&TITLE&"</h1><h5 style=""color: red;"">"&Str.Item("strREMARK")&"</h5></td>" & vbCr)
   Response.Write("<td width=""20%"" align=""right""><h5>"&FormatDateTime(Now,vbShortDate)&"<br/>"&session("user")&"" & vbCr)   '<br/>Timeout: "&Session.Timeout&" minutter</h5></td>" & vbCr)
   Response.Write("</tr>" & vbCr)
   Response.Write("</table>" & vbCr)
   Response.Write("</form>" & vbCr)
end sub
'**************************************************************************************************
%>