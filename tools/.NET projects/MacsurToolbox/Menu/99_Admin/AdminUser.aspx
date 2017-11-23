<%@ Page Title="" Language="C#" MasterPageFile="~/MacsurToolbox.Master" AutoEventWireup="true" CodeBehind="AdminUser.aspx.cs" Inherits="MacsurToolbox.Menu._99_Admin.AdminUser" %>

<%@ Register Assembly="Telerik.Web.UI" Namespace="Telerik.Web.UI" TagPrefix="telerik" %>
<%@ Register TagPrefix="admin" TagName="User" Src="~/Controls/99_Admin/ctrlUser.ascx" %>
<%@ Register TagPrefix="admin" TagName="Role" Src="~/Controls/99_Admin/ctrlRole.ascx" %>
<asp:Content ID="ContentHead" ContentPlaceHolderID="HeadContent" runat="server">
</asp:Content>
<asp:Content ID="ContentMain" ContentPlaceHolderID="MainContent" runat="server">
   <asp:Label ID="LabelStatus" runat="server" Font-Bold="True" ForeColor="Red" Font-Size="Large"></asp:Label>
   <h2>
      User administration
   </h2>
   <telerik:RadTabStrip ID="RadTabStripAdminUser" runat="server" MultiPageID="RadMultiPageAdminUser" SelectedIndex="0" ShowBaseLine="True" AutoPostBack="True" Skin="Default">
      <Tabs>
         <telerik:RadTab runat="server" Text="Users" Selected="True">
         </telerik:RadTab>
         <telerik:RadTab runat="server" Text="Roles">
         </telerik:RadTab>
      </Tabs>
   </telerik:RadTabStrip>
   <telerik:RadMultiPage ID="RadMultiPageAdminUser" runat="server" Width="978px" SelectedIndex="0" Style="border-left: grey 1px solid; border-right: grey 1px solid; border-bottom: grey 1px solid" Enabled="True">
      <telerik:RadPageView ID="RadPageViewUser" runat="server" Width="100%">
         <admin:User ID="controlUser" runat="server" />
      </telerik:RadPageView>
      <telerik:RadPageView ID="RadPageViewRole" runat="server" Width="100%">
         <admin:Role ID="controlRole" runat="server" />
      </telerik:RadPageView>
   </telerik:RadMultiPage>
</asp:Content>
