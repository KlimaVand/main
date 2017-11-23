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
Private Const TITLE           = "Provider"
Private Const PROVIDER_HREF   = "http://agro.au.dk/en/research/sektioner/climate-and-water/"
Private Const PROVIDER_INST   = "Aarhus University, Dept. of Agroecology"
Private Const PROVIDER_GROUP  = "Research section: Climate and Water"
Private Const WEBMASTER_NAME  = "Margit Jorgensen"
Private Const WEBMASTER_EMAIL = "MargitS.Jorgensen@agrsci.dk"

BeginHeader TITLE,"",""
CreateStyleLink "styles/stylesheet.css"
EndHeader
BeginBody
CreatePage
EndBody
'**************************************************************************************************
sub CreatePage()
'   Response.Write("<address>Web site provided by "&PROVIDER_INST&" <a href="""&PROVIDER_HREF&""" target=""_top"">"&PROVIDER_GROUP&"</a>. Report technical problems to webmaster: <a href=""mailto:"&WEBMASTER_EMAIL&""">"&WEBMASTER_NAME&"</a>." & vbCr)
   Response.Write("<address>Web site provided by "&PROVIDER_INST&" <a href="""&PROVIDER_HREF&""" target=""_top"">"&PROVIDER_GROUP&"</a>. Report technical problems to <a href=""mailto:"&WEBMASTER_EMAIL&""">Webmaster</a>." & vbCr)
   Response.Write("<br/>Optimized for screen size 1024x768</address>" & vbCr)
end sub
'**************************************************************************************************
%>