<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="SimpleModelChart.aspx.cs" Inherits="AgroModelToolbox.Show.SimpleModelChart" %>

<%@ Register TagPrefix="output" TagName="SimpleModelChart" Src="~/Controls/03_Output/ctrlSimpleModelChart.ascx" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
   <title></title>
   <link href="~/Styles/AgroModelToolbox.css" rel="stylesheet" type="text/css" />
</head>
<body>
   <form id="formSimpleModelChart" runat="server">
   <telerik:RadScriptManager ID="RadScriptManagerSimpleModelChart" runat="server" />
   <div>
   <asp:Label ID="LabelStatus" runat="server" Font-Bold="True" ForeColor="Red" Font-Size="Large"></asp:Label>
   <output:SimpleModelChart ID="controlSimpleModelChart" runat="server" />
   </div>
   </form>
</body>
</html>
