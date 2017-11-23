<%@ Control Language="C#" AutoEventWireup="true" CodeBehind="ctrlUser.ascx.cs" Inherits="AgroModelToolbox.Controls._99_Admin.ctrlUser" %>
<%@ Register Assembly="Telerik.Web.UI" Namespace="Telerik.Web.UI" TagPrefix="telerik" %>
<telerik:RadScriptBlock ID="RadScriptBlockUserManagement" runat="server">
   <script type="text/javascript">
      var popUp;
      function PopUpShowingUser(sender, eventArgs)
      {
         popUp = eventArgs.get_popUp();
         var gridWidth = sender.get_element().offsetWidth;
         var popUpWidth = popUp.style.width.substr(0, popUp.style.width.indexOf("px"));
         popUp.style.left = ((gridWidth - popUpWidth) / 2 + sender.get_element().offsetLeft).toString() + "px";
         popUp.style.top = "200px";
      }
   </script>
</telerik:RadScriptBlock>
<asp:Label ID="LabelMessage" runat="server" Font-Bold="True" ForeColor="Red"></asp:Label>
<asp:Panel ID="PanelUser" runat="server" BorderColor="White" BorderWidth="2" BorderStyle="Solid">
   <telerik:RadGrid ID="RadGridUserMembership" runat="server" AllowSorting="True" AutoGenerateColumns="False" CellSpacing="0" DataSourceID="SqlDataSourceUserMembership" GridLines="None" OnDataBound="RadGridUserMembership_DataBound" Height="400px" OnDeleteCommand="RadGridUserMembership_DeleteCommand" OnSelectedIndexChanged="RadGridUserMembership_SelectedIndexChanged">
      <ClientSettings>
         <Scrolling AllowScroll="True" UseStaticHeaders="True" />
      </ClientSettings>
      <MasterTableView DataSourceID="SqlDataSourceUserMembership" DataKeyNames="UserId">
         <Columns>
            <telerik:GridBoundColumn DataField="ApplicationId" DataType="System.Guid" FilterControlAltText="Filter ApplicationId column" HeaderText="ApplicationId" SortExpression="ApplicationId" UniqueName="ApplicationId" Visible="false">
            </telerik:GridBoundColumn>
            <telerik:GridBoundColumn DataField="UserId" DataType="System.Guid" FilterControlAltText="Filter UserId column" HeaderText="UserId" SortExpression="UserId" UniqueName="UserId" Visible="false">
            </telerik:GridBoundColumn>
            <telerik:GridBoundColumn DataField="UserName" FilterControlAltText="Filter UserName column" HeaderText="Init" SortExpression="UserName" UniqueName="UserName">
               <HeaderStyle Width="50px" HorizontalAlign="Left" Wrap="false" />
               <ItemStyle VerticalAlign="Top" HorizontalAlign="Left" Wrap="false" />
            </telerik:GridBoundColumn>
            <telerik:GridBoundColumn DataField="FullName" FilterControlAltText="Filter FullName column" HeaderText="Name" SortExpression="FullName" UniqueName="FullName">
               <HeaderStyle HorizontalAlign="Left" Wrap="false" />
               <ItemStyle VerticalAlign="Top" HorizontalAlign="Left" Wrap="false" />
            </telerik:GridBoundColumn>
            <telerik:GridBoundColumn DataField="Email" FilterControlAltText="Filter Email column" HeaderText="Email" SortExpression="Email" UniqueName="Email">
               <HeaderStyle Width="250px" HorizontalAlign="Left" Wrap="false" />
               <ItemStyle VerticalAlign="Top" HorizontalAlign="Left" Wrap="false" />
            </telerik:GridBoundColumn>
            <telerik:GridCheckBoxColumn DataField="IsApproved" DataType="System.Boolean" FilterControlAltText="Filter IsApproved column" HeaderText="Approved" SortExpression="IsApproved" UniqueName="IsApproved">
               <HeaderStyle Width="75px" HorizontalAlign="Center" Wrap="false" />
               <ItemStyle VerticalAlign="Top" HorizontalAlign="Center" Wrap="false" />
            </telerik:GridCheckBoxColumn>
            <telerik:GridCheckBoxColumn DataField="IsLockedOut" DataType="System.Boolean" FilterControlAltText="Filter IsLockedOut column" HeaderText="Locked out" SortExpression="IsLockedOut" UniqueName="IsLockedOut">
               <HeaderStyle Width="75px" HorizontalAlign="Center" Wrap="false" />
               <ItemStyle VerticalAlign="Top" HorizontalAlign="Center" Wrap="false" />
            </telerik:GridCheckBoxColumn>
            <telerik:GridBoundColumn DataField="CreateDate" DataType="System.DateTime" FilterControlAltText="Filter CreateDate column" HeaderText="Created" SortExpression="CreateDate" UniqueName="CreateDate" DataFormatString="{0:dd-MM-yyyy}">
               <HeaderStyle Width="75px" HorizontalAlign="Left" Wrap="false" />
               <ItemStyle VerticalAlign="Top" HorizontalAlign="Left" Wrap="false" />
            </telerik:GridBoundColumn>
            <telerik:GridBoundColumn DataField="LastActivityDate" DataType="System.DateTime" FilterControlAltText="Filter LastActivityDate column" HeaderText="Last activity" SortExpression="LastActivityDate" UniqueName="LastActivityDate" DataFormatString="{0:dd-MM-yyyy}">
               <HeaderStyle Width="75px" HorizontalAlign="Left" Wrap="false" />
               <ItemStyle VerticalAlign="Top" HorizontalAlign="Left" Wrap="false" />
            </telerik:GridBoundColumn>
            <telerik:GridButtonColumn ButtonType="ImageButton" CommandName="Select" FilterControlAltText="Filter SelectUser column" UniqueName="SelectUser" ImageUrl="~/Images/obs.gif">
               <HeaderStyle Width="50px" HorizontalAlign="Center" Wrap="false" />
               <ItemStyle VerticalAlign="Top" HorizontalAlign="Center" Wrap="false" />
            </telerik:GridButtonColumn>
            <telerik:GridButtonColumn ButtonType="ImageButton" CommandName="Delete" FilterControlAltText="Filter DeleteUser column" Text="Delete" UniqueName="DeleteUser" ConfirmText="Please confirm that you want to delete the current user" ImageUrl="~/Images/delete.gif">
               <HeaderStyle Width="50px" HorizontalAlign="Center" Wrap="false" />
               <ItemStyle VerticalAlign="Top" HorizontalAlign="Center" Wrap="false" />
            </telerik:GridButtonColumn>
         </Columns>
      </MasterTableView>
   </telerik:RadGrid>
   <br />
   <telerik:RadGrid ID="RadGridUserAdditional" runat="server" AutoGenerateColumns="False" CellSpacing="0" DataSourceID="SqlDataSourceUserAdditional" GridLines="None" AllowAutomaticUpdates="True">
      <ClientSettings>
         <ClientEvents OnPopUpShowing="PopUpShowingUser" />
      </ClientSettings>
      <MasterTableView DataKeyNames="UserID" DataSourceID="SqlDataSourceUserAdditional" EditMode="PopUp">
         <Columns>
            <telerik:GridBoundColumn DataField="UserID" DataType="System.Guid" FilterControlAltText="Filter UserID column" HeaderText="UserID" ReadOnly="True" SortExpression="UserID" UniqueName="UserID" Visible="false">
            </telerik:GridBoundColumn>
            <telerik:GridBoundColumn DataField="UserInit" FilterControlAltText="Filter UserInit column" HeaderText="UserInit" SortExpression="UserInit" UniqueName="UserInit" Visible="false">
            </telerik:GridBoundColumn>
            <telerik:GridBoundColumn DataField="FirstName" FilterControlAltText="Filter FirstName column" HeaderText="First name" SortExpression="FirstName" UniqueName="FirstName">
               <HeaderStyle HorizontalAlign="Left" Wrap="false" />
               <ItemStyle VerticalAlign="Top" HorizontalAlign="Left" Wrap="false" />
            </telerik:GridBoundColumn>
            <telerik:GridBoundColumn DataField="LastName" FilterControlAltText="Filter LastName column" HeaderText="Last name" SortExpression="LastName" UniqueName="LastName">
               <HeaderStyle HorizontalAlign="Left" Wrap="false" />
               <ItemStyle VerticalAlign="Top" HorizontalAlign="Left" Wrap="false" />
            </telerik:GridBoundColumn>
            <telerik:GridBoundColumn DataField="Institution" FilterControlAltText="Filter Institution column" HeaderText="Institution" SortExpression="Institution" UniqueName="Institution">
               <HeaderStyle HorizontalAlign="Left" Wrap="false" />
               <ItemStyle VerticalAlign="Top" HorizontalAlign="Left" Wrap="false" />
            </telerik:GridBoundColumn>
            <telerik:GridBoundColumn DataField="Department" FilterControlAltText="Filter Department column" HeaderText="Department" SortExpression="Department" UniqueName="Department">
               <HeaderStyle HorizontalAlign="Left" Wrap="false" />
               <ItemStyle VerticalAlign="Top" HorizontalAlign="Left" Wrap="false" />
            </telerik:GridBoundColumn>
            <telerik:GridBoundColumn DataField="CountryName" FilterControlAltText="Filter CountryName column" HeaderText="Country" SortExpression="CountryName" UniqueName="CountryName">
               <HeaderStyle HorizontalAlign="Left" Wrap="false" />
               <ItemStyle VerticalAlign="Top" HorizontalAlign="Left" Wrap="false" />
            </telerik:GridBoundColumn>
            <telerik:GridCheckBoxColumn DataField="Active" DataType="System.Boolean" FilterControlAltText="Filter Active column" HeaderText="Active" SortExpression="Active" UniqueName="Active">
               <HeaderStyle Width="50" HorizontalAlign="Center" Wrap="false" />
               <ItemStyle VerticalAlign="Top" HorizontalAlign="Center" Wrap="false" />
            </telerik:GridCheckBoxColumn>
            <telerik:GridEditCommandColumn ButtonType="ImageButton" FilterControlAltText="Filter EditCommandColumn column" EditImageUrl="~/Images/edit.gif">
               <HeaderStyle Width="50" HorizontalAlign="Center" Wrap="false" />
               <ItemStyle VerticalAlign="Top" HorizontalAlign="Center" Wrap="false" />
            </telerik:GridEditCommandColumn>
         </Columns>
         <EditFormSettings EditFormType="Template" CaptionFormatString="Edit user {0}" CaptionDataField="UserInit">
            <EditColumn UniqueName="EditCommandColumn" FilterControlAltText="Filter EditCommandColumn column">
            </EditColumn>
            <PopUpSettings Width="450px" Modal="true" />
            <FormTemplate>
               <table cellpadding="2">
                  <tr>
                     <td>
                        <asp:Label ID="LabelUserInit" runat="server" Text="Init"></asp:Label>
                     </td>
                     <td>
                        <telerik:RadTextBox ID="RadTextBoxUserInit" runat="server" Width="50px" ReadOnly="True" Text="<%# Bind('UserInit') %>">
                        </telerik:RadTextBox>
                     </td>
                  </tr>
                  <tr>
                     <td>
                        <asp:Label ID="LabelFirstName" runat="server" Text="First name"></asp:Label>
                     </td>
                     <td>
                        <telerik:RadTextBox ID="RadTextBoxFirstName" runat="server" Width="350px" MaxLength="50" Text="<%# Bind('FirstName') %>">
                        </telerik:RadTextBox>
                     </td>
                  </tr>
                  <tr>
                     <td>
                        <asp:Label ID="LabelLastName" runat="server" Text="Last name"></asp:Label>
                     </td>
                     <td>
                        <telerik:RadTextBox ID="RadTextBoxLastName" runat="server" Width="350px" MaxLength="50" Text="<%# Bind('LastName') %>">
                        </telerik:RadTextBox>
                     </td>
                  </tr>
                  <tr>
                     <td>
                        <asp:Label ID="LabelInstitution" runat="server" Text="Institution"></asp:Label>
                     </td>
                     <td>
                        <telerik:RadTextBox ID="RadTextBoxInstitution" runat="server" Width="350px" MaxLength="50" Text="<%# Bind('Institution') %>">
                        </telerik:RadTextBox>
                     </td>
                  </tr>
                  <tr>
                     <td>
                        <asp:Label ID="LabelDepartment" runat="server" Text="Department"></asp:Label>
                     </td>
                     <td>
                        <telerik:RadTextBox ID="RadTextBoxDepartment" runat="server" Width="350px" MaxLength="50" Text="<%# Bind('Department') %>">
                        </telerik:RadTextBox>
                     </td>
                  </tr>
                  <tr>
                     <td>
                        <asp:Label ID="LabelCountry" runat="server" Text="Country"></asp:Label>
                     </td>
                     <td>
                        <telerik:RadComboBox ID="RadComboBoxCountry" runat="server" Width="200px" DataSourceID="SqlDataSourceCountryList" DataTextField="CountryName" DataValueField="CountryID" SelectedValue="<%# Bind('CountryID') %>">
                        </telerik:RadComboBox>
                     </td>
                  </tr>
                  <tr>
                     <td>
                        <asp:Label ID="LabelActive" runat="server" Text="Active"></asp:Label>
                     </td>
                     <td>
                        <asp:CheckBox ID="CheckBoxActive" runat="server" Checked="<%# Bind('Active') %>" />
                     </td>
                  </tr>
                  <tr>
                     <td colspan="2">
                        <asp:Button ID="ButtonSaveEdit" runat="server" Text="Save" CausesValidation="False" CommandName="Update" />
                        <asp:Button ID="ButtonCancelEdit" runat="server" Text="Cancel" CausesValidation="False" CommandName="Cancel" />
                     </td>
                  </tr>
               </table>
            </FormTemplate>
         </EditFormSettings>
      </MasterTableView>
   </telerik:RadGrid>
   <br />
   <telerik:RadButton ID="RadButtonNewUser" runat="server" Text="New user">
   </telerik:RadButton>
   <telerik:RadButton ID="RadButtonChangePassword" runat="server" Text="Change password">
   </telerik:RadButton>
</asp:Panel>
<telerik:RadWindowManager ID="RadWindowManagerUser" runat="server">
   <Windows>
      <telerik:RadWindow ID="RadWindowNewUser" runat="server" Title="New user" Height="310px" Width="450px" KeepInScreenBounds="True" Modal="True" Style="display: none;" DestroyOnClose="True" EnableAriaSupport="True" ShowContentDuringLoad="False" Behaviors="Close" EnableShadow="True" VisibleStatusbar="False" Behavior="Close">
         <ContentTemplate>
            <table border="0" cellpadding="2">
               <tr valign="top">
                  <td>
                     <asp:Label ID="LabelUserNameNew" runat="server" Text="Init"></asp:Label>
                  </td>
                  <td>
                     <telerik:RadTextBox ID="RadTextBoxUserNameNew" runat="server" Width="50px" MaxLength="3">
                     </telerik:RadTextBox>
                  </td>
               </tr>
               <tr valign="top">
                  <td>
                     <asp:Label ID="LabelPassword" runat="server" Text="Password"></asp:Label>
                  </td>
                  <td>
                     <telerik:RadTextBox ID="RadTextBoxPasswordNew" runat="server" Width="100px" MaxLength="10">
                     </telerik:RadTextBox>
                  </td>
               </tr>
               <tr valign="top">
                  <td>
                     <asp:Label ID="LabelFirstName" runat="server" Text="Firstname"></asp:Label>
                  </td>
                  <td>
                     <telerik:RadTextBox ID="RadTextBoxFirstNameNew" runat="server" Width="300px" MaxLength="50">
                     </telerik:RadTextBox>
                  </td>
               </tr>
               <tr valign="top">
                  <td>
                     <asp:Label ID="LabelLastName" runat="server" Text="Lastname"></asp:Label>
                  </td>
                  <td>
                     <telerik:RadTextBox ID="RadTextBoxLastNameNew" runat="server" Width="300px" MaxLength="50">
                     </telerik:RadTextBox>
                  </td>
               </tr>
               <tr valign="top">
                  <td>
                     <asp:Label ID="LabelEmail" runat="server" Text="E-mail"></asp:Label>
                  </td>
                  <td>
                     <telerik:RadTextBox ID="RadTextBoxEmailNew" runat="server" Width="300px" MaxLength="50">
                     </telerik:RadTextBox>
                  </td>
               </tr>
               <tr valign="top">
                  <td>
                     <asp:Label ID="LabelInstitution" runat="server" Text="Institution"></asp:Label>
                  </td>
                  <td>
                     <telerik:RadTextBox ID="RadTextBoxInstitutionNew" runat="server" Width="300px" MaxLength="50">
                     </telerik:RadTextBox>
                  </td>
               </tr>
               <tr valign="top">
                  <td>
                     <asp:Label ID="LabelDepartment" runat="server" Text="Department"></asp:Label>
                  </td>
                  <td>
                     <telerik:RadTextBox ID="RadTextBoxDepartmentNew" runat="server" Width="300px" MaxLength="50">
                     </telerik:RadTextBox>
                  </td>
               </tr>
               <tr valign="top">
                  <td>
                     <asp:Label ID="LabelCountryID" runat="server" Text="CountryID"></asp:Label>
                  </td>
                  <td>
                     <telerik:RadComboBox ID="RadComboBoxCountry" runat="server" Width="200px" DataSourceID="SqlDataSourceCountryList" DataValueField="CountryID" DataTextField="CountryName">
                     </telerik:RadComboBox>
                  </td>
               </tr>
               <tr valign="top">
                  <td colspan="2">
                     <telerik:RadButton ID="RadButtonSaveNewUser" runat="server" Text="Save" OnClick="RadButtonSaveNewUser_Click">
                     </telerik:RadButton>
                     <telerik:RadButton ID="RadButtonCancelNewUser" runat="server" Text="Cancel" CommandName="Cancel">
                     </telerik:RadButton>
                  </td>
               </tr>
            </table>
         </ContentTemplate>
      </telerik:RadWindow>
      <telerik:RadWindow ID="RadWindowChangePassword" runat="server" Title="Change password" Height="110px" Width="500px" KeepInScreenBounds="True" Modal="True" Style="display: none;" DestroyOnClose="True" EnableAriaSupport="True" ShowContentDuringLoad="False" Behaviors="Close" EnableShadow="True" VisibleStatusbar="False" Behavior="Close">
         <ContentTemplate>
            <table border="0" cellpadding="2">
               <tr valign="top">
                  <td>
                     <asp:Label ID="LabelNewPassword" runat="server"></asp:Label>
                  </td>
                  <td>
                     <telerik:RadTextBox ID="RadTextBoxNewPassword" runat="server" Width="100px" MaxLength="10">
                     </telerik:RadTextBox>
                  </td>
               </tr>
               <tr valign="top">
                  <td colspan="2">
                     <telerik:RadButton ID="RadButtonSaveChangePassword" runat="server" Text="Save" OnClick="RadButtonSaveChangePassword_Click">
                     </telerik:RadButton>
                     <telerik:RadButton ID="RadButtonCancelChangePassword" runat="server" Text="Cancel" CommandName="Cancel">
                     </telerik:RadButton>
                  </td>
               </tr>
            </table>
         </ContentTemplate>
      </telerik:RadWindow>
   </Windows>
</telerik:RadWindowManager>
<asp:SqlDataSource ID="SqlDataSourceCountryList" runat="server" ConnectionString="<%$ ConnectionStrings:conAgroModel %>" SelectCommand="SELECT CountryID, CountryName FROM Country ORDER BY CountryName"></asp:SqlDataSource>
<asp:SqlDataSource ID="SqlDataSourceUserMembership" runat="server" ConnectionString="<%$ ConnectionStrings:ApplicationServices %>"></asp:SqlDataSource>
<asp:SqlDataSource ID="SqlDataSourceUserAdditional" runat="server" ConnectionString="<%$ ConnectionStrings:ApplicationServices %>" SelectCommand="SELECT UserAdditional.UserID, UserAdditional.UserInit, UserAdditional.FirstName, UserAdditional.LastName, UserAdditional.Institution, UserAdditional.Department, UserAdditional.CountryID, UserAdditional.Active, AgroModel.dbo.Country.CountryName FROM UserAdditional INNER JOIN AgroModel.dbo.Country ON UserAdditional.CountryID = AgroModel.dbo.Country.CountryID WHERE (UserAdditional.UserID = @UserID) ORDER BY UserAdditional.UserInit" UpdateCommand="UPDATE [UserAdditional] SET [FirstName] = @FirstName, [LastName] = @LastName, [Institution] = @Institution, [Department] = @Department, [CountryID] = @CountryID, [Active] = @Active WHERE [UserID] = @UserID">
   <SelectParameters>
      <asp:ControlParameter ControlID="RadGridUserMembership" Name="UserID" PropertyName="SelectedValue" Type="Object" />
   </SelectParameters>
   <UpdateParameters>
      <asp:Parameter Name="FirstName" Type="String" />
      <asp:Parameter Name="LastName" Type="String" />
      <asp:Parameter Name="Institution" Type="String" />
      <asp:Parameter Name="Department" Type="String" />
      <asp:Parameter Name="CountryID" Type="String" />
      <asp:Parameter Name="Active" Type="Boolean" />
      <asp:Parameter Name="UserID" Type="Object" />
   </UpdateParameters>
</asp:SqlDataSource>
