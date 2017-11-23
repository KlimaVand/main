<%@ Control Language="C#" AutoEventWireup="true" CodeBehind="ctrlFarm.ascx.cs" Inherits="FarmAC.Controls.ctrlFarm" %>

<asp:Panel ID="Panel1" runat="server">
    <style>
        .animation {
            position: relative;
            top: 25%;
            left: 20px;
            animation: mymove 10s infinite;
            animation-timing-function: linear;
            -webkit-animation: mymove 5s infinite;
            -webkit-animation-timing-function: linear;
        }

        @keyframes mymove {
            from {
                left: 20px;
            }

            to {
                left: 500px;
            }
        }

        @-webkit-keyframes mymove {
            from {
                left: 20px;
            }

            to {
                left: 500px;
            }
        }
    </style>
    <!--<script type="text/javascript"> var FarmNPopupWin = ''</script>
<script type="text/javascript" src="scripts/formvalidation.js"></script>
<span id="veil" style="position: absolute; top: 0px; left: 0px; height: 100%; width: 100%; background-color: #C8E1F3; display: none;"><blink class="animation">Please wait...</blink></span>
<script type="text/javascript">function veil() { document.getElementById('veil').style.display = 'block'; if (document.getElementById('dynamichide')) { document.getElementById('dynamichide').style.display = 'none'; } }</script>-->
    <table class="data" border="0" style="float: left;">
        <tr>
            <th align="left">Farmer</th>
            <th align="left">&nbsp;</th>
            <td rowspan="4">
                <h5>If local parameterfiles are present, the model will use these.<br />
                    For upload, the files can contain only one or two agroecological zones:<br />
                    If they are too big, they will not be uploaded.<br />
                    The uploaded files have to be named the same way as the default files.<br />
                    When local parameterfiles are removed. The model will use default-files<br />
                    <br />
                </h5>
                <!--<form method="post" enctype="multipart/form-data" action="uploadownparameters.asp">-->
                <table border="0">
                    <tr>
                        <td>
                            <asp:Label ID="filePresent" runat="server" Text="No custom files uploaded" />
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <asp:FileUpload ID="FileUpload1" runat="server" />
                            <asp:Button ID="UploadFile" runat="server" Text="Upload File" OnClick="UploadFileEvent" />
                            <asp:Button ID="RemoveOldFile" runat="server" Text="Remove Old Files" OnClick="RemoveOldFileEvent" />
                        </td>
                    </tr>
                </table>
                <!--</form>-->
                <br />
                <!--<form action="ClearLocalFiles.asp" method="post">
</form>-->
                <!--<script language="javascript">window.parent.frames[0].location.reload()</script>-->
                <%--<form name="formFarm" action="FarmUpdate.asp" method="post" id="dynamichide" onsubmit="">--%>
                <input type="hidden" name="DeleteBoughtManureID" id="DeleteBoughtManureID" value="" />
                <input type="hidden" name="DeleteSoldManureID" id="DeleteSoldManureID" value="" />
                <input type="hidden" name="aDeleteScenarioID" id="aDeleteScenarioID" value="" />
            </td>
        </tr>
        <tr>
            <th class="data" align="left">Name</th>
            <td class="data" align="left">
                <%--          <input type="text" name="inputName" value="Mrs Farmer (AnimalChange) on Production" maxlength="180" size="50" onChange="this.style.color='#ff0000';document.forms.update.saveUpdates.value='true';"/>--%>
                <asp:TextBox ID="inputName" runat="server" OnTextChanged="inputName_TextChanged"></asp:TextBox>
            </td>
        </tr>
        <tr>
            <th class="data" align="left">Address</th>
            <td class="data" align="left">
                <%--          <textarea rows="3" cols="38" name="inputAddress" onChange="this.style.color='#ff0000';document.forms.update.saveUpdates.value='true';"></textarea>--%>
                <asp:TextBox ID="inputAddress" runat="server" TextMode="MultiLine" OnTextChanged="inputAddress_TextChanged"></asp:TextBox>

            </td>
        </tr>
        <tr>
            <th class="data" align="left">Description</th>
            <td>
                <%--<textarea rows="10" cols="38" name="inputDescription" value="" onChange="this.style.color='#ff0000';document.forms.update.saveUpdates.value='true';"/></textarea>--%>
                <asp:TextBox ID="inputDescription" runat="server" TextMode="MultiLine" OnTextChanged="inputAddress_TextChanged" Height="226px" Width="166px"></asp:TextBox>
            </td>
        </tr>
    </table>
    <br />
    <table class="data" border="0" cellpadding="5" style="clear: both;">
        <tr>
            <th align="left">Farm type</th>
            <th align="left">Agro ecological zone</th>
            <th align="left">Atmospheric N deposition</th>
        </tr>
        <tr>
            <td class="data" align="left">
                <%--<input type="hidden" name="selectSoilType" value="1"/>--%>
                <asp:HiddenField ID="selectSoilType" runat="server" />
                <%--<input type = "hidden" name="selectYieldLevel" value="100" />--%>
                <asp:HiddenField ID="selectYieldLevel" runat="server" />
                <%--<select name="selectFarmType" onchange="this.style.color='#ff0000';document.forms.update.saveUpdates.value='true';"><option value="1" selected>Arable</option><option value="2">Pig</option><option value="3">Cattle</option><option value="4">Arable  intensive</option><option value="5">Arable  extensive</option><option value="6">Pig  intensive</option><option value="7">Pig  extensive</option><option value="8">Cattle  intensive</option><option value="9">Cattle  extensive</option></select>--%>
                <asp:DropDownList ID="selectFarmType" runat="server"></asp:DropDownList>
            </td>
            <td class="data" align="right">
                <%--          <select name="selectAgroZone" onChange="this.style.color='#ff0000';document.forms.update.saveUpdates.value='true';"><option value="1">Maritime (European land-based)</option><option value="2" selected>Continental (European land-based)</option><option value="4">Mediterranean (European land-based)</option><option value="7">South Africa (Semi arid) Kalahari</option><option value="8">Brazil Campos (Sub-humid)</option><option value="9">Humid (non-European land-based)</option><option value="11">Humid Equatorial(Amazonia)</option><option value="12">West Africa -Senegal</option><option value="13"></option></select>--%>
                <asp:DropDownList ID="selectAgroZone" runat="server" OnSelectedIndexChanged="selectAgroZone_SelectedIndexChanged"></asp:DropDownList>
            </td>
            <td>
                <%--<input type="text" name="atmosphericDeposition" value="15.6" size="5" onChange="this.style.color='#ff0000';document.forms.update.saveUpdates.value='true';"/>--%>
                <asp:TextBox ID="atmosphericDeposition" runat="server" OnTextChanged="atmosphericDeposition_TextChanged"></asp:TextBox>
                &nbsp;&nbsp;Kg N/ha/year
            </td>
        </tr>
        <tr>
            <td colspan="10">
                <hr />
            </td>
        </tr>
        <tr>
            <th align="left">Scenario</th>
            <th align="left"></th>
        </tr>
        <tr>
            <td class="data" align="left">
                <%--<select name="selectScenario" onChange="document.forms.formFarm.action='FarmSelectedScenarioUpdate.asp';document.forms.formFarm.submit();"><option value="1">sc 1</option><option value="2">Scenario 2</option><option value="3" selected>Scenario 3 Baseline</option><option value="4">Copy of Scenario 2</option><option value="5">Scenario 5</option>       </select>--%>
                <asp:DropDownList ID="selectScenario" runat="server"></asp:DropDownList>
                &nbsp;<%--<button style="width: 0px; height: 0px; font-size: 0px; border: 0px;" onClick="javascript:veil();document.forms.formFarm.action='FarmUpdate.asp';document.forms.formFarm.submit();"></button><button style="height: 20px; font-size: 10px" onClick="FarmNPopupWin=window.open('FarmScenarioNameUpdate.asp','FarmNPopupWin','toolbar=0,scrollbars=no,left=50,top=385,width=300,height=80');">Rename</button>--%><asp:Button ID="Button_Rename" runat="server" Text="Rename" OnClick="RenameEvent" /><asp:Button ID="OK" runat="server" Text="OK" OnClick="OKEvent" /><asp:TextBox ID="NewName" runat="server" Text="NewName" /><asp:Label ID="TextArea1" TextMode="multiline" Columns="50" Rows="5" runat="server" Height="50px" Width="800px" />
            </td>
            <td class="data"></td>
        </tr>
    </table>
    <table>
        <tr>
            <th align="left">Bought manure</th>
            <th align="left"></th>
            <th align="left">&nbsp;</th>
        </tr>
        <tr>
            <td>
                <table>
                    <tr>
                        <td>
                            <%-- <button style="width: 40px; height: 20px; font-size: 10px" onClick="javascript:veil();document.forms.formFarm.action='FarmBoughtManureNew.asp';document.forms.formFarm.submit();">New</button>--%>
                            <asp:Button ID="Button_New" runat="server" Text="New" OnClick="NewManure" />
                        </td>
                    </tr>
                    <%--<input type="hidden" name="nBM" id="nBM" value="-1" />--%>
                    <asp:HiddenField ID="nBM" runat="server" />
                    <asp:Panel ID="Panel2" runat="server"></asp:Panel>
                </table>
            </td>
            <td></td>
            <td>
                <%--<input type="hidden" name="nSM" id="nSM" value="-1" />--%>
                <asp:HiddenField ID="nSM" runat="server" />
    </table>
    <br />
    <table class="data" border="0">
        <tr>
            <td class="data" align="left">
                <%--<button style="width: 120px; height: 20px; font-size: 10px" onClick="veil();document.forms.formFarm.action='FarmScenarioNew.asp'" >New scenario</button>--%>
                <asp:Button ID="Button_NewScenario" runat="server" Text="New scenario" OnClick="Button_NewScenario_Click" />
            </td>
            <td class="data" align="left">
                <%--<input type="hidden" name="CopyScenario" value="" />--%>
                <asp:HiddenField ID="CopyScenario" runat="server" />
                <%--       <button style="width: 120px;height: 20px; font-size: 10px" onClick="javascript:veil();with(document.forms.formFarm){action='FarmScenarioNew.asp';CopyScenario.value='CopyScenario';submit();}">Copy Scenario</button>--%>
                <asp:Button ID="Button_CopyScenario" runat="server" Text="Copy Scenario" OnClick="Button_CopyScenario_Click" />
            </td>
            <td class="data" align="left">
                <%--          <button style="width: 120px; height: 20px; font-size: 10px" onClick="veil();document.forms.formFarm.action='FarmScenarioDelete.asp';document.forms.formFarm.submit();">Delete scenario</button>--%>
                <asp:Button ID="Button_DeleteScenario" runat="server" Text="Delete scenario" OnClick="Button_DeleteScenario_Click" />
            </td>
        </tr>
        <tr>
            <td colspan="10">
                <hr />
            </td>
        </tr>
        <tr>
            <td class="data" align="left">
                <%--          <button style="width: 120px; height: 20px; font-size: 10px" onClick="javascript:veil();document.forms.formFarm.action='FarmUpdate.asp';document.forms.formFarm.submit();">Save</button>--%>
                <asp:Button ID="Button_Save" runat="server" Text="Save" />
            </td>
        </tr>
        <tr>
            <td colspan="3" class="data" align="left">
                <table>
                    <tr>
                        <td class="data" align="left">
                            <%--          <button name="adaptation" value="1" style="width: 150px; height: 20px; font-size: 10px" onClick="javascript:veil();document.forms.formFarm.action='RunModelService.asp?version=1';document.forms.formFarm.submit();">Run model Adaptation</button>--%>
                            <asp:Button ID="adaptation" runat="server" Text="Run model Adaptation" OnClick="adaptation_Click" />
                            <%--          <button name="projection" value="1" style="width: 150px; height: 20px; font-size: 10px" onClick="javascript:veil();document.forms.formFarm.action='RunModelService.asp?version=1';document.forms.formFarm.submit();">Run model Projection</button>--%>
                            <asp:Button ID="projection" runat="server" Text="Run model Projection" OnClick="Projection_Click" />
                            <%--          <button style="width: 120px; height: 20px; font-size: 10px" onClick="javascript:document.forms.formFarm.target='_parent';document.forms.formFarm.action='TestModelService.asp';document.forms.formFarm.submit();">Create xml</button></td>--%>
                            <asp:Button ID="Button_CreateXML" runat="server" Text="Create xml" /></td>
                    </tr>
                </table>
            </td>
        </tr>
        <tr>
            <td><a href="/upload/AnimalChange/download/FarmAC.zip">download current version of the model</a></td>
        </tr>
        <tr>
            <td><a href="/upload/AnimalChange/download/inputFiles.zip">current version of parameter-files</a>
            </td>
        </tr>
    </table>
</asp:Panel>
