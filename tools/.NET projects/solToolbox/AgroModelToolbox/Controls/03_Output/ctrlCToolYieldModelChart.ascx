<%@ Control Language="C#" AutoEventWireup="true" CodeBehind="ctrlCToolYieldModelChart.ascx.cs" Inherits="AgroModelToolbox.Controls._03_Output.ctrlCToolYieldModelChart" %>
<%@ Register TagPrefix="AgroModelToolbox" TagName="InputData" Src="ctrlInputData.ascx" %>
<%@ Register TagPrefix="AgroModelToolbox" TagName="InputDataOverview" Src="ctrlInputDataOverview.ascx" %>
<style type="text/css">
    .style1
    {
        text-align: center;
    }
</style>
<telerik:RadTabStrip ID="RadTabStripCtoolYield" runat="server" MultiPageID="RadMultiPageFarmAC" SelectedIndex="0" ShowBaseLine="True" AutoPostBack="True">
   <Tabs>
      <telerik:RadTab runat="server" Text="Select" Selected="true">
      </telerik:RadTab>
      <telerik:RadTab runat="server" Text="Input data overview">
      </telerik:RadTab>
   </Tabs>
</telerik:RadTabStrip>
<telerik:RadMultiPage ID="RadMultiPageStripCtoolYield" runat="server" SelectedIndex="0" Style="border-left: grey 1px solid; border-right: grey 1px solid; border-bottom: grey 1px solid">
   <telerik:RadPageView ID="RadPageViewInputData" runat="server" Width="100%">
      <AgroModelToolbox:InputData ID="controlInputData" runat="server" />
   </telerik:RadPageView>
   <telerik:RadPageView ID="RadPageViewInputDataOverview" runat="server" Width="100%">
      <AgroModelToolbox:InputData ID="InputDataOverview" runat="server" />
   </telerik:RadPageView>
</telerik:RadMultiPage>
