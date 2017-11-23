<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Default.aspx.cs" Inherits="Default" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title>Farm Animal Change</title>
    <link rel="StyleSheet" href="styles/FarmAC.css" type="text/css" />
</head>
<body>
<form id="formLogin" runat="server">
<table align="center" border="0" class="green" style="padding: 5px;">
<tr>
<td colspan="2" align="center"><h2>Welcome to Farm AC</h2></td>
</tr>
<tr>
    <td></td>
</tr>
<tr>
<td colspan="2" align="center"><img src="images/farm.png" border="0" alt="farm.png"/></td>
</tr>
<tr>
    <td></td>
</tr>
<tr>
<th align="left">
</th>
    <td align="right"><asp:DropDownList runat="server" OnSelectedIndexChanged="changeLanguage" Textmode="password" id="LanuageSelction" Width="130" /></td>
</tr>
<tr>
<th align="left">User name</th>
<td align="right"><asp:TextBox id="inputLoginName" runat="server" Width="130" /></td><td></td>
</tr>
<tr>
<th align="left">User password</th>
<td align="right"><asp:TextBox runat="server" Textmode="password" id="inputPassword" Width="130" /></td>
</tr>
<tr>
<td colspan="2" align="center"> &nbsp;
</td>
</tr>
<tr>
<td colspan="2" align="center">
<%--<button style="width: 130px; height: 20px; font-size: 10px" onclick="javascript:if(Form_Validator(document.formLogin)){document.forms.formLogin.action='LoginVerify.asp';document.forms.formLogin.submit();}">Login</button>--%>
    <asp:Button ID="Button_Submit" runat="server" OnClick="Button_Submit_Click" Text="Login" Width="200px" />
</td>
</tr>
<tr>
<td colspan="2" align="center">
</td>
</tr>
</table>
</form></body>
</html>
