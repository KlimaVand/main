<%@ Page Title="" Language="C#" MasterPageFile="~/AgroModelToolbox.Master" AutoEventWireup="true" CodeBehind="CToolModelChart.aspx.cs" Inherits="AgroModelToolbox.Menu._03_Output.CtoolModelChart" %>

<%@ Register TagPrefix="output" TagName="CToolModelChart" Src="~/Controls/03_Output/ctrlCToolModelChart.ascx" %>
<asp:Content ID="ContentHead" ContentPlaceHolderID="HeadContent" runat="server">
</asp:Content>
<asp:Content ID="ContentMain" ContentPlaceHolderID="MainContent" runat="server">
   <div align="center">
      <asp:Label ID="LabelStatus" runat="server" Font-Bold="True" ForeColor="Red" Font-Size="Large"></asp:Label>
      <output:CToolModelChart ID="controlCToolModelChart" runat="server" />
   </div>
</asp:Content>
