<%@ Page Title="" Language="C#" MasterPageFile="~/AgroModelToolbox.Master" AutoEventWireup="true" CodeBehind="SimpleModelChart.aspx.cs" Inherits="AgroModelToolbox.Menu._03_Output.SimpleModelChart" %>

<%@ Register TagPrefix="output" TagName="SimpleModelChart" Src="~/Controls/03_Output/ctrlSimpleModelChart.ascx" %>
<asp:Content ID="ContentHead" ContentPlaceHolderID="HeadContent" runat="server">
</asp:Content>
<asp:Content ID="ContentMain" ContentPlaceHolderID="MainContent" runat="server">
   <div>
      <asp:Label ID="LabelStatus" runat="server" Font-Bold="True" ForeColor="Red" Font-Size="Large"></asp:Label>
      <output:SimpleModelChart ID="controlSimpleModelChart" runat="server" />
   </div>
</asp:Content>
