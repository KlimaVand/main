<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="ShowResult.aspx.cs" Inherits="AnimalChange_ModelInterface.ShowResult" %>

<%@ Register assembly="Telerik.Web.UI" namespace="Telerik.Web.UI" tagprefix="telerik" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title></title>

    <script type="text/javascript" id="telerikClientEvents1">
//<![CDATA[

	function RadUpload1_Adding(sender,args)
	{
	//Add JavaScript handler code here
	}
//]]>
</script>
<style>
.animation
{
position:relative;
top: 25%;
left: 20px;
animation:mymove 10s infinite;
animation-timing-function:linear;
-webkit-animation:mymove 5s infinite;
-webkit-animation-timing-function:linear;
}
@keyframes mymove
{
from {left:20px;}
to {left:500px;}
}
@-webkit-keyframes mymove
{
from {left:20px;}
to {left:500px;}
}
</style>
</head>
<body class="data" style="background-color: #C8E1F3;">
<span id="veil" style="position: absolute; top: 0px; left: 0px; height: 100%; width: 100%; background-color: #C8E1F3; display: none;"><blink class="animation">Please wait...</blink></span>
<script language="javascript">    function veil() { document.getElementById('veil').style.display = 'block'; if (document.getElementById('dynamichide')) { document.getElementById('dynamichide').style.display = 'none'; } }</script>
    <form id="dynamichide" runat="server">
<div id="dynamichide">
    <asp:Label ID="Label1" runat="server"></asp:Label>
    </div><br />
    <asp:GridView ID="GridView1" runat="server" CellPadding="4" ForeColor="#333333" 
        BorderColor="#C8E1F3" BorderWidth="30px" Caption="FarmNBalance" 
        HorizontalAlign="Left" BackColor="#C8E1F3">
        <RowStyle BackColor="#EFF3FB" HorizontalAlign="Right" />
        <FooterStyle BackColor="#507CD1" Font-Bold="True" ForeColor="White" />
        <PagerStyle BackColor="#2461BF" ForeColor="White" HorizontalAlign="Center" />
        <SelectedRowStyle BackColor="#D1DDF1" Font-Bold="True" ForeColor="#333333" 
            HorizontalAlign="Right" />
        <HeaderStyle BackColor="#507CD1" Font-Bold="True" ForeColor="White" />
        <EditRowStyle BackColor="#2461BF" />
        <AlternatingRowStyle BackColor="White" />
    </asp:GridView>
    <asp:GridView ID="GridView2" runat="server" CellPadding="4" ForeColor="#333333" 
        BorderColor="#C8E1F3" BorderWidth="30px" Caption="DirectGHG" 
        HorizontalAlign="Left" BackColor="#C8E1F3">
        <RowStyle BackColor="#EFF3FB" HorizontalAlign="Right" />
        <FooterStyle BackColor="#507CD1" Font-Bold="True" ForeColor="White" />
        <PagerStyle BackColor="#2461BF" ForeColor="White" HorizontalAlign="Center" />
        <SelectedRowStyle BackColor="#D1DDF1" Font-Bold="True" ForeColor="#333333" />
        <HeaderStyle BackColor="#507CD1" Font-Bold="True" ForeColor="White" />
        <EditRowStyle BackColor="#2461BF" />
        <AlternatingRowStyle BackColor="White" />
    </asp:GridView>
    <asp:GridView ID="GridView3" runat="server" CellPadding="4" ForeColor="#333333" 
        BorderColor="#C8E1F3" BorderStyle="Solid" BorderWidth="30px" 
        Caption="IndirectGHG" HorizontalAlign="Left">
        <RowStyle BackColor="#EFF3FB" HorizontalAlign="Right" />
        <FooterStyle BackColor="#507CD1" Font-Bold="True" ForeColor="White" />
        <PagerStyle BackColor="#2461BF" ForeColor="White" HorizontalAlign="Center" />
        <SelectedRowStyle BackColor="#D1DDF1" Font-Bold="True" ForeColor="#333333" />
        <HeaderStyle BackColor="#507CD1" Font-Bold="True" ForeColor="White" />
        <EditRowStyle BackColor="#2461BF" />
        <AlternatingRowStyle BackColor="White" />
    </asp:GridView>
    </form>
</body>
</html>
