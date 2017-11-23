<%
'**************************************************************************************************
' Author: Margit
'**************************************************************************************************
option explicit
%>
<!-- #include file="includes/NonPublicInc.asp" -->
<!-- #include file="includes/General.asp" -->
<!-- #include file="includes/adovbs.asp" -->
<!-- #include file="includes/dbInterface.asp" -->
<!-- #include file="includes/LanguageDictionary.asp" -->
<!-- #include file="CattleHerd.asp" -->
<!-- #include file="SheepHerd.asp" -->
<!-- #include file="Farm.asp" -->
<!-- #include file="FieldPlan.asp" -->
<!-- #include file="Rotation.asp" -->
<!-- #include file="ManureFertilizer.asp" -->
<!-- #include file="PigHerd.asp" -->
<!-- #include file="Balance.asp" -->
<!-- #include file="Result.asp" -->
<!-- #include file="Documentation.asp" -->
<%
Private Const TITLE = "FarmAC"
Server.ScriptTimeout=500

dim version
dim time20, timeMeasureString1, timeMeasureString2, timeMeasureString3, timeMeasureString4

if Session("version") = "" then Session("version") = Request.QueryString("version")
if Session("version") = "" then Session("version") = 1

Session("tabNumber") = Request.QueryString("TB")
time3=now
BeginHeader TITLE,"",""
CreateStyleLink "styles/stylesheet.css"
'BeginBodyData
CreatePage
'response.write "<br/>Time1 :" & time1
'response.write "<br/>Time2 :" & time2
if request.querystring("debug")=1 then
response.write "<br/>Time3 Data.asp:" & time3
response.write "<br/>Time4 Farm:" & time4
response.write "<br/>Time5 Farm:" & time5
response.write "<br/>Time6 Rotation:" & time6
response.write "<br/>Time7 Rotation:" & time7
response.write "<br/>Time8 Yield:" & time8
response.write "<br/>Time9 Yield:" & time9
response.write "<br/>Time10 Ruminants:" & time10
response.write "<br/>Time11 Ruminants:" & time11
response.write "<br/>Time12 Non Ruminants:" & time12
response.write "<br/>Time13 Non Ruminants:" & time13
response.write "<br/>Time14 Manure:" & time14
response.write "<br/>Time15 Manure:" & time15
response.write "<br/>Time16 Balance:" & time16
response.write "<br/>Time17 Balance:" & time17
response.write "<script language=""JavaScript""> var sel=this.document.forms[0].elements; for (var i=0;i<sel.length;i++){sel[i].disabled='true'};</script>"
end if
if Session("view")="disabled" then
response.write "<script language=""JavaScript""> var sel=this.document.forms[0].elements; for (var i=0;i<sel.length;i++){sel[i].disabled='true'};var img=this.document.getElementsByTagName(""img""); for (var i=0;i<img.length;i++){img[i].onclick='';img[i].onmouseover='';};</script>"
Session("view")="disabled"
else
Session("view")="normal"
end if
EndBody
'**************************************************************************************************
sub CreatePage()
   select case CInt(Session("tabNumber"))
      case 1 BeginBodyData
             CreateTabFarm()
      case 2 BeginBodyData
             CreateTabField()  
      case 3 BeginBodyData
             CreateTabRotation()  
      case 4 BeginBodyData
             CreateTabCattle() 
'      case 5 CreateTabSheep()    
      case 5 BeginBodyData
             CreateTabPig()    
      case 6 BeginBodyData
             CreateTabManure() 
      case 7 BeginBodyData
             CreateTabBalance() 
      case 8 BeginBodyData
             CreateTabResult() 
      case 9 BeginBodyData
             CreateTabResultC() 
      case 10 BeginBodyData
              CreateTabResultGHG() 
      case 11 BeginBodyData
              CreateTabDocumentation() 
   end select
end sub
'**************************************************************************************************
%>
