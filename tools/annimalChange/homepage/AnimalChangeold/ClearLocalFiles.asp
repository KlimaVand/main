<%
'**************************************************************************************************
' Author: Margit Jorgensen
'**************************************************************************************************
option explicit
%>
<!-- #include file="includes/General.asp" -->
<!-- #include file="includes/adovbs.asp" -->
<!-- #include file="includes/dbInterface.asp" -->
<%
dim aFarmNumber
dim fs,fo,f,fldr, msg
fldr=cstr("c:/upload/AnimalChange/") & Session("FarmNumber")
  set fs=Server.CreateObject("Scripting.FileSystemObject")
'  set fo = fs.GetFolder("c:\upload\AnimalChange\")
   If (fs.FolderExists(fldr)) Then
      set fo = fs.GetFolder(fldr)
	    for each f in fo.files
		fs.DeleteFile(f)
	    next
   End If
Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
%>