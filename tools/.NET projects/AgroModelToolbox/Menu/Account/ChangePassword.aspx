<%@ Page Title="Change Password" Language="C#" MasterPageFile="~/AgroModelToolbox.master" AutoEventWireup="true" CodeBehind="ChangePassword.aspx.cs" Inherits="AgroModelToolbox.Account.ChangePassword" %>

<asp:Content ID="HeaderContent" runat="server" ContentPlaceHolderID="HeadContent">
</asp:Content>
<asp:Content ID="BodyContent" runat="server" ContentPlaceHolderID="MainContent">
   <h2 align="center">
      Change Password
   </h2>
   <asp:ChangePassword ID="ChangeUserPassword" runat="server" CancelDestinationPageUrl="~/" EnableViewState="false" RenderOuterTable="false" SuccessPageUrl="ChangePasswordSuccess.aspx">
      <ChangePasswordTemplate>
         <table border="0" align="center" cellpadding="5" cellspacing="0">
            <tr>
               <td colspan="2" align="center">
                  <asp:Label ID="LabelPlease" runat="server">New passwords are required to be a minimum of <%= Membership.MinRequiredPasswordLength %> characters in length.</asp:Label>
               </td>
            </tr>
            <tr>
               <td align="left">
                  <asp:Label ID="CurrentPasswordLabel" runat="server" AssociatedControlID="CurrentPassword">Old Password:</asp:Label>
               </td>
               <td align="right">
                  <asp:RequiredFieldValidator ID="CurrentPasswordRequired" runat="server" ControlToValidate="CurrentPassword" CssClass="failureNotification" ErrorMessage="Password is required." ToolTip="Old Password is required." ValidationGroup="ChangeUserPasswordValidationGroup">*</asp:RequiredFieldValidator>
                  <asp:TextBox ID="CurrentPassword" runat="server" CssClass="passwordEntry" TextMode="Password" Width="100px"></asp:TextBox>
               </td>
            </tr>
            <tr>
               <td align="left">
                  <asp:Label ID="NewPasswordLabel" runat="server" AssociatedControlID="NewPassword">New Password:</asp:Label>
               </td>
               <td align="right">
                  <asp:RequiredFieldValidator ID="NewPasswordRequired" runat="server" ControlToValidate="NewPassword" CssClass="failureNotification" ErrorMessage="New Password is required." ToolTip="New Password is required." ValidationGroup="ChangeUserPasswordValidationGroup">*</asp:RequiredFieldValidator>
                  <asp:TextBox ID="NewPassword" runat="server" CssClass="passwordEntry" TextMode="Password" Width="100px"></asp:TextBox>
               </td>
            </tr>
            <tr>
               <td align="left">
                  <asp:Label ID="ConfirmNewPasswordLabel" runat="server" AssociatedControlID="ConfirmNewPassword">Confirm New Password:</asp:Label>
               </td>
               <td align="right">
                  <asp:CompareValidator ID="NewPasswordCompare" runat="server" ControlToCompare="NewPassword" ControlToValidate="ConfirmNewPassword" CssClass="failureNotification" Display="Dynamic" ErrorMessage="The Confirm New Password must match the New Password entry." ValidationGroup="ChangeUserPasswordValidationGroup">*</asp:CompareValidator>
                  <asp:RequiredFieldValidator ID="ConfirmNewPasswordRequired" runat="server" ControlToValidate="ConfirmNewPassword" CssClass="failureNotification" Display="Dynamic" ErrorMessage="Confirm New Password is required." ToolTip="Confirm New Password is required." ValidationGroup="ChangeUserPasswordValidationGroup">*</asp:RequiredFieldValidator>
                  <asp:TextBox ID="ConfirmNewPassword" runat="server" CssClass="passwordEntry" TextMode="Password" Width="100px"></asp:TextBox>
               </td>
            </tr>
            <tr>
               <td colspan="2" align="center">
                  <asp:Button ID="CancelPushButton" runat="server" CausesValidation="False" CommandName="Cancel" Text="Cancel" />
                  <asp:Button ID="ChangePasswordPushButton" runat="server" CommandName="ChangePassword" Text="Change Password" ValidationGroup="ChangeUserPasswordValidationGroup" />
               </td>
            </tr>
            <tr>
               <td colspan="2" align="center">
                  <asp:Literal ID="FailureText" runat="server"></asp:Literal>
                  <asp:ValidationSummary ID="ChangeUserPasswordValidationSummary" runat="server" CssClass="failureNotification" ValidationGroup="ChangeUserPasswordValidationGroup" />
               </td>
            </tr>
         </table>
      </ChangePasswordTemplate>
   </asp:ChangePassword>
</asp:Content>
