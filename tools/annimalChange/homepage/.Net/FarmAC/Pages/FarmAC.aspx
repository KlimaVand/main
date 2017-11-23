<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="FarmAC.aspx.cs" Inherits="FarmAC.Pages.FarmAC" %>

<%@ Register TagPrefix="FarmAC" TagName="Farm" Src="~/Controls/ctrlFarm.ascx" %>
<%@ Register TagPrefix="FarmAC" TagName="CropSequence" Src="~/Controls/ctrlCropSequence.ascx" %>
<%@ Register TagPrefix="FarmAC" TagName="Ruminants" Src="~/Controls/ctrlRuminants.ascx" %>
<%@ Register TagPrefix="FarmAC" TagName="NonRuminants" Src="~/Controls/ctrlNonRuminants.ascx" %>
<%@ Register TagPrefix="FarmAC" TagName="Balance" Src="~/Controls/ctrlBalance.ascx" %>
<%@ Register TagPrefix="FarmAC" TagName="Manure" Src="~/Controls/ctrlManure.ascx" %>
<%@ Register TagPrefix="FarmAC" TagName="Yield" Src="~/Controls/ctrlYield.ascx" %>
<%@ Register TagPrefix="FarmAC" TagName="ResultC" Src="~/Controls/resultC.ascx" %>
<%@ Register TagPrefix="FarmAC" TagName="ResultN" Src="~/Controls/resultN.ascx" %>
<%@ Register TagPrefix="FarmAC" TagName="ResultGHG" Src="~/Controls/resultGHG.ascx" %>
<%@ Register TagPrefix="FarmAC" TagName="Documentation" Src="~/Controls/ctrlDocumentation.ascx" %>
<%@ Register TagPrefix="FarmAC" TagName="Parameterising" Src="~/Controls/ctrlDocumentation2.ascx" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">

    <title></title>
    <link rel="Stylesheet" href="~/Styles/FarmAC.css" />
    <style type="text/css">
        .Initial {
            display: block;
            padding: 4px 18px 4px 18px;
            float: left;
            background: url("../Images/InitialImage.png") no-repeat right top;
            color: white;
            font-weight: bold;
        }
        .InitialWithoutModel {
            display: block;
            padding: 4px 18px 4px 18px;
            float: left;
            background: url("../Images/InitialImage.png") no-repeat right top;
            color: red;
            font-weight: bold;
        }

            .Initial:hover {
                color:black;
                background: url("../Images/SelectedButton.png") no-repeat right top;
            }

        .Clicked {
            float: left;
            display: block;
            background: url("../Images/SelectedButton.png") no-repeat right top;
            padding: 4px 18px 4px 18px;
            color: Black;
            font-weight: bold;
            color: black;
        }
        .ClickedWithoutModel {
            float: left;
            display: block;
            background: url("../Images/SelectedButton.png") no-repeat right top;
            padding: 4px 18px 4px 18px;
            color: Black;
            font-weight: bold;
            color: red;
        }
        .View {
            background-color: rgb(228,231,190);
            width: 100%;
            box-shadow: 1px 1px 1px gray;

        }
    </style>
</head>
<body>
    <form id="formFarmAC" runat="server">
            <asp:ScriptManager ID="smMain" runat="server" EnablePageMethods="true" />
        <div class="page">
            <table width="95%" align="center">
                <tr>
                    <td>
                         
                   
                       <asp:Label ID="PanelHeader" runat="server">  </asp:Label>
                        
                        <br />
                        <br />
                        <br />
                        <br />
                    </td>
                </tr>
                <tr>
                    <td colspan="3">
                        <asp:Button Text="Farm" BorderStyle="None" ID="Tab1" CssClass="Initial" runat="server"
                            OnClick="Tab1_Click" />
                        <asp:Button Text="Crop sequence" BorderStyle="None" ID="Tab2" CssClass="Initial" runat="server"
                            OnClick="Tab2_Click" />
                        <asp:Button Text="Yield" BorderStyle="None" ID="Tab3" CssClass="Initial" runat="server"
                            OnClick="Tab3_Click" />
                        <asp:Button Text="Ruminants" BorderStyle="None" ID="Tab4" CssClass="Initial" runat="server"
                            OnClick="Tab4_Click" />
                        <asp:Button Text="Non Ruminants" BorderStyle="None" ID="Tab5" CssClass="Initial" runat="server"
                            OnClick="Tab5_Click" />
                        <asp:Button Text="Manure" BorderStyle="None" ID="Tab6" CssClass="Initial" runat="server"
                            OnClick="Tab6_Click" />
                        <asp:Button Text="Balance" BorderStyle="None" ID="Tab7" CssClass="Initial" runat="server"
                            OnClick="Tab7_Click" />
                        <asp:Button Text="Result N" BorderStyle="None" ID="Tab8" CssClass="Initial" runat="server"
                            OnClick="Tab8_Click" />
                        <asp:Button Text="Result C" BorderStyle="None" ID="Tab9" CssClass="Initial" runat="server"
                            OnClick="Tab9_Click" />
                        <asp:Button Text="Result GHG" BorderStyle="None" ID="Tab10" CssClass="Initial" runat="server"
                            OnClick="Tab10_Click" />
                         <asp:Button Text="Documentation" BorderStyle="None" ID="Tab11" CssClass="Initial" runat="server"
                            OnClick="Tab11_Click" />
                         <asp:Button Text="Parameterising" BorderStyle="None" ID="Tab12" CssClass="Initial" runat="server"
                            OnClick="Tab12_Click" />
                        <asp:MultiView ID="MainView" runat="server">
                            <asp:View ID="View1" runat="server">
                                <table class="View">
                                    <tr>
                                        <td>
                                            <br />
                                            <br />
                                            <h3>
                                                <FarmAC:Farm ID="controlFarm" runat="server" />
                                                    <h3>
                                                        <br />
                                                        <br />
                                                </h3>
                                                </h3>
                                        </td>
                                    </tr>
                                </table>
                            </asp:View>
                            <asp:View ID="View2" runat="server">
                                <table class="View">
                                    <tr>
                                        <td>
                                            <br />
                                            <br />
                                            <h3>
                                                <FarmAC:CropSequence ID="controlCropSequence" runat="server" />
                                                <h3>
                                                    <br />
                                                    <br />
                                                </h3>
                                            </h3>
                                        </td>
                                    </tr>
                                </table>
                            </asp:View>
                            <asp:View ID="View3" runat="server">
                                <table class="View">
                                    <tr>
                                        <td>
                                            <br />
                                            <br />
                                            <h3>
                                                <FarmAC:Yield ID="controlYield" runat="server" />
                                                <h3>
                                                    <br />
                                                    <br />
                                                </h3>
                                            </h3>
                                        </td>
                                    </tr>
                                </table>
                            </asp:View>
                            <asp:View ID="View4" runat="server">
                                <table class="View">
                                    <tr>
                                        <td>
                                            <br />
                                            <br />
                                            <h3>
                                                <FarmAC:Ruminants ID="controlRuminants" runat="server" />
                                                <h3>
                                                    <br />
                                                    <br />
                                                </h3>
                                            </h3>
                                        </td>
                                    </tr>
                                </table>
                            </asp:View>
                            <asp:View ID="View5" runat="server">
                                <table class="View">
                                    <tr>
                                        <td>
                                            <br />
                                            <br />
                                            <h3>
                                                <FarmAC:NonRuminants ID="controlNonRuminants" runat="server" />
                                                <h3>
                                                    <br />
                                                    <br />
                                                </h3>
                                            </h3>
                                        </td>
                                    </tr>
                                </table>
                            </asp:View>
                            <asp:View ID="View6" runat="server">
                                <table class="View">
                                    <tr>
                                        <td>
                                            <br />
                                            <br />
                                            <h3>
                                                <FarmAC:Manure ID="controlManure" runat="server" />
                                                <h3>
                                                    <br />
                                                    <br />
                                                </h3>
                                            </h3>
                                        </td>
                                    </tr>
                                </table>
                            </asp:View>
                            <asp:View ID="View7" runat="server">
                                <table class="View">
                                    <tr>
                                        <td>
                                            <br />
                                            <br />
                                            <h3>
                                                <FarmAC:Balance ID="controlBalance" runat="server" />
                                                <h3>
                                                    <br />
                                                    <br />
                                                </h3>
                                            </h3>
                                        </td>
                                    </tr>
                                </table>
                            </asp:View>
                            <asp:View ID="View8" runat="server">
                                <table class="View">
                                    <tr>
                                        <td>
                                            <br />
                                            <br />
                                            <h3>
                                                <FarmAC:ResultN ID="result" runat="server" />
                                                <h3>
                                                    <br />
                                                    <br />
                                                </h3>
                                            </h3>
                                        </td>
                                    </tr>
                                </table>
                            </asp:View>
                            <asp:View ID="View9" runat="server">
                                <table class="View">
                                    <tr>
                                        <td>
                                            <br />
                                            <br />
                                            <h3>
                                                <FarmAC:ResultC ID="resultC" runat="server" />
                                                <h3>
                                                    <br />
                                                    <br />
                                                </h3>
                                            </h3>
                                        </td>
                                    </tr>
                                </table>
                            </asp:View>
                            <asp:View ID="View10" runat="server">
                                <table class="View">
                                    <tr>
                                        <td>
                                            <br />
                                            <br />
                                            <h3>
                                                <FarmAC:ResultGHG ID="resultGH" runat="server" />
                                                <h3>
                                                    <br />
                                                    <br />
                                                </h3>
                                            </h3>
                                        </td>
                                    </tr>
                                </table>
                            </asp:View>
                            <asp:View ID="View11" runat="server">
                                <table class="View">
                                    <tr>
                                        <td>
                                            <br />
                                            <br />
                                            <h3>
                                                <FarmAC:Documentation ID="Documentation" runat="server" />
                                                <h3>
                                                    <br />
                                                    <br />
                                                </h3>
                                            </h3>
                                        </td>
                                    </tr>
                                </table>
                            </asp:View>
                             <asp:View ID="View12" runat="server">
                                <table class="View">
                                    <tr>
                                        <td>
                                            <br />
                                            <br />
                                            <h3>
                                                <FarmAC:Parameterising ID="Parameterising" runat="server" />
                                                <h3>
                                                    <br />
                                                    <br />
                                                </h3>
                                            </h3>
                                        </td>
                                    </tr>
                                </table>
                            </asp:View>
                        </asp:MultiView>
                    </td>
                </tr>
            </table>
            <br /><br />
            <br /><br />
        </div>
        <div class="footer">
            Web site provided by <a href="http://agro.au.dk/en/" target="_blank">Aarhus University, Faculty of Science and Technology, Department of Agroecology</a>.<br />
            Report technical problems to webmaster: <a href="mailto:jonas.vejlin@agro.au.dk">Jonas Vejlin</a> or <a href="mailto:margits.jorgensen@agro.au.dk">Margit Jørgensen</a>.
        </div>

    </form>

</body>
</html>
