<%
option explicit
%>
<!-- #include file="includes/General.asp" -->
<!-- #include file="includes/adovbs.asp" -->
<!-- #include file="includes/dbInterface.asp" -->
<%
dim skemaID, arrVVM, dictVVM, x, password
 skemaID=request("skemaID")
 password = "" 
 if skemaID<>"" then 
   InitFarmNDB
   set dictVVM = CreateObject("Scripting.Dictionary")
   x = CallSP ("GetVVM_ID " & Cstr(skemaID), arrVVM, dictVVM)	   
   CloseFarmNDB
if x>0 then password = arrVVM(0,dictVVM.item("FarmNumber"))
'else
'password = "" 
end if

   Response.Write("<form action=""VVM_ID.asp"" method=""post"">" & vbCr)
   Response.Write("    <table border=""0"">" & vbCr)   

   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <td class=""data"" align=""left"">Skema</td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left""><input type=""text"" name=""skemaID"" value="""&skemaID&""" maxlength=""50"" size=""25"" onChange=""submit();""/><input type=""button"" value=""OK""/></td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <td class=""data"" align=""left"">password</td>" & vbCr)
   Response.Write("      <td class=""data"" align=""left""><input type=""text"" value="""&password&""" maxlength=""50"" size=""25""/></td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   
   Response.Write("    </table>" & vbCr)   
%>
