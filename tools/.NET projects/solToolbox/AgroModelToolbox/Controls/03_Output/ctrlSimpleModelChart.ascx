<%@ Control Language="C#" AutoEventWireup="true" CodeBehind="ctrlSimpleModelChart.ascx.cs" Inherits="AgroModelToolbox.Controls._03_Output.ctrlSimpleModelChart" %>
<%@ Register Assembly="netchartdir" Namespace="ChartDirector" TagPrefix="chart" %>
<style type="text/css">
    .style1
    {
        text-align: center;
    }
</style>
<asp:Label ID="LabelMessage" runat="server" Font-Bold="true" ForeColor="Red"></asp:Label>

          <asp:Table ID="Table1" runat="server" style="position: relative; left: 0px; top: 20px; float: left;">
              <asp:TableRow runat="server" Font-Bold="True">
                  <asp:TableCell runat="server" BorderStyle="Solid" BorderWidth="1px" BackColor="Gray" ForeColor="White">Variable</asp:TableCell>
                  <asp:TableCell runat="server" BorderStyle="Solid" BorderWidth="1px" BackColor="Red" ForeColor="White" Width="100px">Value red scenario</asp:TableCell>
                  <asp:TableCell runat="server" BorderStyle="Solid" BorderWidth="1px" BackColor="Blue" ForeColor="White" Width="100px">Value blue scenario</asp:TableCell>
              </asp:TableRow>
              <asp:TableRow runat="server">
                  <asp:TableCell runat="server" BorderStyle="Solid" BorderWidth="1px">Soil C content in top 25 cm</asp:TableCell>
                  <asp:TableCell runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="Dropdown_C30_2" runat="server" AutoPostBack="false">
          <asp:ListItem Value="1">1 %</asp:ListItem>
          <asp:ListItem Value="3">3 %</asp:ListItem>
          <asp:ListItem Value="5">5 %</asp:ListItem>
          <asp:ListItem Value="7">7 %</asp:ListItem>
          <asp:ListItem Value="9">9 %</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
                  <asp:TableCell runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="Dropdown_C30" runat="server" AutoPostBack="false">
          <asp:ListItem Value="1">1 %</asp:ListItem>
          <asp:ListItem Value="3" Selected="True">3 %</asp:ListItem>
          <asp:ListItem Value="5">5 %</asp:ListItem>
          <asp:ListItem Value="7">7 %</asp:ListItem>
          <asp:ListItem Value="9">9 %</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
              </asp:TableRow>
              <asp:TableRow ID="TableRow7" runat="server">
                  <asp:TableCell ID="TableCell16" runat="server" BorderStyle="Solid" BorderWidth="1px">Soil C/N ratio in top 25 cm</asp:TableCell>
                  <asp:TableCell ID="TableCell17" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_CN30_2" runat="server" AutoPostBack="false">
          <asp:ListItem Value="9">9 </asp:ListItem>
          <asp:ListItem Value="12" Selected="True">12 </asp:ListItem>
          <asp:ListItem Value="15">15 </asp:ListItem>
          <asp:ListItem Value="18">18 </asp:ListItem>
          <asp:ListItem Value="21">21 </asp:ListItem>
          </asp:DropDownList></asp:TableCell>
                  <asp:TableCell ID="TableCell18" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_CN30" runat="server" AutoPostBack="false">
          <asp:ListItem Value="9">9 </asp:ListItem>
          <asp:ListItem Value="12" Selected="True">12 </asp:ListItem>
          <asp:ListItem Value="15">15 </asp:ListItem>
          <asp:ListItem Value="18">18 </asp:ListItem>
          <asp:ListItem Value="21">21 </asp:ListItem>
          </asp:DropDownList></asp:TableCell>
              </asp:TableRow>
              <asp:TableRow ID="TableRow2" runat="server">
                  <asp:TableCell ID="TableCell4" runat="server" BorderStyle="Solid" BorderWidth="1px">Soil C content in 25-50 cm</asp:TableCell>
                  <asp:TableCell ID="TableCell5" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="Dropdown_C3060_2" runat="server" AutoPostBack="false">
          <asp:ListItem Value="1">1 %</asp:ListItem>
          <asp:ListItem Value="3">3 %</asp:ListItem>
          <asp:ListItem Value="5">5 %</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
                  <asp:TableCell ID="TableCell6" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="Dropdown_C3060" runat="server" AutoPostBack="false">
          <asp:ListItem Value="1">1 %</asp:ListItem>
          <asp:ListItem Value="3">3 %</asp:ListItem>
          <asp:ListItem Value="5">5 %</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
              </asp:TableRow>
              <asp:TableRow ID="TableRow8" runat="server">
                  <asp:TableCell ID="TableCell19" runat="server" BorderStyle="Solid" BorderWidth="1px">Soil C/N ratio in 25-50 cm</asp:TableCell>
                  <asp:TableCell ID="TableCell20" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_CN3060_2" runat="server" AutoPostBack="false">
          <asp:ListItem Value="9">9 </asp:ListItem>
          <asp:ListItem Value="12" Selected="True">12 </asp:ListItem>
          <asp:ListItem Value="15">15 </asp:ListItem>
          <asp:ListItem Value="18">18 </asp:ListItem>
          <asp:ListItem Value="21">21 </asp:ListItem>
          </asp:DropDownList></asp:TableCell>
                  <asp:TableCell ID="TableCell21" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_CN3060" runat="server" AutoPostBack="false">
          <asp:ListItem Value="9">9 </asp:ListItem>
          <asp:ListItem Value="12" Selected="True">12 </asp:ListItem>
          <asp:ListItem Value="15">15 </asp:ListItem>
          <asp:ListItem Value="18">18 </asp:ListItem>
          <asp:ListItem Value="21">21 </asp:ListItem>
          </asp:DropDownList></asp:TableCell>
              </asp:TableRow>
              <asp:TableRow ID="TableRow11" runat="server">
                  <asp:TableCell ID="TableCell28" runat="server" BorderStyle="Solid" BorderWidth="1px">Share of N covered by manure</asp:TableCell>
                  <asp:TableCell ID="TableCell29" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_Norg_share_2" runat="server" AutoPostBack="false">
          <asp:ListItem>0</asp:ListItem>
          <asp:ListItem Selected="True">0.1 </asp:ListItem>
          <asp:ListItem>0.2</asp:ListItem>
          <asp:ListItem>0.3</asp:ListItem>
          <asp:ListItem>0.4</asp:ListItem>
          <asp:ListItem>0.5</asp:ListItem>
          <asp:ListItem>0.6</asp:ListItem>
          <asp:ListItem>0.7</asp:ListItem>
          <asp:ListItem>0.8</asp:ListItem>
          <asp:ListItem>0.9</asp:ListItem>
          <asp:ListItem>1</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
                  <asp:TableCell ID="TableCell30" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_Norg_share" runat="server" AutoPostBack="false">
          <asp:ListItem>0</asp:ListItem>
          <asp:ListItem Selected="True">0.1 </asp:ListItem>
          <asp:ListItem>0.2</asp:ListItem>
          <asp:ListItem>0.3</asp:ListItem>
          <asp:ListItem>0.4</asp:ListItem>
          <asp:ListItem>0.5</asp:ListItem>
          <asp:ListItem>0.6</asp:ListItem>
          <asp:ListItem>0.7</asp:ListItem>
          <asp:ListItem>0.8</asp:ListItem>
          <asp:ListItem>0.9</asp:ListItem>
          <asp:ListItem>1</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
              </asp:TableRow>
              <asp:TableRow runat="server">
                  <asp:TableCell runat="server" Width="160px" BorderStyle="Solid" BorderWidth="1px">avg. N input in residues and manure pr 5 years</asp:TableCell>
                  <asp:TableCell runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_Nr_2" runat="server" AutoPostBack="false">
          <asp:ListItem Value="0">0 kg N/ha</asp:ListItem>
          <asp:ListItem Value="50">50 kg N/ha</asp:ListItem>
          <asp:ListItem Value="100">100 kg N/ha</asp:ListItem>
          <asp:ListItem Value="150">150 kg N/ha</asp:ListItem>
          <asp:ListItem Value="200">200 kg N/ha</asp:ListItem>
          <asp:ListItem Value="250">250 kg N/ha</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
                  <asp:TableCell runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_Nr" runat="server" AutoPostBack="false">
          <asp:ListItem Value="0">0 kg N/ha</asp:ListItem>
          <asp:ListItem Value="50">50 kg N/ha</asp:ListItem>
          <asp:ListItem Value="100" Selected="True">100 kg N/ha</asp:ListItem>
          <asp:ListItem Value="150">150 kg N/ha</asp:ListItem>
          <asp:ListItem Value="200">200 kg N/ha</asp:ListItem>
          <asp:ListItem Value="250">250 kg N/ha</asp:ListItem>
          </asp:DropDownList ></asp:TableCell>
              </asp:TableRow>
              <asp:TableRow runat="server">
                  <asp:TableCell runat="server" BorderStyle="Solid" BorderWidth="1px">N input in green manure prior to crop</asp:TableCell>
                  <asp:TableCell runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_Nc_2" runat="server" AutoPostBack="false">
          <asp:ListItem Value="0">0 kg N/ha</asp:ListItem>
          <asp:ListItem Value="50">50 kg N/ha</asp:ListItem>
          <asp:ListItem Value="100">100 kg N/ha</asp:ListItem>
          <asp:ListItem Value="150">150 kg N/ha</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
                  <asp:TableCell runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_Nc" runat="server" AutoPostBack="false">
          <asp:ListItem Value="0">0 kg N/ha</asp:ListItem>
          <asp:ListItem Value="50" Selected="True">50 kg N/ha</asp:ListItem>
          <asp:ListItem Value="100">100 kg N/ha</asp:ListItem>
          <asp:ListItem Value="150">150 kg N/ha</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
              </asp:TableRow>
              <asp:TableRow ID="TableRow1" runat="server">
                  <asp:TableCell ID="TableCell1" runat="server" BorderStyle="Solid" BorderWidth="1px">Crop specific potential yield</asp:TableCell>
                  <asp:TableCell ID="TableCell2" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_Yp_2" runat="server" AutoPostBack="false">
          <asp:ListItem Value="8">8 t/ha</asp:ListItem>
          <asp:ListItem Value="9" Selected="True">9 t/ha</asp:ListItem>
          <asp:ListItem Value="10">10 t/ha</asp:ListItem>
          <asp:ListItem Value="11">11 t/ha</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
                  <asp:TableCell ID="TableCell3" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_Yp" runat="server" AutoPostBack="false">
          <asp:ListItem Value="8">8 t/ha</asp:ListItem>
          <asp:ListItem Value="9">9 t/ha</asp:ListItem>
          <asp:ListItem Value="10" Selected="True">10 t/ha</asp:ListItem>
          <asp:ListItem Value="11">11 t/ha</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
              </asp:TableRow>
              <asp:TableRow ID="TableRow4" runat="server">
                  <asp:TableCell ID="TableCell7" runat="server" BorderStyle="Solid" BorderWidth="1px">N Recovery Efficiency (mineral fertilizer)</asp:TableCell>
                  <asp:TableCell ID="TableCell8" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_NRE_2" runat="server" AutoPostBack="false">
          <asp:ListItem Value="0.5">0.5 kg N/kg N</asp:ListItem>
          <asp:ListItem Value="0.6">0.6 kg N/kg N</asp:ListItem>
          <asp:ListItem Value="0.7" Selected="True">0.7 kg N/kg N</asp:ListItem>
          <asp:ListItem Value="0.8">0.8 kg N/kg N</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
                  <asp:TableCell ID="TableCell9" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_NRE" runat="server" AutoPostBack="false">
          <asp:ListItem Value="0.5">0.5 kg N/kg N</asp:ListItem>
          <asp:ListItem Value="0.6">0.6 kg N/kg N</asp:ListItem>
          <asp:ListItem Value="0.7" Selected="True">0.7 kg N/kg N</asp:ListItem>
          <asp:ListItem Value="0.8">0.8 kg N/kg N</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
              </asp:TableRow>
              <asp:TableRow ID="TableRow5" runat="server">
                  <asp:TableCell ID="TableCell10" runat="server" BorderStyle="Solid" BorderWidth="1px">N Replacement rate (manure)</asp:TableCell>
                  <asp:TableCell ID="TableCell11" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_NRR_2" runat="server" AutoPostBack="false">
          <asp:ListItem Value="0.5">0.5 kg N/kg N</asp:ListItem>
          <asp:ListItem Value="0.6">0.6 kg N/kg N</asp:ListItem>
          <asp:ListItem Value="0.7">0.7 kg N/kg N</asp:ListItem>
          <asp:ListItem Value="0.8" Selected="True">0.8 kg N/kg N</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
                  <asp:TableCell ID="TableCell12" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_NRR" runat="server" AutoPostBack="false">
          <asp:ListItem Value="0.5">0.5 kg N/kg N</asp:ListItem>
          <asp:ListItem Value="0.6">0.6 kg N/kg N</asp:ListItem>
          <asp:ListItem Value="0.7">0.7 kg N/kg N</asp:ListItem>
          <asp:ListItem Value="0.8" Selected="True">0.8 kg N/kg N</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
              </asp:TableRow>
              <asp:TableRow ID="TableRow6" runat="server">
                  <asp:TableCell ID="TableCell13" runat="server" BorderStyle="Solid" BorderWidth="1px">N percent in grain</asp:TableCell>
                  <asp:TableCell ID="TableCell14" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_NPCT_2" runat="server" AutoPostBack="false">
          <asp:ListItem Value="1.0">1.0 %</asp:ListItem>
          <asp:ListItem Value="1.5" Selected="True">1.5 %</asp:ListItem>
          <asp:ListItem Value="2.0">2.0 %</asp:ListItem>
          <asp:ListItem Value="2.5">2.5 %</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
                  <asp:TableCell ID="TableCell15" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_NPCT" runat="server" AutoPostBack="false">
          <asp:ListItem Value="1.0">1.0 %</asp:ListItem>
          <asp:ListItem Value="1.5" Selected="True">1.5 %</asp:ListItem>
          <asp:ListItem Value="2.0">2.0 %</asp:ListItem>
          <asp:ListItem Value="2.5">2.5 %</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
              </asp:TableRow>
              <asp:TableRow ID="TableRow9" runat="server">
                  <asp:TableCell ID="TableCell22" runat="server" BorderStyle="Solid" BorderWidth="1px">Potential ET during growing season</asp:TableCell>
                  <asp:TableCell ID="TableCell23" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_EP_season_2" runat="server" AutoPostBack="false">
          <asp:ListItem Value="200">200 mm</asp:ListItem>
          <asp:ListItem Value="300" Selected="True">300 mm</asp:ListItem>
          <asp:ListItem Value="400">400 mm</asp:ListItem>
          <asp:ListItem Value="500">500 mm</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
                  <asp:TableCell ID="TableCell24" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_EP_season" runat="server" AutoPostBack="false">
          <asp:ListItem Value="200">200 mm</asp:ListItem>
          <asp:ListItem Value="300" Selected="True">300 mm</asp:ListItem>
          <asp:ListItem Value="400">400 mm</asp:ListItem>
          <asp:ListItem Value="500">500 mm</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
              </asp:TableRow>
              <asp:TableRow ID="TableRow10" runat="server">
                  <asp:TableCell ID="TableCell25" runat="server" BorderStyle="Solid" BorderWidth="1px">Precipitation during growing season</asp:TableCell>
                  <asp:TableCell ID="TableCell26" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_P_season_2" runat="server" AutoPostBack="false">
          <asp:ListItem Value="100">100 mm</asp:ListItem>
          <asp:ListItem Value="200" Selected="True">200 mm</asp:ListItem>
          <asp:ListItem Value="300">300 mm</asp:ListItem>
          <asp:ListItem Value="400">400 mm</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
                  <asp:TableCell ID="TableCell27" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_P_season" runat="server" AutoPostBack="false">
          <asp:ListItem Value="100">100 mm</asp:ListItem>
          <asp:ListItem Value="200" Selected="True">200 mm</asp:ListItem>
          <asp:ListItem Value="300">300 mm</asp:ListItem>
          <asp:ListItem Value="400">400 mm</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
              </asp:TableRow>
              <asp:TableRow ID="TableRow12" runat="server">
                  <asp:TableCell ID="TableCell31" runat="server" BorderStyle="Solid" BorderWidth="1px">Tillage</asp:TableCell>
                  <asp:TableCell ID="TableCell32" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_RTlogic_2" runat="server" AutoPostBack="false">
          <asp:ListItem Value="0">Conventional</asp:ListItem>
          <asp:ListItem Selected="True" Value="1">Reduced</asp:ListItem>
          <asp:ListItem Value="2">No tillage</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
                  <asp:TableCell ID="TableCell33" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:DropDownList ID="DropDown_RTlogic" runat="server" AutoPostBack="false">
          <asp:ListItem Value="0">Conventional</asp:ListItem>
          <asp:ListItem Selected="True" Value="1">Reduced</asp:ListItem>
          <asp:ListItem Value="2">No tillage</asp:ListItem>
          </asp:DropDownList></asp:TableCell>
              </asp:TableRow>
              <asp:TableRow ID="TableRow13" runat="server">
                  <asp:TableCell ID="TableCell34" runat="server" BorderStyle="Solid" BorderWidth="1px">Tillage depth [cm]</asp:TableCell>
                  <asp:TableCell ID="TableCell35" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:TextBox ID="TextBox_D_2" runat="server" CausesValidation="True" MaxLength="3" Width="100">1</asp:TextBox></asp:TableCell>
                  <asp:TableCell ID="TableCell36" runat="server" BorderStyle="Solid" BorderWidth="1px"><asp:TextBox ID="TextBox_D" runat="server" CausesValidation="True" MaxLength="3" Width="100">1</asp:TextBox></asp:TableCell>
              </asp:TableRow>


              <asp:TableRow ID="TableRow3" runat="server">
                  <asp:TableCell >&nbsp;</asp:TableCell>
                  <asp:TableCell >&nbsp;</asp:TableCell>
                  <asp:TableCell >&nbsp;</asp:TableCell>
              </asp:TableRow>


          </asp:Table>
<div style="position: relative; top:20px; float: right;"><chart:WebChartViewer ID="WebChartViewerSimpleModelChart" runat="server" /><br />
<asp:Button ID="Button2" runat="server" Text="OK" style="position: relative; top:24px;"></asp:Button><br /></div>

