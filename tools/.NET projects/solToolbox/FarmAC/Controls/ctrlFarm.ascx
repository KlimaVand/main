<%@ Control Language="C#" AutoEventWireup="true" CodeBehind="ctrlFarm.ascx.cs" Inherits="FarmAC.Controls.ctrlFarm" %>
<telerik:RadScriptBlock ID="RadScriptBlockScenario" runat="server">
   <script type="text/javascript">
      var popUp;
      function PopUpShowingFarm(sender, eventArgs)
      {
         popUp = eventArgs.get_popUp();
         var gridWidth = window.innerWidth; //sender.get_element().offsetWidth;
         var popUpWidth = popUp.style.width.substr(0, popUp.style.width.indexOf("px"));
         popUp.style.left = ((gridWidth - popUpWidth) / 2 + sender.get_element().offsetLeft).toString() + "px";
         popUp.style.top = "200px";
      }
   </script>
</telerik:RadScriptBlock>
<table border="0" cellpadding="2">
   <tr valign="top">
      <td>
         <telerik:RadGrid ID="RadGridFarm" runat="server" AutoGenerateColumns="False" DataSourceID="SqlDataSourceFarm" ResolvedRenderMode="Classic" OnDataBound="RadGridFarm_DataBound" Width="100%">
            <ClientSettings>
               <ClientEvents OnPopUpShowing="PopUpShowingFarm" />
            </ClientSettings>
            <MasterTableView AllowAutomaticDeletes="True" AllowAutomaticUpdates="True" AllowMultiColumnSorting="True" AllowSorting="True" DataSourceID="SqlDataSourceFarm" ShowFooter="True" EditMode="PopUp">
               <Columns>
                  <telerik:GridBoundColumn DataField="FarmNumber" DataType="System.Int64" 
                       FilterControlAltText="Filter FarmNumber column" HeaderText="FarmNumber" 
                       SortExpression="FarmNumber" UniqueName="FarmNumber">
                  </telerik:GridBoundColumn>
                  <telerik:GridBoundColumn DataField="FarmOwnerName" 
                       FilterControlAltText="Filter FarmOwnerName column" HeaderText="FarmOwnerName" 
                       SortExpression="FarmOwnerName" UniqueName="FarmOwnerName">
                  </telerik:GridBoundColumn>
                  <telerik:GridBoundColumn DataField="FarmOwnerAddress" 
                       FilterControlAltText="Filter FarmOwnerAddress column" 
                       HeaderText="FarmOwnerAddress" SortExpression="FarmOwnerAddress" 
                       UniqueName="FarmOwnerAddress" Visible="False">
                  </telerik:GridBoundColumn>
                  <telerik:GridBoundColumn DataField="FarmOwnerZipCode" 
                       FilterControlAltText="Filter FarmOwnerZipCode column" 
                       HeaderText="FarmOwnerZipCode" SortExpression="FarmOwnerZipCode" 
                       UniqueName="FarmOwnerZipCode" Visible="False">
                  </telerik:GridBoundColumn>
                  <telerik:GridBoundColumn DataField="FarmOwnerTown" 
                       FilterControlAltText="Filter FarmOwnerTown column" HeaderText="FarmOwnerTown" 
                       SortExpression="FarmOwnerTown" UniqueName="FarmOwnerTown" Visible="False">
                  </telerik:GridBoundColumn>
                  <telerik:GridBoundColumn DataField="SoilType" DataType="System.Int32" 
                       FilterControlAltText="Filter SoilType column" HeaderText="SoilType" 
                       SortExpression="SoilType" UniqueName="SoilType" Visible="False">
                  </telerik:GridBoundColumn>
                  <telerik:GridCheckBoxColumn DataField="Irrigation" DataType="System.Boolean" 
                       FilterControlAltText="Filter Irrigation column" HeaderText="Irrigation" 
                       SortExpression="Irrigation" UniqueName="Irrigation" Visible="False">
                  </telerik:GridCheckBoxColumn>
                  <telerik:GridBoundColumn DataField="Delivery" DataType="System.Int32" 
                       FilterControlAltText="Filter Delivery column" HeaderText="Delivery" 
                       SortExpression="Delivery" UniqueName="Delivery" Visible="False">
                  </telerik:GridBoundColumn>
                  <telerik:GridBoundColumn DataField="FarmType" DataType="System.Int32" 
                       FilterControlAltText="Filter FarmType column" HeaderText="FarmType" 
                       SortExpression="FarmType" UniqueName="FarmType" Visible="False">
                  </telerik:GridBoundColumn>
                  <telerik:GridBoundColumn DataField="SelectedScenario" DataType="System.Int32" 
                       FilterControlAltText="Filter SelectedScenario column" 
                       HeaderText="SelectedScenario" SortExpression="SelectedScenario" 
                       UniqueName="SelectedScenario" Visible="False">
                  </telerik:GridBoundColumn>
                  <telerik:GridBoundColumn DataField="YieldLevel" DataType="System.Double" 
                       FilterControlAltText="Filter YieldLevel column" HeaderText="YieldLevel" 
                       SortExpression="YieldLevel" UniqueName="YieldLevel" Visible="False">
                  </telerik:GridBoundColumn>
                  <telerik:GridBoundColumn DataField="ManureVersion" DataType="System.Int32" 
                       FilterControlAltText="Filter ManureVersion column" HeaderText="ManureVersion" 
                       SortExpression="ManureVersion" UniqueName="ManureVersion" Visible="False">
                  </telerik:GridBoundColumn>
                  <telerik:GridBoundColumn DataField="ScenarioName" FilterControlAltText="Filter ScenarioName column" HeaderText="ScenarioName" SortExpression="ScenarioName" UniqueName="ScenarioName">
                  </telerik:GridBoundColumn>
                  <telerik:GridEditCommandColumn ButtonType="ImageButton" FilterControlAltText="Filter EditCommandColumn column" UniqueName="EditScenario" EditImageUrl="~/Images/edit.gif">
                     <HeaderStyle Width="50px" HorizontalAlign="Center" VerticalAlign="Top" Wrap="False" />
                     <ItemStyle HorizontalAlign="Center" VerticalAlign="Top" Wrap="False" />
                  </telerik:GridEditCommandColumn>
                  <telerik:GridButtonColumn ButtonType="ImageButton" CommandName="Select" FilterControlAltText="Filter SelectScenario column" UniqueName="SelectScenario" ImageUrl="~/Images/detail.gif" Text="Select">
                     <HeaderStyle Width="50px" HorizontalAlign="Center" VerticalAlign="Top" Wrap="false" />
                     <ItemStyle HorizontalAlign="Center" VerticalAlign="Top" Wrap="false" />
                  </telerik:GridButtonColumn>
                  <telerik:GridButtonColumn ButtonType="ImageButton" CommandName="Delete" FilterControlAltText="Filter DeleteScenario column" UniqueName="DeleteScenario" ImageUrl="~/Images/delete.gif" ConfirmText="Please confirm that you want to delete the current farm">
                     <HeaderStyle Width="50px" HorizontalAlign="Center" VerticalAlign="Top" Wrap="false" />
                     <ItemStyle HorizontalAlign="Center" VerticalAlign="Top" Wrap="false" />
                  </telerik:GridButtonColumn>
               </Columns>
               <EditFormSettings EditFormType="Template" CaptionDataField="FarmOwnerName" CaptionFormatString="Edit farm '{0}'">
                  <FormTemplate>
                     <table>
                        <tr>
                           <td>
                              <asp:Label ID="LabelFarmOwnerName" runat="server" Text="Name"></asp:Label>
                           </td>
                           <td>
                              <telerik:RadTextBox ID="RadTextBoxFarmOwnerName" runat="server" Width="200px" MaxLength="50" Text="<%# Bind('FarmOwnerName') %>">
                              </telerik:RadTextBox>
                           </td>
                        </tr>
                        <tr>
                           <td>
                              <asp:Label ID="LabelSoilType" runat="server" Text="Soil type"></asp:Label>
                           </td>
                           <td>
                              <telerik:RadComboBox ID="RadComboBoxSoilType" runat="server" DataSourceID="SqlDataSourceSoilType" DataTextField="SoilTypeDescription" DataValueField="SoilTypeID" ResolvedRenderMode="Classic" SelectedValue="<%# Bind('SoilType') %>">
                              </telerik:RadComboBox>
                           </td>
                        </tr>
                        <tr valign="top">
                           <td colspan="2">
                              <telerik:RadButton ID="RadButtonUpdateFarm" Text="Update" runat="server" CausesValidation="False" CommandName="Update" />
                              <telerik:RadButton ID="RadButtonCancelFarm" Text="Cancel" runat="server" CausesValidation="False" CommandName="Cancel" />
                           </td>
                        </tr>
                     </table>
                  </FormTemplate>
                  <EditColumn ButtonType="PushButton" FilterControlAltText="Filter EditCommandColumn column">
                  </EditColumn>
                  <PopUpSettings Modal="true" Width="500px" />
               </EditFormSettings>
            </MasterTableView>
         </telerik:RadGrid>
      </td>
   </tr>
   <tr valign="top">
      <td>
         <telerik:RadButton ID="RadButtonNewFarm" runat="server" Text="New">
         </telerik:RadButton>
      </td>
   </tr>
</table>
<asp:SqlDataSource ID="SqlDataSourceFarm" runat="server" 
    ConnectionString="<%$ ConnectionStrings:FarmAC %>" 
    SelectCommand="SELECT Farm.FarmNumber, Farm.FarmOwnerName, Farm.FarmOwnerAddress, Farm.FarmOwnerZipCode, Farm.FarmOwnerTown, Farm.SoilType, Farm.Irrigation, Farm.Delivery, Farm.FarmType, Farm.SelectedScenario, Farm.YieldLevel, Farm.ManureVersion, Scenario.ScenarioName, Farm.CreatedDate, Farm.Creator, Farm.RecentEditedDate, Farm.RecentEditor FROM Farm INNER JOIN Farm_User ON Farm.FarmNumber = Farm_User.FarmNumber INNER JOIN Scenario ON Farm.FarmNumber = Scenario.FarmNumber AND Farm.SelectedScenario = Scenario.ScenarioID WHERE (Farm_User.UserID = @aUserID)" 
    DeleteCommand="DELETE FROM Farm WHERE (FarmNumber = @FarmNumber) DELETE FROM FieldPlanRotation WHERE (FarmNumber = @FarmNumber)" 
    
    
    UpdateCommand="UPDATE Farm SET FarmOwnerName =@aFarmOwnerName, FarmOwnerAddress = @aFarmOwnerAddress, FarmOwnerZipCode = @aFarmOwnerZipCode, FarmOwnerTown = @aFarmOwnerTown, SoilType = @aSoilType, Irrigation = @aIrrigationType, FarmType = @aFarmType, SelectedScenario = @aSelectedScenario, YieldLevel = @aYieldLevel, AgroEcologicalZone = @aAgroZone, FarmDescription = @aFarmDescription, AtmosphericNDeposition = @aAtmosphericDeposition WHERE (FarmNumber = @aFarmNumber)">
   <DeleteParameters>
       <asp:ControlParameter ControlID="RadGridFarm" Name="FarmNumber" 
                PropertyName="SelectedValue" Type="Int64" />
   </DeleteParameters>
   <SelectParameters>
      <asp:Parameter DefaultValue="2" Name="aUserID" Type="String" />
   </SelectParameters>
   <UpdateParameters>
      <asp:Parameter Name="aFarmOwnerName"/>
      <asp:Parameter Name="aFarmOwnerAddress"/>
      <asp:Parameter Name="aFarmOwnerZipCode"/>
      <asp:Parameter Name="aFarmOwnerTown"/>
      <asp:Parameter Name="aSoilType"/>
      <asp:Parameter Name="aIrrigationType"/>
      <asp:Parameter Name="aFarmType"/>
      <asp:Parameter Name="aSelectedScenario"/>
      <asp:Parameter Name="aYieldLevel"/>
      <asp:Parameter Name="aAgroZone"/>
      <asp:Parameter Name="aFarmDescription"/>
      <asp:Parameter Name="aAtmosphericDeposition"/>
      <asp:Parameter Name="original_FarmNumber"/>
   </UpdateParameters>
</asp:SqlDataSource>
<asp:SqlDataSource ID="SqlDataSourceSoilType" runat="server" ConnectionString="<%$ ConnectionStrings:FarmAC %>" SelectCommand="GetSelectListSoilType" SelectCommandType="StoredProcedure"></asp:SqlDataSource>
