<%@ Control Language="C#" AutoEventWireup="true" CodeBehind="ctrlInputData.ascx.cs" Inherits="AgroModelToolbox.Controls._03_Output.ctrlInputData" %>
<%@ Register Assembly="netchartdir" Namespace="ChartDirector" TagPrefix="chart" %>
<asp:Panel runat="server" Height="660px">
      <asp:Label ID="LabelMessage" runat="server" Font-Bold="True" ForeColor="Red" Font-Size="Large"></asp:Label>
<asp:imagebutton ID="ImagebuttonMap" runat="server" 
    ImageUrl="~/Images/GridOgPunkter.jpg" 
    style="position: relative; top:0px; float: left;" 
    onclick="ImagebuttonMap_Click"></asp:imagebutton>
    <asp:HiddenField ID="HiddenField_Grid_Code" runat="server" Value="121113" />

<asp:Panel ID="ChartHolder" style="position: relative; top:0px; float: right;" Visible="False" runat="server">
      <chart:WebChartViewer ID="WebChartViewerCToolModelChart_1" runat="server" />
<br />
    <chart:WebChartViewer ID="WebChartViewerSimpleModelChart" runat="server" />
</asp:Panel>
<asp:PlaceHolder ID="PlaceHolderInput" runat="server" Visible="False">
<asp:Table ID="Table1" runat="server" style="position: relative; left: 0px; top: 0px; float: left;">
              <asp:TableRow ID="TableRow1" runat="server" Font-Bold="True">
                  <asp:TableCell ID="TableCell1" runat="server" BorderStyle="Solid" BorderWidth="1px" BackColor="Gray" ForeColor="White">Variable</asp:TableCell>
                  <asp:TableCell ID="TableCell2" runat="server" BorderStyle="Solid" BorderWidth="1px" BackColor="Red" ForeColor="White" Width="100px">Typical farm<br />for region</asp:TableCell>
                  <asp:TableCell ID="TableCell3" runat="server" BorderStyle="Solid" BorderWidth="1px" BackColor="Blue" ForeColor="White" Width="100px">Experiment farm</asp:TableCell>
              </asp:TableRow>
              <asp:TableRow ID="TableRow12" runat="server">
                  <asp:TableCell ID="TableCell31" runat="server" BorderStyle="Solid" BorderWidth="1px">Tillage</asp:TableCell>
                  <asp:TableCell ID="TableCell32" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_RTlogic" runat="server" AutoPostBack="false">
          <asp:ListItem Value="0">Conventional</asp:ListItem>
          <asp:ListItem Selected="True" Value="1">Reduced</asp:ListItem>
          <asp:ListItem Value="2">No tillage</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
                  <asp:TableCell ID="TableCell33" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_RTlogic_2" runat="server" AutoPostBack="false">
          <asp:ListItem Value="0">Conventional</asp:ListItem>
          <asp:ListItem Selected="True" Value="1">Reduced</asp:ListItem>
          <asp:ListItem Value="2">No tillage</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
              </asp:TableRow>
              <asp:TableRow ID="TableRow2" runat="server">
                  <asp:TableCell ID="TableCell4" runat="server" BorderStyle="Solid" BorderWidth="1px">Initial C(t/ha)</asp:TableCell>
                  <asp:TableCell ID="TableCell5" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:TextBox ID="TextBox_InitC" runat="server" CausesValidation="True" MaxLength="3" Width="100" Text="38">38</asp:TextBox></asp:TableCell>
                  <asp:TableCell ID="TableCell6" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:TextBox ID="TextBox_InitC_2" runat="server" CausesValidation="True" MaxLength="3" Width="100" Text="134">134</asp:TextBox></asp:TableCell>
              </asp:TableRow>
</asp:Table>
<br /><asp:Button ID="Button2" runat="server" Text="OK" style="position: relative; float: left;"></asp:Button>
</asp:PlaceHolder>
      <asp:SqlDataSource ID="SqlDataSource1" runat="server"></asp:SqlDataSource>
</asp:Panel>

