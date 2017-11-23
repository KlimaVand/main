<%
'**************************************************************************************************
' Author: Poul Lassen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
option explicit
%>
<!-- #include file="includes/General.asp" -->
<%

'BeginHeader TITLE,"Nitrogen",""
'CreateStyleLink "styles/stylesheet.css"
'EndHeader
'BeginBody
'CreatePage
'EndBody

Session("hasLogin")   = "no"
Session("user")       = ""
Session("farmNumber") = ""
Session("farmOwner")  = ""

Response.Redirect("default.asp")
Response.End
'**************************************************************************************************
'sub CreatePage()
'   dim originalLocale, currentLocale
'   
'   originalLocale = GetLocale()        ' Get the original locale
'   currentLocale  = SetLocale("en-gb") ' Set the locale to UK
'
'   Response.Write("<form name=""formHeader"" action=""DummyHeader.asp"" method=""post"">" & vbCr)
'   Response.Write("<table width=""100%"" border=""0"">" & vbCr)
'   Response.Write("<tr>" & vbCr)
'   Response.Write("<td width=""25%"" align=""left""><h5>Ejendom: "&session("farmNumber")&"<br/>"&session("farmOwner")&"</h5></td>" & vbCr)
'   Response.Write("<td width=""50%"" align=""center""><h1>"&TITLE&"</h1><h3>"&SUBTITLE&"</h3></td>" & vbCr)
'   Response.Write("<td width=""25%"" align=""right"">" & vbCr)
'   Response.Write("<h5>"&FormatDateTime(Now,vbLongDate)&"<br/>" & vbCr)
'   Response.Write("User: "&session("user")&"</h5>" & vbCr)
'   if Session("hasLogin") = 0 then
'      Response.Write("<button style=""width: 50px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formHeader.action='Login.asp';document.forms.formHeader.submit();"">Login</button>" & vbCr)
'   else
'      Response.Write("<button style=""width: 50px; height: 20px; font-size: 10px"" onClick=""javascript:document.forms.formHeader.action='Logout.asp';document.forms.formHeader.submit();"">Logout</button>" & vbCr)
'   end if
'   Response.Write("</td>" & vbCr)
'   Response.Write("</tr>" & vbCr)
'   Response.Write("</table>" & vbCr)
'   Response.Write("</form>" & vbCr)
'
'   currentLocale  = SetLocale(originalLocale) ' Restore the original locale
'end sub
'**************************************************************************************************
%>