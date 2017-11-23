<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="CToolYieldModelChart.aspx.cs" Inherits="AgroModelToolbox.Show.CToolYieldModelChart" %>

<%@ Register TagPrefix="output" TagName="CToolYieldModel" Src="~/Controls/03_Output/ctrlCToolYieldModelChart.ascx" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title></title>
   <link href="~/Styles/AgroModelToolbox.css" rel="stylesheet" type="text/css" />
</head>
<body>
   <form id="formCToolYieldModelChart" runat="server">
   <telerik:RadScriptManager ID="RadScriptManagerCToolModelChart" runat="server" />
   <div class="page">
      <h1>test</h1>
   <output:CToolYieldModel ID="controlCToolYieldModel" runat="server" />
   </div>
   </form>
</body>
</html>
