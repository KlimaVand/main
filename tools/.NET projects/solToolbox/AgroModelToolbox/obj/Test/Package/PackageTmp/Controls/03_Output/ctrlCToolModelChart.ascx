<%@ Control Language="C#" AutoEventWireup="true" CodeBehind="ctrlCToolModelChart.ascx.cs" Inherits="AgroModelToolbox.Controls._03_Output.ctrlCtoolModelChart" %>
<%@ Register Assembly="netchartdir" Namespace="ChartDirector" TagPrefix="chart" %>
<br />
<asp:Label ID="LabelMessage" runat="server" Font-Bold="true" ForeColor="Red"></asp:Label>
<br />
<asp:DropDownList ID="DropDownList1" runat="server">
<%----%>    <asp:ListItem Value="1">Default</asp:ListItem> <%--
    <asp:ListItem Value="7">Ultuna J</asp:ListItem>--%>
    <%--<asp:ListItem Value="2">C14 scenario</asp:ListItem>--%>
<%--    <asp:ListItem Value="6">Grass scenario</asp:ListItem>
    <asp:ListItem Value="7">Manure scenario</asp:ListItem>--%>
<%--        <asp:ListItem Value="4">Manure scenario</asp:ListItem> --%>
</asp:DropDownList>
<asp:DropDownList ID="DropDownList2" runat="server">
    <asp:ListItem Value="5">Ultuna MINSTR</asp:ListItem>
    <asp:ListItem Value="6">Ultuna FLL</asp:ListItem>
    <asp:ListItem Value="7">Ultuna J</asp:ListItem>
    <asp:ListItem Value="8">Slagelse</asp:ListItem>
    <asp:ListItem Value="9">Northern Hungary 1</asp:ListItem>
    <asp:ListItem Value="10">Northern Hungary 2</asp:ListItem>
    <%--<asp:ListItem Value="2">C14 scenario</asp:ListItem>
    <asp:ListItem Value="3">Grass scenario</asp:ListItem>
    <asp:ListItem Value="4">Manure scenario</asp:ListItem>--%>
</asp:DropDownList>
<br />
<br />
<table>
   <tr>
      <td colspan="2" align="center">
      <asp:Button ID="OK" runat="server" Text="OK"  />
      </td>
  </tr>
   <tr>
      <td>
         <chart:WebChartViewer ID="WebChartViewerCToolModelChart_1" runat="server" />
      </td>
      <td>
         <chart:WebChartViewer ID="WebChartViewerCToolModelChart_2" runat="server" />
<%--      <asp:Label ID="Label_C3060" runat="server">Soil C content in 30-60 cm</asp:Label>
      <asp:DropDownList ID="Dropdown_C3060" runat="server" AutoPostBack="false">
          <asp:ListItem Value="1">1 %</asp:ListItem>
          <asp:ListItem Value="2">2 %</asp:ListItem>
          <asp:ListItem Value="3">3 %</asp:ListItem>
          <asp:ListItem Value="4">4 %</asp:ListItem>
          </asp:DropDownList>&nbsp;&nbsp;&nbsp;  
      <asp:Label ID="Label_Nr" runat="server">avg. N input in residues and manure pr 5 years</asp:Label>
      <asp:DropDownList ID="DropDown_Nr" runat="server" AutoPostBack="false">
          <asp:ListItem Value="90">90 Kg/ha</asp:ListItem>
          <asp:ListItem Value="95">95 Kg/ha</asp:ListItem>
          <asp:ListItem Value="100">100 Kg/ha</asp:ListItem>
          <asp:ListItem Value="105">105 Kg/ha</asp:ListItem>
          <asp:ListItem Value="110">110 Kg/ha</asp:ListItem>
          <asp:ListItem Value="115">115 Kg/ha</asp:ListItem>
          <asp:ListItem Value="120">120 Kg/ha</asp:ListItem>
          <asp:ListItem Value="125">125 Kg/ha</asp:ListItem>
          <asp:ListItem Value="130">130 Kg/ha</asp:ListItem>
          <asp:ListItem Value="135">135 Kg/ha</asp:ListItem>
          <asp:ListItem Value="140">140 Kg/ha</asp:ListItem>
          <asp:ListItem Value="145">145 Kg/ha</asp:ListItem>
          <asp:ListItem Value="150">150 Kg/ha</asp:ListItem>
          </asp:DropDownList>&nbsp;&nbsp;&nbsp;  
      <asp:Label ID="Label_Yp" runat="server">Crop specific potential yield</asp:Label>
      <asp:DropDownList ID="DropDown_Yp" runat="server" AutoPostBack="false">
          <asp:ListItem Value="9.5">&nbsp;9.5 t/ha</asp:ListItem>
          <asp:ListItem Value="10">10.0 t/ha</asp:ListItem>
          <asp:ListItem Value="10.5">10.5 t/ha</asp:ListItem>
          <asp:ListItem Value="11">11.0 t/ha</asp:ListItem>
          <asp:ListItem Value="11.5">11.5 t/ha</asp:ListItem>
          </asp:DropDownList><br />--%>
<%--         <asp:RadioButtonList ID="RadioButtonListOption" runat="server" AutoPostBack="True" RepeatDirection="Vertical" RepeatLayout="Flow">
            <asp:ListItem Value="1">Example 1: Soil C content in 30-60 cm = 1%, avg. N input in residues and manure pr 5 years = 100 kg N/ha, potential yield = 9.5 t/ha</asp:ListItem>
            <asp:ListItem Value="2">Example 2: Soil C content in 30-60 cm = 5%, avg. N input in residues and manure pr 5 years = 150 kg N/ha, potential yield = 9.5 t/ha</asp:ListItem>
            <asp:ListItem Value="3">Example 3: Soil C content in 30-60 cm = 3%, avg. N input in residues and manure pr 5 years = 100 kg N/ha, potential yield = 10.5 t/ha</asp:ListItem>
         </asp:RadioButtonList>--%>
<%--         <asp:HyperLink ID="HyperLinkTotalAmount" runat="server" NavigateUrl="~/totalAmount.xls" Visible="False">TotalAmount</asp:HyperLink>
         <asp:HyperLink ID="HyperLinkTransport" runat="server" NavigateUrl="~/transport.xls" Visible="False">Transport</asp:HyperLink>
          <asp:Label ID="LabelResult" runat="server" Text=""></asp:Label>--%>
      </td>
   </tr>
   <tr>

<%--      <td>
         <chart:WebChartViewer ID="WebChartViewerSimpleModelChart" runat="server" />
      </td>--%>
   </tr>
</table>
<telerik:RadGrid ID="RadGrid1" runat="server" 
    BorderStyle="None" ResolvedRenderMode="Classic" Skin="Default" 
    Visible="False">
    <ClientSettings>
        <Scrolling AllowScroll="True" UseStaticHeaders="True" />
    </ClientSettings>
</telerik:RadGrid>
<br />
<telerik:RadGrid ID="RadGrid2" runat="server" 
    BorderStyle="None" ResolvedRenderMode="Classic" Skin="Default" 
    Visible="False">
    <ClientSettings>
        <Scrolling AllowScroll="True" UseStaticHeaders="True" />
    </ClientSettings>
</telerik:RadGrid>

