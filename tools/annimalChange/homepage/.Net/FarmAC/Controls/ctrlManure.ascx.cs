using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.HtmlControls;
using System.Web.UI.WebControls;

namespace FarmAC.Controls
{
    public partial class ctrlManure : System.Web.UI.UserControl
    {
        string language;
        protected void Page_Load(object sender, EventArgs e)
        {
            if (HttpContext.Current.Session["UserInit"] != null)
                if (HttpContext.Current.Session["UserInit"].ToString() == "7")
            {
                if (HttpContext.Current.Session["languageSelected"] != null)
                    language = HttpContext.Current.Session["languageSelected"].ToString();
                else
                    language = "uk";
                CreateTabManure();
            }
        }


        string tid0, tid1, tid2, tid3, tid4;

        string nManureFertilizerDelivery, dictManureFertilizerDelivery, MeanUtilization, totalManureNContent, totalManureShareOfNNeed;
        DataTable arrManureFertilizerDelivery;
        string arrA;  // array of ManureTypes
        string arrB;  // array of fields(FieldPlanRotations)
        string arrFertOut, arrFieldOut, scale, warning;
        string dummy1, dummy2, NPercent, nFieldPlanRotation;
        DataTable arrFieldPlanRotation;
        string dictFieldPlanRotation;
        string nFarmDelivery, dictFarmDelivery;
        DataTable arrFarmDelivery;
        string nFarmDeliverys, dictFarmDeliverys;
        DataTable arrFarmDeliverys;
        string dictManureNContent;
        DataTable arrManureNContent;
        string nManureNContent;
        string dictManureNModelled, nManureNModelled;
        DataTable arrManureNModelled;
        string aMonth;

        string aDeliveryAmoun = "0";

        List<Button> newBotten = new List<Button>();

        List<List<DropDownList>> AllMonth = new List<List<DropDownList>>();
        List<List<DropDownList>> AllSelManure_ = new List<List<DropDownList>>();
        List<List<DropDownList>> AllSelDelivery_ = new List<List<DropDownList>>();
        List<List<TextBox>> AllDeliveryAmount = new List<List<TextBox>>();



        void CreateTabManure()
        {


            dbInterface database = new dbInterface();
            database.InitFarmNDB();

            //  CreateLanguageDictionary
            Panel1.Controls.Add(new LiteralControl(("<script language=" + "javascript" + ">window.parent.frames[1].location.reload()</script>" + "\r")));
            //  DefineManureListJScript_27


            Panel1.Controls.Add(new LiteralControl(("<br>" + "\r")));

            Panel1.Controls.Add(new LiteralControl(("<h3>" + (database.GetLanguageString("strNContentAbStorage", language)) + "</h3><br/>" + "\r")));
            List<dbInterface.data> paramter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "@FarmNumber";
            data.value = HttpContext.Current.Session["farmNumber"].ToString();
            paramter.Add(data);
            data.name = "@ScenarioID";
            data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            paramter.Add(data);

            arrManureNModelled = database.GetValuesFromDB("GetManureBalance", paramter);

            Panel1.Controls.Add(new LiteralControl(("  <table border=" + "0" + " cellpadding=" + "5" + " cellspacing=" + "3" + ">" + "\r")));
            Panel1.Controls.Add(new LiteralControl(("    <tr>" + "\r")));
            Panel1.Controls.Add(new LiteralControl(("     <th align=" + "left" + ">" + (database.GetLanguageString("strMANURETYPE", language)) + " </th>" + "\r")));

            Panel1.Controls.Add(new LiteralControl(("      <td></td><th align=" + "left" + " colspan=" + "2" + ">" + (database.GetLanguageString("strFIELDPROD", language)) + "</th>" + "\r")));
            Panel1.Controls.Add(new LiteralControl(("      <th align=" + "left" + " colspan=" + "2" + ">" + (database.GetLanguageString("strBOUGHT", language)) + "</th>" + "\r")));

            Panel1.Controls.Add(new LiteralControl(("      <th align=" + "left" + " colspan=" + "2" + ">" + (database.GetLanguageString("strHOUSINGUSE", language)) + "</th>" + "\r")));
            Panel1.Controls.Add(new LiteralControl(("    </tr>" + "\r")));
            for (int r = 0; r < arrManureNModelled.Rows.Count; r++)
            {
                if (arrManureNModelled.Rows[r]["AmountProduced"].ToString() != "0" || arrManureNModelled.Rows[r]["Bought"].ToString() != "0")
                {
                    Panel1.Controls.Add(new LiteralControl(("    <tr>" + "\r")));
                    Panel1.Controls.Add(new LiteralControl(("      <th align=" + "left" + ">" + arrManureNModelled.Rows[r]["StorageName"] + "</th>" + "\r")));
                    Panel1.Controls.Add(new LiteralControl(("      <td></td><td align=" + "right" + ">" + Math.Round(double.Parse(arrManureNModelled.Rows[r]["AmountProduced"].ToString()), 2) + "</td>" + "\r")));
                    if(arrManureNModelled.Rows[r]["Bought"].ToString()=="")
                        Panel1.Controls.Add(new LiteralControl(("      <td></td><td align=" + "right" + ">" + 0 + "</td>" + "\r")));
                    else
                        Panel1.Controls.Add(new LiteralControl(("      <td></td><td align=" + "right" + ">" + arrManureNModelled.Rows[r]["Bought"] + "</td>" + "\r")));
                    if (arrManureNModelled.Rows[r]["Amount"] == "")
                    {
                        string tmp = arrManureNModelled.Rows[r]["Amount"].ToString();
                        Panel1.Controls.Add(new LiteralControl(("      <td></td><td align=" + "right" + ">" + arrManureNModelled.Rows[r]["Amount"] + "</td>" + "\r")));
                    }
                    else
                    {
                        string tmp = arrManureNModelled.Rows[r]["Amount"].ToString();
                        Panel1.Controls.Add(new LiteralControl(("      <td></td><td align=" + "right" + ">" + arrManureNModelled.Rows[r]["Amount"] + "</td>" + "\r")));
                    }
                    Panel1.Controls.Add(new LiteralControl(("    </tr>" + "\r")));
                }
            }
            Panel1.Controls.Add(new LiteralControl(("    <tr><td></td>" + "\r")));
            Panel1.Controls.Add(new LiteralControl(("    </tr>" + "\r")));
            Panel1.Controls.Add(new LiteralControl(("  </table>" + "\r")));

            Panel1.Controls.Add(new LiteralControl(("<br>" + "\r")));

            Panel1.Controls.Add(new LiteralControl(("<h3>" + (database.GetLanguageString("strDISTMANURE", language)) + "</h3><br/>" + "\r")));


            Panel1.Controls.Add(new LiteralControl(("<form name=" + "formManureFertilizer" + " action=" + "ManureFertilizerUpdate.asp" + " method=" + "post" + " accept-charset=" + "iso-8859-1" + " id=" + "dynamichide" + " style=" + "display: block;" + ">" + "\r")));
            Panel1.Controls.Add(new LiteralControl(("  <table border=" + "0" + " cellpadding=" + "3" + " cellspacing=" + "0" + ">" + "\r")));


            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = HttpContext.Current.Session["farmNumber"].ToString();
            paramter.Add(data);
            data.name = "@aScenario";
            data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            paramter.Add(data);
            arrFieldPlanRotation = database.GetValuesFromDB("GetFieldplanRotationsForManure ", paramter);

            paramter = new List<dbInterface.data>();
            arrFarmDelivery = database.GetValuesFromDB("GetSelectListDeliveryType ", paramter);

            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = HttpContext.Current.Session["farmNumber"].ToString();
            paramter.Add(data);
            data.name = "@aScenarioID";
            data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            paramter.Add(data);
            arrManureNContent = database.GetValuesFromDB("GetManure_N_Content", paramter);
            CreateTableManureDelivery();


            Panel1.Controls.Add(new LiteralControl(("<br/>" + "\r")));
            Panel1.Controls.Add(new LiteralControl(("<br/>" + "\r")));
            database.CloseFarmNDB();

        }
        void CreateTableManureDelivery()
        {
            string aDeliveryAmount = "0";
            string f, b, r;
            string totalNDelivered, totalNLoss;
            string aDeliveryID, t;
            string fpID;
            int aStorageID = 0;

            dbInterface database = new dbInterface();

            Panel1.Controls.Add(new LiteralControl(("    <tr>" + "\r")));
            Panel1.Controls.Add(new LiteralControl(("      <th align=" + "left" + ">" + (database.GetLanguageString("strFIELD", language)) + "<br/></th>" + "\r")));

            Panel1.Controls.Add(new LiteralControl(("      <th align=" + "left" + ">" + (database.GetLanguageString("strCROP", language)) + "</th>" + "\r")));
            Panel1.Controls.Add(new LiteralControl(("      <th align=" + "left" + ">" + (database.GetLanguageString("strTYPEDIST", language)) + "<br/>" + (database.GetLanguageString("strfertilizer", language)) + "<br/></th>" + "\r")));
            Panel1.Controls.Add(new LiteralControl(("      <th align=" + "left" + "><span style=" + "position:relative;left:80px;" + ">" + (database.GetLanguageString("strSPREADER", language)) + "<br/>" + (database.GetLanguageString("strMETHOD", language)) + "</span></th>" + "\r")));
            Panel1.Controls.Add(new LiteralControl(("      <th align=" + "left" + ">" + (database.GetLanguageString("strKGNBARE", language)) + "/" + (database.GetLanguageString("strHA", language)) + "</th>" + "\r")));

            Panel1.Controls.Add(new LiteralControl(("      <th align=" + "left" + "></th>" + "\r")));
            Panel1.Controls.Add(new LiteralControl(("    </tr>" + "\r")));


            for (int a = 0; a < arrFieldPlanRotation.Rows.Count; a++)
            {

                AllMonth.Add(new List<DropDownList>());
                AllSelManure_.Add(new List<DropDownList>());
                AllSelDelivery_.Add(new List<DropDownList>());
                AllDeliveryAmount.Add(new List<TextBox>());
                fpID = arrFieldPlanRotation.Rows[a]["FieldPlanRotationID"].ToString();
                Panel1.Controls.Add(new LiteralControl(("    <tr>" + "\r")));
                string name = arrFieldPlanRotation.Rows[a]["RotationName"].ToString();
                string CropName = arrFieldPlanRotation.Rows[a]["CropName"].ToString();
                Panel1.Controls.Add(new LiteralControl(("      <td align=" + "left" + ">" + arrFieldPlanRotation.Rows[a]["RotationName"] + "</td>" + "\r")));
                Panel1.Controls.Add(new LiteralControl(("      <td align=" + "left" + ">" + database.GetLanguageString("str" + arrFieldPlanRotation.Rows[a]["CropName"], language) + "</td>" + "\r")));
                Panel1.Controls.Add(new LiteralControl(("      <td align=" + "left" + " colspan=" + "4" + ">" + "\r")));

                Panel1.Controls.Add(new LiteralControl(("<table border=" + "0" + " cellspacing=" + "0" + " width=" + "100%" + " style=" + "position:relative; top: -8px;" + ">" + "\r")));
                Panel1.Controls.Add(new LiteralControl(("<tbody id=" + "tbodyManure_" + a + "" + " name=" + "tbodyManure_" + a + "" + ">" + "\r")));
                List<dbInterface.data> paramter = new List<dbInterface.data>();
                dbInterface.data data = new dbInterface.data();
                data.name = "@aFarmNumber";
                data.value = HttpContext.Current.Session["farmNumber"].ToString();
                paramter.Add(data);
                data.name = "@aScenarioID";
                data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
                paramter.Add(data);
                data.name = "@aRotationID";
                data.value = arrFieldPlanRotation.Rows[a]["RotationID"].ToString();
                paramter.Add(data);
                data.name = "@aFieldPlanRotationID";
                data.value = arrFieldPlanRotation.Rows[a]["FieldPlanRotationID"].ToString();
                paramter.Add(data);

                arrManureFertilizerDelivery = database.GetValuesFromDB("GetManureFertilizerDelivery ", paramter);//ændres, så den kaldes for hver fieldplanrotationID

                int amout = arrManureFertilizerDelivery.Rows.Count;
                for (int i = 0; i < arrManureFertilizerDelivery.Rows.Count; i++)
                {

                    Panel1.Controls.Add(new LiteralControl(("<tr>" + "\r")));
                    Panel1.Controls.Add(new LiteralControl(("<td>" + "\r")));
                    DropDownList selManure_ = new DropDownList();
                    selManure_.ID = "selManure_" + a + " " + i.ToString();
                    selManure_.Items.Add(database.GetLanguageString("strSelectManureFertilizer", language));
                    selManure_.Items[0].Attributes.Add("dbValue", "-1");
                    //  Panel1.Controls.Add(new LiteralControl(("<select id="+"selManure_"+a+""+" name="+"selManure_"+a+""+" onchange="+"checkForNewLine("+a+");setManureString("+a+");"+" style="+"position:relative;top:-3Px;background-color: white;"+" onkeydown="+"if(event.keyCode==13) {veil();document.forms.formManureFertilizer.action='ManureFertilizerUpdate.asp';document.forms.formManureFertilizer.submit();};"+">" +"\r")));
                    //Panel1.Controls.Add(new LiteralControl(("  <option value="+"-1"+">"+("strSelectManureFertilizer")+"</option>" +"\r")));
                    for (int nf = 0; nf < arrManureNContent.Rows.Count; nf++)
                    {
                        if (arrManureFertilizerDelivery.Rows.Count == 0)
                            aStorageID = -1;
                        else
                        {
                            if (arrManureNContent.Rows.Count != 0)
                            {
                                if (nf < arrManureNContent.Rows.Count)
                                {
                                    aStorageID = int.Parse(arrManureFertilizerDelivery.Rows[i][5].ToString());
                                    paramter = new List<dbInterface.data>();
                                    data = new dbInterface.data();
                                    data.name = "@StorageID";
                                    data.value = aStorageID.ToString();
                                    paramter.Add(data);
                                    arrFarmDeliverys = database.GetValuesFromDB("GetSelectListDeliveryTypePrStorageType", paramter);
                                }
                                else
                                {
                                    aStorageID = -1;

                                }
                            }

                            if (aStorageID.ToString() == arrManureNContent.Rows[nf][0].ToString())
                            {
                                //  Panel1.Controls.Add(new LiteralControl(("  <option value=" + "" + arrManureNContent.Rows[nf][0] + "" + " selected>" + ("str" + arrManureNContent.Rows[nf][1] + "+") + "</option>" + "\r")));
                                selManure_.Items.Add(arrManureNContent.Rows[nf][0] + " " + arrManureNContent.Rows[nf][1]);
                                selManure_.Items[selManure_.Items.Count - 1].Attributes.Add("dbValue", arrManureNContent.Rows[nf][0].ToString());
                                selManure_.SelectedIndex = selManure_.Items.Count - 1;
                            }
                            else
                            {
                                //Panel1.Controls.Add(new LiteralControl(("  <option value=" + "" + arrManureNContent.Rows[nf][0] + "" + ">" + ("str" + arrManureNContent.Rows[nf][1] + "+") + "</option>" + "\r")));
                                selManure_.Items.Add(arrManureNContent.Rows[nf][0] + " " + arrManureNContent.Rows[nf][1]);
                                selManure_.Items[selManure_.Items.Count - 1].Attributes.Add("dbValue", arrManureNContent.Rows[nf][0].ToString());
                            }
                        }
                    }
                    selManure_.Items.Add(database.GetLanguageString("strAmmonium nitrate", language));
                    selManure_.Items[selManure_.Items.Count - 1].Attributes.Add("dbValue", "31");
                    if (arrManureFertilizerDelivery.Rows[i][5].ToString() == "31")
                        selManure_.SelectedIndex = selManure_.Items.Count - 1;


                    string st = arrManureFertilizerDelivery.Rows[i][5].ToString();
                    selManure_.Items.Add(database.GetLanguageString("strAmmonium sulphate", language));
                    selManure_.Items[selManure_.Items.Count - 1].Attributes.Add("dbValue", "32");
                    if (arrManureFertilizerDelivery.Rows[i][5].ToString() == "32")
                        selManure_.SelectedIndex = selManure_.Items.Count - 1;


                    selManure_.Items.Add(database.GetLanguageString("strCalcium ammonium nitrate", language));
                    selManure_.Items[selManure_.Items.Count - 1].Attributes.Add("dbValue", "33");
                    if (arrManureFertilizerDelivery.Rows[i][5].ToString() == "33")
                        selManure_.SelectedIndex = selManure_.Items.Count - 1;


                    selManure_.Items.Add(database.GetLanguageString("strCalcium nitrate", language));
                    selManure_.Items[selManure_.Items.Count - 1].Attributes.Add("dbValue", "34");
                    if (arrManureFertilizerDelivery.Rows[i][5].ToString() == "34")
                        selManure_.SelectedIndex = selManure_.Items.Count - 1;



                    selManure_.Items.Add(database.GetLanguageString("strDiammonium phosphate", language));
                    selManure_.Items[selManure_.Items.Count - 1].Attributes.Add("dbValue", "35");
                    if (arrManureFertilizerDelivery.Rows[i][5].ToString() == "35")
                        selManure_.SelectedIndex = selManure_.Items.Count - 1;


                    selManure_.Items.Add(database.GetLanguageString("strMonoammonium phosphate", language));
                    selManure_.Items[selManure_.Items.Count - 1].Attributes.Add("dbValue", "36");
                    if (arrManureFertilizerDelivery.Rows[i][5].ToString() == "36")
                        selManure_.SelectedIndex = selManure_.Items.Count - 1;

                    selManure_.Items.Add(database.GetLanguageString("strSulphur-coated urea", language));
                    selManure_.Items[selManure_.Items.Count - 1].Attributes.Add("dbValue", "37");
                    if (arrManureFertilizerDelivery.Rows[i][5].ToString() == "37")
                        selManure_.SelectedIndex = selManure_.Items.Count - 1;


                    selManure_.Items.Add(database.GetLanguageString("strUrea ammonium nitrate", language));
                    selManure_.Items[selManure_.Items.Count - 1].Attributes.Add("dbValue", "38");
                    if (arrManureFertilizerDelivery.Rows[i][5].ToString() == "38")
                        selManure_.SelectedIndex = selManure_.Items.Count - 1;


                    selManure_.Items.Add(database.GetLanguageString("strUrea ammonium sulphate", language));
                    selManure_.Items[selManure_.Items.Count - 1].Attributes.Add("dbValue", "39");
                    if (arrManureFertilizerDelivery.Rows[i][5].ToString() == "39")
                        selManure_.SelectedIndex = selManure_.Items.Count - 1;


                    selManure_.Items.Add(database.GetLanguageString("strUrea", language));
                    selManure_.Items[selManure_.Items.Count - 1].Attributes.Add("dbValue", "40");
                    if (arrManureFertilizerDelivery.Rows[i][5].ToString() == "40")
                        selManure_.SelectedIndex = selManure_.Items.Count - 1;


                    selManure_.Items.Add(database.GetLanguageString("strNitrateInhibitor", language));
                    selManure_.Items[selManure_.Items.Count - 1].Attributes.Add("dbValue", "41");
                    if (arrManureFertilizerDelivery.Rows[i][5].ToString() == "41")
                        selManure_.SelectedIndex = selManure_.Items.Count - 1;


                    selManure_.Items.Add(database.GetLanguageString("strNPK-fertiliser", language));
                    selManure_.Items[selManure_.Items.Count - 1].Attributes.Add("dbValue", "42");
                    if (arrManureFertilizerDelivery.Rows[i][5].ToString() == "42")
                        selManure_.SelectedIndex = selManure_.Items.Count - 1;



                    Panel1.Controls.Add(selManure_);
                    AllSelManure_[a].Add(selManure_);
                    DropDownList selDelivery_ = new DropDownList();
                    selDelivery_.ID = "selDelivery_" + a + " " + i.ToString();
                    selDelivery_.Items.Add(database.GetLanguageString("strSelectDeliveryMethod", language));
                    selDelivery_.Items[0].Attributes.Add("dbValue", "-1");
                    // Panel1.Controls.Add(new LiteralControl(("<select id="+"selDelivery_"+a+""+" name="+"selDelivery_"+a+""+" onchange="+"setDeliveryString("+a+");"+" style="+"width: 180px;position:relative;top:-3Px;background-color: white;"+" onkeydown="+"if(event.keyCode==13) {veil();document.forms.formManureFertilizer.action='ManureFertilizerUpdate.asp';document.forms.formManureFertilizer.submit();};"+">" +"\r")));
                    //Panel1.Controls.Add(new LiteralControl(("  <option value="+"-1"+">"+("strSelectDeliveryMethod")+"</option>" +"\r")));
                    if (arrManureFertilizerDelivery.Rows.Count > 0)
                    {

                        for (int nm = 0; nm < arrFarmDelivery.Rows.Count; nm++)
                        {
                            if (arrManureFertilizerDelivery.Rows.Count > 0)
                            {
                                aDeliveryID = arrManureFertilizerDelivery.Rows[i][7].ToString();
                                aDeliveryAmount = arrManureFertilizerDelivery.Rows[i][9].ToString();//'/arrManureFertilizerDelivery(i,11)
                                aMonth = arrManureFertilizerDelivery.Rows[i][13].ToString();
                            }
                            else
                            {
                                aDeliveryID = "-1";
                                aDeliveryAmount = "0";
                                aMonth = "-1";
                            }

                            object tdfsg = arrFarmDelivery.Rows[nm][0];
                            string stuff = arrFarmDelivery.Rows[nm][1].ToString();
                            if (aDeliveryID == arrFarmDelivery.Rows[nm][0].ToString())
                            {
                                selDelivery_.Items.Add(arrFarmDelivery.Rows[nm][0] + " " + arrFarmDelivery.Rows[nm][1]);
                                selDelivery_.Items[selDelivery_.Items.Count - 1].Attributes.Add("dbValue", arrFarmDelivery.Rows[nm][0].ToString());
                                selDelivery_.SelectedIndex = selDelivery_.Items.Count - 1;
                                //  Panel1.Controls.Add(new LiteralControl(("  <option value=" + "" + arrFarmDelivery.Rows[nm][0] + "" + " selected>" + ("str" + arrFarmDelivery.Rows[nm][1] + "+") + "</option>" + "\r")));
                            }
                            else
                            {
                                selDelivery_.Items.Add(arrFarmDelivery.Rows[nm][0] + " " + arrFarmDelivery.Rows[nm][1]);
                                selDelivery_.Items[selDelivery_.Items.Count - 1].Attributes.Add("dbValue", arrFarmDelivery.Rows[nm][0].ToString());
                                // Panel1.Controls.Add(new LiteralControl(("  <option value=" + "" + arrFarmDelivery.Rows[nm][0] + "" + ">" + ("str" + arrFarmDelivery.Rows[nm][1] + "+") + "</option>" + "\r")));
                            }


                        }
                    }
                    else
                    {
                        for (int nm = 0; nm < arrFarmDeliverys.Rows.Count; nm++)
                        {
                            if (arrManureFertilizerDelivery.Rows.Count > 0)
                            {
                                aDeliveryID = arrManureFertilizerDelivery.Rows[i][7].ToString();
                                aDeliveryAmount = arrManureFertilizerDelivery.Rows[i][9].ToString();//'/arrManureFertilizerDelivery(i,11)
                                aMonth = arrManureFertilizerDelivery.Rows[i][13].ToString();
                            }
                            else
                            {
                                aDeliveryID = "-1";
                                aDeliveryAmount = "0";
                                aMonth = "-1";
                            }
                            if (aDeliveryID == arrFarmDelivery.Rows[nm][0].ToString())
                            {
                                selDelivery_.Items.Add(arrFarmDelivery.Rows[nm][0] + " " + arrFarmDelivery.Rows[nm][1]);
                                selDelivery_.Items[selDelivery_.Items.Count - 1].Attributes.Add("dbValue", arrFarmDelivery.Rows[nm][0].ToString());
                                selDelivery_.SelectedIndex = selDelivery_.Items.Count - 1;
                                //  Panel1.Controls.Add(new LiteralControl(("  <option value=" + "" + arrFarmDelivery.Rows[nm][0] + "" + " selected>" + ("str" + arrFarmDelivery.Rows[nm][1] + "+") + "</option>" + "\r")));
                            }
                            else
                            {
                                selDelivery_.Items.Add(arrFarmDelivery.Rows[nm][0] + " " + arrFarmDelivery.Rows[nm][1]);
                                selDelivery_.Items[selDelivery_.Items.Count - 1].Attributes.Add("dbValue", arrFarmDelivery.Rows[nm][0].ToString());
                                // Panel1.Controls.Add(new LiteralControl(("  <option value=" + "" + arrFarmDelivery.Rows[nm][0] + "" + ">" + ("str" + arrFarmDelivery.Rows[nm][1] + "+") + "</option>" + "\r")));
                            }
                        }
                    }

                    Panel1.Controls.Add(selDelivery_);
                    AllSelDelivery_[a].Add(selDelivery_);
                    // Panel1.Controls.Add(new LiteralControl(("</select>" +"\r")));

                    TextBox amount = new TextBox();
                    amount.ID = "Amount_" + a + " " + i.ToString();
                    amount.Text = aDeliveryAmount.ToString();
                    Panel1.Controls.Add(amount);
                    AllDeliveryAmount[a].Add(amount);
                    //  Panel1.Controls.Add(new LiteralControl(("<input id="+"Amount_"+a+""+" name="+"Amount_"+a+""+" type="+"text"+" style="+"width: 55px;position:relative;top:-3Px;background-color: white;"+" value="+""+aDeliveryAmount+""+" onchange="+"setAmountString("+a+");"+" onkeydown="+"if(event.keyCode==13) {veil();document.forms.formManureFertilizer.action='ManureFertilizerUpdate.asp';document.forms.formManureFertilizer.submit();};"+"/>" +"\r")));
                    CreateMonthSelect(a.ToString(), i.ToString(), "white", int.Parse(aMonth));
                    HtmlButton delete = new HtmlButton();
                    delete.InnerHtml = "Delete";
                    delete.ID = "delete" + a.ToString() + " " + i.ToString();
                    delete.Attributes.Add("arrFieldPlanRotation", a.ToString());
                    delete.Attributes.Add("arrManureFertilizerDelivery", i.ToString());
                    delete.ServerClick += new EventHandler(deleteEvent);
                    Panel1.Controls.Add(delete);
                    //Panel1.Controls.Add(new LiteralControl(("<img style="+"position:relative;top:3Px;"+" src="+"images/del.gif"+" onclick="+"delRow(this,"+a+");"+" onMouseover="+"mouseOverGif(this)"+"  onMouseout="+"mouseOutGif(this)"+"/>" +"\r")));
                    Panel1.Controls.Add(new LiteralControl(("</td>" + "\r")));
                    Panel1.Controls.Add(new LiteralControl(("</tr>" + "\r")));

                }
                Panel1.Controls.Add(new LiteralControl(("<tr>" + "\r")));
                Panel1.Controls.Add(new LiteralControl(("<td>" + "\r")));
                DropDownList selManureLast = new DropDownList();
                selManureLast.Items.Add(database.GetLanguageString("strSelectManureFertilizer",language));
                selManureLast.Items[0].Attributes.Add("dbValue", "-1");
                selManureLast.SelectedIndex = 0;
                for (int nf = 0; nf < arrManureNContent.Rows.Count; nf++)
                {
                    selManureLast.Items.Add(arrManureNContent.Rows[nf][0] + " " + arrManureNContent.Rows[nf][1]);
                    selManureLast.Items[selManureLast.Items.Count - 1].Attributes.Add("dbValue", arrManureNContent.Rows[nf][0].ToString());

                }

                selManureLast.Items.Add(database.GetLanguageString("strAmmonium nitrate", language));
                selManureLast.Items[selManureLast.Items.Count - 1].Attributes.Add("dbValue", "31");

                selManureLast.Items.Add(database.GetLanguageString("strAmmonium sulphate", language));
                selManureLast.Items[selManureLast.Items.Count - 1].Attributes.Add("dbValue", "32");

                selManureLast.Items.Add(database.GetLanguageString("strCalcium ammonium nitrate", language));
                selManureLast.Items[selManureLast.Items.Count - 1].Attributes.Add("dbValue", "33");

                selManureLast.Items.Add(database.GetLanguageString("strCalcium nitrate", language));
                selManureLast.Items[selManureLast.Items.Count - 1].Attributes.Add("dbValue", "34");

                selManureLast.Items.Add(database.GetLanguageString("strDiammonium phosphate", language));
                selManureLast.Items[selManureLast.Items.Count - 1].Attributes.Add("dbValue", "35");

                selManureLast.Items.Add(database.GetLanguageString("strMonoammonium phosphate", language));
                selManureLast.Items[selManureLast.Items.Count - 1].Attributes.Add("dbValue", "36");

                selManureLast.Items.Add(database.GetLanguageString("strSulphur-coated urea", language));
                selManureLast.Items[selManureLast.Items.Count - 1].Attributes.Add("dbValue", "37");

                selManureLast.Items.Add(database.GetLanguageString("strUrea ammonium nitrate", language));
                selManureLast.Items[selManureLast.Items.Count - 1].Attributes.Add("dbValue", "38");

                selManureLast.Items.Add(database.GetLanguageString("strUrea ammonium sulphate", language));
                selManureLast.Items[selManureLast.Items.Count - 1].Attributes.Add("dbValue", "39");

                selManureLast.Items.Add(database.GetLanguageString("strUrea", language));
                selManureLast.Items[selManureLast.Items.Count - 1].Attributes.Add("dbValue", "40");

                selManureLast.Items.Add(database.GetLanguageString("strNitrateInhibitor", language));
                selManureLast.Items[selManureLast.Items.Count - 1].Attributes.Add("dbValue", "41");

                selManureLast.Items.Add(database.GetLanguageString("strNPK-fertiliser", language));
                selManureLast.Items[selManureLast.Items.Count - 1].Attributes.Add("dbValue", "42");
                AllSelManure_[a].Add(selManureLast);

                DropDownList selDeliveryLast = new DropDownList();
                selDeliveryLast.Items.Add(database.GetLanguageString("strSelectDeliveryMethod", language));
                selDeliveryLast.Items[0].Attributes.Add("dbValue", "-1");
                selDeliveryLast.SelectedIndex = 0;



                for (int nm = 0; nm < arrFarmDelivery.Rows.Count; nm++)
                {
                    selDeliveryLast.Items.Add(arrFarmDelivery.Rows[nm][0] + " " + arrFarmDelivery.Rows[nm][1]);
                    selDeliveryLast.Items[selDeliveryLast.Items.Count - 1].Attributes.Add("dbValue", arrFarmDelivery.Rows[nm][0].ToString());

                }

                Panel1.Controls.Add(selManureLast);
                Panel1.Controls.Add(selDeliveryLast);
                AllSelDelivery_[a].Add(selDeliveryLast);
                TextBox amounts = new TextBox();
                amounts.ID = "Amount_" + a;
                amounts.Text = "0";
                Panel1.Controls.Add(amounts);
                AllDeliveryAmount[a].Add(amounts);
                CreateMonthSelect(a.ToString(), "99", "lightyellow", -1);
                HtmlButton newBot = new HtmlButton();

                newBot.ID = "newBotten" + a.ToString();
                newBot.InnerHtml = "New manure";
                newBot.ServerClick += new EventHandler(newManure);
                Panel1.Controls.Add(newBot);
                Panel1.Controls.Add(new LiteralControl(("</td>" + "\r")));
                Panel1.Controls.Add(new LiteralControl(("</tr>" + "\r")));
                Panel1.Controls.Add(new LiteralControl(("</tbody>" + "\r")));
                Panel1.Controls.Add(new LiteralControl(("</table>" + "\r")));

                Panel1.Controls.Add(new LiteralControl(("      </td>" + "\r")));

                Panel1.Controls.Add(new LiteralControl(("    </tr>" + "\r")));

                // Panel1.Controls.Add(new LiteralControl(("<tr><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td><button style="+"width: 50px; height: 20px; font-size: 10px"+" onClick="+"{veil();document.forms.formManureFertilizer.action='ManureFertilizerUpdate.asp';document.forms.formManureFertilizer.submit();}"+">"+("strSAVE")+"</button></td></tr>")));
                Panel1.Controls.Add(new LiteralControl(("    <tr>" + "\r")));
                Panel1.Controls.Add(new LiteralControl(("      <td colspan=" + "10" + "><hr/></td>" + "\r")));

                Panel1.Controls.Add(new LiteralControl(("    </tr>" + "\r")));


            }
            Panel1.Controls.Add(new LiteralControl("<tr><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td>"));
            HtmlButton save = new HtmlButton();
            save.ID = "Save";
            save.InnerHtml = "Save";
            save.ServerClick += new EventHandler(newManure);
            Panel1.Controls.Add(save);
            Panel1.Controls.Add(new LiteralControl("  </td></tr>"));
            Panel1.Controls.Add(new LiteralControl(("    </tr>" + "\r")));

            Panel1.Controls.Add(new LiteralControl(("  </table>" + "\r")));
            Panel1.Controls.Add(new LiteralControl(("<input type=" + "hidden" + " name=" + "hiddenNFieldPlanRotation" + " value=" + "" + nFieldPlanRotation + "" + "/>" + "\r")));
            Panel1.Controls.Add(new LiteralControl(("</form>" + "\r")));
            //CreateUpdateFormManure

        }
        protected void deleteEvent(object sender, EventArgs e)
        {
            HtmlButton button = (HtmlButton)sender;
            int rotation = int.Parse(button.Attributes["arrFieldPlanRotation"].ToString());
            int Manure = int.Parse(button.Attributes["arrManureFertilizerDelivery"].ToString());
            AllMonth[rotation].RemoveAt(Manure);
            AllSelManure_[rotation].RemoveAt(Manure);
            AllSelDelivery_[rotation].RemoveAt(Manure);
            AllDeliveryAmount[rotation].RemoveAt(Manure);
            ManureFertilizerUpdate update = new ManureFertilizerUpdate();
            HttpContext.Current.Session["ShowResult"] = false;
            update.ProcessRequest(HttpContext.Current, AllMonth, AllSelManure_, AllSelDelivery_, AllDeliveryAmount);
            HttpContext.Current.Session["UserInit"] = "7";
            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);
        }
        protected void newManure(object sender, EventArgs e)
        {
            ManureFertilizerUpdate update = new ManureFertilizerUpdate();

            update.ProcessRequest(HttpContext.Current, AllMonth, AllSelManure_, AllSelDelivery_, AllDeliveryAmount);
            HttpContext.Current.Session["UserInit"] = "7";
            HttpContext.Current.Session["ShowResult"] = false;
            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);
        }
        void CreateMonthSelect(string b, string a, string color, int value)
        {
            dbInterface database = new dbInterface();
            DropDownList month = new DropDownList();
            month.ID = "selMonth_" + a + "_" + b;

            month.Items.Add(database.GetLanguageString("strSelectDeliveryMonth", language));
            month.Items[0].Attributes.Add("dbValue", "0");
            month.Items.Add(database.GetLanguageString("strJanuary", language));
            month.Items[1].Attributes.Add("dbValue", "1");
            month.Items.Add(database.GetLanguageString("strFebruary", language));
            month.Items[2].Attributes.Add("dbValue", "2");
            month.Items.Add(database.GetLanguageString("strMarch", language));
            month.Items[3].Attributes.Add("dbValue", "3");
            month.Items.Add(database.GetLanguageString("strApril", language));
            month.Items[4].Attributes.Add("dbValue", "4");
            month.Items.Add(database.GetLanguageString("strMay", language));
            month.Items[5].Attributes.Add("dbValue", "5");
            month.Items.Add(database.GetLanguageString("strJune", language));
            month.Items[6].Attributes.Add("dbValue", "6");
            month.Items.Add(database.GetLanguageString("strJuly", language));
            month.Items[7].Attributes.Add("dbValue", "7");
            month.Items.Add(database.GetLanguageString("strAugust", language));
            month.Items[8].Attributes.Add("dbValue", "8");
            month.Items.Add(database.GetLanguageString("strSeptember", language));
            month.Items[9].Attributes.Add("dbValue", "9");
            month.Items.Add(database.GetLanguageString("strOctober", language));
            month.Items[10].Attributes.Add("dbValue", "10");
            month.Items.Add(database.GetLanguageString("strNovember", language));
            month.Items[11].Attributes.Add("dbValue", "11");
            month.Items.Add(database.GetLanguageString("strDecember", language));
            month.Items[12].Attributes.Add("dbValue", "12");
            month.SelectedIndex = value;
            Panel1.Controls.Add(month);
            AllMonth[int.Parse(b)].Add(month);
            /*
                   Panel1.Controls.Add(new LiteralControl(("<select id="+"selMonth_"+a+""+" name="+"selMonth_"+a+""+" style="+"position:relative;top:-3Px;background-color: "+color+";"+" onchange="+"setMonthString("+a+");"+" onkeydown="+"veil();document.forms.formManureFertilizer.action='ManureFertilizerUpdate.asp';document.forms.formManureFertilizer.submit();"+">" +"\r")));
                   Panel1.Controls.Add(new LiteralControl(("  <option value="+"-1"+"")));
                   Panel1.Controls.Add(new LiteralControl((">"+("strSelectDeliveryMonth")+"</option>" +"\r")));
                   Panel1.Controls.Add(new LiteralControl(("  <option value="+"1"+"")));
                   if( value == 1) 
                       Panel1.Controls.Add(new LiteralControl( " selected"));
                   Panel1.Controls.Add(new LiteralControl((">"+("strJanuary")+"</option>" +"\r")));
                   Panel1.Controls.Add(new LiteralControl(("  <option value="+"2"+"")));
                   if (value == 2) 
                       Panel1.Controls.Add(new LiteralControl( " selected"));
                   Panel1.Controls.Add(new LiteralControl((">"+("strFebruary")+"</option>" +"\r")));
                   Panel1.Controls.Add(new LiteralControl(("  <option value="+"3"+"")));
                   if (value == 3) 
                       Panel1.Controls.Add(new LiteralControl( " selected"));
                   Panel1.Controls.Add(new LiteralControl((">"+("strMarch")+"</option>" +"\r")));
                   Panel1.Controls.Add(new LiteralControl(("  <option value="+"4"+"")));
                   if( value == 4) 
                       Panel1.Controls.Add(new LiteralControl( " selected"));
                   Panel1.Controls.Add(new LiteralControl((">"+("strApril")+"</option>" +"\r")));
                   Panel1.Controls.Add(new LiteralControl(("  <option value="+"5"+"")));
                   if (value == 5) 
                       Panel1.Controls.Add(new LiteralControl( " selected"));
                   Panel1.Controls.Add(new LiteralControl((">"+("strMay")+"</option>" +"\r")));
                   Panel1.Controls.Add(new LiteralControl(("  <option value="+"6"+"")));
                   if (value == 6) 
                       Panel1.Controls.Add(new LiteralControl( " selected"));
                   Panel1.Controls.Add(new LiteralControl((">"+("strJune")+"</option>" +"\r")));
                   Panel1.Controls.Add(new LiteralControl(("  <option value="+"7"+"")));
                   if (value == 7) 
                       Panel1.Controls.Add(new LiteralControl( " selected"));
                   Panel1.Controls.Add(new LiteralControl((">"+("strJuly")+"</option>" +"\r")));
                   Panel1.Controls.Add(new LiteralControl(("  <option value="+"8"+"")));
                   if (value == 8) 
                       Panel1.Controls.Add(new LiteralControl( " selected"));
                   Panel1.Controls.Add(new LiteralControl((">"+("strAugust")+"</option>" +"\r")));
                   Panel1.Controls.Add(new LiteralControl(("  <option value="+"9"+"")));
                   if (value == 9) 
                       Panel1.Controls.Add(new LiteralControl( " selected"));
                   Panel1.Controls.Add(new LiteralControl((">"+("strSeptember")+"</option>" +"\r")));
                   Panel1.Controls.Add(new LiteralControl(("  <option value="+"10"+"")));
                   if (value == 10) 
                       Panel1.Controls.Add(new LiteralControl( " selected"));
                   Panel1.Controls.Add(new LiteralControl((">"+("strOctober")+"</option>" +"\r")));
                   Panel1.Controls.Add(new LiteralControl(("  <option value="+"11"+"")));
                   if (value == 11) 
                       Panel1.Controls.Add(new LiteralControl( " selected"));
                   Panel1.Controls.Add(new LiteralControl((">"+("strNovember")+"</option>" +"\r")));
                   Panel1.Controls.Add(new LiteralControl(("  <option value="+"12"+"")));
                   if( value == 12) 
                       Panel1.Controls.Add(new LiteralControl( " selected"));
                   Panel1.Controls.Add(new LiteralControl((">"+("strDecember")+"</option>" +"\r")));
                   Panel1.Controls.Add(new LiteralControl(("</select>" +"\r")));*/


        }

        void CreateUpdateFormManure()
        {
            Panel1.Controls.Add(new LiteralControl(("<form name=" + "update" + " action=" + "javascript:document.forms.formManureFertilizer.submit();" + " method=" + "post" + ">" + "\r")));
            Panel1.Controls.Add(new LiteralControl(("<input type=" + "hidden" + " name=" + "saveUpdates" + " value=" + "false" + " />" + "\r")));
            Panel1.Controls.Add(new LiteralControl(("</form>" + "\r")));
        }
        /**
        sub DefineManureListJScript_27()
        %>
        <script language="javascript">

           function checkForNewLine(a)
           {
            var oTBody = document.getElementById('tbodyManure_'+a+'');
            var oLastRow = oTBody.lastChild;
            var newLine = false;
                var oInput = document.getElementById ("oInput");
            while (oLastRow.tagName != "TR")
            {
                oLastRow = oLastRow.previousSibling;
            }	
            for (var i = 0; i < oLastRow.childNodes.length; i++)
            {
                var oCell = oLastRow.childNodes[i];
                for (var j = 0; j < oCell.childNodes.length; j++)
                {
                    if (oCell.childNodes[j].tagName == "SELECT") 
                    {
                        var oCombo = oCell.childNodes[j];
                    }
                    if (oCell.childNodes[j].tagName == "INPUT")
                    {
                        oInput = oCell.childNodes[j];
                    }
                }
            }
            if ((oCombo.selectedIndex != 0) || (oInput.value.length > 0))
            {
                //Dette svarer til at der indsættes en ny række i tabellen, når brugeren begynder at indtaste data i den nederste række
                newLine = true;
            }
   
            if (newLine)
            {
                appendNewSelect(oTBody);
            }
           }
 
           function appendNewSelect(element) 
           {	
            var oTBody = element
            var oLastRow = oTBody.lastChild;
            //nedenstående loop er nødvendigt fordi IE og Netscape opfatter lastChild forskelligt. IE: direkte efterkommere. Netscape: sidste efterkommer (ikke nødvendigvis direkte)
            while (oLastRow.tagName != "TR")
            {
                oLastRow = oLastRow.previousSibling;	
            }
            var oNewRow = oLastRow.cloneNode(true);
            oTBody.appendChild(oNewRow);
           }

           function getSectionRowIndex(oElement)
           {
            var oParent = oElement.parentNode;
            while (oParent.tagName != "TR")
            {
                oParent = oParent.parentNode;
            }
            return oParent.sectionRowIndex;
           }
 
           function delRow(oElement,a)
           {
            var oTBody = document.getElementById ('tbodyManure_'+a+'');
            var actTBodyRowIndex = getSectionRowIndex(oElement);
        /*    	if (actTBodyRowIndex == 0) //første række i body - efter overskriften
            {
                var oRow = oTBody.firstChild;
                while (oRow.tagName != "TR")
                {
                    oRow = oRow.nextSibling;
                }
                var oRowChild = oRow.firstChild;
                if (oRowChild.hasChildNodes())
                //IE
                {
                    oRowChild.firstChild.selectedIndex = 0;

                }
                else
                {
                    while (!oRowChild.hasChildNodes())		
                    {
                        oRowChild = oRowChild.nextSibling;
                    }
                    oRowChild = oRowChild.firstChild;
                    while (oRowChild.tagName != "SELECT")		
                    {
                        oRowChild = oRowChild.nextSibling;
                    }
                    oRowChild.selectedIndex = 0;
                }
            }
            else
            {*/
        /*
         oTBody.deleteRow(actTBodyRowIndex);
 //   	}
         setManureString(a);
         setDeliveryString(a);
         setAmountString(a);
         setMonthString(a);
    }
 
    function getManure(a){
     var cManure = document.getElementsByName('selManure_'+a+'');
     var strManure = "+";
     for (var i = 0; i < cManure.length; i++)
     {
         if (parseInt(cManure[i].value,10) > 0)
         {
                 strManure += cManure[i].value +", ";
         }
     }
     return strManure;
    }

    function getDelivery(a){
     var cDelivery = document.getElementsByName('selDelivery_'+a+'');
     var strDelivery = "+";
     for (var i = 0; i < cDelivery.length; i++)
     {
         if (parseInt(cDelivery[i].value,10) > 0)
         {
                 strDelivery += cDelivery[i].value +", ";
         }
     }
     return strDelivery;
    }

    function getAmount(a){
     var cManure = document.getElementsByName('selManure_'+a+'');
     var strManure = "+";
     var cAmount = document.getElementsByName('Amount_'+a+'');
     var strAmount = "+";
     for (var i = 0; i < cManure.length; i++)
     {
         if (parseInt(cManure[i].value,10) > 0)
         {
                 strAmount += cAmount[i].value +"; ";
         }
     }
     return strAmount;
    }

    function getMonths(a){
     var cMonth = document.getElementsByName('selMonth_'+a+'');
     var strMonth = "+";
     for (var i = 0; i < cMonth.length; i++)
     {
         if (parseInt(cMonth[i].value,10) > 0)
         {
                 strMonth += cMonth[i].value +", ";
         }
     }
     return strMonth;
    }

    function blank(a,i){
     var util = document.getElementById('util_'+a+'_'+i+'');
     var deliv = document.getElementById('deliv_'+a+'_'+i+'');
     var loss = document.getElementById('loss_'+a+'_'+i+'');
     util.innerHTML='';
     deliv.innerHTML='';
     loss.innerHTML='';
    }

 
    function mouseOverGif(oImg)
    {
     oImg.setAttribute("src","Images/del1.gif");
    }
   
    function mouseOutGif(oImg)
    {
     oImg.setAttribute("src","Images/del.gif");
    }
 
    function setManureString(a){
     document.getElementById('hiddenManureList_'+a+'').value = getManure(a);
    }
    function setDeliveryString(a){
     document.getElementById('hiddenDeliveryList_'+a+'').value = getDelivery(a);
    }
    function setAmountString(a){
     document.getElementById('hiddenAmountList_'+a+'').value = getAmount(a);
    }
    function setMonthString(a){
     document.getElementById('hiddenMonthList_'+a+'').value = getMonths(a);
    }
 </script>
 <%
 end sub

 %>
 */
    }
}