using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace FarmAC.Controls
{
    public partial class ctrlNonRuminants : System.Web.UI.UserControl
    {
        string language;
        protected void Page_Load(object sender, EventArgs e)
        {
            if (HttpContext.Current.Session["UserInit"] != null)
                if (HttpContext.Current.Session["UserInit"].ToString() == "6")
            {
                if (HttpContext.Current.Session["languageSelected"] != null)
                    language = HttpContext.Current.Session["languageSelected"].ToString();
                else
                    language = "uk";
                CreateTabPig();
            }
        }
        dbInterface database;
        List<TextBox> AllInputAnimalYearSows = new List<TextBox>();
        List<TextBox> AllInputAnimalYearPiglet = new List<TextBox>();
        List<TextBox> AllInputProductionVar1 = new List<TextBox>();
        List<TextBox> AllInputProductionVar2 = new List<TextBox>();
        List<DropDownList> AllSelFeedstuff = new List<DropDownList>();
        List<TextBox> AllFeedstuffAmountSows = new List<TextBox>();
        List<TextBox> AllFeedstuffAmountPiglet = new List<TextBox>();
        List<DropDownList> AllSelectStableType1 = new List<DropDownList>();
        List<DropDownList> AllSelectStableType2 = new List<DropDownList>();
        List<List<DropDownList>> AllSelectManureStorageType1 = new List<List<DropDownList>>();
        List<List<DropDownList>> AllSelectManureStorageType2 = new List<List<DropDownList>>();


        List<List<DropDownList>> AllSelectPigType = new List<List<DropDownList>>();

        string farmNumber;
        string scenarioNumber;
        void CreateTabPig()
        {
            farmNumber = HttpContext.Current.Session["farmNumber"].ToString();
            scenarioNumber = HttpContext.Current.Session["scenarioNumber"].ToString();
            int aStableType1ID, aStableType2ID;
            DataTable arrStableType1 = null, arrStableType2 = null;
            List<List<string>> arrPigTypeNotSows = null;
            int aPigTypeID;
            DataTable arrPigHerd;
            DataTable arrPigType;
            DataTable arrFeedStuff;
            int aFeedStuffID = 0;
            double aFeedstuffAmount = 0;
            string pf;

            int nPigHerd;
            string strFeedplan1, strFeedplan2, strAmount1, strAmount2;
            DataTable arrPigHerdFeedplanList;
            string aPigHerdFeedplanID, strFeedplan = "+", strAmount = "+";
            int f = 0;
            string aManureStorageID;

            DataTable arrManureStorage;
            string aPigHerdManureStorageID;

            DataTable arrPigHerdManureStorageList;
            DataTable arrStorageType;
            database = new dbInterface();
            database.InitFarmNDB();
            //CreateLanguageDictionary
            List<dbInterface.data> paramter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = HttpContext.Current.Session["farmNumber"].ToString();
            paramter.Add(data);
            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            paramter.Add(data);
            arrPigHerd = database.GetValuesFromDB("GetPigHerdList", paramter);
            paramter = new List<dbInterface.data>();
            arrPigType = database.GetValuesFromDB("GetSelectListPigType ", paramter);


            PigTypeNotSows(arrPigType, ref arrPigTypeNotSows);



            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@AgroEcologicalZone";
            data.value = "-1";
            paramter.Add(data);
            arrFeedStuff = database.GetValuesFromDB("GetFeedStuffList", paramter);

            DefineChangeTextScriptPig();
            // DefineFeedstuffListJScript_8

            Panel1.Controls.Add(new LiteralControl("<form name=" + "formPigHerd" + " action=" + "DummyPigHerd.asp" + " method=" + "post" + " id=" + "dynamichide" + " style=" + "display: block;" + ">" + "\r"));

            Panel1.Controls.Add(new LiteralControl("  <table class=" + "data" + " border=" + "0" + " style=" + "width: 1800px;" + ">" + "\r"));
            // column headings
            Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <td colspan=" + "7" + "><h4>" + "strSøer" + "</h4></td>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));


            Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + " style=" + "width: 130px;" + ">" + "strAntal årssøer" + "&nbsp;</th>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + " style=" + "width: 190px;" + ">" + "strAntal fravænnede grise" + "&nbsp;</th>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + " style=" + "width: 182px;" + ">" + "strVægt ved fravænning" + "&nbsp;</th>" + "\r"));

            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + " style=" + "width: 625px;" + ">" + "strFeedstuffPlan" + "&nbsp;</th>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + "strStaldtype" + " - " + "strdrægtige søer" + "&nbsp;</th>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + "strStaldtype" + " - " + "strdiegivende søer" + "&nbsp;</th>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));

            for (int i = 0; i < arrPigHerd.Rows.Count; i++)
            {
                string test = arrPigHerd.Rows[i]["PigTypeID"].ToString();

                if (arrPigHerd.Rows[i]["PigTypeID"].ToString() == "1")
                {
                    f = (int)arrPigHerd.Rows[i]["PigTypeID"];
                    Scripting.DictionaryClass dictPigHerdFeedplanList = new Scripting.DictionaryClass();
                    paramter = new List<dbInterface.data>();
                    data = new dbInterface.data();
                    data.name = "@aFarmNumber";
                    data.value = HttpContext.Current.Session["farmNumber"].ToString();
                    paramter.Add(data);
                    data = new dbInterface.data();
                    data.name = "@aScenarioID";
                    data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
                    paramter.Add(data);
                    data.name = "@aPigHerdID";
                    data.value = arrPigHerd.Rows[i]["PigTypeID"].ToString();
                    paramter.Add(data);
                    arrPigHerdFeedplanList = database.GetValuesFromDB("GetPigHerdFeedplanList", paramter);

                    aPigTypeID = (int)arrPigHerd.Rows[i]["PigTypeID"];
                    if (aPigTypeID == 1)
                    {
                        string tm0 = arrPigType.Rows[0]["PigTypeID"].ToString();
                        if (int.Parse(arrPigType.Rows[0]["PigTypeID"].ToString()) == aPigTypeID)
                        {
                            paramter = new List<dbInterface.data>();
                            data = new dbInterface.data();
                            data.name = "@aPigTypeID";
                            data.value = arrPigType.Rows[0]["PigTypeID"].ToString();
                            paramter.Add(data);
                            data = new dbInterface.data();
                            data.name = "@aPigTypeSubID";
                            data.value = "2";
                            paramter.Add(data);
                            Scripting.DictionaryClass dictStableType = new Scripting.DictionaryClass();
                            arrStableType1 = database.GetValuesFromDB("GetSelectListStableTypePig", paramter);
                            int stuff = arrStableType1.Rows.Count;
                            paramter = new List<dbInterface.data>();
                            data = new dbInterface.data();
                            data.name = "@aPigTypeID";
                            data.value = arrPigType.Rows[0]["PigTypeID"].ToString();
                            paramter.Add(data);
                            data = new dbInterface.data();
                            data.name = "@aPigTypeSubID";
                            data.value = "3";
                            paramter.Add(data);
                            arrStableType2 = database.GetValuesFromDB("GetSelectListStableTypePig", paramter);

                        }
                        else
                        {
                            paramter = new List<dbInterface.data>();
                            data = new dbInterface.data();
                            data.name = "@aPigTypeID";
                            data.value = arrPigType.Rows[0]["PigTypeID"].ToString();
                            paramter.Add(data);
                            data = new dbInterface.data();
                            data.name = "@aPigTypeSubID";
                            data.value = "1";
                            paramter.Add(data);
                            arrStableType1 = database.GetValuesFromDB("GetSelectListStableTypePig", paramter);
                            int stuff = arrStableType1.Rows.Count;
                            paramter = new List<dbInterface.data>();
                            data = new dbInterface.data();
                            data.name = "@aPigTypeID";
                            data.value = arrPigType.Rows[0]["PigTypeID"].ToString(); ;
                            paramter.Add(data);
                            data = new dbInterface.data();
                            data.name = "@aPigTypeSubID";
                            data.value = "1";
                            paramter.Add(data);
                            arrStableType2 = database.GetValuesFromDB("GetSelectListStableTypePig", paramter);
                        }
                    }




                    aStableType1ID = (int)arrPigHerd.Rows[i]["Stable1"];
                    aStableType2ID = (int)arrPigHerd.Rows[i]["Stable2"];

                    Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));

                    Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + ">"));
                    Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "hiddenPigHerdID_" + i + "+" + " value=" + "+" + arrPigHerd.Rows[i]["PigHerdID"] + "+" + "/>"));
                    Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "selectPigType_" + i + "+" + " value=" + "+" + aPigTypeID + "+" + "/>"));

                    Panel1.Controls.Add(new LiteralControl("&nbsp;&nbsp;"));

                    TextBox inputAnimalYear = new TextBox();
                    inputAnimalYear.ID = "inputAnimalYearPiglet_" + i.ToString();
                    inputAnimalYear.Text = arrPigHerd.Rows[i]["AnimalYear"].ToString();
                    inputAnimalYear.TextChanged += new EventHandler(saveEvent);
                    AllInputAnimalYearSows.Add(inputAnimalYear);
                    Panel1.Controls.Add(inputAnimalYear);
                    Panel1.Controls.Add(new LiteralControl("&nbsp;" + "str" + arrPigHerd.Rows[i]["AnimalYearunitText"] + "&nbsp;</td>" + "\r")); ;
                    //column 3 = produktionsniveau
                    Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + ">&nbsp;&nbsp;"));

                    TextBox inputProductionVar1 = new TextBox();
                    inputProductionVar1.ID = "inputProductionVar1" + i.ToString();
                    inputProductionVar1.Text = arrPigHerd.Rows[i]["ProductionVar1"].ToString();
                    inputProductionVar1.TextChanged += new EventHandler(saveEvent);
                    AllInputProductionVar1.Add(inputProductionVar1);
                    Panel1.Controls.Add(inputProductionVar1);
                    Panel1.Controls.Add(new LiteralControl(" &nbsp;" + "str" + arrPigHerd.Rows[i]["ProductionVar1UnitText"] + "<input type=" + "hidden" + " name=" + "inputProductionVar3_" + i + "+" + " value=" + "+" + arrPigHerd.Rows[i]["ProductionVar3"] + "+" + "/>&nbsp;</td>" + "\r")); ;
                    Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + ">&nbsp;&nbsp;"));

                    TextBox inputProductionVar2 = new TextBox();
                    inputProductionVar2.ID = "inputProductionVar2" + i.ToString();
                    inputProductionVar2.Text = arrPigHerd.Rows[i]["ProductionVar2"].ToString();
                    inputProductionVar2.TextChanged += new EventHandler(saveEvent);
                    AllInputProductionVar2.Add(inputProductionVar2);
                    Panel1.Controls.Add(inputProductionVar2);
                    Panel1.Controls.Add(new LiteralControl(" &nbsp;" + "str" + arrPigHerd.Rows[i]["ProductionVar1UnitText"] + "&nbsp;</td>" + "\r")); ;
                    // column 4 = Foder mængde
                    Panel1.Controls.Add(new LiteralControl("      <input type=" + "hidden" + " name=" + "inputFeedLevel_" + i + "+" + " value=" + "+" + arrPigHerd.Rows[i]["FeedLevel"] + "+" + " />" + "\r"));
                    // column 5 = Protein niveau   
                    Panel1.Controls.Add(new LiteralControl("      <input type=" + "hidden" + " name=" + "inputProteinLevel_" + i + "+" + " value=" + "+" + arrPigHerd.Rows[i]["ProteinLevel"] + "+" + "/>" + "\r")); ;
                    // column 6 = stable type   
                    Panel1.Controls.Add(new LiteralControl("<td>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("<table border=" + "0" + " cellspacing=" + "0" + " width=" + "100%" + ">" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("<tbody id=" + "tbodyFeedstuff_" + f + "+" + " name=" + "tbodyFeedstuff_" + f + "+" + ">" + "\r"));
                    if (arrPigHerdFeedplanList.Rows.Count == 0)
                    {
                        Panel1.Controls.Add(new LiteralControl("<tr>" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("<td>" + "\r"));
                        //   Panel1.Controls.Add(new LiteralControl("<select id=" + "selFeedstuff_" + f + "+" + " name=" + "selFeedstuff_" + f + "+" + " onchange=" + "document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';checkForNewLine(" + f + ");setFeedstuffString(" + f + ");" + " style=" + "position:relative;top:-3Px;background-color: lightyellow;" + ">" + "\r"));
                        //   Panel1.Controls.Add(new LiteralControl("  <option value=" + "-1" + ">" + "strSELECT_FEEDSTUFF" + "</option>" + "\r"));
                        DropDownList selFeedstuff_ = new DropDownList();
                        selFeedstuff_.ID = "selFeedstuff_" + f.ToString();
                        selFeedstuff_.Items.Add("strSELECT_FEEDSTUFF");
                        selFeedstuff_.Items[0].Attributes.Add("dbValue", "-1");
                        for (int nf = 0; nf < arrFeedStuff.Rows.Count; nf++)
                        {
                            if (aFeedStuffID == (int)arrFeedStuff.Rows[nf][0])
                            {
                                // Panel1.Controls.Add(new LiteralControl("  <option value=" + "+" + arrFeedStuff.Rows[nf][0] + "+" + " selected>" + arrFeedStuff.Rows[nf][1] + "</option>" + "\r"));
                                selFeedstuff_.Items.Add(arrFeedStuff.Rows[nf][0].ToString() + " " + arrFeedStuff.Rows[nf][1]);
                                selFeedstuff_.Items[selFeedstuff_.Items.Count - 1].Attributes.Add("dbValue", arrFeedStuff.Rows[nf][0].ToString());
                                selFeedstuff_.SelectedIndex = (nf + 1);
                            }
                            else
                            {
                                selFeedstuff_.Items.Add(arrFeedStuff.Rows[nf][0].ToString() + " " + arrFeedStuff.Rows[nf][1]);
                                selFeedstuff_.Items[selFeedstuff_.Items.Count - 1].Attributes.Add("dbValue", arrFeedStuff.Rows[nf][0].ToString());
                                //  Panel1.Controls.Add(new LiteralControl("  <option value=" + "+" + arrFeedStuff.Rows[nf][0] + "+" + ">" + arrFeedStuff.Rows[nf][1] + "</option>" + "\r")); ;
                            }

                        }
                        //Panel1.Controls.Add(new LiteralControl("</select>" + "\r"));
                        AllSelFeedstuff.Add(selFeedstuff_);
                        Panel1.Controls.Add(selFeedstuff_);
                        // Panel1.Controls.Add(new LiteralControl("<input type=" + "text" + " name=" + "feedstuffAmount_" + f + "+" + " id=" + "feedstuffAmount_" + f + "+" + " style=" + "width: 35px;position:relative;top:-3Px;background-color: lightyellow;" + " onchange=" + "document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';this.value.replace(',','.');setFeedstuffString(" + f + ");" + " value=" + "0" + "> " + "strKgDM/animal/day" + "+" + "\r"));
                        TextBox feedstuffAmount_ = new TextBox();
                        feedstuffAmount_.ID = "feedstuffAmount_" + f.ToString();
                        feedstuffAmount_.Text = "0";
                        feedstuffAmount_.TextChanged += new EventHandler(saveEvent);
                        AllFeedstuffAmountSows.Add(feedstuffAmount_);
                        Panel1.Controls.Add(feedstuffAmount_);
                        Panel1.Controls.Add(new LiteralControl("<img style=" + "position:relative;top:3Px;" + " src=" + "images/del.gif" + " onclick=" + "delRow(this," + f + ");" + " onMouseover=" + "mouseOverGif(this)" + "  onMouseout=" + "mouseOutGif(this)" + "/>&nbsp;&nbsp;" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("</tr>" + "\r"));
                    }
                    else
                    {
                        for (int fp = 0; fp < arrPigHerdFeedplanList.Rows.Count - 1; fp++)
                        {
                            strFeedplan = strFeedplan + arrPigHerdFeedplanList.Rows[fp][0] + ",";
                            strAmount = strAmount + arrPigHerdFeedplanList.Rows[fp][1] + ",";
                            Panel1.Controls.Add(new LiteralControl("<tr>" + "\r"));
                            Panel1.Controls.Add(new LiteralControl("<td>" + "\r"));
                            Panel1.Controls.Add(new LiteralControl("<select id=" + "selFeedstuff_" + f + "+" + " name=" + "selFeedstuff_" + f + "+" + " onchange=" + "document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';checkForNewLine(" + f + ");setFeedstuffString(" + f + ");" + " style=" + "position:relative;top:-3Px;background-color: white;" + ">" + "\r"));
                            Panel1.Controls.Add(new LiteralControl("  <option value=" + "-1" + ">" + "strSELECT_FEEDSTUFF" + "</option>" + "\r"));
                            for (int nf = 0; nf < arrFeedStuff.Rows.Count - 1; nf++)
                            {
                                if (arrPigHerdFeedplanList.Rows.Count != 0)
                                {
                                    if (fp < arrPigHerdFeedplanList.Rows.Count)
                                    {
                                        aFeedStuffID = int.Parse(arrPigHerdFeedplanList.Rows[fp][0].ToString());

                                        aFeedstuffAmount = double.Parse(arrPigHerdFeedplanList.Rows[fp][1].ToString());
                                    }
                                }
                                else
                                {
                                    aFeedStuffID = -1;
                                    aFeedstuffAmount = 0;
                                }
                                if (aFeedStuffID == (int)arrFeedStuff.Rows[nf][0])
                                    Panel1.Controls.Add(new LiteralControl("  <option value=" + "+" + arrFeedStuff.Rows[nf][0] + "+" + " selected>" + arrFeedStuff.Rows[nf][1] + "</option>" + "\r"));
                                else
                                    Panel1.Controls.Add(new LiteralControl("  <option value=" + "+" + arrFeedStuff.Rows[nf][0] + "+" + ">" + arrFeedStuff.Rows[nf][0] + "</option>" + "\r"));

                            }
                            Panel1.Controls.Add(new LiteralControl("</select>" + "\r"));
                            TextBox feedstuffAmount_ = new TextBox();
                            feedstuffAmount_.ID = "feedstuffAmountdddddd" + i.ToString();
                            feedstuffAmount_.Text = aFeedstuffAmount.ToString();
                            feedstuffAmount_.TextChanged += new EventHandler(saveEvent);
                            AllFeedstuffAmountPiglet.Add(feedstuffAmount_);
                            Panel1.Controls.Add(feedstuffAmount_);
                            Panel1.Controls.Add(new LiteralControl("strKgDM/animal/day" + "+" + "\r"));
                            // Panel1.Controls.Add(new LiteralControl("<input id=" + "feedstuffAmount_" + f + "+" + " name=" + "feedstuffAmount_" + f + "+" + " type=" + "text" + " style=" + "width: 35px;position:relative;top:-3Px;background-color: white;" + " value=" + "+" + aFeedstuffAmount + "+" + " onchange=" + "document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';this.value=this.value.replace(',','.');setFeedstuffString(" + f + ");" + "/> " + "strKgDM/animal/day" + "+" + "\r"));
                            Panel1.Controls.Add(new LiteralControl("<img style=" + "position:relative;top:3Px;" + " src=" + "images/del.gif" + " onclick=" + "delRow(this," + f + ");" + " onMouseover=" + "mouseOverGif(this)" + "  onMouseout=" + "mouseOutGif(this)" + "/>&nbsp;&nbsp;" + "\r"));
                            Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                            Panel1.Controls.Add(new LiteralControl("</tr>" + "\r"));
                            aFeedStuffID = -1;
                        }
                        Panel1.Controls.Add(new LiteralControl("<tr>" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("<td>" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("<select id=" + "selFeedstuff_" + f + "+" + " name=" + "selFeedstuff_" + f + "+" + " onchange=" + "document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';checkForNewLine(" + f + ");setFeedstuffString(" + f + ");" + " style=" + "position:relative;top:-3Px;background-color: lightyellow;" + ">" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("  <option value=" + "-1" + ">" + "strSELECT_FEEDSTUFF" + "</option>" + "\r"));
                        for (int nf = 0; nf < arrFeedStuff.Rows.Count - 1; nf++)
                            Panel1.Controls.Add(new LiteralControl("  <option value=" + "+" + arrFeedStuff.Rows[nf][0] + "+" + ">" + arrFeedStuff.Rows[nf][1] + "</option>" + "\r")); ;

                        Panel1.Controls.Add(new LiteralControl("</select>" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("<input type=" + "text" + " name=" + "feedstuffAmount_" + f + "+" + " id=" + "feedstuffAmount_" + f + "+" + " style=" + "width: 35px;position:relative;top:-3Px;background-color: lightyellow;" + " onchange=" + "document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';this.value=this.value.replace(',','.');setFeedstuffString(" + f + ");" + " value=" + "0" + "> " + "strKgDM/animal/day" + "+" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("<img style=" + "position:relative;top:3Px;" + " src=" + "images/del.gif" + " onclick=" + "delRow(this," + f + ");" + " onMouseover=" + "mouseOverGif(this)" + "  onMouseout=" + "mouseOutGif(this)" + "/>&nbsp;&nbsp;" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("</tr>" + "\r"));
                    }
                    Panel1.Controls.Add(new LiteralControl("</tbody>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("</table>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "hiddenFeedstuffList_" + f + "+" + "  id=" + "hiddenFeedstuffList_" + f + "+" + " value=" + "+" + strFeedplan + "+" + ">"));
                    strFeedplan = "+";
                    Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "hiddenAmountList_" + f + "+" + "  id=" + "hiddenAmountList_" + f + "+" + " value=" + "+" + strAmount + "+" + ">"));
                    Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + " style=" + "vertical-align: top;" + ">"));
                    DropDownList SelectStableType1 = new DropDownList();
                    SelectStableType1.ID = "SelectStableType1_" + i.ToString();
                    for (int n = 0; n < arrStableType1.Rows.Count; n++)
                    {
                        SelectStableType1.Items.Add(arrStableType1.Rows[n][0] + " " + arrStableType1.Rows[n][1]);
                        SelectStableType1.Items[SelectStableType1.Items.Count - 1].Attributes.Add("dbValue", arrStableType1.Rows[n][0].ToString());
                        if (aStableType1ID.ToString() == arrStableType1.Rows[n][0].ToString())
                            SelectStableType1.SelectedIndex = n;
                    }
                    AllSelectStableType1.Add(SelectStableType1);
                    Panel1.Controls.Add(SelectStableType1);
                    //Panel1.Controls.Add(new LiteralControl("<select style=" + "width: 220px;" + " name=" + "selectStableType1_" + i + "+" + " onChange=" + "javascript:document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';" + ">"));
                    //CreateSelectList_Language aStableType1ID,nStableType1,arrStableType1
                    //Panel1.Controls.Add(new LiteralControl("</select><br/><br/>"));
                    Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " id=" + "storageChanged1_" + i + "+" + " name=" + "storageChanged1_" + i + "+" + " value=" + "0" + "/>"));
                    paramter = new List<dbInterface.data>();
                    data = new dbInterface.data();
                    data.name = "@aFarmNumber";
                    data.value = HttpContext.Current.Session["farmNumber"].ToString();
                    paramter.Add(data);
                    data = new dbInterface.data();
                    data.name = "@aScenarioID";
                    data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
                    paramter.Add(data);
                    data = new dbInterface.data();
                    data.name = "@aPigHerdID";
                    data.value = arrPigHerd.Rows[i]["PigHerdID"].ToString();
                    paramter.Add(data);
                    data = new dbInterface.data();
                    data.name = "@aStableSubTypeID";
                    data.value = "1";
                    paramter.Add(data);

                    arrPigHerdManureStorageList = database.GetValuesFromDB("GetPigHerdManureStorageList", paramter);

                    Scripting.DictionaryClass dictStorageType = new Scripting.DictionaryClass();
                    paramter = new List<dbInterface.data>();
                    data = new dbInterface.data();
                    data.name = "@aPigTypeID";
                    data.value = aPigTypeID.ToString();
                    paramter.Add(data);
                    data = new dbInterface.data();
                    data.name = "@aStableID";
                    data.value = aStableType1ID.ToString();
                    paramter.Add(data);
                    arrStorageType = database.GetValuesFromDB("GetStorageTypesPrPigStable", paramter);

                    Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "nManureStorage1_" + i + "+" + "  id=" + "nManureStorage1_" + i + "+" + " value=" + "+" + arrStorageType.Rows.Count + "+" + "/>"));
                    Panel1.Controls.Add(new LiteralControl("<b>" + "strManureStorageType" + "</b><br/>"));
                    for (int ms = 0; ms < arrStorageType.Rows.Count; ms++)
                    {

                        AllSelectManureStorageType1.Add(new List<DropDownList>());
                        paramter = new List<dbInterface.data>();
                        data = new dbInterface.data();
                        data.name = "@aPigTypeID";
                        data.value = aPigTypeID.ToString();
                        paramter.Add(data);
                        data = new dbInterface.data();
                        data.name = "@aStableID";
                        data.value = aStableType1ID.ToString();
                        paramter.Add(data);
                        data = new dbInterface.data();
                        data.name = "@aStorageID";
                        data.value = arrStorageType.Rows[ms][0].ToString();
                        paramter.Add(data);
                        arrManureStorage = database.GetValuesFromDB("GetSelectListPigManureStorageType", paramter);
                        aPigHerdManureStorageID = arrManureStorage.Rows[0][0].ToString();
                        if (arrPigHerdManureStorageList.Rows.Count > 0)
                        {
                            if (ms < arrPigHerdManureStorageList.Rows.Count)
                            {
                                aPigHerdManureStorageID = arrPigHerdManureStorageList.Rows[ms][1].ToString();
                            }
                        }
                        DropDownList SelectManureStorageType1 = new DropDownList();
                        SelectManureStorageType1.ID = "SelectManureStorageType1" + i.ToString();
                        for (int n = 0; n < arrManureStorage.Rows.Count; n++)
                        {
                            SelectManureStorageType1.Items.Add(arrManureStorage.Rows[n][0] + " " + arrManureStorage.Rows[n][1]);
                            SelectManureStorageType1.Items[SelectManureStorageType1.Items.Count - 1].Attributes.Add("dbValue", arrManureStorage.Rows[n][0].ToString());
                            if (aPigHerdManureStorageID.ToString() == arrManureStorage.Rows[n][0].ToString())
                                SelectManureStorageType1.SelectedIndex = n;
                        }
                        AllSelectManureStorageType1[ms].Add(SelectManureStorageType1);
                        Panel1.Controls.Add(SelectManureStorageType1);

                        //   Panel1.Controls.Add(new LiteralControl("<select style=" + "width: 220px;" + " name=" + "selectManureStorageType1_" + i + "+" + " onChange=" + "document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';" + ">"));
                        // CreateSelectList_ aPigHerdManureStorageID,nManureStorage,arrManureStorage
                        //  Panel1.Controls.Add(new LiteralControl("</select>"));
                        if (ms < arrStorageType.Rows.Count)
                            Panel1.Controls.Add(new LiteralControl("<br/><br/>"));


                    }
                    Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + " style=" + "vertical-align: top;" + ">"));
                    //Panel1.Controls.Add(new LiteralControl("<select style=" + "width: 220px;" + " name=" + "selectStableType2_" + i + "+" + " onChange=" + "javascript:document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';" + ">"));
                    // CreateSelectList_Language aStableType2ID,nStableType2,arrStableType2
                    //Panel1.Controls.Add(new LiteralControl("</select><br/><br/>"));
                    DropDownList selectStableType2 = new DropDownList();
                    selectStableType2.ID = "selectStableType2" + i.ToString();
                    int stf = arrStableType2.Rows.Count;
                    for (int n = 0; n < arrStableType2.Rows.Count; n++)
                    {
                        selectStableType2.Items.Add(arrStableType2.Rows[n][0] + " " + arrStableType2.Rows[n][1]);
                        selectStableType2.Items[selectStableType2.Items.Count - 1].Attributes.Add("dbValue", arrStableType2.Rows[n][0].ToString());
                        if (aStableType2ID.ToString() == arrStableType2.Rows[n][0].ToString())
                            selectStableType2.SelectedIndex = n;
                    }
                    AllSelectStableType2.Add(selectStableType2);
                    Panel1.Controls.Add(selectStableType2);
                    Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " id=" + "storageChanged2_" + i + "+" + " name=" + "storageChanged2_" + i + "+" + " value=" + "0" + "/>"));

                    paramter = new List<dbInterface.data>();
                    data = new dbInterface.data();
                    data.name = "@aFarmNumber";
                    data.value = HttpContext.Current.Session["farmNumber"].ToString();
                    paramter.Add(data);
                    data = new dbInterface.data();
                    data.name = "@aScenarioID";
                    data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
                    paramter.Add(data);
                    data = new dbInterface.data();
                    data.name = "@aPigHerdID";
                    data.value = arrPigHerd.Rows[i]["PigHerdID"].ToString();
                    paramter.Add(data);
                    data.name = "@aStableSubTypeID";
                    data.value = "2";
                    paramter.Add(data);
                    arrPigHerdManureStorageList = database.GetValuesFromDB("GetPigHerdManureStorageList", paramter);
                    paramter = new List<dbInterface.data>();
                    data = new dbInterface.data();
                    data.name = "@aPigTypeID";
                    data.value = aPigTypeID.ToString();
                    paramter.Add(data);
                    data = new dbInterface.data();
                    data.name = "@aStableID";
                    data.value = aStableType2ID.ToString();
                    paramter.Add(data);
                    arrStorageType = database.GetValuesFromDB("GetStorageTypesPrPigStable ", paramter);
                    Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "nManureStorage2_" + i + "+" + "  id=" + "nManureStorage2_" + i + "+" + " value=" + "+" + arrStorageType.Rows.Count + "+" + "/>"));

                    Panel1.Controls.Add(new LiteralControl("<b>" + "strManureStorageType" + "</b><br/>"));
                    for (int ms = 0; ms < arrStorageType.Rows.Count; ms++)
                    {
                        AllSelectManureStorageType2.Add(new List<DropDownList>());
                        paramter = new List<dbInterface.data>();
                        data = new dbInterface.data();
                        data.name = "@aPigTypeID";
                        data.value = aPigTypeID.ToString();
                        paramter.Add(data);
                        data = new dbInterface.data();
                        data.name = "@aStableID";
                        data.value = aStableType2ID.ToString();
                        paramter.Add(data);
                        data = new dbInterface.data();
                        data.name = "@aStorageID";
                        data.value = arrStorageType.Rows[ms][0].ToString();
                        paramter.Add(data);
                        arrManureStorage = database.GetValuesFromDB("GetSelectListPigManureStorageType", paramter);
                        aPigHerdManureStorageID = arrManureStorage.Rows[0][0].ToString();
                        if (arrPigHerdManureStorageList.Rows.Count > 0)
                        {
                            if (ms < arrPigHerdManureStorageList.Rows.Count)
                            {
                                aPigHerdManureStorageID = arrPigHerdManureStorageList.Rows[ms][1].ToString();
                            }
                        }
                        DropDownList SelectManureStorageType2 = new DropDownList();
                        SelectManureStorageType2.ID = "SelectManureStorageType2" + i.ToString();
                        for (int n = 0; n < arrManureStorage.Rows.Count; n++)
                        {
                            SelectManureStorageType2.Items.Add(arrManureStorage.Rows[n][0] + " " + arrManureStorage.Rows[n][1]);
                            SelectManureStorageType2.Items[SelectManureStorageType2.Items.Count - 1].Attributes.Add("dbValue", arrManureStorage.Rows[n][0].ToString());
                            if (aPigHerdManureStorageID.ToString() == arrManureStorage.Rows[n][0].ToString())
                                SelectManureStorageType2.SelectedIndex = n;
                        }
                        AllSelectManureStorageType2[ms].Add(SelectManureStorageType2);

                        Panel1.Controls.Add(SelectManureStorageType2);
                        Panel1.Controls.Add(new LiteralControl("<select style=" + "width: 220px;" + " name=" + "selectManureStorageType2_" + i + "+" + " onChange=" + "document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';" + ">"));
                        //CreateSelectList_ aPigHerdManureStorageID,nManureStorage,arrManureStorage
                        Panel1.Controls.Add(new LiteralControl("</select>"));

                        if (ms < arrStorageType.Rows.Count)
                            Panel1.Controls.Add(new LiteralControl("<br/><br/>"));

                    }
                    Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                    // column 7 = delete buttons   
                    Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + ">"));
                    Button strDelete = new Button();
                    strDelete.ID = "strDelete" + i.ToString();
                    strDelete.Text = "Delete";
                    strDelete.Click += new EventHandler(DeleteEvent);
                    strDelete.Attributes.Add("PigHerdID", arrPigHerd.Rows[i]["PigHerdID"].ToString());
                    Panel1.Controls.Add(strDelete);
                    Panel1.Controls.Add(new LiteralControl("<button style=" + "width: 50px; height: 20px; font-size: 10px" + " onClick=" + "javascript:if(confirm('" + "strBekræft" + "')) {veil();document.forms.formPigHerd.action='PigHerdDelete.asp?PigHerdID=" + arrPigHerd.Rows[i]["PigHerdID"] + "';document.forms.formPigHerd.submit();}" + ">" + ("strDelete") + "</button>"));
                    Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));

                    Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " colspan=" + "7" + "><hr/></td>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));
                    //dictStableType = null;

                    dictPigHerdFeedplanList = null;

                    strFeedplan = "+";
                    strAmount = "+";

                }
            }
            //  row for buttons

            Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " colspan=" + "7" + ">&nbsp;&nbsp;"));
            //Panel1.Controls.Add(new LiteralControl("<button style=" + "width: 100px; height: 20px; font-size: 10px" + " onClick=" + "javascript:veil();document.forms.formPigHerd.action='PigHerdNew.asp';document.forms.formPigHerd.hiddenPigType.value='1';document.forms.formPigHerd.submit();" + ">" + ("strNEW") + "</button>"));
            Button newPigHerdNew = new Button();
            newPigHerdNew.ID = "PigHerdNew";
            newPigHerdNew.Text = "New";
            newPigHerdNew.Click += new EventHandler(newEvent);
            Panel1.Controls.Add(newPigHerdNew);
            Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));

            Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));

            Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " colspan=" + "7" + "><hr/></td>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("  </table>" + "\r"));
            //    Tabel for reste


            Panel1.Controls.Add(new LiteralControl("  <table class=" + "data" + " border=" + "0" + " width=" + "100%" + ">" + "\r"));
            // column headings

            Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <td colspan=" + "7" + "><h4>" + ("strSmågrise") + "+" + ("strog") + "+" + ("strSlagtesvin") + "</h4></td>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));


            Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + " style=" + "width: 140px;" + ">Type&nbsp;</th>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + " style=" + "width: 140px;" + ">" + database.GetLanguageString("strANANIMAL", language) + "&nbsp;</th>" + "\r"));

            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + " style=" + "width: 625px;" + ">" + database.GetLanguageString("strFeedstuffPlan", language) + "&nbsp;</th>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + ("strStaldtype") + "&nbsp;</th>" + "\r"));

            Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));
            for (int i = 0; i < (arrPigHerd.Rows.Count); i++)
            {
                string test = arrPigHerd.Rows[i]["PigTypeID"].ToString();
                AllSelectPigType.Add(new List<DropDownList>());
                if (arrPigHerd.Rows[i]["PigTypeID"].ToString() == "2")
                {

                    paramter = new List<dbInterface.data>();
                    data = new dbInterface.data();
                    data.name = "@aFarmNumber";
                    data.value = HttpContext.Current.Session["farmNumber"].ToString();
                    paramter.Add(data);

                    data = new dbInterface.data();
                    data.name = "@aScenarioID";
                    data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
                    paramter.Add(data);

                    data = new dbInterface.data();
                    data.name = "@aCattleHerdID";
                    data.value = arrPigHerd.Rows[i]["PigHerdID"].ToString();
                    paramter.Add(data);
                    arrPigHerdManureStorageList = database.GetValuesFromDB("GetCattleHerdManureStorageList", paramter);
                    f = (int)arrPigHerd.Rows[i]["PigHerdID"];

                    aPigTypeID = (int)arrPigHerd.Rows[i]["PigHerdID"];
                    paramter = new List<dbInterface.data>();
                    data = new dbInterface.data();
                    data.name = "@aFarmNumber";
                    data.value = HttpContext.Current.Session["farmNumber"].ToString();
                    paramter.Add(data);

                    data = new dbInterface.data();
                    data.name = "@aScenarioID";
                    data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
                    paramter.Add(data);

                    data = new dbInterface.data();
                    data.name = "@aPigHerdID";
                    data.value = arrPigHerd.Rows[i]["PigHerdID"].ToString();
                    paramter.Add(data);
                    arrPigHerdFeedplanList = database.GetValuesFromDB("GetPigHerdFeedplanList ", paramter);
                    Scripting.DictionaryClass dictStableType = null;
                    if (aPigTypeID > 1)
                    {
                        dictStableType = new Scripting.DictionaryClass();
                    }

                    paramter = new List<dbInterface.data>();
                    data = new dbInterface.data();
                    data.name = "@aPigTypeID";
                    data.value = aPigTypeID.ToString();
                    paramter.Add(data);
                    data = new dbInterface.data();
                    data.name = "@aPigTypeSubID";
                    data.value = "1";
                    paramter.Add(data);

                    arrStableType1 = database.GetValuesFromDB("GetSelectListStableTypePig", paramter);


                    aStableType1ID = (int)arrPigHerd.Rows[i]["Stable1"];
                    aStableType2ID = 0;

                    Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
                    // column 1 = pig type
                    Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + ">"));
                    Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "hiddenPigHerdID_" + i + "+" + " value=" + "+" + arrPigHerd.Rows[i]["PigHerdID"] + "+" + "/>"));
                    //   Panel1.Controls.Add(new LiteralControl("&nbsp;&nbsp;<select name="+"selectPigType_"+i+"+"+" onChange="+"ChangeTextPig("+i+",this.options[this.selectedIndex].value);document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';"+">"));  
                    // CreateSelectListLanguage aPigTypeID,Ubound(arrPigTypeNotSows,2)+1,arrPigTypeNotSows
                    // Panel1.Controls.Add(new LiteralControl("</select>"));  
                    DropDownList selectPigType = new DropDownList();
                    selectPigType.ID = "selectPigType_" + i.ToString();
                    for (int t = 0; t < arrPigTypeNotSows[0].Count; t++)
                    {
                        selectPigType.Items.Add(arrPigTypeNotSows[0][t] + " " + arrPigTypeNotSows[1][t]);
                        selectPigType.Items[selectPigType.Items.Count - 1].Attributes.Add("dbValue", arrPigTypeNotSows[0][t].ToString());
                        if (aPigTypeID.ToString() == arrPigTypeNotSows[0][t])
                            selectPigType.SelectedIndex = t;
                    }
                    AllSelectPigType[i].Add(selectPigType);
                    Panel1.Controls.Add(selectPigType);
                    Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                    // column 2 = antal   

                    //Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + ">&nbsp;&nbsp;<input type=" + "text" + " name=" + "inputAnimalYear_" + i + "+" + " value=" + "+" + arrPigHerd.Rows[i]["AnimalYear"] + "+" + " maxlength=" + "5" + " size=" + "5" + " onChange=" + "document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';" + "/>&nbsp;" + "str" + arrPigHerd.Rows[i]["AnimalYearunitText"] + "&nbsp;</td>" + "\r")); ;  
                    Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + ">&nbsp;&nbsp;"));//<input type=" + "text" + " name=" + "inputAnimalYear_" + i + "+" + " value=" + "+" + arrPigHerd.Rows[i]["AnimalYear"] + "+" + " maxlength=" + "5" + " size=" + "5" + " onChange=" + "document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';" + "/>
                    TextBox inputAnimalYear = new TextBox();
                    inputAnimalYear.ID = "inputAnimalYearSows_" + i.ToString();
                    inputAnimalYear.Text = arrPigHerd.Rows[i]["AnimalYear"].ToString();
                    inputAnimalYear.TextChanged += new EventHandler(saveEvent);
                    AllInputAnimalYearSows.Add(inputAnimalYear);
                    Panel1.Controls.Add(inputAnimalYear);
                    Panel1.Controls.Add(new LiteralControl("&nbsp;" + "str" + arrPigHerd.Rows[i]["AnimalYearunitText"] + "&nbsp;</td>" + "\r")); ;


                    // column 3 = produktionsniveau
                    Panel1.Controls.Add(new LiteralControl("      <input type=" + "hidden" + " name=" + "inputProductionVar1_" + i + "+" + " value=" + "+" + arrPigHerd.Rows[i]["ProductionVar1"] + "+" + " maxlength=" + "5" + " size=" + "5" + " onChange=" + "document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';" + "/>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("      <input type=" + "hidden" + " name=" + "inputProductionVar2_" + i + "+" + " value=" + "+" + arrPigHerd.Rows[i]["ProductionVar2"] + "+" + " maxlength=" + "5" + " size=" + "5" + " onChange=" + "document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';" + "/><input type=" + "hidden" + " name=" + "inputProductionVar3_" + i + "+" + " value=" + "+" + arrPigHerd.Rows[i]["ProductionVar3"] + "+" + "/>" + "\r")); ;
                    // column 4 = Foder mængde
                    Panel1.Controls.Add(new LiteralControl("      <input type=" + "hidden" + " name=" + "inputFeedLevel_" + i + "+" + " value=" + "+" + arrPigHerd.Rows[i]["FeedLevel"] + "+" + "/>" + "\r")); ;
                    // column 5 = Protein niveau   
                    Panel1.Controls.Add(new LiteralControl("      <input type=" + "hidden" + " name=" + "inputProteinLevel_" + i + "+" + " value=" + "+" + arrPigHerd.Rows[i]["ProteinLevel"] + "+" + "/>" + "\r")); ;
                    // column 6 = stable type   
                    Panel1.Controls.Add(new LiteralControl("<td>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("<table id=" + "dynamichide" + " style=" + "display: block;" + " border=" + "0" + " cellspacing=" + "0" + " width=" + "100%" + ">" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("<tbody id=" + "tbodyFeedstuff_" + f + "+" + " name=" + "tbodyFeedstuff_" + f + "+" + ">" + "\r"));

                    if (arrPigHerdFeedplanList.Rows.Count == 0)
                    {
                        Panel1.Controls.Add(new LiteralControl("<tr>" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("<td>" + "\r"));
                        DropDownList selFeedstuff_ = new DropDownList();
                        selFeedstuff_.ID = "selFeedstuff_" + f.ToString();
                        selFeedstuff_.Items.Add("strSELECT_FEEDSTUFF");
                        selFeedstuff_.Items[0].Attributes.Add("dbValue", "-1");
                        for (int nf = 0; nf < (arrFeedStuff.Rows.Count - 1); nf++)
                        {
                            selFeedstuff_.Items.Add(arrFeedStuff.Rows[nf][0].ToString() + " " + arrFeedStuff.Rows[nf][1].ToString());
                            selFeedstuff_.Items[selFeedstuff_.Items.Count - 1].Attributes.Add("dbValue", arrFeedStuff.Rows[nf][0].ToString());
                        }
                        Panel1.Controls.Add(selFeedstuff_);
                        AllSelFeedstuff.Add(selFeedstuff_);
                        //         Panel1.Controls.Add(new LiteralControl("<select id="+"selFeedstuff_"+f+"+"+" name="+"selFeedstuff_"+f+"+"+" onchange="+"document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';checkForNewLine("+f+");setFeedstuffString("+f+");"+" style="+"position:relative;top:-3Px;background-color: lightyellow;"+">" + "\r"));
                        //	      Panel1.Controls.Add(new LiteralControl("  <option value="+"-1"+" selected>"+("strSELECT_FEEDSTUFF")+"</option>" + "\r"));
                        //   for (int nf = 0; nf < (arrFeedStuff.Rows.Count- 1); nf++)
                        //   {
                        //        Panel1.Controls.Add(new LiteralControl("  <option value=" + "+" + arrFeedStuff.Rows[nf][0] + "+" + ">" + arrFeedStuff.Rows[nf][1] + "</option>" + "\r"));
                        //		   }
                        //     Panel1.Controls.Add(new LiteralControl("</select>" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("<input type=" + "text" + " name=" + "feedstuffAmount_" + f + "+" + " id=" + "feedstuffAmount_" + f + "+" + " style=" + "width: 35px;position:relative;top:-3Px;background-color: lightyellow;" + " onchange=" + "document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';this.value=this.value.replace(',','.');setFeedstuffString(" + f + ");" + " value=" + "0" + "> " + ("strKgDM/animal/day") + "+" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("<img style=" + "position:relative;top:3Px;" + " src=" + "images/del.gif" + " onclick=" + "delRow(this," + f + ");" + " onMouseover=" + "mouseOverGif(this)" + "  onMouseout=" + "mouseOutGif(this)" + "/>&nbsp;&nbsp;" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("</tr>" + "\r"));
                    }
                    else
                    {
                        for (int fp = 0; fp < arrPigHerdFeedplanList.Rows.Count - 1; fp++)
                        {
                            strFeedplan = strFeedplan + arrPigHerdFeedplanList.Rows[fp][0] + ",";
                            strAmount = strAmount + arrPigHerdFeedplanList.Rows[fp][1] + ",";
                            Panel1.Controls.Add(new LiteralControl("<tr>" + "\r"));
                            Panel1.Controls.Add(new LiteralControl("<td>" + "\r"));
                            Panel1.Controls.Add(new LiteralControl("<select id=" + "selFeedstuff_" + f + "+" + " name=" + "selFeedstuff_" + f + "+" + " onchange=" + "document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';checkForNewLine(" + f + ");setFeedstuffString(" + f + ");" + " style=" + "position:relative;top:-3Px;background-color: white;" + ">" + "\r"));
                            Panel1.Controls.Add(new LiteralControl("  <option value=" + "-1" + ">" + ("strSELECT_FEEDSTUFF") + "</option>" + "\r"));
                            for (int nf = 0; nf < arrFeedStuff.Rows.Count - 1; nf++)
                            {
                                if (arrPigHerdFeedplanList.Rows.Count == 0)
                                {
                                    if (fp < arrPigHerdFeedplanList.Rows.Count)
                                        aFeedStuffID = (int)(arrPigHerdFeedplanList.Rows[fp][0]);
                                    aFeedstuffAmount = (int)(arrPigHerdFeedplanList.Rows[fp][1]);
                                }
                                else
                                {
                                    aFeedStuffID = -1;
                                    aFeedstuffAmount = 0;
                                }
                                if (aFeedStuffID == (int)arrFeedStuff.Rows[nf][0])
                                    Panel1.Controls.Add(new LiteralControl("  <option value=" + "+" + arrFeedStuff.Rows[nf][0] + "+" + " selected>" + arrFeedStuff.Rows[nf][1] + "</option>" + "\r"));
                                else
                                    Panel1.Controls.Add(new LiteralControl("  <option value=" + "+" + arrFeedStuff.Rows[nf][0] + "+" + ">" + arrFeedStuff.Rows[nf][1] + "</option>" + "\r"));

                            }
                            Panel1.Controls.Add(new LiteralControl("</select>" + "\r"));
                            Panel1.Controls.Add(new LiteralControl("<input id=" + "feedstuffAmount_" + f + "+" + " name=" + "feedstuffAmount_" + f + "+" + " type=" + "text" + " style=" + "width: 35px;position:relative;top:-3Px;background-color: white;" + " value=" + "+" + aFeedstuffAmount + "+" + " onchange=" + "document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';this.value=this.value.replace(',','.');setFeedstuffString(" + f + ");" + "/> " + ("strKgDM/animal/day") + "+" + "\r"));
                            Panel1.Controls.Add(new LiteralControl("<img style=" + "position:relative;top:3Px;" + " src=" + "images/del.gif" + " onclick=" + "delRow(this," + f + ");" + " onMouseover=" + "mouseOverGif(this)" + "  onMouseout=" + "mouseOutGif(this)" + "/>&nbsp;&nbsp;" + "\r"));
                            Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                            Panel1.Controls.Add(new LiteralControl("</tr>" + "\r"));
                            aFeedStuffID = -1;
                        }
                        Panel1.Controls.Add(new LiteralControl("<tr>" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("<td>" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("<select id=" + "selFeedstuff_" + f + "+" + " name=" + "selFeedstuff_" + f + "+" + " onchange=" + "document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';checkForNewLine(" + f + ");setFeedstuffString(" + f + ");" + " style=" + "position:relative;top:-3Px;background-color: lightyellow;" + ">" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("  <option value=" + "-1" + ">" + "strSELECT_FEEDSTUFF" + "</option>" + "\r"));
                        for (int nf = 0; nf < arrFeedStuff.Rows.Count - 1; nf++)
                        {
                            Panel1.Controls.Add(new LiteralControl("  <option value=" + "+" + arrFeedStuff.Rows[nf][0] + "+" + ">" + arrFeedStuff.Rows[nf][1] + "</option>" + "\r"));
                        }
                        Panel1.Controls.Add(new LiteralControl("</select>" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("<input type=" + "text" + " name=" + "feedstuffAmount_" + f + "+" + " id=" + "feedstuffAmount_" + f + "+" + " style=" + "width: 35px;position:relative;top:-3Px;background-color: lightyellow;" + " onchange=" + "document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';this.value=this.value.replace(',','.');setFeedstuffString(" + f + ");" + " value=" + "0" + "> " + "strKgDM/animal/day" + "+" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("<img style=" + "position:relative;top:3Px;" + " src=" + "images/del.gif" + " onclick=" + "delRow(this," + f + ");" + " onMouseover=" + "mouseOverGif(this)" + "  onMouseout=" + "mouseOutGif(this)" + "/>&nbsp;&nbsp;" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("</tr>" + "\r"));
                    }
                    Panel1.Controls.Add(new LiteralControl("</tbody>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("</table>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "hiddenFeedstuffList_" + f + "+" + "  id=" + "hiddenFeedstuffList_" + f + "+" + " value=" + "+" + strFeedplan + "+" + ">"));
                    strFeedplan = "+";
                    Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "hiddenAmountList_" + f + "+" + "  id=" + "hiddenAmountList_" + f + "+" + " value=" + "+" + strAmount + "+" + ">"));
                    Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + " style=" + "vertical-align: top;" + ">"));
                    DropDownList SelectStableType1 = new DropDownList();
                    SelectStableType1.ID = "SelectStableType44441_" + i.ToString();
                    for (int n = 0; n < arrStableType1.Rows.Count; n++)
                    {
                        string tmp = arrStableType1.Rows[n][0] + " " + arrStableType1.Rows[n][1];
                        SelectStableType1.Items.Add(arrStableType1.Rows[n][0] + " " + arrStableType1.Rows[n][1]);
                        SelectStableType1.Items[SelectStableType1.Items.Count - 1].Attributes.Add("dbValue", arrStableType1.Rows[n][0].ToString());
                        int stdd = SelectStableType1.Items.Count;
                        if (aStableType1ID.ToString() == arrStableType1.Rows[n][0].ToString())
                            SelectStableType1.SelectedIndex = n;
                    }
                    AllSelectStableType1.Add(SelectStableType1);
                    Panel1.Controls.Add(SelectStableType1);
                    //  Panel1.Controls.Add(new LiteralControl("<select style="+"width: 220px;"+" name="+"selectStableType1_"+i+"+"+" onChange="+"javascript:this.style.color='#ff0000';"+">"));
                    //CreateSelectList_Language aStableType1ID,nStableType1,arrStableType1
                    // Panel1.Controls.Add(new LiteralControl("</select><br/><br/>"))   ;
                    Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " id=" + "storageChanged1_" + i + "+" + " name=" + "storageChanged1_" + i + "+" + " value=" + "0" + "/>"));

                    paramter = new List<dbInterface.data>();
                    data = new dbInterface.data();
                    data.name = "@aFarmNumber";
                    data.value = HttpContext.Current.Session["farmNumber"].ToString();
                    paramter.Add(data);
                    data = new dbInterface.data();
                    data.name = "@aScenarioID";
                    data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
                    paramter.Add(data);
                    data = new dbInterface.data();
                    data.name = "@aPigHerdID";
                    data.value = arrPigHerd.Rows[i]["PigHerdID"].ToString();
                    paramter.Add(data);
                    data = new dbInterface.data();
                    data.name = "@aStableSubTypeID";
                    data.value = "1";
                    paramter.Add(data);
                    arrPigHerdManureStorageList = database.GetValuesFromDB("GetPigHerdManureStorageList ", paramter);
                    paramter = new List<dbInterface.data>();
                    data = new dbInterface.data();
                    data.name = "@aPigTypeID";
                    data.value = aPigTypeID.ToString();
                    paramter.Add(data);
                    data = new dbInterface.data();
                    data.name = "@aStableID";
                    data.value = aStableType1ID.ToString();
                    paramter.Add(data);

                    arrStorageType = database.GetValuesFromDB("GetStorageTypesPrPigStable ", paramter);

                    Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "nManureStorage1_" + i + "+" + "  id=" + "nManureStorage1_" + i + "+" + " value=" + "+" + arrStorageType.Rows.Count + "+" + "/>"));
                    Panel1.Controls.Add(new LiteralControl("<b>" + ("strManureStorageType") + "</b><br/>"));
                    for (int ms = 0; ms < arrStorageType.Rows.Count; ms++)
                    {

                        paramter = new List<dbInterface.data>();
                        data = new dbInterface.data();
                        data.name = "@aPigTypeID";
                        data.value = aPigTypeID.ToString();
                        paramter.Add(data);
                        data.name = "@aStableID";
                        data.value = aStableType1ID.ToString();
                        paramter.Add(data);
                        data.name = "@aStorageID";
                        data.value = arrStorageType.Rows[ms][0].ToString();
                        paramter.Add(data);
                        arrManureStorage = database.GetValuesFromDB("GetSelectListPigManureStorageType ", paramter);
                        aPigHerdManureStorageID = arrManureStorage.Rows[0][0].ToString();
                        if (arrPigHerdManureStorageList.Rows.Count > 0)
                        {
                            if (ms < arrPigHerdManureStorageList.Rows.Count)
                            {
                                aPigHerdManureStorageID = arrPigHerdManureStorageList.Rows[ms][1].ToString();
                            }
                        }

                        DropDownList SelectManureStorageType1 = new DropDownList();
                        SelectManureStorageType1.ID = "SelectManureStorageType1dfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdf" + i.ToString();
                        for (int n = 0; n < arrManureStorage.Rows.Count; n++)
                        {
                            SelectManureStorageType1.Items.Add(arrManureStorage.Rows[n][0] + " " + arrManureStorage.Rows[n][1]);
                            SelectManureStorageType1.Items[SelectManureStorageType1.Items.Count - 1].Attributes.Add("dbValue", arrManureStorage.Rows[n][0].ToString());
                            if (aPigHerdManureStorageID.ToString() == arrManureStorage.Rows[n][0].ToString())
                                SelectManureStorageType1.SelectedIndex = n;
                        }
                        AllSelectStableType1.Add(SelectManureStorageType1);
                        Panel1.Controls.Add(SelectManureStorageType1);
                        //  Panel1.Controls.Add(new LiteralControl("<select style="+"width: 220px;"+" name="+"selectManureStorageType1_"+i+"+"+" onChange="+"document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';"+">"));  
                        //CreateSelectList_ aPigHerdManureStorageID,nManureStorage,arrManureStorage
                        //    Panel1.Controls.Add(new LiteralControl("</select>"));
                        if (ms < arrStorageType.Rows.Count)
                            Response.Write("<br/><br/>");

                    }
                    Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                    // column 7 = delete buttons   
                    Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + ">"));
                    Panel1.Controls.Add(new LiteralControl("<button style=" + "width: 50px; height: 20px; font-size: 10px" + " onClick=" + "javascript:if(confirm('" + "strBekræft" + "')) {veil();document.forms.formPigHerd.action='PigHerdDelete.asp?PigHerdID=" + arrPigHerd.Rows[i]["PigHerdID"] + "';document.forms.formPigHerd.submit();}" + ">" + "strDelete" + "</button>"));
                    Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));

                    Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " colspan=" + "7" + "><hr/></td>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));
                    dictStableType = null;


                    strFeedplan = "+";
                    strAmount = "+";
                }
            }
            Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " colspan=" + "7" + ">"));
            Panel1.Controls.Add(new LiteralControl("&nbsp;&nbsp;<button style=" + "width: 100px; height: 20px; font-size: 10px" + " onClick=" + "javascript:veil();document.forms.formPigHerd.action='PigHerdNew.asp';document.forms.formPigHerd.submit();" + ">" + ("strNEW") + "</button>"));
            Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));


            Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));

            Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " colspan=" + "8" + "><hr/></td>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " colspan=" + "7" + ">"));
        //    Panel1.Controls.Add(new LiteralControl("&nbsp;&nbsp;<button style=" + "width: 100px; height: 20px; font-size: 10px" + " onClick=" + "javascript:veil();document.forms.formPigHerd.action='PigHerdUpdate.asp';document.forms.formPigHerd.submit();" + ">" + "strSAVE" + "</button>"));
            Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "hiddenNPigHerd" + " value=" + "" + arrPigHerd.Rows.Count + "+" + "/>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "hiddenPigType" + " id=" + "hiddenPigType" + " value=" + "2" + "/>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));

            Panel1.Controls.Add(new LiteralControl("  </table>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("</form>" + "\r"));
            CreateUpdateFormPig();

            database.CloseFarmNDB();


        }
        void CreatePigHerdTotal()
        {

            //dim nParameter, arrParameter, aPigTypeID, nPigHerdTotal, arrPigHerdTotal, dictPigHerdTotal
            // dim nFarmTotal, arrFarmTotal, dictFarmTotal, nPigType, arrPigType, dictPigType
            DataTable arrPigHerdTotal;
            int nFarmTotalm, aPigTypeID, nPigHerdTotal;
            DataTable arrParameter;
            DataTable arrPigType;

            DataTable arrFarmTotal;
            List<dbInterface.data> paramter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "@aProductionTypeID";
            data.value = "4";
            paramter.Add(data);
            //nPigType

            arrParameter = database.GetValuesFromDB("GetParameterList", paramter);

            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = (HttpContext.Current.Session["farmNumber"]).ToString();
            paramter.Add(data);
            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = (HttpContext.Current.Session["scenarioNumber"]).ToString();
            paramter.Add(data);
            arrPigType = database.GetValuesFromDB("GetPigHerdPigTypeList", paramter);
            data = new dbInterface.data();
            data.name = "@aProductionTypeID";
            data.value = ("4");
            paramter.Add(data);
            arrFarmTotal = database.GetValuesFromDB("GetFarmTotalList", paramter);


            // rows for totals
            Panel1.Controls.Add(new LiteralControl("  <br/>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("  <h4>+nbsp;" + ("strbehovprodgødning") + "</h4>" + "\r"));

            Panel1.Controls.Add(new LiteralControl("  <table class=" + "data" + " border=" + "0" + " cellspacing=" + "0" + " cellpadding=" + "0" + "  width=" + "80%" + ">" + "\r"));
            // row for header
            Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th class=" + "data" + " align=" + "left" + " nowrap>+nbsp;+nbsp;" + database.GetLanguageString("strHusdyrhold", language) + "</th>" + "\r"));

            for (int p = 0; p < arrParameter.Rows.Count; p++)
            {
                Panel1.Controls.Add(new LiteralControl("      <th class=" + "data" + " align=" + "right" + ">+nbsp;+nbsp;" + "str" + arrParameter.Rows[p]["ParameterName"] + "<br/>(" + "str" + arrParameter.Rows[p]["ParameterUnit"] + ")</th>" + "\r"));
            }
            Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));
            // rows for feeding requirements

            for (int t = 0; t < arrPigType.Rows.Count; t++)
            {

                aPigTypeID = (int)arrPigType.Rows[t]["PigType"];

                paramter = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.name = "@aFarmNumber";
                data.value = (string)(HttpContext.Current.Session["farmNumber"]);
                paramter.Add(data);
                data = new dbInterface.data();
                data.name = "@aScenarioID";
                data.value = (string)(HttpContext.Current.Session["scenarioNumber"]);
                paramter.Add(data);
                data = new dbInterface.data();
                data.name = "@aPigTypeID";
                data.value = (aPigTypeID.ToString());
                paramter.Add(data);
                arrPigHerdTotal = database.GetValuesFromDB("GetPigHerdTotalList", paramter);


                Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
                Panel1.Controls.Add(new LiteralControl("      <td nowrap>+nbsp;+nbsp;" + arrPigType.Rows[t]["PigType"] + " - " + "str" + arrPigType.Rows[t]["PigTypeName"] + "</td>" + "\r"));
                int ParameterValue1 = 0;


                for (int c = 0; c < arrPigHerdTotal.Rows.Count; c++)
                {
                    if ((int)arrPigHerdTotal.Rows[c]["ParameterValue1"] != 0)
                        Panel1.Controls.Add(new LiteralControl("      <td align=" + "right" + ">" + (int)arrPigHerdTotal.Rows[c]["ParameterValue1"] + "</td>" + "\r"));
                    else
                        Panel1.Controls.Add(new LiteralControl("      <td align=" + "right" + ">+nbsp;</td>" + "\r"));

                }
                Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));

            }

            Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <td colspan=" + "+" + arrParameter.Rows.Count + 2 + "+" + "><hr/></td>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));
            // row for total
            Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th class=" + "data" + " align=" + "left" + " nowrap>+nbsp;+nbsp;" + ("strTOTALALLGROUPS") + "</th>" + "\r"));


            for (int f = 0; f < arrFarmTotal.Rows.Count; f++)
            {
                Panel1.Controls.Add(new LiteralControl("      <th class=" + "data" + " align=" + "right" + ">" + arrFarmTotal.Rows[f]["ParameterValue"] + "</th>" + "\r"));
            }
            Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("  </table>" + "\r"));

        }

        void DefineChangeTextScriptPig()
        {

            List<dbInterface.data> paramter = new List<dbInterface.data>();

            DataTable arrPigType = database.GetValuesFromDB("GetPigTypeList", paramter);

            Panel1.Controls.Add(new LiteralControl("<script language=" + "JavaScript" + " type=" + "text/javascript" + ">" + "\r"));
            Panel1.Controls.Add(new LiteralControl("<!--" + "\r"));

            Panel1.Controls.Add(new LiteralControl("  function ChangeTextPig(i,t)" + "\r"));
            Panel1.Controls.Add(new LiteralControl("  {" + "\r"));
            Panel1.Controls.Add(new LiteralControl("     switch (t) {" + "\r"));

            for (int j = 0; (j < arrPigType.Rows.Count - 1); j++)
            {
                Panel1.Controls.Add(new LiteralControl("     case '" + arrPigType.Rows[j]["PigTypeID"] + "' :                         eval(" + "document.forms.formPigHerd.inputProductionVar1_" + "+i+" + ".value='" + arrPigType.Rows[j]["ProductionVar1Default"] + "'" + ");" + "\r")); ;
                Panel1.Controls.Add(new LiteralControl("                      eval(" + "document.forms.formPigHerd.inputProductionVar2_" + "+i+" + ".value='" + arrPigType.Rows[j]["ProductionVar2Default"] + "'" + ");" + "\r")); ;
                Panel1.Controls.Add(new LiteralControl("                      eval(" + "document.forms.formPigHerd.inputFeedLevel_" + "+i+" + ".value='" + arrPigType.Rows[j]["FeedLevelDefault"] + "'" + ");" + "\r")); ;
                Panel1.Controls.Add(new LiteralControl("                      eval(" + "document.forms.formPigHerd.inputProteinLevel_" + "+i+" + ".value='" + arrPigType.Rows[j]["ProteinLevelDefault"] + "'" + ");" + "\r")); ;
                Panel1.Controls.Add(new LiteralControl("                      break;" + "\r"));
            }
            Panel1.Controls.Add(new LiteralControl("                }" + "\r"));
            Panel1.Controls.Add(new LiteralControl("  }" + "\r"));

            Panel1.Controls.Add(new LiteralControl("//-->" + "\r"));
            Panel1.Controls.Add(new LiteralControl("</script>" + "\r"));
        }


        void CreateUpdateFormPig()
        {
            Panel1.Controls.Add(new LiteralControl("<form name=" + "update" + " action=" + "javascript:document.forms.formPigHerd.action='PigHerdUpdate.asp';document.forms.formPigHerd.submit();" + " method=" + "post" + ">" + "\r"));
            Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "saveUpdates" + " value=" + "false" + ">" + "\r"));
            Panel1.Controls.Add(new LiteralControl("</form>" + "\r"));
        }

        void PigTypeNotSows(DataTable arrPigType, ref List<List<string>> arrPigTypeNotSows)
        {
            arrPigTypeNotSows = new List<List<string>>();
            arrPigTypeNotSows.Add(new List<string>());
            arrPigTypeNotSows.Add(new List<string>());
            int j = 0;
            for (int i = 0; i < arrPigType.Rows.Count; i++)
            {
                if (arrPigType.Rows[i][0].ToString() != "1")
                {

                    //redim preserve arrPigTypeNotSows(1,j)
                    arrPigTypeNotSows[0].Add(arrPigType.Rows[i][0].ToString());
                    arrPigTypeNotSows[1].Add(arrPigType.Rows[i][1].ToString());

                    j = j + 1;
                }
            }
        }

        void saveEvent(Object sender, EventArgs e)
        {

        }
        void DeleteEvent(Object sender, EventArgs e)
        {
            Button knap = (Button)sender;
            string PigHerdID = knap.Attributes["PigHerdID"];
            PigHerdDelete tmp = new PigHerdDelete();
            tmp.ProcessRequest(HttpContext.Current, PigHerdID, farmNumber, scenarioNumber);
            HttpContext.Current.Session["UserInit"] = "6";
            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);
        }
        void newEvent(Object sender, EventArgs e)
        {
            PigHerdNew tmp = new PigHerdNew();
            tmp.ProcessRequest(HttpContext.Current, farmNumber, scenarioNumber);
        }

    }



}