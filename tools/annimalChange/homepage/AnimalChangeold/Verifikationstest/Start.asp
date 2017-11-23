<%
'**************************************************************************************************
' Author: Poul Lassen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
option explicit
%>
<!-- #include file="includes/General.asp" -->
<%
Private Const TITLE = "Farm N"
dim version

Session("version") = Request.QueryString("version")
if Session("version") = "" then Session("version") = 1
if Session("User")="" then 
  response.redirect("Default.asp")
	response.end
else

BeginHeader TITLE,"",""
CreateStyleLink "styles/stylesheet.css"
CreateScript
EndHeader
CreatePage
end if
'**************************************************************************************************
sub CreateScript()
   Response.Write("<script language=""javascript"">" & vbCr)
   Response.Write("<!--" & vbCr)
   Response.Write("   if(top.frames.length != 0)" & vbCr)
   Response.Write("      {top.location=self.document.location};" & vbCr)
   Response.Write("      window.name='FarmNparent';" & vbCr)
   Response.Write("//-->" & vbCr)
   Response.Write("</script>" & vbCr)
end sub
'**************************************************************************************************
sub CreatePage()
   Response.Write("<frameset rows=""55,21,*,30"" border=""0""                                   frameborder=""no"" marginwidth=""0"" marginheight=""0"" topmargin=""0"" leftmargin=""0"" framespacing=""0"">" & vbCr)
   Response.Write("  <frame src=""header.asp""                           name=""frameHeader""   frameborder=""no"" marginwidth=""5"" marginheight=""5"" scrolling=""no""   noresize>" & vbCr)
   Response.Write("  <frame src=""tab.asp""                              name=""frameTab""      frameborder=""no"" marginwidth=""0"" marginheight=""0"" scrolling=""no"" noresize>" & vbCr)
   Response.Write("  <frame src=""data.asp?TB="&Session("tabNumber")&"&version="&Session("version")&""" name=""frameData""     frameborder=""no"" marginwidth=""0"" marginheight=""0"" scrolling=""yes""  noresize>" & vbCr)
   Response.Write("  <frame src=""Provider.asp""                         name=""frameProvider"" frameborder=""no"" marginwidth=""0"" marginheight=""0"" scrolling=""no""   noresize>" & vbCr)
   Response.Write("</frameset>" & vbCr)
end sub
'**************************************************************************************************
%>