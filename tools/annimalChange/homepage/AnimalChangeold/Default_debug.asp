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
Session("hasLogin") = ""
Session("loginTry") = ""
'Session("farmNumber") = ""
Session("view") = "normal"
Session("User") = ""
'session("farmOwner") = ""
'session("scenarioName") = ""

BeginHeader TITLE,"",""
CreateStyleLink "styles/stylesheet.css"
DefineScript
EndHeader
BeginBody
InitFarmNDB
CreateLanguageDictionary
CloseFarmNDB

   Response.Write("<div style=""text-align:center"">" & vbCr)
'   Response.Write("<h1 style=""color: red;"">This is the old version of the site. The new version is found <a href=""http://localhost/animalchange"">here</a></h1>" & vbCr)'henviser til den nye version af sitet
'   Response.Write("<h1>"&Str.Item("strUPDATING")&".</h1>" & vbCr)'bruges ved opdatering mellem servere
   Response.Write("</div>" & vbCr)
if Session("loginTry") = "no" then
   Response.Write("<div style=""text-align:center"">" & vbCr)
   Response.Write("<h1>"&Str.Item("strNOT_LOGGED_IN")&".</h1>" & vbCr)
'   Response.Write("<h1 style=""color: red;"">This is the old version of the site. The new version is found <a href=""http://localhost/animalchange"">here</a></h1>" & vbCr)'henviser til den nye version af sitet
'   Response.Write("<h1>"&Str.Item("strUPDATING")&".</h1>" & vbCr)'bruges ved opdatering mellem servere
   Response.Write("</div>" & vbCr)
   Response.Write("<hr/>" & vbCr)
   CreatePage
else
   CreatePage
end if
EndBody
'**************************************************************************************************
sub CreatePage()
   Response.Write("<form name=""formLogin"" action="""" method=""post"">" & vbCr)
   Response.Write("<table align=""center"" border=""0"">" & vbCr)
'   Response.Write("<tr><td>The site is closed for maintenance</td></tr>" & vbCr)'denne linie åbnes ved opdatering mellem servere
'   Response.end
   Response.Write("<tr>" & vbCr)                    'denne blok lukkes ved opdatering mellem servere
   Response.Write("<td colspan=""2"" align=""center""><h1>"&Str.Item("strWELCOME")&"</h1></td>" & vbCr)
   Response.Write("</tr>" & vbCr)
   Response.Write("<tr>" & vbCr)
   Response.Write("<td colspan=""2"" align=""center""><img src=""images/farm.gif"" width=""200"" height=""128"" border=""0"" alt=""farm.gif""/></td>" & vbCr)
   Response.Write("</tr>" & vbCr)
   Response.Write("<tr>" & vbCr)
   Response.Write("<th align=""left"">"& vbCr)
   Response.write Str.Item("languageselection") & Chr(13) 
   Response.Write("</th>" & vbCr)
   Response.Write("<td align=""right"">" & vbCr)
   CreateLanguageSelectionBox
   Response.Write("</td>" & vbCr)
   Response.Write("</tr>" & vbCr)
   Response.Write("<tr>" & vbCr)
   Response.Write("<th align=""left"">"&Str.Item("username")&"</th>" & vbCr)
   Response.Write("<td align=""right""><input type=""text"" name=""inputLoginName"" style=""width: 130px;"" /></td>" & vbCr)
   Response.Write("</tr>" & vbCr)
   Response.Write("<tr>" & vbCr)
   Response.Write("<th align=""left"">"&Str.Item("userpassword")&"</th>" & vbCr)
   Response.Write("<td align=""right""><input type=""password"" name=""inputPassword"" style=""width: 130px;"" /></td>" & vbCr)
   Response.Write("</tr>" & vbCr)
   Response.Write("<tr>" & vbCr)
   Response.Write("<td colspan=""2"" align=""center"">" & vbCr)
   Response.Write("<button style=""width: 130px; height: 20px; font-size: 10px"" onclick=""javascript:if(Form_Validator(document.formLogin)){document.forms.formLogin.action='LoginVerify.asp';document.forms.formLogin.submit();}"">Login</button>" & vbCr)
   Response.Write("</td>" & vbCr)
   Response.Write("</tr>" & vbCr)

   Response.Write("<tr>" & vbCr)
   Response.Write("<td colspan=""2"" align=""center""> &nbsp;" & vbCr)
      Response.Write("</td>" & vbCr)
   Response.Write("</tr>" & vbCr)

   Response.Write("<tr>" & vbCr)
   Response.Write("<td colspan=""2"" align=""center"">" & vbCr)      

'   Response.write("Latest news: <a href=""documentation/AnimalChange news.pdf"" target=""new""><img src=""images/info.gif"" border=""no"" alt=""info.gif""/></a>" & vbCr)

   Response.Write("</td>" & vbCr)
   Response.Write("</tr>" & vbCr)
   
   
   Response.Write("</table>" & vbCr)
   Response.Write("</form>" & vbCr)


end sub
'**************************************************************************************************
sub DefineScript()
   Response.Write("<script type=""text/javascript"" src=""scripts/formvalidation.js""></script>" & vbCr)
   Response.Write("<script type=""text/javascript"">" & vbCr)
   ' Form_Validator uses general functions from formvalidation.js to validate the form
   ' It is specific to the form of this page
   Response.Write("  function Form_Validator(theForm)" & vbCr)
   Response.Write("  {" & vbCr)
   Response.Write("    OKval=InputValidator(theForm.inputLoginName, 1, 3, ""Brugernavn"", 1);" & vbCr)
   Response.Write("    if (OKval)" & vbCr)
   Response.Write("       {OKval=InputValidator(theForm.inputPassword, 3, 10, ""Brugerkode"", 1);}" & vbCr)
   Response.Write("    return (OKval);" & vbCr)
   Response.Write("  }" & vbCr)
   Response.Write("</script>" & vbCr)
end sub
'**************************************************************************************************
sub CreateLanguageSelectionBox()
   

Response.write "<select style=""width: 130px;"" name=""sprog"" size=""1"" onchange=""document.forms.formLogin.action='default.asp';document.forms.formLogin.submit();"">"
Response.Write "<option "
If (session("Language") = "dk" ) Then Response.Write "selected=""selected"" "
Response.Write "value=""dk"">" & Str.Item("sprog1") & "</option>" & Chr(13)   '" & Str.Item("sprog1") & "
Response.Write "<option "
If (session("Language") = "uk" ) Then Response.Write "selected=""selected"" "
Response.Write "value=""uk"">" & Str.Item("sprog2") & "</option>" & Chr(13)   '" & Str.Item("sprog2") & "
Response.Write "<option "
If (session("Language") = "fr" ) Then Response.Write "selected=""selected"" "
Response.Write "value=""fr"">" & Str.Item("sprog3") & "</option>" & Chr(13)   '" & Str.Item("sprog2") & "
Response.write "</select>" & Chr(13)


end sub
'**************************************************************************************************
%>