<%
'**************************************************************************************************
' Author: Poul Lassen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
option explicit
%>
<!-- #include file="includes/General.asp" -->
<!-- #include file="includes/MailServiceSetup.asp" -->
<!-- #include file="includes/adovbs.asp" -->
<!-- #include file="includes/dbInterface.asp" -->
<%
Private Const TITLE = "Send Password"

dim inputEmail, nPerson, arrPerson, dictPerson

inputEmail = Trim(LCase(Request.Form("inputEmail")))

InitFarmNDB
   set dictPerson = CreateObject("Scripting.Dictionary")
   nPerson = CallSP ("GetUserByEmail " & inputEmail,arrPerson,dictPerson)
   'nPerson = GetUserByEmail(inputEmail, arrPerson)
CloseFarmNDB

BeginHeader TITLE,"",""
CreateStyleLink "styles/stylesheet.css"
EndHeader
BeginBody
CreatePage
EndBody
'**************************************************************************************************
sub CreatePage()
   Response.Write("<center>" & vbCr)
   Response.Write("<h1>Farm N</h1>" & vbCr)
   Response.Write("<img src=""images/farm.gif"" width=""200"" height=""128"" border=""0"">" & vbCr)

   if nPerson = 0 then
      Response.Write("<h3>Din email adresse (" &inputEmail& ") findes ikke i vores database</h3>" & vbCr)
   end if

   if nPerson = 1 then
      InitMail "Farm N","Poul.Lassen@agrsci.dk"
      mailer.AddRecipient "Farm N",inputEmail
      mailer.Subject = "Brugernavn og brugerkode til Farm N"
      mailer.BodyText = "Til " & arrPerson(0,dictPerson.item("UserName"))
      mailer.BodyText = " "
      mailer.BodyText = "Dette er dit brugernavn og brugerkode til Farm N, som er registreret i vores database:"
      mailer.BodyText = " "
      mailer.BodyText = "    Brugernavn: "&arrPerson(0,dictPerson.item("UserLoginName"))&""
      mailer.BodyText = "    Brugerkode: "&arrPerson(0,dictPerson.item("UserPassword"))&""
      mailer.BodyText = " "
      mailer.BodyText = "Med Venlig Hilsen"
      mailer.BodyText = ""
      mailer.BodyText = "Poul Lassen"
'
      if mailer.SendMail then
         CloseMail
         Response.Write("<h3>Dit brugernavn og brugerkode er sendt til din email adresse (" &inputEmail& ")</h3>" & vbCr)
      else
         CloseMail
         Response.Write("<h3>Det var ikke muligt at sende dit brugernavn og brugerkode til din email adresse (" &inputEmail& ")</h3>" & vbCr)
      end if
   end if

   if nPerson > 1 then
      Response.Write("<h1>System fejl !!!</h1>" & vbCr)
      Response.Write("<h3>Der er registreret mere end en person med email adressen (" &inputEmail& ")</h3>" & vbCr)
   end if

   Response.Write("</center>" & vbCr)
end sub
'**************************************************************************************************
%>