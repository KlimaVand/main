<%@ Control Language="C#" AutoEventWireup="true" CodeBehind="ctrlTabPageFarmAC.ascx.cs" Inherits="FarmAC.Controls.ctrlTabPageFarmAC" %>
<%@ Register TagPrefix="FarmAC" TagName="Farm" Src="ctrlFarm.ascx" %>
<%@ Register TagPrefix="FarmAC" TagName="CropSequence" Src="ctrlCropSequence.ascx" %>
<%@ Register TagPrefix="FarmAC" TagName="Ruminants" Src="ctrlRuminants.ascx" %>
<%@ Register TagPrefix="FarmAC" TagName="NonRuminants" Src="ctrlNonRuminants.ascx" %>
<%@ Register TagPrefix="FarmAC" TagName="Balance" Src="ctrlBalance.ascx" %>
<telerik:RadTabStrip ID="RadTabStripFarmAC" runat="server" MultiPageID="RadMultiPageFarmAC" SelectedIndex="0" ShowBaseLine="True" AutoPostBack="True">
   <Tabs>
      <telerik:RadTab runat="server" Text="Farm" Selected="true">
      </telerik:RadTab>
      <telerik:RadTab runat="server" Text="Crop sequence">
      </telerik:RadTab>
      <telerik:RadTab runat="server" Text="Ruminants">
      </telerik:RadTab>
      <telerik:RadTab runat="server" Text="Non-ruminants">
      </telerik:RadTab>
      <telerik:RadTab runat="server" Text="Balance">
      </telerik:RadTab>
   </Tabs>
</telerik:RadTabStrip>
<telerik:RadMultiPage ID="RadMultiPageFarmAC" runat="server" Width="1018px" SelectedIndex="0" Style="border-left: grey 1px solid; border-right: grey 1px solid; border-bottom: grey 1px solid">
   <telerik:RadPageView ID="RadPageViewFarm" runat="server" Width="100%">
      <FarmAC:Farm ID="controlFarm" runat="server" />
   </telerik:RadPageView>
   <telerik:RadPageView ID="RadPageViewCropSequence" runat="server" Width="100%">
      <FarmAC:CropSequence ID="controlCropSequence" runat="server" />
   </telerik:RadPageView>
   <telerik:RadPageView ID="RadPageViewRuminatns" runat="server" Width="100%">
      <FarmAC:Ruminants ID="controlRuminants" runat="server" />
   </telerik:RadPageView>
   <telerik:RadPageView ID="RadPageViewNonRuminats" runat="server" Width="100%">
      <FarmAC:NonRuminants ID="controlNonRuminats" runat="server" />
   </telerik:RadPageView>
   <telerik:RadPageView ID="RadPageViewBalance" runat="server" Width="100%">
      <FarmAC:Balance ID="controlBalance" runat="server" />
   </telerik:RadPageView>
</telerik:RadMultiPage>
