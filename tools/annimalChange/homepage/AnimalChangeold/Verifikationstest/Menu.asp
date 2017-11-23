<%
'**************************************************************************************************
' Author: Poul Lassen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
option explicit
%>
<!-- #include file="includes/General.asp" -->
<!-- #include file="includes/adovbs.asp" -->
<!-- #include file="includes/dbInterface.asp" -->
<%
Private Const TITLE           = "Menu"
Private Const PROVIDER_HREF   = "http://www.agrsci.dk/jbs/info/index_uk.shtml"
Private Const PROVIDER_INST   = "Danish Institute of Agricultural Science"
Private Const PROVIDER_GROUP  = "INFO Reasearch Group"
Private Const WEBMASTER_NAME  = "Poul Lassen"
Private Const WEBMASTER_EMAIL = "Poul.Lassen@agrsci.dk"

dim nFarmTotal, arrFarmTotal

InitFarmNDB
nFarmTotal = GetFarmTotalList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),3,arrFarmTotal)
CloseFarmNDB

BeginHeader TITLE,"",""
CreateStyleLink "styles/stylesheet.css"
EndHeader
BeginBody
CreatePage
EndBody
'**************************************************************************************************
sub CreatePage()
   dim i
   Response.Write("<h1>Menu</h1>" & vbCr)

   Response.Write("<table border=""0"">" & vbCr)
   for i = 0 to 8
      Response.Write("<tr>" & vbCr)
      Response.Write("<td>"&arrFarmTotal(1,i)&"</td>" & vbCr)
      Response.Write("<td align=""right"">"&FormatNumber(arrFarmTotal(5,i),1)&"</td>" & vbCr)
      Response.Write("<td width=""30"">"&arrFarmTotal(0,i)&"</td>" & vbCr)
      Response.Write("</tr>" & vbCr)
   next
   Response.Write("</table>" & vbCr)

   Response.Write("<form name=""formMenu"" action=""Calculate.asp"" method=""post"">" & vbCr)
   Response.Write("<button style=""width: 200px; height: 50px; font-size: 20px"" onClick=""javascript:document.forms.formMenu.submit();"">Beregn</button>")
   Response.Write("</form>" & vbCr)
   Response.Write("<hr/>" & vbCr)
   Response.Write("<address>Web site provided by<br/>"&PROVIDER_INST&"<br/><a href="""&PROVIDER_HREF&""" target=""_top"">"&PROVIDER_GROUP&"</a>.<br/>Report technical problems to webmaster:<br/><a href=""mailto:"&WEBMASTER_EMAIL&""">"&WEBMASTER_NAME&"</a>." & vbCr)
   Response.Write("<br/>Optimized for screen size 1024x768</address>" & vbCr)
end sub
'**************************************************************************************************
%>