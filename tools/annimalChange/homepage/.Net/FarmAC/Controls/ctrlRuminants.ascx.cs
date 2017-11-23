
using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.HtmlControls;
using System.Web.UI.WebControls;
using System.Xml;
using System.Globalization;
using System.Threading;
using System.Diagnostics;

namespace FarmAC.Controls
{
    public partial class WebUserControl1 : System.Web.UI.UserControl
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (HttpContext.Current.Session["UserInit"] != null)
                if (HttpContext.Current.Session["UserInit"].ToString() == "5")
            {
                CreateTabCattle();
            }
            //Panel1.Controls.Clear();
        }



        int aFeedstuffID;

        Label messages = new Label();
        int dictStableType;
        int[] dictCattleHerdFeedplanList;
        int[] dictCattleHerdManureStorageList;
        dbInterface database;
        DataTable arrCattleHerd;
        List<DropDownList> allSelectCattleType = new List<DropDownList>();
        List<List<DropDownList>> AllSelectManureStorageType = new List<List<DropDownList>>();


        List<DropDownList> AllSelectStableType = new List<DropDownList>();
        List<TextBox> AllInputAnimalYear = new List<TextBox>();
        List<TextBox> AllInputGrazing = new List<TextBox>();
        List<List<TextBox>> AllFeedstuffAmount = new List<List<TextBox>>();
        List<List<HtmlButton>> AllNewFeedstuffDelete = new List<List<HtmlButton>>();
        List<List<DropDownList>> AllSelFeedstuff = new List<List<DropDownList>>();

        List<HtmlButton> AllNewFeedstuff = new List<HtmlButton>();
        DataTable arrCattleHerdFeedplanList;
        void CreateTabCattle()
        {
            Panel1.Width = 1800;
     
            string language;
            if (HttpContext.Current.Session["languageSelected"] != null)
                language = HttpContext.Current.Session["languageSelected"].ToString();
            else
                language = "uk";


            int aCattleTypeID;

            DataTable arrCattleType;
            int aStableTypeID;
            DataTable arrStableType;

            DataTable arrFeedStuff;
            string arrFeedStuffWithGrazing, aFeedStuffID, g;


            string aCattleHerdFeedplanID;
            int fp;


            string aGrazingFeedStuffID, gf;
            int pf;

            DataTable arrGrazingResidueFeedStuff;
            string aGrazingResidueFeedStuffID;
            string arrParameter;
            DataTable arrGrazingFeedStuff;
            string aStorageTypeID;
            DataTable arrStorageType;
            string aManureStorageID;

            DataTable arrManureStorage;
            string message;
            DataTable arrCattleHerdManureStorageList;
            int aCattleHerdManureStorageID = 0; ;

            message = Request["message"];
            //   response.write message 
            database = new dbInterface();
            database.InitFarmNDB();
            // CreateLanguageDictionary
            List<dbInterface.data> paramter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = HttpContext.Current.Session["farmNumber"].ToString();
            paramter.Add(data);
            data.name = "@aScenarioID";

            data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            paramter.Add(data);
            arrCattleHerd = database.GetValuesFromDB("GetCattleHerdList", paramter);
            int st = arrCattleHerd.Rows.Count;
            paramter = new List<dbInterface.data>();
            data.name = "@AgroEcologicalZone";
            data.value = HttpContext.Current.Session["AgroEcologicalZone"].ToString();
            paramter.Add(data);
            arrCattleType = database.GetValuesFromDB("GetSelectListCattleType", paramter);
            paramter = new List<dbInterface.data>();
            arrFeedStuff = database.GetValuesFromDB("GetFeedStuffList", paramter);

            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = HttpContext.Current.Session["farmNumber"].ToString();
            paramter.Add(data);
            data.name = "@aScenarioID";
            data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            paramter.Add(data);
            arrGrazingFeedStuff = database.GetValuesFromDB("GetProducedFeedstuffList", paramter);

            int size = arrGrazingFeedStuff.Rows.Count;
            arrGrazingResidueFeedStuff = database.GetValuesFromDB("GetProducedResidueGrazed", paramter);

            DefineChangeTextScript();
            // DefineFeedstuffListJScript_18();
            Panel1.Controls.Add(new LiteralControl("<script language=" + "javascript" + ">window.parent.frames[1].location.reload()</script>" + "\r"));

            Panel1.Controls.Add(new LiteralControl("<form name=" + "formCattleHerd" + " action=" + "CattleHerdUpdate.asp" + " method=" + "post" + " id=" + "dynamichide" + " style=" + "display: block;" + ">" + "\r"));
            Panel1.Controls.Add(new LiteralControl("  <table class=" + "data" + " border=" + "0" + " style=" + "width: 1800px;" + ">" + "\r"));

            Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + " style=" + "width: 100px;" + ">" + database.GetLanguageString("strHusdyrhold", language) + "</th>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + " style=" + "width: 100px;" + ">" + database.GetLanguageString("strANANIMAL", language) + "</th>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + " style=" + "width: 270px;" + "></th>" + "\r"));

            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + " style=" + "width: 680px;" + ">" + database.GetLanguageString("strFeedstuffPlan", language) + "</th>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + database.GetLanguageString("strHOUSETYPE", language) + "</th>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + database.GetLanguageString("strManureStorageType", language) + "</th>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">&nbsp;</th>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));
            int fdfsf = arrCattleHerd.Rows.Count;
            for (int i = 0; i < (arrCattleHerd.Rows.Count); i++)
            {
                AllFeedstuffAmount.Add(new List<TextBox>());
                AllSelectManureStorageType.Add(new List<DropDownList>());
                AllNewFeedstuffDelete.Add(new List<HtmlButton>());
                List<DropDownList> tmp = new List<DropDownList>();
                AllSelFeedstuff.Add(tmp);
                int sdfsdf = AllSelFeedstuff[0].Count();
                paramter = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.name = "@aFarmNumber";
                data.value = HttpContext.Current.Session["farmNumber"].ToString();
                paramter.Add(data);
                data.name = "@aScenario";
                data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
                paramter.Add(data);
                data.name = "@aCattleHerdID";
                data.value = arrCattleHerd.Rows[i]["CattleHerdID"].ToString();
                paramter.Add(data);
                arrCattleHerdFeedplanList = database.GetValuesFromDB("GetCattleHerdFeedplanList", paramter);

                paramter = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.name = "@aFarmNumber";
                data.value = HttpContext.Current.Session["farmNumber"].ToString();
                paramter.Add(data);
                data.name = "@aScenario";
                data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
                paramter.Add(data);
                data.name = "@aCattleHerdID";
                data.value = arrCattleHerd.Rows[i]["CattleHerdID"].ToString();
                paramter.Add(data);
                arrCattleHerdManureStorageList = database.GetValuesFromDB("GetCattleHerdManureStorageList", paramter);

                aCattleTypeID = (int)arrCattleHerd.Rows[i]["CattleType"];

                paramter = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.name = "@aCattleTypeID";
                data.value = aCattleTypeID.ToString();
                paramter.Add(data);
                arrStableType = database.GetValuesFromDB("GetSelectListStableType ", paramter);

                aStableTypeID = (int)arrCattleHerd.Rows[i]["Stable"];
                Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));

                Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + ">"));

                //  Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "hiddenCattleHerdID_" + i + "+" + " value=" + "+" + arrCattleHerd.Rows[i]["CattleHerdID"] + "+" + ">"));
                //  Panel1.Controls.Add(new LiteralControl("<select name=" + "selectCattleType_" + i + "+" + " onChange=" + "ChangeText(" + i + ",this.options[this.selectedIndex].value);document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';" + ">"));

                DropDownList SelectCattleType = new DropDownList();
                SelectCattleType.ID = "SelectCattleType" + i.ToString();
                for (int j = 0; j < arrCattleType.Rows.Count; j++)
                {
                    SelectCattleType.Items.Add(arrCattleType.Rows[j][0].ToString() + " " +database.GetLanguageString("str"+ arrCattleType.Rows[j][1].ToString(),language));
                    SelectCattleType.Items[j].Attributes.Add("dbValue", arrCattleType.Rows[j][0].ToString());
                    if (arrCattleType.Rows[j][0].ToString() == aCattleTypeID.ToString())
                    {

                        SelectCattleType.SelectedIndex = j;
                    }
                }
                allSelectCattleType.Add(SelectCattleType);
                allSelectCattleType[allSelectCattleType.Count - 1].SelectedIndexChanged += new EventHandler(saveEvent);
                allSelectCattleType[allSelectCattleType.Count - 1].SelectedIndexChanged += new EventHandler(reload);
                allSelectCattleType[allSelectCattleType.Count - 1].AutoPostBack = true;

                Panel1.Controls.Add(SelectCattleType);
                //  CreateSelectList_Language aCattleTypeID,nCattleType,arrCattleType
                //  Panel1.Controls.Add(new LiteralControl("</select>"));
                Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                // column 2 = animals pr. year (årsdyr)   
                Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + ">&nbsp;&nbsp;"));
                TextBox inputAnimalYear = new TextBox();
                inputAnimalYear.Text = arrCattleHerd.Rows[i]["AnimalYear"].ToString();
                inputAnimalYear.ID = "inputAnimalYear" + i.ToString();
                inputAnimalYear.Width = 30;
                AllInputAnimalYear.Add(inputAnimalYear);
                Panel1.Controls.Add(inputAnimalYear);
                Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                //   Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + ">&nbsp;&nbsp;<input type=" + "text" + " name=" + "inputAnimalYear_" + i + "+" + " value=" + "+" + arrCattleHerd.Rows[i]["Grazing"] + "+" + " maxlength=" + "5" + " size=" + "2" + " onChange=" + "document.forms.formCattleHerd.action='CattleHerdUpdate.asp';document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';" + " onkeydown=" + "return false();" + " onkeypress=" + "return false();" + " /></td>" + "\r"));
                // column 3 = grazing
                Panel1.Controls.Add(new LiteralControl("      <input type=" + "hidden" + " name=" + "inputGrazing_" + i + "+" + " value=" + "+" + arrCattleHerd.Rows[i]["Grazing"] + "+" + "/>" + "\r"));
                // column 4 = production level

                if (aCattleTypeID == 7)
                    Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + "><input type=" + "hidden" + " name=" + "inputProductionLevel_" + i + "+" + " id=" + "inputProductionLevel_" + i + "+" + " value=" + "+" + arrCattleHerd.Rows[i]["ProductionLevel"] + "+" + "/>&nbsp;<input type=" + "hidden" + " id=" + "ProductionLevelUnit_" + i + "+" + " name=" + "ProductionLevelUnit_" + i + "+" + " value=" + "+" + "str" + arrCattleHerd.Rows[i]["ProductionLevelUnit"] + "+" + "></input>&nbsp;&nbsp;<input class=" + "+" + " type=" + "hidden" + " name=" + "inputProductionLevel2_" + i + "+" + " id=" + "inputProductionLevel2_" + i + "+" + " value=" + "+" + arrCattleHerd.Rows[i]["ProductionLevelUnit"] + "+" + "/>&nbsp;<input class=" + "show" + " type=" + "hidden" + " id=" + "ProductionLevelUnit2_" + i + "+" + " name=" + "ProductionLevelUnit2_" + i + "+" + " value=" + "+" + "strkg kalv" + "+" + "></input></td>" + "\r"));
                else
                    Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + "><input type=" + "hidden" + " name=" + "inputProductionLevel_" + i + "+" + " id=" + "inputProductionLevel_" + i + "+" + " value=" + "+" + arrCattleHerd.Rows[i]["ProductionLevel"] + "+" + "/>&nbsp;<input type=" + "hidden" + " id=" + "ProductionLevelUnit_" + i + "+" + " name=" + "ProductionLevelUnit_" + i + "+" + " value=" + "+" + "str" + arrCattleHerd.Rows[i]["ProductionLevelUnit"] + "+" + "></input>&nbsp;&nbsp;<input class=" + "hide" + " type=" + "hidden" + " name=" + "inputProductionLevel2_" + i + "+" + " id=" + "inputProductionLevel2_" + i + "+" + " value=" + "+" + arrCattleHerd.Rows[i]["ProductionLevelUnit"] + "+" + "/>&nbsp;<input class=" + "hide" + " type=" + "hidden" + " id=" + "ProductionLevelUnit2_" + i + "+" + " name=" + "ProductionLevelUnit2_" + i + "+" + " value=" + "+" + "strkg kalv" + "+" + "></input></td>" + "\r"));


                Panel1.Controls.Add(new LiteralControl("<td>" + "\r"));
                Panel1.Controls.Add(new LiteralControl("<table border=" + "0" + " cellspacing=" + "0" + " width=" + "100%" + ">" + "\r"));
                Panel1.Controls.Add(new LiteralControl("<tbody id=" + "tbodyFeedstuff_" + i + "+" + " name=" + "tbodyFeedstuff_" + i + "+" + ">" + "\r"));

                List<DropDownList> tmpFeedstuff = new List<DropDownList>();
                if (arrCattleHerdFeedplanList.Rows.Count == 0)
                {
                    pf = 0;
                    Panel1.Controls.Add(new LiteralControl("<tr>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("<td>" + "\r"));

                    for (int l = 0; l < AllSelFeedstuff[i].Count; l++)
                    {
                        Panel1.Controls.Add(AllSelFeedstuff[i][l]);
                        AllSelFeedstuff[i][l].SelectedIndexChanged += new EventHandler(feedStuffEvent);
                        AllSelFeedstuff[i][l].AutoPostBack = true;
                    }
                    HtmlButton newFeedstuff = new HtmlButton();
                    newFeedstuff.InnerText = "new Feedstuff";
                    newFeedstuff.ID = "FeedStuffButton" + i.ToString();
                    newFeedstuff.ServerClick += new EventHandler(feedStuffEvent);
                    AllNewFeedstuff.Add(newFeedstuff);
                    Panel1.Controls.Add(newFeedstuff);
                    Panel1.Controls.Add(new LiteralControl("<br>"));
                    // Panel1.Controls.Add(new LiteralControl("<select id=" + "selFeedstuff_" + i + "+" + " name=" + "selFeedstuff_" + i + "+" + " onchange=" + "checkForNewLine(" + i + ");setFeedstuffString(" + i + ");" + " style=" + "position:relative;top:-3Px;background-color: lightyellow;" + ">" + "\r"));
                    // Panel1.Controls.Add(new LiteralControl("  <option value=" + "-1" + ">" + ("strSELECT_FEEDSTUFF") + "</option>" + "\r"));
                    /*  for (int nf = 0; nf < arrFeedStuff.Rows.Count - 1; nf++)
                      {
                          if (arrGrazingFeedStuff.Rows.Count != 0)
                          {
                              if (arrGrazingFeedStuff.Rows[pf][0] == arrFeedStuff.Rows[nf][0])
                              {
                                  Panel1.Controls.Add(new LiteralControl("  <option value=" + "+" + arrFeedStuff.Rows[nf][0] + "+" + " class=" + "back" + ">" + arrFeedStuff.Rows[nf][1] + "</option>" + "\r"));
                                  if (pf < arrFeedStuff.Rows.Count - 1)
                                      pf = pf + 1;
                                  else
                                      Panel1.Controls.Add(new LiteralControl("  <option value=" + "+" + arrFeedStuff.Rows[nf][0] + "+" + ">" + arrFeedStuff.Rows[nf][1] + "</option>" + "\r"));

                              }
                              else
                                  Panel1.Controls.Add(new LiteralControl("  <option value=" + "+" + arrFeedStuff.Rows[nf][0] + "+" + ">" + arrFeedStuff.Rows[nf][1] + "</option>" + "\r"));
                          }
                      }*/
                    //   Panel1.Controls.Add(new LiteralControl("</select>" + "\r"));
                    /*
                    TextBox FeedstuffAmountbox=new TextBox();
                    FeedstuffAmountbox.ID = "FeedstuffAmountbox" + i.ToString();
                    FeedstuffAmountbox.Text= aFeedstuffAmount.ToString();
                    AllFeedstuffAmount.Add(FeedstuffAmountbox);
                    Panel1.Controls.Add(FeedstuffAmountbox);

                    HtmlButton NewFeedstuffDelete = new HtmlButton();

                    NewFeedstuffDelete.ID = "Delete" + i.ToString();
                    NewFeedstuffDelete.InnerHtml = "Delete";
                    AllNewFeedstuffDelete.Add(NewFeedstuffDelete);
                    Panel1.Controls.Add(NewFeedstuffDelete);
                     * */

                    //Panel1.Controls.Add(new LiteralControl("<input id=" + "feedstuffAmount_" + i + "+" + " name=" + "feedstuffAmount_" + i + "+" + " type=" + "text" + " style=" + "width: 35px;position:relative;top:-3Px;background-color: lightyellow;" + " value=" + "+" + aFeedstuffAmount + "+" + " onchange=" + "this.value=this.value.replace(',','.');setFeedstuffString(" + i + ");" + "/> " + ("strKgDM/animal/day") + "+" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("<img style=" + "position:relative;top:3Px;" + " src=" + "images/del.gif" + " onclick=" + "delRow(this," + i + ");" + " onMouseover=" + "mouseOverGif(this)" + "  onMouseout=" + "mouseOutGif(this)" + "/>&nbsp;&nbsp;" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));

                    Panel1.Controls.Add(new LiteralControl("</tr>" + "\r"));
                }
                else
                {

                    for (fp = 0; fp < ((int)arrCattleHerdFeedplanList.Rows.Count); fp++)
                    {
                        pf = 0;
                        Panel1.Controls.Add(new LiteralControl("<tr>" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("<td>" + "\r"));
                        //Panel1.Controls.Add(new LiteralControl("<select id=" + "selFeedstuff_" + i + "+" + " name=" + "selFeedstuff_" + i + "+" + " onchange=" + "checkForNewLine(" + i + ");setFeedstuffString(" + i + ");" + " style=" + "position:relative;top:-3Px;background-color: white;" + ">" + "\r"));
                        //Panel1.Controls.Add(new LiteralControl("  <option value=" + "-1" + ">" + "strSELECT_FEEDSTUFF" + "</option>" + "\r"));
                        DropDownList selFeedstuff = new DropDownList();
                        selFeedstuff.AutoPostBack = true;
                        selFeedstuff.ID = "arrCattleHerdFeedplanList" + fp.ToString() + "_" + i.ToString();
                        selFeedstuff.Attributes.Add("fp", fp.ToString());
                        for (int nf = 0; nf < (arrFeedStuff.Rows.Count); nf++)
                        {
                            int stff = arrCattleHerdFeedplanList.Rows.Count;


                            if (arrCattleHerdFeedplanList.Rows.Count != 0)
                            {
                                if (fp < (int)arrCattleHerdFeedplanList.Rows.Count)
                                {
                                    aFeedstuffID = (int)arrCattleHerdFeedplanList.Rows[fp][0];


                                }
                                else
                                {
                                    aFeedstuffID = -1;

                                }
                            }

                            string tmp1 = aFeedstuffID.ToString();
                            string tmp2 = arrFeedStuff.Rows[nf][0].ToString();
                            if (tmp2.CompareTo("584") == 0)
                            {
                                int tmp3 = arrGrazingFeedStuff.Rows.Count;

                            }

                            
                                if (arrGrazingFeedStuff.Rows.Count != 0)
                                {
                                    string tmpsss = arrGrazingFeedStuff.Rows[pf][0].ToString();
                                    if (arrGrazingFeedStuff.Rows[pf][0].ToString().CompareTo( arrFeedStuff.Rows[nf][0].ToString())==0)
                                    {
                                        // Panel1.Controls.Add(new LiteralControl("  <option value=" + "+" + arrFeedStuff.Rows[nf][0] + "+" + " class=" + "back" + " selected>" + arrFeedStuff.Rows[nf][1] + "</option>" + "\r"));
                                        selFeedstuff.Items.Add(arrFeedStuff.Rows[nf][0].ToString() + " " + arrFeedStuff.Rows[nf][1]);
                                        selFeedstuff.Items[selFeedstuff.Items.Count - 1].Attributes.Add("dbValue", arrFeedStuff.Rows[nf][0].ToString());

                                        selFeedstuff.Items[selFeedstuff.Items.Count - 1].Attributes.Add("style", "background-color: green;color: white");
                                     
                                        if (pf < arrGrazingFeedStuff.Rows.Count - 1)
                                            pf = pf + 1;
                                        else
                                        {
                                            //Panel1.Controls.Add(new LiteralControl("  <option value=" + "+" + arrFeedStuff.Rows[nf][0] + "+" + " selected>" + arrFeedStuff.Rows[nf][1] + "</option>" + "\r"));
                                            selFeedstuff.SelectedIndex = nf;
                                        }
                                    }
                                    else
                                    {
                                        //   Panel1.Controls.Add(new LiteralControl("  <option value=" + "+" + arrFeedStuff.Rows[nf][0] + "+" + " selected>" + arrFeedStuff.Rows[nf][1] + "</option>" + "\r"));
                                        selFeedstuff.Items.Add(arrFeedStuff.Rows[nf][0].ToString() + " " + arrFeedStuff.Rows[nf][1]);
                                        selFeedstuff.Items[selFeedstuff.Items.Count - 1].Attributes.Add("dbValue", arrFeedStuff.Rows[nf][0].ToString());
                                       // selFeedstuff.SelectedIndex = nf;
                                        //  selFeedstuff.Items[selFeedstuff.Items.Count - 1].Attributes.Add("style", "background-color: green;color: white");
                                    }
                                }
                                else if (arrGrazingFeedStuff.Rows.Count != 0)
                                {
                                    if (arrGrazingFeedStuff.Rows[pf][0] == arrFeedStuff.Rows[nf][0])
                                    {
                                        // Panel1.Controls.Add(new LiteralControl("  <option value=" + "+" + arrFeedStuff.Rows[nf][0] + "+" + "  class=" + "back" + ">" + arrFeedStuff.Rows[nf][1] + "</option>" + "\r"));
                                        selFeedstuff.Items.Add(arrFeedStuff.Rows[nf][0].ToString() + " " + arrFeedStuff.Rows[nf][1]);
                                        selFeedstuff.Items[selFeedstuff.Items.Count - 1].Attributes.Add("dbValue", arrFeedStuff.Rows[nf][0].ToString());
                                        if (pf < arrGrazingFeedStuff.Rows.Count - 1)
                                            pf = pf + 1;
                                        else
                                        {
                                            selFeedstuff.Items.Add(arrFeedStuff.Rows[nf][0].ToString() + " " + arrFeedStuff.Rows[nf][1]);
                                            selFeedstuff.Items[selFeedstuff.Items.Count - 1].Attributes.Add("dbValue", arrFeedStuff.Rows[nf][0].ToString());
                                        }
                                        // Panel1.Controls.Add(new LiteralControl("  <option value=" + "+" + arrFeedStuff.Rows[nf][0] + "+" + ">" + arrFeedStuff.Rows[nf][1] + "</option>" + "\r"));

                                    }
                                }
                                else
                                {
                                    //Panel1.Controls.Add(new LiteralControl("  <option value=" + "+" + arrFeedStuff.Rows[nf][0] + "+" + ">" + arrFeedStuff.Rows[nf][1] + "</option>" + "\r"));
                                    selFeedstuff.Items.Add(arrFeedStuff.Rows[nf][0].ToString() + " " + arrFeedStuff.Rows[nf][1]);
                                    selFeedstuff.Items[selFeedstuff.Items.Count - 1].Attributes.Add("dbValue", arrFeedStuff.Rows[nf][0].ToString());
                                   
                                }

                           



                        }
                        for (int index = 0; index < selFeedstuff.Items.Count; index++)
                        {
                            object ID=selFeedstuff.Items[index].Attributes["dbValue"];
                            string sts = ID.ToString();
                            if (int.Parse(sts)==aFeedstuffID)
                            {
                                selFeedstuff.SelectedIndex = index;
                            }
                        }
                        int count = selFeedstuff.Items.Count;
                        if (selFeedstuff.Items.Count == 0)
                        {
                            selFeedstuff.Items.Add("Select feedStuff");

                            selFeedstuff.Items[0].Attributes.Add("dbValue", "-1");


                            selFeedstuff.SelectedIndex = 0;
                        }
                        tmpFeedstuff.Add(selFeedstuff);
                        //Panel1.Controls.Add(new LiteralControl("</select>" + "\r"));
                        // Panel1.Controls.Add(new LiteralControl("<input id=" + "feedstuffAmount_" + i + "+" + " name=" + "feedstuffAmount_" + i + "+" + " type=" + "text" + " style=" + "width: 35px;position:relative;top:-3Px;background-color: white;" + " value=" + "+" + aFeedstuffAmount + "+" + " onchange=" + "this.value=this.value.replace(',','.');setFeedstuffString(" + i + ");" + "/> " + "strKgDM/animal/day" + "+" + "\r"));
                        // Panel1.Controls.Add(new LiteralControl("<img style=" + "position:relative;top:3Px;" + " src=" + "images/del.gif" + " onclick=" + "delRow(this," + i + ");" + " onMouseover=" + "mouseOverGif(this)" + "  onMouseout=" + "mouseOutGif(this)" + "/>&nbsp;&nbsp;" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("</tr>" + "\r"));
                    }

                    for (int l = 0; l < tmpFeedstuff.Count; l++)
                    {
                        bool found = false;
                        int tpdsgfsdg = AllSelFeedstuff[i].Count;
                        for (int k = 0; k < AllSelFeedstuff[i].Count; k++)
                        {
                            if (tmpFeedstuff[l].Attributes["fp"] == AllSelFeedstuff[i][k].Attributes["fp"])
                            {
                                found = true;
                                break;
                            }
                        }
                        if (found == false)
                        {
                            AllSelFeedstuff[i].Add(tmpFeedstuff[l]);
                        }
                    }
                    for (int l = 0; l < AllSelFeedstuff[i].Count; l++)
                    {

                        if (AllSelFeedstuff[i][l].Items.Count != 0)
                        {
                            Panel1.Controls.Add(AllSelFeedstuff[i][l]);
                            AllSelFeedstuff[i][l].SelectedIndexChanged += new EventHandler(saveEvent);
                            AllSelFeedstuff[i][l].SelectedIndexChanged += new EventHandler(reload);
                            AllSelFeedstuff[i][l].AutoPostBack = true;
                            TextBox FeedstuffAmountbox = new TextBox();
                            FeedstuffAmountbox.ID = "FeedstuffAmountbox" + i.ToString() + " " + l.ToString();
                            try
                            {
                                FeedstuffAmountbox.Text = arrCattleHerdFeedplanList.Rows[l][1].ToString();
                            }
                            catch
                            {
                                FeedstuffAmountbox.Text = "0";
                            }
                            FeedstuffAmountbox.Width = 30;
                            AllFeedstuffAmount[i].Add(FeedstuffAmountbox);
                            Panel1.Controls.Add(FeedstuffAmountbox);
                            Panel1.Controls.Add(new LiteralControl("KgDM/animal/day"));
                            HtmlButton NewFeedstuffDelete = new HtmlButton();
                            NewFeedstuffDelete.ServerClick += new EventHandler(feedDeleteEvent);
                            NewFeedstuffDelete.ID = "DeleteFeed." + i.ToString() + " " + l.ToString();
                            NewFeedstuffDelete.InnerHtml = "Delete";
                            AllNewFeedstuffDelete[i].Add(NewFeedstuffDelete);
                            Panel1.Controls.Add(NewFeedstuffDelete);

                        }
                    }


                    HtmlButton newFeedstuff = new HtmlButton();
                    newFeedstuff.InnerText = "new Feedstuff";
                    newFeedstuff.ID = "FeedStuffButton" + i.ToString();
                    newFeedstuff.ServerClick += new EventHandler(feedStuffEvent);
                    AllNewFeedstuff.Add(newFeedstuff);
                    Panel1.Controls.Add(newFeedstuff);


                    pf = 0;
                    Panel1.Controls.Add(new LiteralControl("<tr>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("<td>" + "\r"));
                    // Panel1.Controls.Add(new LiteralControl("<select id=" + "selFeedstuff_" + i + "+" + " name=" + "selFeedstuff_" + i + "+" + " onchange=" + "checkForNewLine(" + i + ");setFeedstuffString(" + i + ");" + " style=" + "position:relative;top:-3Px;background-color: lightyellow;" + ">" + "\r"));
                    //Panel1.Controls.Add(new LiteralControl("  <option value=" + "-1" + ">" + "strSELECT_FEEDSTUFF" + "</option>" + "\r"));
                    /*for (int nf = 0; nf < (arrFeedStuff.Rows.Count - 1); nf++)
                    {
                        if (arrGrazingFeedStuff.Rows.Count != 0)
                        {
                            if (arrGrazingFeedStuff.Rows[pf][0] == arrFeedStuff.Rows[nf][0])
                            {
                                Panel1.Controls.Add(new LiteralControl("  <option value=" + "+" + arrFeedStuff.Rows[nf][0] + "+" + " class=" + "back" + ">" + arrFeedStuff.Rows[nf][1] + "</option>" + "\r"));

                                if (pf < arrGrazingFeedStuff.Rows.Count - 1)
                                {
                                    pf = pf + 1;
                                }
                                else
                                {
                                    Panel1.Controls.Add(new LiteralControl("  <option value=" + "+" + arrFeedStuff.Rows[nf][0] + "+" + ">" + arrFeedStuff.Rows[nf][1] + "</option>" + "\r"));
                                }
                            }
                            else
                                Panel1.Controls.Add(new LiteralControl("  <option value=" + "+" + arrFeedStuff.Rows[nf][0] + "+" + ">" + arrFeedStuff.Rows[nf][1] + "</option>" + "\r"));

                        }
                    }*/
                    //Panel1.Controls.Add(new LiteralControl("</select>" + "\r"));
                    //Panel1.Controls.Add(new LiteralControl("<input id=" + "feedstuffAmount_" + i + "+" + " name=" + "feedstuffAmount_" + i + "+" + " type=" + "text" + " style=" + "width: 35px;position:relative;top:-3Px;background-color: lightyellow;" + " value=" + "0" + " onchange=" + "this.value=this.value.replace(',','.');setFeedstuffString(" + i + ");" + "/> " + "strKgDM/animal/day" + "+" + "\r"));
                    //Panel1.Controls.Add(new LiteralControl("<img style=" + "position:relative;top:3Px;" + " src=" + "images/del.gif" + " onclick=" + "delRow(this," + i + ");" + " onMouseover=" + "mouseOverGif(this)" + "  onMouseout=" + "mouseOutGif(this)" + "/>&nbsp;&nbsp;" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("</tr>" + "\r"));
                }

                Panel1.Controls.Add(new LiteralControl("</tbody>" + "\r"));
                Panel1.Controls.Add(new LiteralControl("</table>" + "\r"));

                //  Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "hiddenFeedstuffList_" + i + "+" + "  id=" + "hiddenFeedstuffList_" + i + "+" + " value=" + "+" + strFeedplan + "+" + "/>"));
                // Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "hiddenAmountList_" + i + "+" + "  id=" + "hiddenAmountList_" + i + "+" + " value=" + "+" + strAmount + "+" + "/>"));
                Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));


                Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + ">"));
                //      Panel1.Controls.Add(new LiteralControl("&nbsp;&nbsp;<select name=" + "selectStableType_" + i + "+" + " onChange=" + "document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';document.forms.formCattleHerd.action='CattleHerdUpdate.asp';document.getElementById('storageChanged_" + i + "').value='1';document.forms.formCattleHerd.submit();" + ">"));
                DropDownList SelectStableType = new DropDownList();
                SelectStableType.ID = "SelectStableTyperrrr" + i.ToString();

                for (int tp = 0; tp < arrStableType.Rows.Count; tp++)
                {
                    SelectStableType.Items.Add(arrStableType.Rows[tp][0].ToString() + " " +database.GetLanguageString( "str"+arrStableType.Rows[tp][1].ToString(),language));
                    SelectStableType.Items[tp].Attributes.Add("dbValue", arrStableType.Rows[tp][0].ToString());
                    if (aStableTypeID.ToString() == arrStableType.Rows[tp][0].ToString())
                    {
                        SelectStableType.SelectedIndex = tp;
                        string tmp1 = arrStableType.Rows[tp][0].ToString();
                        string tmp2 = arrStableType.Rows[tp][1].ToString();
                    }
                }
                AllSelectStableType.Add(SelectStableType);
                AllSelectStableType[AllSelectStableType.Count - 1].SelectedIndexChanged += new EventHandler(saveEvent);
                AllSelectStableType[AllSelectStableType.Count - 1].SelectedIndexChanged += new EventHandler(reload);
                AllSelectStableType[AllSelectStableType.Count - 1].AutoPostBack = true;
                //  CreateSelectList_Language aStableTypeID,nStableType,arrStableType
                //  Panel1.Controls.Add(new LiteralControl("</select>"));
                Panel1.Controls.Add(SelectStableType);
                paramter = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.name = "@aCattleTypeID";
                data.value = aCattleTypeID.ToString(); ;
                paramter.Add(data);
                data = new dbInterface.data();
                data.name = "@aStableID";
                data.value = aStableTypeID.ToString();
                paramter.Add(data);
                arrStorageType = database.GetValuesFromDB("GetStorageTypesPrStable", paramter);
                Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "nManureStorage_" + i + "+" + "  id=" + "nManureStorage_" + i + "+" + " value=" + "+" + arrStorageType.Rows.Count + "+" + "/>"));
                Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + ">"));

                for (int ms = 0; ms < arrStorageType.Rows.Count; ms++)
                {

                    paramter = new List<dbInterface.data>();
                    data = new dbInterface.data();
                    data.name = "@aCattleTypeID";
                    data.value = aCattleTypeID.ToString(); ;
                    paramter.Add(data);
                    data = new dbInterface.data();
                    data.name = "@aStableID";
                    data.value = aStableTypeID.ToString(); ;
                    paramter.Add(data);
                    data = new dbInterface.data();
                    data.name = "@aStorageID";
                    data.value = arrStorageType.Rows[ms][0].ToString();
                    paramter.Add(data);
                    arrManureStorage = database.GetValuesFromDB("GetSelectListManureStorageType", paramter);
                    if (arrCattleHerdManureStorageList.Rows.Count > 0)
                    {
                        if (ms < arrCattleHerdManureStorageList.Rows.Count)
                        {
                            aCattleHerdManureStorageID = (int)arrCattleHerdManureStorageList.Rows[ms][1];
                        }

                    }
                    //  Panel1.Controls.Add(new LiteralControl("&nbsp;&nbsp;<select style=" + "width: 200px;" + " name=" + "selectManureStorageType_" + i + "+" + " onChange=" + "document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';" + ">"));
                    // CreateSelectList_ aCattleHerdManureStorageID,nManureStorage,arrManureStorage
                    DropDownList selectManureStorageType = new DropDownList();

                    selectManureStorageType.ID = "selectManureStorageType" + i.ToString() + "_" + ms.ToString();
                    for (int it = 0; it < arrManureStorage.Rows.Count; it++)
                    {
                        selectManureStorageType.Items.Add(arrManureStorage.Rows[it][0] + " " + arrManureStorage.Rows[it][1]);
                        selectManureStorageType.Items[it].Attributes.Add("dbValue", arrManureStorage.Rows[it][0].ToString());
                        if (aCattleHerdManureStorageID.ToString() == arrManureStorage.Rows[it][0].ToString())
                            selectManureStorageType.SelectedIndex = it;

                    }


                    AllSelectManureStorageType[i].Add(selectManureStorageType);
                    AllSelectManureStorageType[i][AllSelectManureStorageType[i].Count - 1].SelectedIndexChanged += new EventHandler(saveEvent);
                    AllSelectManureStorageType[i][AllSelectManureStorageType[i].Count - 1].SelectedIndexChanged += new EventHandler(reload);
                    AllSelectManureStorageType[i][AllSelectManureStorageType[i].Count - 1].AutoPostBack = true;
                    //  Panel1.Controls.Add(new LiteralControl("</select>"));
                    Panel1.Controls.Add(selectManureStorageType);
                    if (ms < arrStorageType.Rows.Count-1)
                    {
                        Panel1.Controls.Add(new LiteralControl("<br/><br/>"));
                    }

                }

                Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " id=" + "storageChanged_" + i + "+" + " name=" + "storageChanged_" + i + "+" + " value=" + "0" + "/>"));
                Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                //column 7 = delete buttons   
                Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + ">"));
                HtmlButton delete = new HtmlButton();
                delete.InnerText = "Delete";
                delete.Attributes.Add("CattleHerdID", arrCattleHerd.Rows[i]["CattleHerdID"].ToString());
                delete.ServerClick += new System.EventHandler(deleteEvent);
                Panel1.Controls.Add(delete);
                // Panel1.Controls.Add(new LiteralControl("<button style=" + "width: 0px; height: 0px; font-size: 0px; border: 0px;" + " onClick=" + "javascript:veil();document.forms.formCattleHerd.action='CattleHerdUpdate.asp';document.forms.formCattleHerd.submit();" + "></button><button style=" + "width: 50px; height: 20px; font-size: 10px" + " onClick=" + "javascript:if(confirm('" + "strBekræft" + "')) {veil();document.forms.formCattleHerd.action='CattleHerdDelete.asp?CattleHerdID=" + arrCattleHerd.Rows[i]["CattleHerdID"] + "';document.forms.formCattleHerd.submit();}" + ">" + "strDelete" + "</button>"));
                Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));
                Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
                // row for buttons
                Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " colspan=" + "7" + "><hr/></td>" + "\r"));
                Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));



            }
            Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " colspan=" + "7" + ">"));
            HtmlButton save = new HtmlButton();
            save.InnerHtml = "Save";
            save.ID = "Save";
            save.ServerClick += new System.EventHandler(saveEvent);
            save.ServerClick += new EventHandler(reload);
            Panel1.Controls.Add(save);

            //Panel1.Controls.Add(new LiteralControl("<button style=" + "width: 100px; height: 20px; font-size: 10px" + " onClick=" + "javascript:veil();document.forms.formCattleHerd.action='CattleHerdUpdate.asp';document.forms.formCattleHerd.submit();" + ">" + ("strSAVE") + "</button>"));
            Panel1.Controls.Add(new LiteralControl("&nbsp;"));

            //  Panel1.Controls.Add(new LiteralControl("<button style="+"width: 100px; height: 20px; font-size: 10px"+" onClick="+"javascript:veil();document.forms.formCattleHerd.action='CattleHerdNew.asp';document.forms.formCattleHerd.submit();"+">"+("strNEW")+"</button>"));
            HtmlButton NewRominants = new HtmlButton();
            NewRominants.InnerHtml = "New";
            NewRominants.ID = "NewRominantEvent";
            NewRominants.ServerClick += new System.EventHandler(NewRominantEvent);
            Panel1.Controls.Add(NewRominants);
            HtmlButton Update = new HtmlButton();
            Update.InnerHtml = "Update";
            Update.ID = "Update";
            Update.ServerClick += new System.EventHandler(UpdateEvent);
            Panel1.Controls.Add(Update);
       
            string error =database.GetConnectionString("ModelOutput") + "error_" + HttpContext.Current.Session["FarmNumber"] + "_" + HttpContext.Current.Session["scenarioNumber"] + ".txt";
            if (File.Exists(error))
            {
                System.IO.StreamReader file = new System.IO.StreamReader(error);
                messages.Text = "There is a problem with running the model: ";
                while (!file.EndOfStream)
                    messages.Text += file.ReadLine() + "\n";
                file.Close();

            }
            messages.ID = "error";

            Panel1.Controls.Add(messages);

            Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "hiddenNCattleHerd" + " value=" + "+" + arrCattleHerd.Rows.Count + "+" + ">" + "\r"));
            Panel1.Controls.Add(new LiteralControl("&nbsp;&nbsp;&nbsp;" + message + "\r"));
            Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));


            Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("  </table>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("</form>" + "\r"));
            CreateUpdateFormCattle();
            if (arrCattleHerd.Rows.Count > 0)
                CreateCattleHerdTotal();

            database.CloseFarmNDB();

        }

        void CreateCattleHerdTotal()
        {
            string language;
            if (HttpContext.Current.Session["languageSelected"] != null)
                language = HttpContext.Current.Session["languageSelected"].ToString();
            else
                language = "uk";
            DataTable arrParameter;
            string aCattleHerdID;

            DataTable arrCattleHerdTotal;
            string nFarmTotal, arrFarmTotal, dictFarmTotal;
            DataTable arrCattleType;
            String aCattleTypeID;
            DataTable arrGrazingBalance;
            string aGrazingProductID;

            List<dbInterface.data> paramter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "@aProductionTypeID";
            data.value = "5";
            paramter.Add(data);
            arrParameter = database.GetValuesFromDB("GetParameterList", paramter);
            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@FarmNumber";
            data.value = HttpContext.Current.Session["farmNumber"].ToString();
            paramter.Add(data);
            data = new dbInterface.data();
            data.name = "@ScenarioID";
            data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            paramter.Add(data);
            arrGrazingBalance = database.GetValuesFromDB("GetGrazingBalance", paramter);
            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = HttpContext.Current.Session["farmNumber"].ToString();
            paramter.Add(data);
            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            paramter.Add(data);
            arrCattleHerd = database.GetValuesFromDB("GetCattleHerdList ", paramter);

            arrCattleType = database.GetValuesFromDB("GetCattleHerdCattleTypeList ", paramter);


            // rows for totals
            Panel1.Controls.Add(new LiteralControl("  <br/>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("  <h4>&nbsp;" + database.GetLanguageString("strbehovprodgødning", language) + "</h4>" + "\r"));

            Panel1.Controls.Add(new LiteralControl("  <table class=" + "data" + " border=" + "0" + " cellspacing=" + "15" + " cellpadding=" + "0" + ">" + "\r"));
            // row for header
            Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th class=" + "data" + " align=" + "left" + ">&nbsp;&nbsp;" + database.GetLanguageString("strHusdyrhold", language) + "</th>" + "\r"));
            for (int p = 0; p < (arrParameter.Rows.Count ); p++)
            {

                Panel1.Controls.Add(new LiteralControl("      <th class=" + "data" + " align=" + "left" + ">" + arrParameter.Rows[p]["Description"] + "<br/>(" + arrParameter.Rows[p]["ParameterUnit"] + ")</th>" + "\r"));
            }
            Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));
            // rows for feeding requirements

            for (int t = 0; t < arrCattleHerd.Rows.Count; t++)
            {
                aCattleHerdID = arrCattleHerd.Rows[t]["CattleHerdID"].ToString();
                paramter = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.name = "@aFarmNumber";
                data.value = (String)HttpContext.Current.Session["farmNumber"];
                paramter.Add(data);
                data = new dbInterface.data();
                data.name = "@aScenarioID";
                data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
                paramter.Add(data);
                data = new dbInterface.data();
                data.name = "@aCattleHerdID";
                data.value = aCattleHerdID;
                paramter.Add(data);
                arrCattleHerdTotal = database.GetValuesFromDB("GetCattleHerdCalculationList", paramter);
                if (arrCattleHerdTotal.Rows.Count != 0)
                {
                    Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));

                    Panel1.Controls.Add(new LiteralControl("      <td>" + arrCattleHerd.Rows[t]["CattleType"] + " - " + database.GetLanguageString("str" + arrCattleHerd.Rows[t]["CattleTypeName"],language) + "</td>" + "\r"));

                    for (int c = 0; c < (arrCattleHerdTotal.Rows.Count ); c++)
                    {
                        string tmp = arrCattleHerdTotal.Rows[c]["Value"].ToString();
                        double tmp2 = double.Parse(tmp);
                        if (tmp2 != 0)
                            Panel1.Controls.Add(new LiteralControl("      <td align=" + "right" + ">" + arrCattleHerdTotal.Rows[c]["Value"] + "</td>" + "\r"));
                        else
                            Panel1.Controls.Add(new LiteralControl("      <td align=" + "right" + ">&nbsp;</td>" + "\r"));

                    }
                    Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));
                }

            }

            Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <td colspan=" + "+" + arrParameter.Rows.Count + 1 + "+" + "><hr/></td>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));
            // row for total
            Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th class=" + "data" + " align=" + "left" + " nowrap>&nbsp;&nbsp;" + database.GetLanguageString("strGrazingBalance", language) + "<br/>&nbsp;&nbsp;(Kg DM)</th><th class=" + "data" + " align=" + "right" + " nowrap>" + database.GetLanguageString("strFIELDPROD", language) + "&nbsp;&nbsp;</th><th class=" + "data" + " align=" + "right" + " nowrap>" + database.GetLanguageString("strHOUSINGUSE", language) + "&nbsp;&nbsp;</th><th class=" + "data" + " align=" + "right" + " nowrap>" + database.GetLanguageString("strBALANCE", language) + "&nbsp;&nbsp;</th><th class=" + "data" + " align=" + "right" + " nowrap>" + database.GetLanguageString("strBALANCE", language) + " %&nbsp;&nbsp;</th>" + "\r"));
            for (int f = 0; f < arrGrazingBalance.Rows.Count; f++)
            {

                Panel1.Controls.Add(new LiteralControl("      <tr><td class=" + "data" + " align=" + "right" + ">" + arrGrazingBalance.Rows[f]["GrazingProductID"] + "</td>" + "\r"));
                Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "right" + ">" + arrGrazingBalance.Rows[f]["SumProduction"] + "</td>" + "\r"));
                Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "right" + ">" + arrGrazingBalance.Rows[f]["SumFeeding"] + "</td>" + "\r"));
                Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "right" + ">" + ((double)arrGrazingBalance.Rows[f]["SumProduction"] - (double)arrGrazingBalance.Rows[f]["SumFeeding"]) + "</td>" + "\r"));

                if (double.Parse(arrGrazingBalance.Rows[f]["SumProduction"].ToString()) > 0)
                {
                    double SumProduction= (double)arrGrazingBalance.Rows[f]["SumProduction"];
                    double SumFeeding = (double)arrGrazingBalance.Rows[f]["SumFeeding"];
                    double result = Math.Round((SumProduction - SumFeeding) / SumProduction * 100,2);
                    Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "right" + ">" + result + "</td></tr>" + "\r"));
                }
            }

            Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));

            Panel1.Controls.Add(new LiteralControl("  </table>" + "\r"));
        }

        void DefineChangeTextScript()
        {

            List<dbInterface.data> paramter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "@AgroEcologicalZone";
            data.value = "-1";
            paramter.Add(data);
            DataTable arrCattleType = database.GetValuesFromDB("GetCattleTypeList", paramter);

            Panel1.Controls.Add(new LiteralControl("<script language=" + "JavaScript" + " type=" + "text/javascript" + ">" + "\r"));
            Panel1.Controls.Add(new LiteralControl("<!--" + "\r"));

            Panel1.Controls.Add(new LiteralControl("  function ChangeText(i,t)" + "\r"));
            Panel1.Controls.Add(new LiteralControl("  {" + "\r"));
            Panel1.Controls.Add(new LiteralControl("     switch (t) {" + "\r"));
            for (int j = 0; j < arrCattleType.Rows.Count - 1; j++)
            {


                Panel1.Controls.Add(new LiteralControl("     case '" + arrCattleType.Rows[j]["CattleTypeID"] + "' :       eval(" + "document.forms.formCattleHerd.inputProductionLevel_" + "+i+" + ".value=" + arrCattleType.Rows[j]["ProductionLevelDefault"] + "+" + ");" + "\r"));
                if ((int)arrCattleType.Rows[j]["CattleTypeID"] == 7 || (int)arrCattleType.Rows[j]["CattleTypeID"] == 1)
                {
                    Panel1.Controls.Add(new LiteralControl(" eval(" + "document.forms.formCattleHerd.inputProductionLevel2_" + "+i+" + ".value=" + arrCattleType.Rows[j]["ProductionLevel2Default"] + "+" + ");" + "\r"));
                    Panel1.Controls.Add(new LiteralControl(" eval(" + "document.forms.formCattleHerd.inputProductionLevel2_" + "+i+" + ".className='';" + ");" + "\r"));
                    Panel1.Controls.Add(new LiteralControl(" eval(" + "document.forms.formCattleHerd.ProductionLevelUnit2_" + "+i+" + ".className='show';" + ");" + "\r"));
                }
                else
                {
                    Panel1.Controls.Add(new LiteralControl(" eval(" + "document.forms.formCattleHerd.inputProductionLevel2_" + "+i+" + ".value=" + arrCattleType.Rows[j]["ProductionLevel2Default"] + "+" + ");" + "\r"));
                    Panel1.Controls.Add(new LiteralControl(" eval(" + "document.forms.formCattleHerd.inputProductionLevel2_" + "+i+" + ".className='hide';" + ");" + "\r"));
                    Panel1.Controls.Add(new LiteralControl(" eval(" + "document.forms.formCattleHerd.ProductionLevelUnit2_" + "+i+" + ".className='hide';" + ");" + "\r"));
                }

                Panel1.Controls.Add(new LiteralControl(" eval(" + "document.forms.formCattleHerd.inputGrazing_" + "+i+" + ".value=" + arrCattleType.Rows[j]["GrazingDefault"] + "+" + ");" + "\r"));
                Panel1.Controls.Add(new LiteralControl(" eval(" + "document.forms.formCattleHerd.ProductionLevelUnit_" + "+i+" + ".value='" + "str" + arrCattleType.Rows[j]["ProductionLevelDefault"] + "+" + "'" + ");" + "\r"));
                Panel1.Controls.Add(new LiteralControl("                      break;" + "\r"));
            }
            Panel1.Controls.Add(new LiteralControl("                }" + "\r"));
            Panel1.Controls.Add(new LiteralControl("  }" + "\r"));

            Panel1.Controls.Add(new LiteralControl("//-->" + "\r"));
            Panel1.Controls.Add(new LiteralControl("</script>" + "\r"));
        }


        void CreateUpdateFormCattle()
        {
            Panel1.Controls.Add(new LiteralControl("<form name=" + "update" + " action=" + "javascript:document.forms.formCattleHerd.action='CattleHerdUpdate.asp';document.forms.formCattleHerd.submit();" + " method=" + "post" + ">" + "\r"));
            Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "saveUpdates" + " value=" + "false" + ">" + "\r"));
            Panel1.Controls.Add(new LiteralControl("</form>" + "\r"));
        }
        void UpdateEvent(Object sender, EventArgs e)
        {
            saveEvent(sender, e);
            utils util = new utils();
            util.deleteFils();
            string[] args = new string[5];
            args[0] = HttpContext.Current.Session["farmNumber"].ToString();
            args[1] = HttpContext.Current.Session["scenarioNumber"].ToString();
            args[2] = "0";
            args[3] = "1";
            args[4] = "-1";
      
            dbInterface db = new dbInterface();
            string generated=db.GenerateInputXML(int.Parse(args[0]), int.Parse(args[1]));
            if (generated == "")
            {
                Thread.CurrentThread.CurrentCulture = CultureInfo.CreateSpecificCulture("en-GB");
   
                ProcessStartInfo psi = new ProcessStartInfo();
                psi.FileName = db.GetConnectionString("exefile"); ;
                psi.Arguments = args[0] + " " + args[1] + " " + args[2] + " " + args[3] + " " + args[4];
                psi.WorkingDirectory = System.IO.Path.GetDirectoryName(db.GetConnectionString("xmlFile"));
                System.Diagnostics.Process p = System.Diagnostics.Process.Start(psi);
                p.WaitForExit();
                string path = db.GetConnectionString("ModelOutput");
                if (!File.Exists(path + "error_" + args[0] + "_" + args[1] + ".txt"))
                {


                   db.saveResults();
                }
            }
            else
            {
                string path = db.GetConnectionString("ModelOutput");
                System.IO.StreamWriter file = new System.IO.StreamWriter(path + "error_" + args[0] + "_" + args[1] + ".txt");
                file.WriteLine("The file has not been generated " + generated);
                file.Close();
            }



            XmlNodeList GrazedItem = null, producedManure = null, energyIntake = null, importedManure = null, exportedManure=null;
            HttpContext context = HttpContext.Current;
            string DataToSend, xmlhttp, objXMLDOM;
            string fs, fo, showpartresult = null;
            List<dbInterface.data> arr1 = new List<dbInterface.data>();
            dbInterface.data data1 = new dbInterface.data();
            string message = null;
            string aFarmNumber = context.Session["farmNumber"].ToString();
            string aScenarioID = context.Session["scenarioNumber"].ToString();
            if (arrCattleHerd.Rows.Count > 0)
            {
                DataToSend = "FarmNumber=" + context.Session["FarmNumber"] + "+ScenarioID=" + context.Session["scenarioNumber"] + "+partParameter1=0+partParameter2=1+partParameter3=-1";


                try
                {
                    FileInfo[] fileName;

                    string path = db.GetConnectionString("ModelOutput");
                    fileName = new DirectoryInfo(path).GetFiles("*.*");

                    if (showpartresult == "0")
                    {

                        foreach (FileInfo f in fileName)
                        {
                            string test = "outputFarm" + context.Session["FarmNumber"] + "ScenarioNr" + context.Session["scenarioNumber"] + ".xml";
                            string tes2t = "outputFarm" + context.Session["FarmNumber"] + "ScenarioNr" + context.Session["scenarioNumber"] + ".xml";
                            string tes5t = f.Name;
                            if (f.Name == "outputFarm" + context.Session["FarmNumber"] + "ScenarioNr" + context.Session["scenarioNumber"] + ".xml" || (f.Name == "outputFarm" + context.Session["FarmNumber"] + "ScenarioNr" + context.Session["scenarioNumber"] + ".xml"))
                            {
                                showpartresult = "3";
                            }
                        }
                    }
                    foreach (FileInfo f in fileName)
                    {
                        if (f.Name == ("error_" + context.Session["FarmNumber"] + "_" + context.Session["scenarioNumber"] + ".txt") || (f.Name == "error_" + context.Session["FarmNumber"] + "_" + context.Session["scenarioNumber"] + ".txt"))
                            showpartresult = "2";
                    }
                    if (showpartresult != "0")
                    {
                        if (showpartresult == "3")
                        {
                            XmlDocument doc = new XmlDocument();
                            path = db.GetConnectionString("ModelOutput");
                            doc.LoadXml(path+"outputFarm" + context.Session["FarmNumber"] + "ScenarioNr" + context.Session["scenarioNumber"] + ".xml");

                            if (doc.OuterXml.ToString().Length > 49)
                                showpartresult = "1";
                        }
                        else
                        {
                            System.Threading.Thread.Sleep(15);
                            foreach (FileInfo f in fileName)
                            {
                                if (f.Name == "outputFarm" + context.Session["FarmNumber"] + "ScenarioNr" + context.Session["scenarioNumber"] + ".xml" || f.Name == "outputFarm" + context.Session["FarmNumber"] + "ScenarioNr" + context.Session["scenarioNumber"] + ".xml")
                                    if (showpartresult != "2")
                                        showpartresult = "1";

                                if (f.Name == ("error_" + context.Session["FarmNumber"] + "_" + context.Session["scenarioNumber"] + ".txt") || f.Name == ("error_" + context.Session["FarmNumber"] + "_" + context.Session["scenarioNumber"] + ".txt"))
                                    showpartresult = "2";

                            }
                        }
                    }
                }
                catch
                {

                }
                
                XmlNodeList energyUseForMaintenance = null, energyUseForGrowth = null, energyUseForMilk = null, avgProductionMilk = null, avgProductionMeat = null;
                XmlNodeList energyFromRemobilisation = null;
                // set objXMLDOM = server.CreateObject("MSXML2.DOMDocument.3.0")
                if (showpartresult == "1")
                {
                     
                    string path = db.GetConnectionString("ModelOutput") + "outputFarm" + HttpContext.Current.Session["FarmNumber"] + "ScenarioNr" + HttpContext.Current.Session["scenarioNumber"] + ".xml";
                    XmlDocument doc = new XmlDocument();

                    doc.Load(path);
                    energyIntake = doc.GetElementsByTagName("energyIntake");
                    energyUseForMaintenance = doc.GetElementsByTagName("energyUseForMaintenance");
                    energyUseForGrowth = doc.GetElementsByTagName("energyUseForGrowth");
                    energyUseForMilk = doc.GetElementsByTagName("energyUseForMilk");
                    energyFromRemobilisation = doc.GetElementsByTagName("energyFromRemobilisation");
                    avgProductionMilk = doc.GetElementsByTagName("avgDailyProductionMilk");
                    avgProductionMeat = doc.GetElementsByTagName("avgProductionMeat");
                    GrazedItem = doc.GetElementsByTagName("GrazedItem");
                    producedManure = doc.GetElementsByTagName("producedManure");
                    importedManure = doc.GetElementsByTagName("importedManure");
                    exportedManure = doc.GetElementsByTagName("exportedManure");
                }


                if (showpartresult == "1")
                {
                    for (int h = 0; h < arrCattleHerd.Rows.Count; h++)
                    {
                        arr1 = new List<dbInterface.data>();
                        data1 = new dbInterface.data();
                        data1.name = "@aFarmNumber";
                        data1.value = aFarmNumber;

                        arr1.Add(data1);
                        data1 = new dbInterface.data();
                        data1.name = "@aScenarioID";
                        data1.value = aScenarioID;

                        arr1.Add(data1);
                        string aCattleHerdID = arrCattleHerd.Rows[h]["CattleHerdID"].ToString();
                        data1 = new dbInterface.data();
                        data1.name = "@aCattleHerdID";
                        data1.value = aCattleHerdID;

                        arr1.Add(data1);

                        data1 = new dbInterface.data();
                        data1.name = "@aParameterID";
                        data1.value = "1001";

                        arr1.Add(data1);

                        data1 = new dbInterface.data();
                        data1.name = "@aValue";
                        data1.value = energyIntake.Item(h).ChildNodes[3].InnerText;
                        arr1.Add(data1);

                        database.GetValuesFromDB("insertUpdateCattleHerdCalculation ", arr1);
                        data1 = new dbInterface.data();
                        data1.name = "@aParameterID";
                        data1.value = "1002";
                        arr1.RemoveAt(3);
                        arr1.Insert(3, data1);
                        if (energyIntake.Item(h).ChildNodes[3].InnerText != "0")
                        {
                            data1 = new dbInterface.data();
                            data1.name = "@aValue";
                            data1.value = energyUseForMaintenance.Item(h).ChildNodes[3].InnerText;
                            arr1.RemoveAt(4);
                            arr1.Insert(4, data1);

                        }
                        else
                        {
                            data1 = new dbInterface.data();
                            data1.name = "@aValue";
                            data1.value = "0";
                            arr1.RemoveAt(4);
                            arr1.Insert(4, data1);

                        }

                        database.GetValuesFromDB("insertUpdateCattleHerdCalculation ", arr1);
                        data1 = new dbInterface.data();
                        data1.name = "aParameterID";
                        data1.value = "1003";
                        arr1.RemoveAt(3);
                        arr1.Insert(3, data1);

                        if (energyIntake.Item(h).ChildNodes[3].InnerText != "0")
                        {
                            data1 = new dbInterface.data();
                            data1.name = "@aValue";
                            data1.value = energyUseForGrowth.Item(h).ChildNodes[3].InnerText;
                            arr1.RemoveAt(4);
                            arr1.Insert(4, data1);

                        }
                        else
                        {
                            data1 = new dbInterface.data();
                            data1.name = "@aValue";
                            data1.value = "0";
                            arr1.RemoveAt(4);
                            arr1.Insert(4, data1);

                        }
                        database.GetValuesFromDB("insertUpdateCattleHerdCalculation ", arr1);
                        data1 = new dbInterface.data();
                        data1.name = "@aParameterID";
                        data1.value = "1004";
                        arr1.RemoveAt(3);
                        arr1.Insert(3, data1);
                        if (energyIntake.Item(h).ChildNodes[3].InnerText != "0")
                        {
                            data1 = new dbInterface.data();
                            data1.name = "@aValue";
                            data1.value = energyUseForMilk.Item(h).ChildNodes[3].InnerText;
                            arr1.RemoveAt(4);
                            arr1.Insert(4, data1);
                        }
                        else
                        {

                            data1 = new dbInterface.data();
                            data1.name = "@aValue";
                            data1.value = "0";
                            arr1.RemoveAt(4);
                            arr1.Insert(4, data1);
                        }
                        database.GetValuesFromDB("insertUpdateCattleHerdCalculation ", arr1);
                        data1 = new dbInterface.data();
                        data1.name = "@aParameterID";
                        data1.value = "1005";
                        arr1.RemoveAt(3);
                        arr1.Insert(3, data1);
                        if (energyIntake.Item(h).ChildNodes[3].InnerText != "0")
                        {

                            data1 = new dbInterface.data();
                            data1.name = "@aValue";
                            data1.value = energyFromRemobilisation.Item(h).ChildNodes[3].InnerText;
                            arr1.RemoveAt(4);
                            arr1.Insert(4, data1);
                        }
                        else
                        {
                            data1 = new dbInterface.data();
                            data1.name = "@aValue";
                            data1.value = "0";
                            arr1.RemoveAt(4);
                            arr1.Insert(4, data1);
                        }

                        database.GetValuesFromDB("insertUpdateCattleHerdCalculation ", arr1);
                        data1 = new dbInterface.data();
                        data1.name = "@aParameterID";
                        data1.value = "1006";
                        arr1.RemoveAt(3);
                        arr1.Insert(3, data1);
                        if (energyIntake.Item(h).ChildNodes[3].InnerText != "0")
                        {
                            data1.name = "@aValue";
                            data1.value = avgProductionMilk.Item(h).ChildNodes[3].InnerText;
                            arr1.RemoveAt(4);
                            arr1.Insert(4, data1);
                        }
                        else
                        {
                            data1 = new dbInterface.data();
                            data1.name = "@aValue";
                            data1.value = "0";
                            arr1.RemoveAt(4);
                            arr1.Insert(4, data1);
                        }

                        database.GetValuesFromDB("insertUpdateCattleHerdCalculation ", arr1);
                        data1 = new dbInterface.data();
                        data1.name = "@aParameterID";
                        data1.value = "1076";
                        arr1.RemoveAt(3);
                        arr1.Insert(3, data1);
                        if (energyIntake.Item(h).ChildNodes[3].InnerText != "0")
                        {

                            data1.name = "@aValue";
                            data1.value = avgProductionMeat.Item(h).ChildNodes[3].InnerText;
                            arr1.RemoveAt(4);
                            arr1.Insert(4, data1);
                        }
                        else
                        {
                            data1 = new dbInterface.data();
                            data1.name = "@aValue";
                            data1.value = "0";
                            arr1.RemoveAt(4);
                            arr1.Insert(4, data1);
                        }

                        database.GetValuesFromDB("insertUpdateCattleHerdCalculation ", arr1);
                    }
                    message = "Model calculations OK";

                }
            }
            if (showpartresult == "1")
            {
                foreach (XmlNode itm in GrazedItem)
                {
                    arr1 = new List<dbInterface.data>();
                    data1 = new dbInterface.data();
                    data1.name = "@aFarmNumber";
                    data1.value = aFarmNumber;

                    arr1.Add(data1);
                    data1 = new dbInterface.data();
                    data1.name = "@aScenarioID";
                    data1.value = aScenarioID;

                    arr1.Add(data1);

                    data1 = new dbInterface.data();
                    data1.name = "@aFeedstuffID";
                    data1.value = itm.ChildNodes[0].ChildNodes[3].InnerText;
                    arr1.Add(data1);

                    data1 = new dbInterface.data();
                    data1.name = "@aSumFeeding";
                    data1.value = itm.ChildNodes[2].ChildNodes[3].InnerText;
                    arr1.Add(data1);

                    data1 = new dbInterface.data();
                    data1.name = "@aSumProduction";
                    data1.value = itm.ChildNodes[1].ChildNodes[3].InnerText;
                    arr1.Add(data1);

                    database.GetValuesFromDB("insertUpdateGrazingBalance ", arr1);
                }

                List<dbInterface.data> arr1Parameter = new List<dbInterface.data>();
                data1 = new dbInterface.data();
                data1.name = "@aFarmNumber";
                data1.value = context.Session["FarmNumber"].ToString();
                arr1Parameter.Add(data1);
                data1 = new dbInterface.data();
                data1.name = "@aScenarioID";
                data1.value = context.Session["scenarioNumber"].ToString();
                arr1Parameter.Add(data1);

                database.GetValuesFromDB("DeleteManureTotal", arr1Parameter);

                int xcounter = 0;

                foreach (XmlNode elem in producedManure)
                {

                    for (int j = 0; j < elem.ChildNodes.Count; j++)
                    {
                        if (elem.ToString() != "")
                        {
                            arr1Parameter = new List<dbInterface.data>();
                            data1 = new dbInterface.data();
                            data1.name = "@aFarmNumber";
                            data1.value = context.Session["FarmNumber"].ToString();
                            arr1Parameter.Add(data1);
                            data1 = new dbInterface.data();
                            data1.name = "@aScenarioID";
                            data1.value = context.Session["scenarioNumber"].ToString();
                            arr1Parameter.Add(data1);


                           
                            
                            

                            data1 = new dbInterface.data();
                            data1.name = "@aStorageTypeID";
                            data1.value = elem.ChildNodes[j].ChildNodes[2].ChildNodes[3].InnerText;
                            arr1Parameter.Add(data1);
                            data1 = new dbInterface.data();
                            data1.name = "@aParameterID";
                            data1.value = "1100";
                            arr1Parameter.Add(data1);

                            data1 = new dbInterface.data();
                            data1.name = "@aParameterValue";
                            data1.value = (double.Parse(elem.ChildNodes[j].ChildNodes[9].ChildNodes[3].InnerText) + double.Parse(elem.ChildNodes[j].ChildNodes[10].ChildNodes[3].InnerText)).ToString();
                            arr1Parameter.Add(data1);
                            data1 = new dbInterface.data();
                            data1.name = "@aStorageName";
                            data1.value = (elem.ChildNodes[j].ChildNodes[0].ChildNodes[3].InnerText);
                            arr1Parameter.Add(data1);

                            data1 = new dbInterface.data();
                            data1.name = "@aSpeciesGroup";
                            data1.value = elem.ChildNodes[j].ChildNodes[1].ChildNodes[3].InnerText;
                            arr1Parameter.Add(data1);

                            database.GetValuesFromDB("InsertUpdateManureTotal ", arr1Parameter);
                        }
                    }
                }
                foreach (XmlNode elem in importedManure)
                {

                    for (int j = 0; j < elem.ChildNodes.Count; j++)
                    {
                        if (elem.ToString() != "")
                        {
                            arr1Parameter = new List<dbInterface.data>();
                            data1 = new dbInterface.data();
                            data1.name = "@aFarmNumber";
                            data1.value = context.Session["FarmNumber"].ToString();
                            arr1Parameter.Add(data1);
                            data1 = new dbInterface.data();
                            data1.name = "@aScenarioID";
                            data1.value = context.Session["scenarioNumber"].ToString();
                            arr1Parameter.Add(data1);






                            data1 = new dbInterface.data();
                            data1.name = "@aStorageTypeID";
                            data1.value = elem.ChildNodes[j].ChildNodes[2].ChildNodes[3].InnerText;
                            arr1Parameter.Add(data1);
                            data1 = new dbInterface.data();
                            data1.name = "@aParameterID";
                            data1.value = "1102";
                            arr1Parameter.Add(data1);

                            data1 = new dbInterface.data();
                            data1.name = "@aParameterValue";
                            data1.value = (double.Parse(elem.ChildNodes[j].ChildNodes[9].ChildNodes[3].InnerText) + double.Parse(elem.ChildNodes[j].ChildNodes[10].ChildNodes[3].InnerText)).ToString();
                            arr1Parameter.Add(data1);
                            data1 = new dbInterface.data();
                            data1.name = "@aStorageName";
                            data1.value = (elem.ChildNodes[j].ChildNodes[0].ChildNodes[3].InnerText);
                            arr1Parameter.Add(data1);

                            data1 = new dbInterface.data();
                            data1.name = "@aSpeciesGroup";
                            data1.value = elem.ChildNodes[j].ChildNodes[1].ChildNodes[3].InnerText;
                            arr1Parameter.Add(data1);

                            database.GetValuesFromDB("InsertUpdateManureTotal ", arr1Parameter);
                        }
                    }
                }


                foreach (XmlNode elem in exportedManure)
                {

                    for (int j = 0; j < elem.ChildNodes.Count; j++)
                    {
                        if (elem.ToString() != "")
                        {
                            arr1Parameter = new List<dbInterface.data>();
                            data1 = new dbInterface.data();
                            data1.name = "@aFarmNumber";
                            data1.value = context.Session["FarmNumber"].ToString();
                            arr1Parameter.Add(data1);
                            data1 = new dbInterface.data();
                            data1.name = "@aScenarioID";
                            data1.value = context.Session["scenarioNumber"].ToString();
                            arr1Parameter.Add(data1);






                            data1 = new dbInterface.data();
                            data1.name = "@aStorageTypeID";
                            data1.value = elem.ChildNodes[j].ChildNodes[2].ChildNodes[3].InnerText;
                            arr1Parameter.Add(data1);
                            data1 = new dbInterface.data();
                            data1.name = "@aParameterID";
                            data1.value = "1101";
                            arr1Parameter.Add(data1);

                            data1 = new dbInterface.data();
                            data1.name = "@aParameterValue";
                            data1.value = (double.Parse(elem.ChildNodes[j].ChildNodes[9].ChildNodes[3].InnerText) + double.Parse(elem.ChildNodes[j].ChildNodes[10].ChildNodes[3].InnerText)).ToString();
                            arr1Parameter.Add(data1);
                            data1 = new dbInterface.data();
                            data1.name = "@aStorageName";
                            data1.value = (elem.ChildNodes[j].ChildNodes[0].ChildNodes[3].InnerText);
                            arr1Parameter.Add(data1);

                            data1 = new dbInterface.data();
                            data1.name = "@aSpeciesGroup";
                            data1.value = elem.ChildNodes[j].ChildNodes[1].ChildNodes[3].InnerText;
                            arr1Parameter.Add(data1);

                            database.GetValuesFromDB("InsertUpdateManureTotal ", arr1Parameter);
                        }
                    }
                }

            }

            database.CloseFarmNDB();
            if (showpartresult == "2")
            {
                string path = db.GetConnectionString("ModelOutput");
                System.IO.StreamReader myFile = new System.IO.StreamReader(path+"error_" + context.Session["FarmNumber"] + "_" + context.Session["scenarioNumber"] + ".txt");
                string myString = myFile.ReadToEnd();

                if (myString.Length != 0)
                    message = "Model error :" + myString;
                else
                    message = "Empty error-file";
                myFile.Close();
            }


                
            //currentLocale  = SetLocale(originalLocale) ' Restore the original locale

            //context.Response.Write( "<form name="+"ret"+" action="+"data.asp?TB="+HttpContext.Current.Session["tabNumber"]+""+" method="+"post"+">" +'\r');
            context.Response.Write("<input type=" + "hidden" + " name=" + "message" + " value=" + "" + message + "" + "/>" + '\r');
            context.Response.Write("</form>" + '\r');
            context.Response.Write("<script language =" + "javascript" + ">document.forms[0].submit();</script>");
            context.Response.Write("</body></html>" + '\r');
            context.Session["ShowResult"] = "False";
            ViewState["UserInit"] = "5";
            HttpContext.Current.Session["UserInit"] = "5";
            HttpContext.Current.Session["ShowResult"] = true;
            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);
        }

        void NewRominantEvent(Object sender, EventArgs e)
        {
            HtmlButton button = (HtmlButton)sender;
            //  int RotationID = int.Parse(button.Attributes["aRotationID"]);
            CattleHerdNew cattle = new CattleHerdNew();
            cattle.ProcessRequest(HttpContext.Current);
            ViewState["UserInit"] = "5";
            HttpContext.Current.Session["UserInit"] = "5";
            saveEvent(sender, e);
            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);

        }
        void deleteEvent(Object sender, EventArgs e)
        {
            HttpContext.Current.Session["ShowResult"] = false;
            HtmlButton button = (HtmlButton)sender;
            string RotationID = button.Attributes["CattleHerdID"].ToString();
            CattleHerdDelete cattle = new CattleHerdDelete();
            cattle.ProcessRequest(HttpContext.Current, RotationID);
            Panel1.Controls.Clear();
            CreateTabCattle();

        }
        void saveEvent(Object sender, EventArgs e)
        {
            HttpContext.Current.Session["ShowResult"] = false;
            //   HtmlButton button = (HtmlButton)sender;
            //  string RotationID = button.Attributes["CattleHerdID"].ToString();
            CattleHerdUpdate cattle = new CattleHerdUpdate();

            /*  if (arrCattleHerdFeedplanList.Rows.Count > 0)
              {
                  for (int fp = 0; fp < (arrCattleHerdFeedplanList.Rows.Count - 1); fp++)
                  {
                      strFeedplan += strFeedplan + arrCattleHerdFeedplanList.Rows[fp][0] + ",";
                      strAmount += strAmount + arrCattleHerdFeedplanList.Rows[fp][1] + ",";
                  }
              }
              else
              {
                  strFeedplan = "";
                  strAmount = "";
              }*/
            List<string> strFeedplan = new List<string>();
            List<string> strAmount = new List<string>();
            for (int k = 0; k < AllSelFeedstuff.Count; k++)
            {
                string Feedplan = "";
                string Amount = "";
                int numberOfFeed = AllSelFeedstuff[k].Count;
                for (int i = 0; i < numberOfFeed; i++)
                {

                    int index = AllSelFeedstuff[k][i].SelectedIndex;

                    if ("Select feedStuff" == AllSelFeedstuff[k][i].Items[index].Text)
                    {
                        Feedplan += "0" + ',';
                    }
                    else
                    {
                        Feedplan += AllSelFeedstuff[k][i].SelectedItem.Attributes["dbValue"] + ',';
                    }

                        int d = AllFeedstuffAmount[k].Count();
                        List<TextBox> t = AllFeedstuffAmount[k];
                        try
                        {
                            Amount += t[i].Text.ToString() + ',';

                        }
                        catch
                        {
                            Amount += "0" + ',';
                        }
                    
                }
                strFeedplan.Add(Feedplan);
                strAmount.Add(Amount);
            }
            cattle.ProcessRequest(HttpContext.Current, arrCattleHerd, allSelectCattleType, AllInputAnimalYear, AllSelectStableType, strFeedplan, strAmount, AllSelectManureStorageType);



            

        }
        void reload(Object sender, EventArgs e)
        {
            HttpContext.Current.Session["UserInit"] = "5";
            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);
        }

        void feedDeleteEvent(Object sender, EventArgs e)
        {
            HtmlButton button = (HtmlButton)sender;
            string[] data = button.ID.Split('.');
            int feed = int.Parse(data[1].Split(' ')[0]);
            int item = int.Parse(data[1].Split(' ')[1]);
            AllSelFeedstuff[feed].RemoveAt(item);
            saveEvent(sender, e);
            HttpContext.Current.Session["UserInit"] = "5";
            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);
        }
        static int i = 0;
        void feedStuffEvent(Object sender, EventArgs e)
        {
            HtmlButton knap = (HtmlButton)sender;
            string FeedStuffButton = knap.ID;
            string output = FeedStuffButton.Replace("FeedStuffButton", "");
            int number = int.Parse(output);
            DropDownList selFeedstuff = new DropDownList();

            i++;
            selFeedstuff.ID = "selFeedstuffddddddddddddddddddddddddddddddd_" + i.ToString();
            selFeedstuff.Items.Add("Select feedStuff");

            selFeedstuff.Items[0].Attributes.Add("dbValue", "-1");

            selFeedstuff.SelectedIndex = 0;
            selFeedstuff.AutoPostBack = true;
            AllSelFeedstuff[number].Add(selFeedstuff);
            ViewState["UserInit"] = "5";
            HttpContext.Current.Session["UserInit"] = "5";
            saveEvent(sender, e);
            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);


        }



    }
}