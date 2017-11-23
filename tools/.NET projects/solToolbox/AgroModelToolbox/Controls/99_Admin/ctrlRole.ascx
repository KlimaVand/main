<%@ Control Language="C#" AutoEventWireup="true" CodeBehind="ctrlRole.ascx.cs" Inherits="AgroModelToolbox.Controls._99_Admin.ctrlRole" %>
<%@ Register assembly="Telerik.Web.UI" namespace="Telerik.Web.UI" tagprefix="telerik" %>
<telerik:RadScriptBlock ID="RadScriptBlockUserManagement" runat="server">
   <script type="text/javascript">
      var popUp;
      function PopUpShowingRole(sender, eventArgs)
      {
         popUp = eventArgs.get_popUp();
         var gridWidth = sender.get_element().offsetWidth;
         var popUpWidth = popUp.style.width.substr(0, popUp.style.width.indexOf("px"));
         popUp.style.left = ((gridWidth - popUpWidth) / 2 + sender.get_element().offsetLeft).toString() + "px";
         popUp.style.top = "150px";
      }
   </script>
</telerik:RadScriptBlock>
<asp:Label ID="LabelMessage" runat="server" Font-Bold="True" ForeColor="Red"></asp:Label>
<table width="100%" border="0">
   <tr valign="top">
      <td width="50%">
         <telerik:RadGrid ID="RadGridRole" runat="server" AutoGenerateColumns="False" CellSpacing="0" DataSourceID="SqlDataSourceRole" GridLines="None" OnDeleteCommand="RadGridRole_DeleteCommand" OnDataBound="RadGridRole_DataBound" onselectedindexchanged="RadGridRole_SelectedIndexChanged">
            <MasterTableView DataKeyNames="RoleId" DataSourceID="SqlDataSourceRole">
               <Columns>
                  <telerik:GridBoundColumn DataField="ApplicationId" DataType="System.Guid" FilterControlAltText="Filter ApplicationId column" HeaderText="ApplicationId" ReadOnly="True" SortExpression="ApplicationId" UniqueName="ApplicationId" Visible="false">
                  </telerik:GridBoundColumn>
                  <telerik:GridBoundColumn DataField="RoleId" DataType="System.Guid" FilterControlAltText="Filter RoleId column" HeaderText="RoleId" SortExpression="RoleId" UniqueName="RoleId" Visible="false">
                  </telerik:GridBoundColumn>
                  <telerik:GridBoundColumn DataField="RoleName" FilterControlAltText="Filter RoleName column" HeaderText="Role" SortExpression="RoleName" UniqueName="RoleName">
                     <HeaderStyle Width="200px" HorizontalAlign="Left" Wrap="false" />
                     <ItemStyle VerticalAlign="Top" HorizontalAlign="Left" Wrap="false" />
                  </telerik:GridBoundColumn>
                  <telerik:GridBoundColumn DataField="LoweredRoleName" FilterControlAltText="Filter LoweredRoleName column" HeaderText="LoweredRoleName" ReadOnly="True" SortExpression="LoweredRoleName" UniqueName="LoweredRoleName" Visible="false">
                  </telerik:GridBoundColumn>
                  <telerik:GridButtonColumn ButtonType="ImageButton" CommandName="Select" FilterControlAltText="Filter SelectRole column" UniqueName="SelectRole" ImageUrl="~/Images/obs.gif">
                     <HeaderStyle Width="50px" HorizontalAlign="Center" Wrap="false" />
                     <ItemStyle VerticalAlign="Top" HorizontalAlign="Center" Wrap="false" />
                  </telerik:GridButtonColumn>
                  <telerik:GridButtonColumn ButtonType="ImageButton" CommandName="Delete" FilterControlAltText="Filter DeleteRole column" UniqueName="DeleteRole" ImageUrl="~/Images/delete.gif" ConfirmText="Please confirm that you want to delete the current role">
                     <HeaderStyle Width="50px" HorizontalAlign="Center" Wrap="false" />
                     <ItemStyle VerticalAlign="Top" HorizontalAlign="Center" Wrap="false" />
                  </telerik:GridButtonColumn>
               </Columns>
            </MasterTableView>
         </telerik:RadGrid>
         <br />
         <telerik:RadButton ID="RadButtonNewRole" runat="server" Text="New role">
         </telerik:RadButton>
      </td>
      <td>
         <telerik:RadGrid ID="RadGridRoleUser" runat="server" AutoGenerateColumns="False" CellSpacing="0" DataSourceID="SqlDataSourceRoleUser" GridLines="None" OnDeleteCommand="RadGridRoleUser_DeleteCommand">
            <MasterTableView DataSourceID="SqlDataSourceRoleUser">
               <Columns>
                  <telerik:GridBoundColumn DataField="RoleId" DataType="System.Guid" FilterControlAltText="Filter RoleId column" HeaderText="RoleId" SortExpression="RoleId" UniqueName="RoleId" Visible="false">
                  </telerik:GridBoundColumn>
                  <telerik:GridBoundColumn DataField="UserName" FilterControlAltText="Filter UserName column" HeaderText="Init" SortExpression="UserName" UniqueName="UserName" Visible="true">
                     <HeaderStyle Width="50px" HorizontalAlign="Left" Wrap="false" />
                     <ItemStyle VerticalAlign="Top" HorizontalAlign="Left" Wrap="false" />
                  </telerik:GridBoundColumn>
                  <telerik:GridBoundColumn DataField="RoleName" FilterControlAltText="Filter RoleName column" HeaderText="Role" SortExpression="RoleName" UniqueName="RoleName" Visible="false">
                  </telerik:GridBoundColumn>
                  <telerik:GridBoundColumn DataField="FullName" FilterControlAltText="Filter FullName column" HeaderText="Name" SortExpression="FullName" UniqueName="FullName">
                     <HeaderStyle HorizontalAlign="Left" Wrap="false" />
                     <ItemStyle VerticalAlign="Top" HorizontalAlign="Left" Wrap="false" />
                  </telerik:GridBoundColumn>
                  <telerik:GridButtonColumn ButtonType="ImageButton" CommandName="Delete" FilterControlAltText="Filter DeleteRoleUser column" UniqueName="DeleteRoleUser" ImageUrl="~/Images/delete.gif" ConfirmText="Please confirm that you want to delete the current user from the current role">
                     <HeaderStyle Width="50px" HorizontalAlign="Center" Wrap="false" />
                     <ItemStyle VerticalAlign="Top" HorizontalAlign="Center" Wrap="false" />
                  </telerik:GridButtonColumn>
               </Columns>
            </MasterTableView>
         </telerik:RadGrid>
         <br />
         <telerik:RadButton ID="RadButtonAddUser" runat="server" Text="Add user">
         </telerik:RadButton>
      </td>
   </tr>
</table>
<telerik:RadWindowManager ID="RadWindowManagerRole" runat="server">
   <Windows>
      <telerik:RadWindow ID="RadWindowNewRole" runat="server" Height="110px" Width="300px" KeepInScreenBounds="True" Modal="True" Style="display: none;" DestroyOnClose="True" EnableAriaSupport="True" Title="New role" ShowContentDuringLoad="False" Behaviors="Close" EnableShadow="True" VisibleStatusbar="False" Behavior="Close">
         <ContentTemplate>
            <table border="0" cellpadding="2">
               <tr valign="top">
                  <td>
                     <asp:Label ID="LabelRoleNameNew" runat="server" Text="Role"></asp:Label>
                  </td>
                  <td>
                     <telerik:RadTextBox ID="RadTextBoxRoleNameNew" runat="server" Width="200px" MaxLength="256">
                     </telerik:RadTextBox>
                  </td>
               </tr>
               <tr valign="top">
                  <td colspan="2">
                     <telerik:RadButton ID="RadButtonSaveNewRole" runat="server" Text="Save" OnClick="RadButtonSaveNewRole_Click">
                     </telerik:RadButton>
                     <telerik:RadButton ID="RadButtonCancelNewRole" runat="server" Text="Cancel" CommandName="Cancel">
                     </telerik:RadButton>
                  </td>
               </tr>
            </table>
         </ContentTemplate>
      </telerik:RadWindow>
      <telerik:RadWindow ID="RadWindowAddUser" runat="server" Height="110px" Width="400px" KeepInScreenBounds="True" Modal="True" Style="display: none;" DestroyOnClose="True" EnableAriaSupport="True" Title="New role" ShowContentDuringLoad="False" Behaviors="Close" EnableShadow="True" VisibleStatusbar="False" Behavior="Close">
         <ContentTemplate>
            <table border="0" cellpadding="2">
               <tr valign="top">
                  <td>
                     <asp:Label ID="LabelUserNew" runat="server" Text="User"></asp:Label>
                  </td>
                  <td>
                     <telerik:RadComboBox ID="RadComboBoxUserNew" runat="server" Width="300px" DataSourceID="SqlDataSourceUser" DataTextField="FullName" DataValueField="UserInit">
                     </telerik:RadComboBox>
                  </td>
               </tr>
               <tr valign="top">
                  <td colspan="2">
                     <telerik:RadButton ID="RadButtonSaveAddUser" runat="server" Text="Save" onclick="RadButtonSaveAddUser_Click">
                     </telerik:RadButton>
                     <telerik:RadButton ID="RadButtonCancelAddUser" runat="server" Text="Cancel" CommandName="Cancel">
                     </telerik:RadButton>
                  </td>
               </tr>
            </table>
         </ContentTemplate>
      </telerik:RadWindow>
   </Windows>
</telerik:RadWindowManager>
<asp:SqlDataSource ID="SqlDataSourceRole" runat="server" ConnectionString="<%$ ConnectionStrings:ApplicationServices %>"></asp:SqlDataSource>
<asp:SqlDataSource ID="SqlDataSourceRoleUser" runat="server" ConnectionString="<%$ ConnectionStrings:ApplicationServices %>" SelectCommand="SELECT UserAdditional.FirstName + ' ' + UserAdditional.LastName + ' [' + AgroModel.dbo.Country.CountryName + ']' AS FullName, aspnet_UsersInRoles.RoleId, aspnet_Users.UserName, aspnet_Roles.RoleName FROM aspnet_UsersInRoles INNER JOIN UserAdditional ON aspnet_UsersInRoles.UserId = UserAdditional.UserID INNER JOIN AgroModel.dbo.Country ON UserAdditional.CountryID = AgroModel.dbo.Country.CountryID INNER JOIN aspnet_Users ON aspnet_UsersInRoles.UserId = aspnet_Users.UserId AND UserAdditional.UserID = aspnet_Users.UserId INNER JOIN aspnet_Roles ON aspnet_UsersInRoles.RoleId = aspnet_Roles.RoleId WHERE (aspnet_UsersInRoles.RoleId = @RoleId)">
   <SelectParameters>
      <asp:ControlParameter ControlID="RadGridRole" Name="RoleId" PropertyName="SelectedValue" />
   </SelectParameters>
</asp:SqlDataSource>
<asp:SqlDataSource ID="SqlDataSourceUser" runat="server" ConnectionString="<%$ ConnectionStrings:ApplicationServices %>"></asp:SqlDataSource>
