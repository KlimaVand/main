<%
'**************************************************************************************************
' Author: Margit Jorgensen, Danish Institute of Agricultural Sciences
' Description: test
'**************************************************************************************************
option explicit
%>
<!-- #include file="includes/General.asp" -->
<%
dim aUserID

aUserID=Right(CStr(round(CDbl(Cdate(date+time))*100000)),9)
response.write "<br>date:  "&date
response.write "<br>time:  "&time
response.write "<br>Cdate(date+time):  "&Cdate(date+time)
response.write "<br>CDbl(Cdate(date)):  "&CDbl(Cdate(date))
response.write "<br>CDbl(Cdate(date+time)):  "&CDbl(Cdate(date+time))
response.write "<br>CDbl(Cdate(time)):  "&CDbl(Cdate(time))
response.write "<br>CDbl(Cdate(03:01:01)):  "&CDbl(Cdate("03:01:01"))
response.write "<br>CDbl(Cdate(date+time))*100000:  "&CDbl(Cdate(date+time))*100000

response.write "<br>aUserID:  "&aUserID
%>


