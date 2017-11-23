<%
'**************************************************************************************************
' Author: Poul Lassen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
option explicit
%>
<!-- #include file="includes/General.asp" -->
<%
Private Const TITLE = "New User"

BeginHeader TITLE,"",""
CreateStyleLink "styles/stylesheet.css"
DefineScript
EndHeader
BeginBody
CreatePage
EndBody
'**************************************************************************************************
sub CreatePage()
   Response.Write("<form name=""formNewUser"" action=""DummyNewUser.asp"" method=""post"">" & vbCr)
   Response.Write("<table align=""center"" border=""0"">" & vbCr)
   Response.Write("<tr>" & vbCr)
   Response.Write("<td colspan=""2"" align=""center""><h1>Oprettelse a ny bruger</h1></td>" & vbCr)
   Response.Write("</tr>" & vbCr)
   Response.Write("<tr>" & vbCr)
   Response.Write("<td>Navn:</td>" & vbCr)
   Response.Write("<td><input type=""text"" name=""inputName"" size=""50"" /></td>" & vbCr)
   Response.Write("</tr>" & vbCr)
   Response.Write("<tr>" & vbCr)
   Response.Write("<td>Email:</td>" & vbCr)
   Response.Write("<td><input type=""text"" name=""inputEmail"" size=""50"" /></td>" & vbCr)
   Response.Write("</tr>" & vbCr)
   Response.Write("<tr>" & vbCr)
   Response.Write("<td colspan=""2"" align=""center"">" & vbCr)
   Response.Write("<button style=""width: 100px; height: 20px; font-size: 10px"" onClick=""javascript:if(Form_Validator(document.formNewUser)){document.forms.formNewUser.action='NewUserRequest.asp';document.forms.formNewUser.submit();}"">Send</button>" & vbCr)
   Response.Write("</td>" & vbCr)
   Response.Write("</tr>" & vbCr)
   Response.Write("</table>" & vbCr)
   Response.Write("</form>" & vbCr)
end sub
'**************************************************************************************************
sub DefineScript()
   Response.Write("<script language=""JavaScript"" src=""scripts/formvalidation.js""></script>" & vbCr)
   Response.Write("<script language=""JavaScript"">" & vbCr)
   ' Form_Validator uses general functions from formvalidation.js to validate the form
   ' It is specific to the form of this page
   Response.Write("  function Form_Validator(theForm)" & vbCr)
   Response.Write("  {" & vbCr)
   Response.Write("    OKval=InputValidator(theForm.inputName, 1, 50, ""Navn"", 1);" & vbCr)
   Response.Write("    if(OKval)" & vbCr)
   Response.Write("       {OKval=InputValidator(theForm.inputEmail, 5, 50, ""Email"", 4);}" & vbCr)
   Response.Write("    return (OKval);" & vbCr)
   Response.Write("  }" & vbCr)
   Response.Write("</script>" & vbCr)
end sub
'**************************************************************************************************
%>