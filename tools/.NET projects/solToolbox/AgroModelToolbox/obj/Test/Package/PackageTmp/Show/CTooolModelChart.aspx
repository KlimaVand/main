<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="CTooolModelChart.aspx.cs" Inherits="AgroModelToolbox.Show.CTooolModelChart" %>

<%@ Register TagPrefix="output" TagName="CToolModelChart" Src="~/Controls/03_Output/ctrlCToolModelChart.ascx" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head id="Head1" runat="server">
   <title></title>
   <link href="~/Styles/AgroModelToolbox.css" rel="stylesheet" type="text/css" />
</head>
<body>
   <form id="formCToolModelChart" runat="server">
   <telerik:RadScriptManager ID="RadScriptManagerCToolModelChart" runat="server" />
   <div>
   <asp:Label ID="LabelStatus" runat="server" Font-Bold="True" ForeColor="Red" Font-Size="Large"></asp:Label>
   <output:CToolModelChart ID="controlCToolModelChart" runat="server" />
   </div>
   </form>
</body>
</html>
