<%
'**************************************************************************************************
' Author: Allan Leck Jensen, Danish Institute of Agricultural Sciences
' Description: This include file should be included in the top of all non-public pages.
'              It will force users without a valid login session to the login page.
'**************************************************************************************************
%>

<%
dim strReturnURL

if Session("haslogin") <> "yes" then
   strReturnURL = Request.ServerVariables("SCRIPT_NAME")
   if Request.ServerVariables("QUERY_STRING") <> "" then
      strReturnURL = strReturnURL + "?" 
      strReturnURL = strReturnURL + Request.ServerVariables("QUERY_STRING") 
   end if
   Session("ReturnUrl") = strReturnURL
   Response.Redirect("default.asp")
   Response.end
else
   Session("ReturnUrl") = ""
end if
%>
