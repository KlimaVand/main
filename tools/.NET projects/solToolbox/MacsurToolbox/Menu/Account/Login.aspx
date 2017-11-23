<%@ Page Title="Log In" Language="C#" MasterPageFile="~/MacsurToolbox.master" AutoEventWireup="true" CodeBehind="Login.aspx.cs" Inherits="MacsurToolbox.Account.Login" %>

<asp:Content ID="HeaderContent" runat="server" ContentPlaceHolderID="HeadContent">
</asp:Content>
<asp:Content ID="BodyContent" runat="server" ContentPlaceHolderID="MainContent">
   <h2 align="center">
      Log In
   </h2>
   <asp:Login ID="LoginUser" runat="server" EnableViewState="false" RenderOuterTable="false">
      <LayoutTemplate>
         <table border="0" align="center" cellpadding="5" cellspacing="0">
            <tr>
               <td colspan="2" align="center">
                  <asp:Label ID="LabelPlease" runat="server">Please enter your username and password.</asp:Label>
               </td>
            </tr>
            <tr>
               <td align="left">
                  <asp:Label ID="UserNameLabel" runat="server" AssociatedControlID="UserName">Username:</asp:Label>
               </td>
               <td align="right">
                  <asp:RequiredFieldValidator ID="UserNameRequired" runat="server" ControlToValidate="UserName" CssClass="failureNotification" ErrorMessage="Username is required." ToolTip="Username is required." ValidationGroup="LoginUserValidationGroup">*</asp:RequiredFieldValidator>
                  <asp:TextBox ID="UserName" runat="server" CssClass="textEntry" Width="100px"></asp:TextBox>
               </td>
            </tr>
            <tr>
               <td align="left">
                  <asp:Label ID="PasswordLabel" runat="server" AssociatedControlID="Password">Password:</asp:Label>
               </td>
               <td align="right">
                  <asp:RequiredFieldValidator ID="PasswordRequired" runat="server" ControlToValidate="Password" CssClass="failureNotification" ErrorMessage="Password is required." ToolTip="Password is required." ValidationGroup="LoginUserValidationGroup">*</asp:RequiredFieldValidator>
                  <asp:TextBox ID="Password" runat="server" CssClass="passwordEntry" TextMode="Password" Width="100px"></asp:TextBox>
               </td>
            </tr>
            <tr>
               <td colspan="2" align="center">
                  <asp:Button ID="LoginButton" runat="server" CommandName="Login" Text="Log In" ValidationGroup="LoginUserValidationGroup" Width="100px" />
               </td>
            </tr>
            <tr>
               <td colspan="2" align="left">
                  <asp:Literal ID="FailureText" runat="server"></asp:Literal>
                  <asp:ValidationSummary ID="LoginUserValidationSummary" runat="server" CssClass="failureNotification" ValidationGroup="LoginUserValidationGroup" />
               </td>
            </tr>
         </table>
      </LayoutTemplate>
   </asp:Login>
</asp:Content>
