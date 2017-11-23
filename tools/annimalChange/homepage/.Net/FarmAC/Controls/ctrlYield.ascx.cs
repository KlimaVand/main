using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.HtmlControls;
using System.Web.UI.WebControls;

namespace FarmAC.Controls
{
    public partial class ctrlYield : System.Web.UI.UserControl
    {

        protected void Page_Load(object sender, EventArgs e)
        {
            if (HttpContext.Current.Session["UserInit"] != null)
                if (HttpContext.Current.Session["UserInit"].ToString() == "4")
            {
                CreateTabRotation();
            }
        }
        List<List<TextBox>> inputGrazingPart_ = new List<List<TextBox>>();
        List<List<TextBox>> aOrganicFertilizer = new List<List<TextBox>>();

        List<List<TextBox>> allmainYield = new List<List<TextBox>>();
         List<List<TextBox>> allStrawYield= new List<List<TextBox>>();
        List<List<DropDownList>> AllSelectStrawUseType = new List<List<DropDownList>>();
        void CreateTabRotation()
        {
            string language;
            if (HttpContext.Current.Session["languageSelected"] != null)
                language = HttpContext.Current.Session["languageSelected"].ToString();
            else
                language = "uk";
            string aSecondCrop, aPreCropSecondCrop;
            string aRotationID, dictRotation;
            string nStrawUse, aStrawUseID, dictStrawUse;
            DataTable arrStrawUse;
            string dictFieldPlanRotation;
            string nSecondCropYield, dictSecondCropYield;
            string SecondCropFE, SecondCropNNeed;
            string vinterKornList, vinterRape, toAarsGraesList, udlaegList;
            string hasSecondary;
            bool hasStraw;
            vinterKornList = "3,4,5,23";
            vinterRape = "8";
            toAarsGraesList = "12,18,19,32,33,34";
            udlaegList = "12,13,14,18,19,29,32,33,34";
            dbInterface database = new dbInterface();
            database.InitFarmNDB();
            //CreateLanguageDictionary
            Panel1.Controls.Add(new LiteralControl(("<script language=" + "javascript" + ">window.parent.frames[1].location.reload()</script>" + "\r")));
            List<dbInterface.data> paramter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = HttpContext.Current.Session["farmNumber"].ToString();
            paramter.Add(data);
            data.name = "@aScenarioID";
            data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            paramter.Add(data);
            DataTable arrRotation = database.GetValuesFromDB("GetRotationList", paramter);
            int fem = arrRotation.Rows.Count;
            // nSecondCropYield 
            DataTable arrSecondCropYield = database.GetValuesFromDB("GetSecondCropYield", paramter);
            int index = 0;
            Panel1.Controls.Add(new LiteralControl(("  <table class=" + "data" + " border=" + "0" + " cellpadding=" + "5" + " cellspacing=" + "0" + ">" + "\r")));
            Panel1.Controls.Add(new LiteralControl(("<form name=" + "formRotation" + " action=" + "RotationUpdate.asp" + " method=" + "post" + ">" + "\r")));
            while (inputGrazingPart_.Count() < arrRotation.Rows.Count)
                inputGrazingPart_.Add(new List<TextBox>());
            while (aOrganicFertilizer.Count() < arrRotation.Rows.Count)
                aOrganicFertilizer.Add(new List<TextBox>());
            while (AllSelectStrawUseType.Count() < arrRotation.Rows.Count)
                AllSelectStrawUseType.Add(new List<DropDownList>());
            while (allmainYield.Count() < arrRotation.Rows.Count)
                allmainYield.Add(new List<TextBox>());
            while (allStrawYield.Count() < arrRotation.Rows.Count)
                allStrawYield.Add(new List<TextBox>());
            for (int r = 0; r < arrRotation.Rows.Count; r++)
            {
                aRotationID = arrRotation.Rows[r]["RotationID"].ToString();

                paramter = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.name = "@aFarmNumber";
                data.value = HttpContext.Current.Session["farmNumber"].ToString();
                paramter.Add(data);
                data.name = "@aScenarioID";
                data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
                paramter.Add(data);
                data.name = "@aRotationID";
                data.value = aRotationID;
                paramter.Add(data);
                DataTable arrFieldPlanRotation = database.GetValuesFromDB("GetFieldPlanRotationList", paramter);
                if (arrFieldPlanRotation.Rows.Count != 0)
                {
                    Panel1.Controls.Add(new LiteralControl(("    <tr>" + "\r")));
                    Panel1.Controls.Add(new LiteralControl(("      <td align=" + "left" + " colspan=" + "4" + "><h4>" + arrRotation.Rows[r]["RotationName"] + "</td>" + "\r")));
                    Panel1.Controls.Add(new LiteralControl(("    </tr>" + "\r")));

                    // column headings
                    Panel1.Controls.Add(new LiteralControl(("    <tr>" + "\r")));
                    Panel1.Controls.Add(new LiteralControl(("      <th align=" + "left" + ">" + database.GetLanguageString("strCROP", language) + "</th>" + "\r")));
                    Panel1.Controls.Add(new LiteralControl(("      <th align=" + "left" + "></th>" + "\r")));
                    //database.GetLanguageString("strgrazed", language) 
                    Panel1.Controls.Add(new LiteralControl(("      <th align=" + "left" + ">" + database.GetLanguageString("str1product", language) + "<br/>" +"Grazed"+ " " + database.GetLanguageString("strYIELD", language) + "<br/><br/>Kg DM/Ha</th>" + "\r")));
                    Panel1.Controls.Add(new LiteralControl(("      <th align=" + "left" + ">" + database.GetLanguageString("str2product", language) + "<br/>" + "Grazed"  + " " + database.GetLanguageString("strYIELD", language) + "<br/><br/>Kg DM/Ha</th>" + "\r")));

                    Panel1.Controls.Add(new LiteralControl(("      <th align=" + "left" + ">" + database.GetLanguageString("strSTRAW", language) + "<br>" + database.GetLanguageString("strUSE", language) + "</th>" + "\r")));
                    Panel1.Controls.Add(new LiteralControl(("      <th align=" + "left" + ">" + database.GetLanguageString("str1product", language) + "<br/>" + database.GetLanguageString("strPotential", language) + " " + database.GetLanguageString("strYIELD", language) + "<br/><br/> kg DM/Ha</th>" + "\r")));
                    Panel1.Controls.Add(new LiteralControl(("      <th align=" + "left" + ">" + database.GetLanguageString("str2product", language) + "<br/>" + database.GetLanguageString("strPotential", language) + " " + database.GetLanguageString("strYIELD", language) + "<br/><br/> kg DM/Ha</th>" + "\r")));
                    Panel1.Controls.Add(new LiteralControl(("    </tr>" + "\r")));

                    for (int f = 0; f < arrFieldPlanRotation.Rows.Count; f++)
                    {
                        aStrawUseID = arrFieldPlanRotation.Rows[f]["StrawUseType"].ToString();
                        aSecondCrop = arrFieldPlanRotation.Rows[f]["SecondCropID"].ToString();
                        aPreCropSecondCrop = arrFieldPlanRotation.Rows[f]["PreCropSecondCrop"].ToString();
                        hasSecondary = arrFieldPlanRotation.Rows[f]["StrawFeedStuffID"].ToString();
                        string tmp = arrFieldPlanRotation.Rows[f]["UseStraw"].ToString();
                        if (tmp == "1")
                            hasStraw = true;
                        else
                            hasStraw = false;
                        if (index < arrSecondCropYield.Rows.Count)
                        {

                            if (arrSecondCropYield.Rows[index]["RotationID"] == arrFieldPlanRotation.Rows[f]["RotationID"] && arrSecondCropYield.Rows[index]["FieldPlanRotationID"] == arrFieldPlanRotation.Rows[f]["FieldPlanRotationID"])
                            {

                                SecondCropFE = arrSecondCropYield.Rows[index]["SecondCrop_FE"].ToString();
                                SecondCropNNeed = arrSecondCropYield.Rows[index]["SecondCropNNeed"].ToString();
                                index = index + 1;
                            }
                            else
                            {
                                SecondCropFE = "0";
                                SecondCropNNeed = "0";
                            }

                        }
                        else
                        {
                            SecondCropFE = "0";
                            SecondCropNNeed = "0";
                        }
                        Panel1.Controls.Add(new LiteralControl(("    <tr>" + "\r")));
                        Panel1.Controls.Add(new LiteralControl(("<input type=" + "hidden" + " name=" + "hiddenFieldPlanRotationID_" + aRotationID + "_" + f + "" + " value=" + "" + arrFieldPlanRotation.Rows[f]["FieldPlanRotationID"] + "" + "><input type=" + "hidden" + " name=" + "hiddenPreviousCropID_" + aRotationID + "_" + f + "+" + " value=" + "" + (arrFieldPlanRotation.Rows[f]["PreviousCrop"]) + "" + "><input type=" + "hidden" + " name=" + "hiddenPreCropOriginalID_" + aRotationID + "_" + f + " value=" + "" + (arrFieldPlanRotation.Rows[f]["PreCropOriginalID"]) + "+" + ">" + "\r")));
                        // column 2 = Efterafgrøde PreCrop
                        Panel1.Controls.Add(new LiteralControl(("<input type=" + "hidden" + " name=" + "hiddenPreCrop_AfterCropID_" + aRotationID + "_" + f + "" + " value=" + "" + arrFieldPlanRotation.Rows[f]["PreCrop_AfterCrop"] + ">" + "\r")));
                        // column 2 = crop
                        Panel1.Controls.Add(new LiteralControl(("      <td align=" + "left" + ">" + database.GetLanguageString("str" + arrFieldPlanRotation.Rows[f]["CropName"], language))));
                        Panel1.Controls.Add(new LiteralControl(("<input type=" + "hidden" + " name=" + "hiddenCropID_" + aRotationID + "_" + f + "" + " value=" + "" + arrFieldPlanRotation.Rows[f]["Crop"] + "" + "></td>" + "\r")));
                        // column 2 = Efterafgrøde Crop
                        Panel1.Controls.Add(new LiteralControl(("</td>" + "\r")));
                        Panel1.Controls.Add(new LiteralControl(("<input type=" + "hidden" + " name=" + "hiddenCrop_AfterCropID_" + aRotationID + "_" + f + "" + " value=" + "" + arrFieldPlanRotation.Rows[f]["Crop_AfterCrop"] + "+" + "><input type=" + "hidden" + " name=" + "hiddenPrePreCropID_" + aRotationID + "_" + f + " value=" + "" + arrFieldPlanRotation.Rows[f]["PrePreCropID"] + ">" + "\r")));
                        // column 15 = area
                        Panel1.Controls.Add(new LiteralControl(("      <td align=" + "right" + "><input type=" + "hidden" + " name=" + "hiddenArea_" + aRotationID + "_" + f + "" + " value=" + "" + arrFieldPlanRotation.Rows[f]["Area"] + "></td>" + "\r")));
                        // column 23 to 25 = cernal-, straw-, secondcrop-yield
                        Panel1.Controls.Add(new LiteralControl(("      <td align=" + "left" + ">" + "\r")));
                        string tmpsss = arrFieldPlanRotation.Rows[f]["FeedStuffID"].ToString();

                        if (arrFieldPlanRotation.Rows[f]["FeedStuffID"].ToString() != "0")
                        {
                            TextBox mainYield = new TextBox();

                            mainYield.ID = "mainYield_" + aRotationID + "_" + f;
                            mainYield.Text = (arrFieldPlanRotation.Rows[f]["MainYield"]).ToString();
                            if (mainYield.Text.ToString() == "")
                                mainYield.Text = "0";
                            mainYield.Attributes.Add("arrFieldPlanRotation", r.ToString());
                            mainYield.TextChanged += new EventHandler(SaveRotationTextChange);
                            mainYield.AutoPostBack = true;
                            mainYield.Attributes.Add("aRotationID", aRotationID.ToString());
                            allmainYield[r].Add(mainYield);
                            if (int.Parse(arrFieldPlanRotation.Rows[f]["FeedStuffID"].ToString()) > 1000)
                            {
                                // Panel1.Controls.Add(new LiteralControl(("<input type=" + "text" + " name=" + "mainYield_" + aRotationID + "_" + f + "+" + " size=" + "4" + " value=" + "" + (arrFieldPlanRotation.Rows[f]["MainYield"]) + " onChange=" + "document.forms.formRotation.action='RotationUpdate.asp?RotationID=" + aRotationID + "';document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';" + " />" + "\r")));
                               
                                Panel1.Controls.Add(mainYield);
                            }
                        }
                        else
                        {
                            TextBox mainYield = new TextBox();

                            mainYield.ID = "mainYield_" + aRotationID + "_" + f;
                            mainYield.Text = (arrFieldPlanRotation.Rows[f]["MainYield"]).ToString();
                            mainYield.Attributes.Add("arrFieldPlanRotation", r.ToString());
                            mainYield.Attributes.Add("aRotationID", aRotationID.ToString());
                            mainYield.TextChanged += new EventHandler(SaveRotationTextChange);
                            mainYield.AutoPostBack = true;
                            allmainYield[r].Add(mainYield);
                           
                           // Panel1.Controls.Add(new LiteralControl(("<input type=" + "hidden" + " name=" + "mainYield_" + aRotationID + "_" + f + "+" + " size=" + "4" + " value=" + "" + (arrFieldPlanRotation.Rows[f]["MainYield"]) + " onChange=" + "document.forms.formRotation.action='RotationUpdate.asp?RotationID=" + aRotationID + "';document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';" + " />" + "\r")));
                        }
                        Panel1.Controls.Add(new LiteralControl(("      </td>" + "\r")));
                        Panel1.Controls.Add(new LiteralControl(("      <td align=" + "left" + ">" + "\r")));
                        string dfd = arrFieldPlanRotation.Rows[f]["StrawFeedStuffID"].ToString();
                        if (arrFieldPlanRotation.Rows[f]["StrawFeedStuffID"].ToString() != "0")
                        {
                            TextBox strawYield = new TextBox();
                            strawYield.ID = "strawYield_" + aRotationID.ToString() + "_" + f.ToString();
                            strawYield.Text = arrFieldPlanRotation.Rows[f]["StrawYield"].ToString();
                            if (strawYield.Text.ToString() == "")
                                strawYield.Text = "0";
                            strawYield.TextChanged += new EventHandler(SaveRotationTextChange);
                            strawYield.Attributes.Add("aRotationID", aRotationID.ToString());
                            strawYield.Attributes.Add("arrFieldPlanRotation", r.ToString());

                            allStrawYield[r].Add(strawYield);
                            if (int.Parse(arrFieldPlanRotation.Rows[f]["StrawFeedStuffID"].ToString()) > 1000 || (arrFieldPlanRotation.Rows[f]["StrawUseType"].ToString()) == "7" || (arrFieldPlanRotation.Rows[f]["StrawUseType"].ToString()) == "8" || (arrFieldPlanRotation.Rows[f]["StrawUseType"].ToString()) == "9")
                            {
                                
                                Panel1.Controls.Add(strawYield);
                              //  Panel1.Controls.Add(new LiteralControl(("<input type=" + "text" + " name=" + "strawYield_" + aRotationID + "_" + f + " size=" + "4" + " value=" + "" + (arrFieldPlanRotation.Rows[f]["StrawYield"] + " onChange=" + "document.forms.formRotation.action='RotationUpdate.asp?RotationID=" + aRotationID + "';document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';" + "/>" + "\r"))));
                            }
                        }
                        else
                        {
                            TextBox strawYield = new TextBox();
                            strawYield.ID = "strawYield_" + aRotationID.ToString() + "_" + f.ToString();
                            strawYield.Text = arrFieldPlanRotation.Rows[f]["StrawYield"].ToString();
                            if (strawYield.Text.ToString() == "")
                                strawYield.Text = "0";
                            strawYield.TextChanged += new EventHandler(SaveRotationTextChange);
                            allStrawYield[r].Add(strawYield);
                           // Panel1.Controls.Add(new LiteralControl(("<input type=" + "hidden" + " name=" + "strawYield_" + aRotationID + "_" + f + "+" + " size=" + "4" + " value=" + "" + (arrFieldPlanRotation.Rows[f]["StrawYield"] + " onChange=" + "document.forms.formRotation.action='RotationUpdate.asp?RotationID=" + aRotationID + "';document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';" + "/>" + "\r"))));
                        }
                            

                        Panel1.Controls.Add(new LiteralControl(("      </td>" + "\r")));

                        string tmpsdsd = arrFieldPlanRotation.Rows[f]["UseStraw"].ToString();
                        string lolo = arrFieldPlanRotation.Rows[f]["StrawFeedStuffID"].ToString();
                        if (arrFieldPlanRotation.Rows[f]["UseStraw"].ToString() != "" && (arrFieldPlanRotation.Rows[f]["StrawFeedStuffID"].ToString() != "0"))
                        {
                            paramter = new List<dbInterface.data>();
                            data = new dbInterface.data();
                            data.name = "@StrawUseBoxType";
                            data.value = arrFieldPlanRotation.Rows[f]["UseStraw"].ToString();
                            paramter.Add(data);
                            arrStrawUse = database.GetValuesFromDB("GetSelectListStrawUseBoxType ", paramter);
                            Panel1.Controls.Add(new LiteralControl(("      <td align=" + "left" + ">")));

                            //Panel1.Controls.Add(new LiteralControl(("<select name=" + "selectStrawUseType_" + aRotationID + "_" + f + "+" + " onChange=" + "document.forms.formRotation.action='RotationUpdate.asp?RotationID=" + aRotationID + "';document.forms.update.saveUpdates.value='true';this.style.color='#ff0000';" + ">")));
                            //  CreateSelectList_Language aStrawUseID,nStrawUse,arrStrawUse
                            DropDownList selectStrawUseType = new DropDownList();
                            selectStrawUseType.ID = "arrFieldPlanRotation"+r.ToString() + f.ToString();
                            selectStrawUseType.Attributes.Add("arrFieldPlanRotation", r.ToString());
                            selectStrawUseType.Attributes.Add("aRotationID", aRotationID.ToString());
                            for (int i = 0; i < arrStrawUse.Rows.Count; i++)
                            {

                                selectStrawUseType.Items.Add(arrStrawUse.Rows[i][0].ToString() + " " + database.GetLanguageString("str"+arrStrawUse.Rows[i][1].ToString(),language));
                                selectStrawUseType.Items[i].Attributes.Add("dbValue", arrStrawUse.Rows[i][0].ToString());
                                if (aStrawUseID == arrStrawUse.Rows[i][0].ToString())
                                    selectStrawUseType.SelectedIndex = i;
                            }
                            if (selectStrawUseType.SelectedIndex == -1)
                                selectStrawUseType.SelectedIndex = 0;
                            selectStrawUseType.SelectedIndexChanged += new EventHandler(SaveRotationEventDropDown);
                            selectStrawUseType.AutoPostBack = true;
                            AllSelectStrawUseType[r].Add(selectStrawUseType);
                         //   int stuff = AllSelectStrawUseType[0][0].SelectedIndex;
                            if (selectStrawUseType.Items.Count!=0)
                            Panel1.Controls.Add(selectStrawUseType);
                           // Panel1.Controls.Add(new LiteralControl(("</select>")));
                            Panel1.Controls.Add(new LiteralControl(("</td>" + "\r")));
                        }
                        else
                        {
                            DropDownList selectStrawUseType = new DropDownList();
                            selectStrawUseType.Attributes.Add("aRotationID", aRotationID);
                            selectStrawUseType.Items.Add("null");
                            selectStrawUseType.Items[0].Attributes.Add("dbValue", "0");
                            AllSelectStrawUseType[r].Add(selectStrawUseType);
                            Panel1.Controls.Add(new LiteralControl(("      <td align=" + "left" + "><input type=" + "hidden" + " name=" + "selectStrawUseType_" + aRotationID + "_" + f + "+" + " value=" + "0" + "/></td>")));
                        }

                        Panel1.Controls.Add(new LiteralControl(("      <input type=" + "hidden" + " name=" + "SecondCrop_" + aRotationID + "_" + f + "+" + " value=" + "" + arrFieldPlanRotation.Rows[f]["SecondCropID"] + "/><input type=" + "hidden" + " name=" + "PreCropSecondCrop_" + aRotationID + "_" + f + " value=" + "" + arrFieldPlanRotation.Rows[f]["PreCropSecondCrop"] + "/>")));
                        if (arrFieldPlanRotation.Rows[f]["FeedStuffID"].ToString() != "0")
                        {
                            Panel1.Controls.Add(new LiteralControl(("      <td>")));
                            TextBox inputGrazingPart = new TextBox();
                            inputGrazingPart.Text = arrFieldPlanRotation.Rows[f]["GrazingPart"].ToString();
                            inputGrazingPart.ID = "GrazingPart" + f.ToString() + "_" + r.ToString();
                            Panel1.Controls.Add(inputGrazingPart);
                            inputGrazingPart_[r].Add(inputGrazingPart);
                            Panel1.Controls.Add(new LiteralControl(("      </td>" + "\r")));
                            // Panel1.Controls.Add(new LiteralControl(("      <td><input type=" + "text" + " size=" + "4" + " name=" + "inputGrazingPart_" + aRotationID + "_" + f + "+" + " value=" + "" + arrFieldPlanRotation.Rows[f]["GrazingPart"] + "/></td>" + "\r")));
                        }
                        else
                        {
                            Panel1.Controls.Add(new LiteralControl(("      <td></td>" + "\r")));
                            TextBox inputGrazingPart = new TextBox();
                            inputGrazingPart.Text = "0";
                            inputGrazingPart_[r].Add(inputGrazingPart);
                        }

                        TextBox OrganicFertilizer = new TextBox();
                        OrganicFertilizer.Text = arrFieldPlanRotation.Rows[f]["OrganicFertilizer"].ToString();


                        if (arrFieldPlanRotation.Rows[f]["StrawFeedStuffID"].ToString() != "0")
                        {
                            Panel1.Controls.Add(new LiteralControl(("      <td>")));
                            Panel1.Controls.Add(OrganicFertilizer);
                            Panel1.Controls.Add(new LiteralControl(("</td>" + "\r")));
                            //Panel1.Controls.Add(new LiteralControl(("      <td><input type=" + "text" + " size=" + "4" + " name=" + "checkboxOrganicFertilizer_" + aRotationID + "_" + f + "+" + " value=" + "" + arrFieldPlanRotation.Rows[f]["OrganicFertilizer"] + "/></td>" + "\r")));
                        }
                        aOrganicFertilizer[r].Add(OrganicFertilizer);

                        Panel1.Controls.Add(new LiteralControl(("\r" + "    </tr>" + "\r")));


                    }


                    Panel1.Controls.Add(new LiteralControl(("    <tr>" + "\r")));
                    Panel1.Controls.Add(new LiteralControl(("      <td colspan=" + "9" + ">")));
                    //Panel1.Controls.Add(new LiteralControl(("<button style=" + "width: 100px; height: 20px; font-size: 10px" + " onClick=" + "javascript:if (document.forms.update.saveUpdates.value=='true') {veil();document.forms.formRotation.action='RotationUpdate.asp?RotationID=" + aRotationID + "';document.forms.formRotation.submit();}" + ">" + "strSAVE" + "</button>")));
                    HtmlButton SaveRotation = new HtmlButton();

                    // Set the properties of the new HtmlButton control.
                    SaveRotation.ID = "SaveRotation" + aRotationID.ToString();
                    SaveRotation.InnerHtml = "Save";
                    SaveRotation.Attributes.Add("arrFieldPlanRotation", r.ToString());
                    SaveRotation.Attributes.Add("aRotationID", aRotationID.ToString());
                    // Create an EventHandler delegate for the method you want to handle the event
                    // and then add it to the list of methods called when the event is raised.
                    SaveRotation.ServerClick += new System.EventHandler(Save);

                    // Add the new HtmlButton control to the Controls collection of the
                    // PlaceHolder control. 
                    Panel1.Controls.Add(SaveRotation);

                    Panel1.Controls.Add(new LiteralControl(("<input type=" + "hidden" + " name=" + "hiddenNFieldPlanRotation_" + aRotationID + " value=" + "" + arrFieldPlanRotation.Rows.Count + ">")));
                    Panel1.Controls.Add(new LiteralControl(("</td>" + "\r")));
                    Panel1.Controls.Add(new LiteralControl(("    </tr>" + "\r")));
                }
                else
                {
                    Panel1.Controls.Add(new LiteralControl(("<tr><td colspan=" + "10" + "><h3>" + "strROTATION" + "+" + "strCOLON" + "+" + aRotationID + " " + "strCALCFAIL" + "</h3></td></tr>" + "\r")));

                    Panel1.Controls.Add(new LiteralControl("nFieldPlanRotation: " + arrFieldPlanRotation.Rows.Count));
                }

                Panel1.Controls.Add(new LiteralControl(("    <tr>" + "\r")));
                Panel1.Controls.Add(new LiteralControl(("      <td colspan=" + "10" + "><hr/></td>" + "\r")));
                Panel1.Controls.Add(new LiteralControl(("    </tr>" + "\r")));

            }
            database.CloseFarmNDB();


            Panel1.Controls.Add(new LiteralControl(("</form>" + "\r")));
            Panel1.Controls.Add(new LiteralControl(("  </table>" + "\r")));
            CreateUpdateFormRotation();

        }
        void CreateUpdateFormRotation()
        {
            Panel1.Controls.Add(new LiteralControl(("<form name=" + "update" + " action=" + "javascript:document.forms.formRotation.submit();" + " method=" + "post" + ">" + "\r")));
            Panel1.Controls.Add(new LiteralControl(("<input type=" + "hidden" + " name=" + "saveUpdates" + " value=" + "true" + ">" + "\r")));
            Panel1.Controls.Add(new LiteralControl(("</form>" + "\r")));
        }
        private void Save(object sender, System.EventArgs e)
        {
            RotationUpdate update = new RotationUpdate();
            HtmlButton button = (HtmlButton)sender;
            int arrFieldPlanRotation = int.Parse(button.Attributes["arrFieldPlanRotation"].ToString());
            List<TextBox> inputGrazingPart_1 = inputGrazingPart_[arrFieldPlanRotation];
            List<TextBox> aOrganicFertilizer1 = aOrganicFertilizer[arrFieldPlanRotation];
            List<DropDownList> AllSelectStrawUseType1 = AllSelectStrawUseType[arrFieldPlanRotation];
            int aRotationID = int.Parse(button.Attributes["aRotationID"].ToString());
            update.ProcessRequest(HttpContext.Current, aRotationID,allStrawYield[arrFieldPlanRotation], inputGrazingPart_[arrFieldPlanRotation], allmainYield[arrFieldPlanRotation], aOrganicFertilizer[arrFieldPlanRotation], AllSelectStrawUseType[arrFieldPlanRotation]);
            HttpContext.Current.Session["ShowResult"] = false;
        }
        private void SaveRotationEventDropDown(object sender, System.EventArgs e)
        {
            RotationUpdate update = new RotationUpdate();
            DropDownList button = (DropDownList)sender;
            int arrFieldPlanRotation = int.Parse(button.Attributes["arrFieldPlanRotation"].ToString());
            int aRotationID = int.Parse(button.Attributes["aRotationID"].ToString());
            update.ProcessRequest(HttpContext.Current, aRotationID, allStrawYield[arrFieldPlanRotation],inputGrazingPart_[arrFieldPlanRotation], allmainYield[arrFieldPlanRotation], aOrganicFertilizer[arrFieldPlanRotation], AllSelectStrawUseType[arrFieldPlanRotation]);
            HttpContext.Current.Session["ShowResult"] = false;

        }
        private void SaveRotationTextChange(object sender, System.EventArgs e)
        {
            RotationUpdate update = new RotationUpdate();
            TextBox button = (TextBox)sender;
            int aRotationID = int.Parse(button.Attributes["aRotationID"].ToString());
            int arrFieldPlanRotation = int.Parse(button.Attributes["arrFieldPlanRotation"].ToString());
            update.ProcessRequest(HttpContext.Current, aRotationID, allStrawYield[arrFieldPlanRotation], inputGrazingPart_[arrFieldPlanRotation], allmainYield[arrFieldPlanRotation], aOrganicFertilizer[arrFieldPlanRotation], AllSelectStrawUseType[arrFieldPlanRotation]);
            HttpContext.Current.Session["ShowResult"] = false;
        }
    }
}