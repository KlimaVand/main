<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="SelectFarm.aspx.cs" Inherits="FarmAC.Pages.SelectFarm" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title></title>
    <link rel="StyleSheet" href="../styles/FarmAC.css" type="text/css" />
</head>

<body>
    <asp:Panel ID="Panel1" runat="server" HorizontalAlign="Center">
        <form id="form1" runat="server">
            <div>
                <table align="center" border="0" class="green" style="padding: 5px;">
                    <tr>
                        <th align="left">Select farm
                        </th>
                    </tr>
                    <tr>
                        <td align="right">
                            <asp:DropDownList ID="selectFarm" runat="server"></asp:DropDownList>
                            </td>
                            <td>
                                <%--<input type="hidden" name="DeleteFarm" value="""/>
<input type="button" style="width: 120px; height: 20px; font-size: 10px" value="Delete selected farm" onClick="if (confirm('Are you sure you want to delete the farm - '+document.forms.selectFarm.selectFarm.options[document.forms.selectFarm.selectFarm.selectedIndex].text+' - ?\nAll related data will be deleted permanently.')){document.forms.selectFarm.DeleteFarm.value='Slet';submit();}"/>--%>
                                <asp:Button ID="DeleteFarm" runat="server" Text="Delete selected farm" OnClick="DeleteFarm_Click" OnClientClick="if (confirm('Are you sure you want to delete the farm ?\nAll related data will be deleted permanently.')) {};" />
                            </td>
                            <td>
                                <%--<input type="submit" name="NewFarm" style="width: 120px; height: 20px; font-size: 10px" value="Create empty farm" />--%>
                                <asp:Button ID="NewFarm" runat="server" Text="Create empty farm" OnClick="NewFarm_Click" />
                            </td>
                            <td>
                                <%--<input type="submit" name="CopyFarm" style="width: 120px; height: 20px; font-size: 10px" value="Copy selected farm" />--%>
                                <asp:Button ID="CopyFarm" runat="server" Text="Copy selected farm" />
                            </td>
                    </tr>
                    <tr>
                        <td>
                            <%--<input type="submit" name="OK" style="width: 100px; height: 20px; font-size: 10px" value="OK" />--%>
                            <asp:Button ID="OK" runat="server" Text="OK" OnClick="OK_Click" /><br /><br />
                        </td>
                    </tr>
<!--                </table>
                <br />
                <br />
                <table>-->
                    <tr>
                        <th colspan="2" align="left">Copy standard farm</th>
                    </tr>
                    <tr>
                        <td colspan="2" align="left">
                            <asp:DropDownList ID="CopyRef_Cow" runat="server" OnSelectedIndexChanged="CopyRef_Cow_SelectedIndexChanged"></asp:DropDownList>
                            <br /><br />
                        </td>
                    </tr>
                </table>
            </div>
        </form>
    </asp:Panel>
</body>
</html>
