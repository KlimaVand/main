<%
sub CreateLanguageDictionary() 
dim sprog, Language
'sprog=request.querystring("sprog")
sprog=Request.Form("sprog")
if not sprog = "" then session("Language")= sprog  
if session("Language")= "" then
session("Language") = "dk"
end if

'session("Language") = "uk"  skal udkommenteres når alle danske strenge er i databasen

Sprog = session("Language")


Set Str = CreateObject("Scripting.Dictionary")

strStoredProcedure = "GetLanguageStringNameList "&Sprog

'SQLQuery = "SELECT KeyWord, StringName_"+Sprog+" FROM Language"
Set Language = Con.Execute(strStoredProcedure)

Do While Not Language.EOF
  Str.Add CStr(Language("KeyWord")), CStr(""+Language("StringName_"+Sprog))
  Language.MoveNext
Loop
Set Language = Nothing


end sub
%>