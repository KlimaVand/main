<%@ Page Title="" Language="C#" MasterPageFile="~/AgroModelToolbox.Master" AutoEventWireup="true" CodeBehind="CtoolYieldModelChart.aspx.cs" Inherits="AgroModelToolbox.Menu._03_Output.CtoolYieldModelChart" %>

<%@ Register TagPrefix="output" TagName="CToolYieldModel" Src="~/Controls/03_Output/ctrlCToolYieldModelChart.ascx" %>
<asp:Content ID="ContentHead" ContentPlaceHolderID="HeadContent" runat="server">
</asp:Content>
<asp:Content ID="ContentMain" ContentPlaceHolderID="MainContent" runat="server">
   <div align="center">
      <asp:Label ID="LabelStatus" runat="server" Font-Bold="True" ForeColor="Red" Font-Size="Large"></asp:Label>
   </div>
      <output:CToolYieldModel ID="controlCToolYieldModel" runat="server" />
   </asp:Content>
