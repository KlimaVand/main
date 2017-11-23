<%
'**************************************************************************************************
' Author: Poul Lassen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
option explicit
%>
<!-- #include file="includes/General.asp" -->
<%
Private Const TITLE = "Request Password"

BeginHeader TITLE,"",""
CreateStyleLink "styles/stylesheet.css"
DefineScript
EndHeader
BeginBody
CreatePage
EndBody
'**************************************************************************************************
sub CreatePage()
   Response.Write("<center>" & vbCr)
   Response.Write("<h1>Farm N</h1>" & vbCr)
   Response.Write("<img src=""images/farm.gif"" width=""200"" height=""128"" border=""0"">" & vbCr)
   Response.Write("<h3>Indtast din email-adresse og få tilsendt brugernavn og brugerkode.</h3>" & vbCr)
   Response.Write("<form name=""formPasswordRequest"" action=""DummyPasswordRequest.asp"" method=""post"">" & vbCr)
   Response.Write("<table align=""center"" border=""0"">" & vbCr)
   Response.Write("<tr>" & vbCr)
   Response.Write("<td class=""hidden"">Email:</td>" & vbCr)
   Response.Write("<td class=""hidden""><input type=""text"" name=""inputEmail"" value="""" size=""50"" onChange='if(InputValidator(document.formPasswordRequest.inputEmail, 5, 50, ""Email"", 4)){}else{return false;}' /></td>" & vbCr)
   Response.Write("</tr>" & vbCr)
   Response.Write("<tr>" & vbCr)
   Response.Write("<td colspan=""2"" align=""center"">" & vbCr)
   Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:if(Form_Validator(document.formPasswordRequest)){document.forms.formPasswordRequest.action='PasswordSend.asp';document.forms.formPasswordRequest.submit();}"">Send</button>" & vbCr)
   Response.Write("</td>" & vbCr)
   Response.Write("</tr>" & vbCr)
   Response.Write("</table>" & vbCr)
   Response.Write("</form>" & vbCr)
   Response.Write("</center>" & vbCr)
end sub
'**************************************************************************************************
sub DefineScript()
   Response.Write("<script language=""JavaScript"" src=""scripts/formvalidation.js""></script>" & vbCr)
   Response.Write("<script language=""JavaScript"">" & vbCr)
   ' Form_Validator uses general functions from formvalidation.js to validate the form
   ' It is specific to the form of this page
   Response.Write("  function Form_Validator(theForm)" & vbCr)
   Response.Write("  {" & vbCr)
   Response.Write("    {OKval=InputValidator(theForm.inputEmail, 5, 50, ""Email"", 4);}" & vbCr)
   Response.Write("    return (OKval);" & vbCr)
   Response.Write("  }" & vbCr)
   Response.Write("</script>" & vbCr)
end sub
'**************************************************************************************************
%>