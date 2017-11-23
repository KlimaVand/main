<%
'**************************************************************************************************
' Author: NN, Danish Institute of Agricultural Sciences
' Description: Description missing
'**************************************************************************************************
' includes/mailservicesetup.asp
' Specification of an email service
dim mailer
'**************************************************************************************************
sub InitMail(aName,aAddress)
   set mailer = Server.CreateObject("SMTPsvg.Mailer")
   mailer.FromName    = aName
   mailer.FromAddress = aAddress
   mailer.RemoteHost  = "mailhost.agrsci.dk"
end sub
'**************************************************************************************************
sub CloseMail()
   Set mailer = nothing
end sub
'**************************************************************************************************
%>