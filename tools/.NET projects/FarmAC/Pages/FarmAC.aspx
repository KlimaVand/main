<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="FarmAC.aspx.cs" Inherits="FarmAC.Pages.FarmAC" %>

<%@ Register TagPrefix="farmAC" TagName="TabPage" Src="../Controls/ctrlTabPageFarmAC.ascx" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
   <title></title>
   <link rel="Stylesheet" href="~/Styles/FarmAC.css" />
</head>
<body>
   <form id="formFarmAC" runat="server">
   <telerik:RadScriptManager ID="RadScriptManagerFarmAC" runat="server" />
   <div class="page">
      <h1>Farm AC</h1>
      <farmAC:TabPage ID="controlTabPage" runat="server" />
   </div>
   <div class="footer">
      Web site provided by <a href="http://agro.au.dk/en/" target="_blank">Aarhus University, Faculty of Science and Technology, Department of Agroecology</a>.<br />
      Report technical problems to webmaster: <a href="mailto:Poul.Lassen@djf.au.dk">Poul Lassen</a>. Optimized for screen size 1024x768
   </div>
   </form>
</body>
</html>
