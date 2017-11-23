using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;
using System.Web.Services.Description;
using System.Web.UI;
using System.Web.UI.HtmlControls;
using System.Web.UI.WebControls;

using System.ComponentModel;
using System.Drawing;

using System.Web.UI.Design;
using System.ComponentModel.Design;
using System.Reflection;
namespace FarmAC.Controls
{
    public partial class ctrlCropSequence : System.Web.UI.UserControl
    {

        CheckBox landless = new CheckBox();
        List<TextBox> allRotationName = new List<TextBox>();
        List<TextBox> allArea = new List<TextBox>();
        List<DropDownList> allSoilType = new List<DropDownList>();
        List<List<DropDownList>> allCrops = new List<List<DropDownList>>();
        List<List<DropDownList>> allMainFeedStuff = new List<List<DropDownList>>();
        List<List<DropDownList>> allStrawFeedStuff = new List<List<DropDownList>>();
        List<List<DropDownList>> allIrregation = new List<List<DropDownList>>();
        List<List<TextBox>> allStartTime = new List<List<TextBox>>();
        List<List<TextBox>> allEndTime = new List<List<TextBox>>();

        string language;
        protected void Page_Load(object sender, EventArgs e)
        {
            object st = HttpContext.Current.Session["UserInit"];
            if (HttpContext.Current.Session["UserInit"] != null)
            if ( HttpContext.Current.Session["UserInit"].ToString() == "3")
            {
                if (HttpContext.Current.Session["languageSelected"] != null)
                    language = HttpContext.Current.Session["languageSelected"].ToString();
                else
                    language = "uk";
                CreateTabField();
            }





        }
        int needUpdateID;
        string needupdateID;
        bool someSelected;
        string aIrrigationType;
        void CreateTabField()
        {

            DataTable arrFieldPlan;

            int f;
            int aCropID;

            DataTable arrRotation;

            int aRotationID = -1;



            DataTable arrFeedStuff;
            string aFeedStuffID;
            DataTable arrMainFeedStuff = null;
            string aMainFeedStuffID;

            DataTable arrStrawFeedStuff = null;
            string aStrawFeedStuffID;
            DataTable arrSoilType;

            string aSoilTypeID;
            string nIrrigationType;
            string aIrrigationTyp = null;
            string[][] arrIrrigationType = new string[2][];
            for (int i = 0; i < 2; i++)
            {
                arrIrrigationType[i] = new string[2];
            }

            DataTable arrCrop = new DataTable();
            DataTable arrAfterCrop;
            string aAfterCropID;
            bool aUseStraw;
            //  bool canHaveAfterCrop;
            string aStartDate, aEndDate, strDateCheck, strStartDateCheck, strEndDateCheck;
            string nYieldLevel;
            int aYieldLevel;
            double aTotalArea;
            double rotationSpan;

            string currentLocale;
            DataTable arrScenarioData;

            // currentLocale = getLocale();
            // setLocale("da");
            DefineValidateScript();

            dbInterface database = new dbInterface();
            database.InitFarmNDB();
            List<dbInterface.data> paramter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = HttpContext.Current.Session["farmNumber"].ToString();
            paramter.Add(data);
            data.name = "@aScenarioID";
            data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            paramter.Add(data);
            arrRotation = database.GetValuesFromDB("GetRotationList", paramter);

            string timeMeasureString1 = "After GetRotationList :" + DateTime.Now;


            paramter = new List<dbInterface.data>();
            arrSoilType = database.GetValuesFromDB("GetSelectListSoilType", paramter);


            string timeMeasureString2 = "After GetSelectListSoilType :" + DateTime.Now;



            try
            {
                paramter = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.name = "@AgroEcologicalZone";
                data.value = HttpContext.Current.Session["AgroEcologicalZone"].ToString();
                paramter.Add(data);
                arrCrop = database.GetValuesFromDB("GetNewSelectListCrop", paramter);
            }
            catch
            {

            }

            timeMeasureString1 = "After GetSelectListCrop :" + DateTime.Now;


            paramter = new List<dbInterface.data>();
            arrAfterCrop = database.GetValuesFromDB("GetSelectListAfterCrop", paramter);

            string timeMeasureString3 = "After GetSelectListAfterCrop :" + DateTime.Now;

            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@AgroEcologicalZone";
            data.name = "-1";
            arrFeedStuff = database.GetValuesFromDB("GetFeedStuffList", paramter);

            string timeMeasureString4 = "After GetFeedStuffList :" + DateTime.Now;


            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@FarmNumber";
            data.value = HttpContext.Current.Session["farmNumber"].ToString();
            paramter.Add(data);
            data = new dbInterface.data();
            data.name = "@ScenarioID";
            data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            paramter.Add(data);
            arrScenarioData = database.GetValuesFromDB("GetScenarioData", paramter);



            nIrrigationType = "2";
            arrIrrigationType[0][0] = "0";
            arrIrrigationType[1][0] = "strRAINFED";
            arrIrrigationType[0][1] = "1";
            arrIrrigationType[1][1] = "strIRRIGATED";
            Panel1.Controls.Add(new LiteralControl("<form name=" + "formFieldPlan" + " action=" + "+" + " method=" + "post" + ">" + "\r"));
            if (arrRotation.Rows.Count < 1)
            {

                landless.ID = "landless";
                landless.CheckedChanged += new EventHandler(landlessEvent);
                landless.AutoPostBack = true;
             
                Panel1.Controls.Add(landless);
                if (!Page.IsPostBack)
                {
                    string tmsp = arrScenarioData.Rows[0]["Landless"].ToString();
                    if (tmsp != "")
                        landless.Checked = bool.Parse(tmsp);
                    else
                        landless.Checked = true;
                }
                // Panel1.Controls.Add(new LiteralControl("  <input type="+"checkbox"+" name="+"landless"+" onchange="+"document.forms.formFieldPlan.action='LandlessUpdate.asp';document.forms.formFieldPlan.submit();"+" value="+"+"+arrScenarioData.Rows[0]["Landless"]+"+"+"+"));
                if (arrScenarioData.Rows[0]["Landless"].ToString() == "True")
                {

                    Panel1.Controls.Add(new LiteralControl(database.GetLanguageString("strLandless", language) + "\r"));
                    Panel1.Controls.Add(new LiteralControl("</form>" + "\r"));
                    CreateUpdateFormFieldPlan();
                    Panel1.Controls.Add(new LiteralControl("</body></html>" + "\r"));

                }
                else
                {
                    Panel1.Controls.Add(new LiteralControl(database.GetLanguageString("strLandless", language) + "\r"));
                }


            }
            if (!(arrScenarioData.Rows[0]["Landless"].ToString() == "True"))
            {
                Panel1.Controls.Add(new LiteralControl("  <table class=" + "data" + " border=" + "0" + " style=" + "width: 1400px;" + ">" + "\r"));



                for (int r = 0; r < (arrRotation.Rows.Count); r++)
                {
                    int needUpdateID = 0;

                    aRotationID = (int)arrRotation.Rows[r]["RotationID"];

                    aSoilTypeID = arrRotation.Rows[r]["SoilType"].ToString();

                    string test = arrRotation.Rows[r]["YieldLevel"].ToString();
                    aYieldLevel = int.Parse(arrRotation.Rows[r]["YieldLevel"].ToString());
                    Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));


                    Panel1.Controls.Add(new LiteralControl("      <td align=" + "left" + " colspan=" + "2" + "><h4>" + database.GetLanguageString("strRotationName", language) + ": </h4>" + "\r"));
                    TextBox RotationName = new TextBox();

                    RotationName.Text = arrRotation.Rows[r]["RotationName"].ToString();
                    Panel1.Controls.Add(RotationName);
                    while (allRotationName.Count() - 1 < aRotationID)
                    {
                        allRotationName.Add(RotationName);
                    }
                    allRotationName[aRotationID] = RotationName;

                    // Panel1.Controls.Add(new LiteralControl("<input type="+"text"+" name="+"RotationName_"+aRotationID+"+"+" value="+"+"+"lololol"+"+"+" maxlength="+"50"+" size="+"30"+" onChange="+"document.forms.formFieldPlan.action='FieldPlanUpdate.asp?RotationID="+aRotationID+"';document.forms.update.action='javascript:document.forms.formFieldPlan.submit();';this.style.color='#ff0000';"+"/>" +"\r"));
                    Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));


                    Panel1.Controls.Add(new LiteralControl("      <td align=" + "left" + " colspan=" + "2+" + "><h4>" + database.GetLanguageString("strSOILTYPE", language) + " </h4>" + "\r")); ;
                    //  Panel1.Controls.Add(new LiteralControl("<select style="+"width: 260px;"+" name="+"SoilType_"+aRotationID+"+"+" onChange="+"if ((this.options[this.selectedIndex].value=='1')||(this.options[this.selectedIndex].value=='2')||(this.options[this.selectedIndex].value=='3')||(this.options[this.selectedIndex].value=='4'))             {document.forms.formFieldPlan.Irrigation_"+aRotationID+".disabled=false;}            else             {document.forms.formFieldPlan.Irrigation_"+aRotationID+".options[0].selected=true;             document.forms.formFieldPlan.Irrigation_"+aRotationID+".disabled=true;};             document.forms.formFieldPlan.action='FieldPlanUpdate.asp?RotationID="+aRotationID+"&Irrigation_"+aRotationID+"=0';document.forms.update.action='javascript:document.forms.formFieldPlan.submit();';this.style.color='#ff0000';"+">" +"\r"));
                    DropDownList SoilType = new DropDownList();
                    for (int i = 0; i < arrSoilType.Rows.Count; i++)
                    {
                        string stuff=arrSoilType.Rows[i][2].ToString();
                        string stuff2=database.GetLanguageString("str"+arrSoilType.Rows[i][1].ToString(), language);
                        SoilType.Items.Add(stuff2 + "  " + arrSoilType.Rows[i][2]);
                        SoilType.Items[i].Attributes.Add("dbValue", arrSoilType.Rows[i][0].ToString());
                        if (aSoilTypeID == arrSoilType.Rows[i][0].ToString())
                            SoilType.SelectedIndex = i;


                    }
                    Panel1.Controls.Add(SoilType);

                    while (allSoilType.Count() - 1 < aRotationID)
                    {
                        allSoilType.Add(SoilType);
                    }
                    allSoilType[aRotationID] = SoilType;
                    //     tmp.CreateSelectList_SoilType(aSoilTypeID, arrSoilType.Rows.c, arrSoilType);
                    Panel1.Controls.Add(new LiteralControl("</select></td>" + "\r")); ;

                    Panel1.Controls.Add(new LiteralControl("      <td align=" + "left" + ">&nbsp;" + "\r"));
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
                    data.name = "@aRotationID";
                    data.value = aRotationID.ToString();
                    paramter.Add(data);
                    arrFieldPlan = database.GetValuesFromDB("GetFieldPlanList", paramter);

                    aTotalArea = (double)(arrRotation.Rows[r]["Area"]);
                    rotationSpan = 1.0;

                    aTotalArea = aTotalArea / rotationSpan;

                    Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                    //   Panel1.Controls.Add(new LiteralControl("      <td align="+"left"+"><h4>"+"strAREA"+": </h4><input type="+"text"+" id="+"inputArea_"+aRotationID+"+"+" name="+"inputArea_"+aRotationID+"+"+" value="+"+"+aTotalArea+"+"+" maxlength="+"5"+" size="+"2"+" onChange="+"document.forms.formFieldPlan.action='FieldPlanUpdate.asp?RotationID="+aRotationID+"';if (isFinite(this.value.replace(',','.'))) {this.value=this.value.replace('.',',');document.forms.update.action='javascript:document.forms.formFieldPlan.submit();';this.style.color='#ff0000';}"+"/>Ha</td>" +"\r"));



                    Panel1.Controls.Add(new LiteralControl("      <td align=" + "left" + "><h4>" + database.GetLanguageString("strAREA", language) + "(HA): </h4>"));
                    TextBox inputArea = new TextBox();
                    inputArea.Text = aTotalArea.ToString();
                    inputArea.ID = "inputArea" + RotationName.Text;
                    inputArea.TextChanged += new EventHandler(SaveRotationText);
                    inputArea.AutoPostBack = true;
                    inputArea.Attributes.Add("aRotationID", aRotationID.ToString());
                    while (allArea.Count() <= aRotationID)
                    {
                        allArea.Add(inputArea);
                    }
                    allArea[aRotationID] = inputArea;
                    Panel1.Controls.Add(inputArea);
                    //Panel1.Controls.Add(new LiteralControl("<input type = "+"hidden"+" name="+"YieldLevel_"+aRotationID+"+"+" value="+"+"+arrRotation.Rows[0]["YieldLevel"]+"+"+" />"));

                    Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r")); ;

                    Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));



                    Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + database.GetLanguageString("strCROP", language) + "</th>" + "\r"));

                    Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + database.GetLanguageString("str1product", language) + "</th>" + "\r"));

                    Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + database.GetLanguageString("str2product", language) + "</th>" + "\r"));




                    Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">Start date&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;End date</th>" + "\r"));//'"&Str.Item("strAfterCrop")&"</th>" +"\r");
                    Panel1.Controls.Add(new LiteralControl("      <td align=" + "left" + "><h4>" + database.GetLanguageString("strIRRIGATION", language) + " </h4>" + "\r")); ;
                    Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">&nbsp;</th>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r")); ;
                    List<DropDownList> arrFieldPlanCrops = new List<DropDownList>();
                    List<DropDownList> arrFieldPlanStrawFeedStuf = new List<DropDownList>();
                    List<DropDownList> arrFieldPlanMainFeedStuff = new List<DropDownList>();
                    List<TextBox> StartTimeList = new List<TextBox>();
                    List<DropDownList> IrregationList = new List<DropDownList>();
                    List<TextBox> EndTimeList = new List<TextBox>();

                    for (f = 0; f <= (arrFieldPlan.Rows.Count - 1); f++)
                    {

                        aCropID = (int)(arrFieldPlan.Rows[f]["Crop"]);
                        aAfterCropID = arrFieldPlan.Rows[f]["AfterCropID"].ToString();


                        //		 canHaveAfterCrop = (bool)(arrFieldPlan.Rows[f]["Can_Have_AfterCrop"]);

                        aFeedStuffID = arrFieldPlan.Rows[f]["FeedStuffID"].ToString();

                        aStrawFeedStuffID = arrFieldPlan.Rows[f]["StrawFeedStuffID"].ToString();
                        string tmp2 = arrFieldPlan.Rows[f]["UseStraw"].ToString();
                        if (tmp2.CompareTo("1") == 0)
                            aUseStraw = true;
                        else
                            aUseStraw = false;


                        aStartDate = arrFieldPlan.Rows[f]["StartDate"].ToString();

                        if (aStartDate == "")
                        {
                            aStartDate = "";
                        }
                        aStartDate = aStartDate.Split(' ')[0];
#if(serverTime)
                        int month = int.Parse(aStartDate.Split('/')[0]);
                        int day = int.Parse(aStartDate.Split('/')[1]);
                        int year = int.Parse(aStartDate.Split('/')[2]);
                       
#else
                        int month = int.Parse(aStartDate.Split('/')[1]);
                        int day = int.Parse(aStartDate.Split('/')[0]);
                        int year = int.Parse(aStartDate.Split('/')[2]);

#endif
                        if (day < 10)
                            aStartDate = "0"+day.ToString();
                        else
                            aStartDate =  day.ToString();
                        if (month < 10)
                            aStartDate += "/0" + month.ToString() + '/' + year.ToString();
                        else
                            aStartDate += "/" + month.ToString() + '/' + year.ToString();
                        aEndDate = arrFieldPlan.Rows[f]["EndDate"].ToString();
                        bool newData=false;
                        if (aEndDate == "")
                        {
                            aEndDate = aStartDate;
                            newData = true;
                        }
                        aEndDate = aEndDate.Split(' ')[0];
#if(serverTime)
                        if(newData==false)
                        {
                        month = int.Parse(aEndDate.Split('/')[0]);
                        day = int.Parse(aEndDate.Split('/')[1]);
                        year = int.Parse(aEndDate.Split('/')[2]);
                        }
#else
                        month = int.Parse(aEndDate.Split('/')[1]);
                        day = int.Parse(aEndDate.Split('/')[0]);
                        year = int.Parse(aEndDate.Split('/')[2]);

#endif


                        if (day < 10)
                            aEndDate = "0" + day.ToString();
                        else
                            aEndDate =  day.ToString();
                        if (month < 10)
                            aEndDate += "/0" + month.ToString() + '/' + year.ToString();
                        else
                            aEndDate += "/" + month.ToString() + '/' + year.ToString();
                        paramter = new List<dbInterface.data>();
                        data = new dbInterface.data();
                        data.name = "@aCropID";
                        data.value = aCropID.ToString();
                        paramter.Add(data);
                        data = new dbInterface.data();
                        data.name = "@AgroEcologicalZone";
                        data.value = HttpContext.Current.Session["AgroEcologicalZone"].ToString();
                        paramter.Add(data);
                        arrMainFeedStuff = database.GetValuesFromDB("GetCropMainFeedstuffList", paramter);

                        arrStrawFeedStuff = database.GetValuesFromDB("GetCropStrawFeedstuffList", paramter);

                        aIrrigationType = arrFieldPlan.Rows[f]["Irrigation"].ToString();
                        if (aIrrigationType == "True")
                            aIrrigationType = "1";
                        else
                            aIrrigationType = "0";

                        Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));

                        Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + ">")); ;


                        //   Panel1.Controls.Add(new LiteralControl("<input type="+"hidden+"+" name="+"hiddenCropID_"+aRotationID+"_"+f+"+"+" value="+"+"+arrFieldPlan.Rows[f]["Crop"]+"+"+">"));
                        //   Panel1.Controls.Add(new LiteralControl("<select name="+"selectCrop_"+aRotationID+"_"+f+"+"+" onChange="+"veil();document.forms.formFieldPlan.action='FieldPlanUpdate.asp?RotationID="+aRotationID+"';document.forms.update.action='javascript:document.forms.formFieldPlan.submit();';this.style.color='#ff0000';submit();"+">"));


                        //CreateSelectList_ aCropID,nCrop,arrCrop
                        Panel1.Controls.Add(new LiteralControl("</select>"));
                        DropDownList CropList = new DropDownList();
                        CropList.ID = "croplist" + RotationName.Text + " " + f;
                        CropList.SelectedIndexChanged += new EventHandler(SaveRotationEventDropDown);
                        CropList.AutoPostBack = true;
                        int indexNumber = 0;
                        for (int i = 0; i < arrCrop.Rows.Count; i++)
                        {
                     //       string cropname=database.GetLanguageString("str" + arrCrop.Rows[i][1].ToString(), language);
                            CropList.Items.Add(arrCrop.Rows[i][0].ToString() + " " + arrCrop.Rows[i][1].ToString());
                            CropList.Items[i].Attributes.Add("dbValue", arrCrop.Rows[i][0].ToString());

                            CropList.Attributes.Add("aRotationID", aRotationID.ToString());
                            CropList.Attributes.Add("arrFieldPlan", arrFieldPlan.Rows.Count.ToString());

                            if (aCropID.ToString() == arrCrop.Rows[i][0].ToString())
                            {
                                indexNumber = i;
                            }
                        }
                        CropList.SelectedIndex = indexNumber;
                        Panel1.Controls.Add(CropList);
                        while (arrFieldPlanCrops.Count() < arrFieldPlan.Rows.Count)
                        {
                            arrFieldPlanCrops.Add(CropList);
                        }
                        arrFieldPlanCrops[f] = CropList;
                        Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + ">"));
                        if (aCropID != 31)
                        {
                            //     Panel1.Controls.Add(new LiteralControl("<select style="+"width: 270px;"+" name="+"selectCropFeedStuff_"+aRotationID+"_"+f+"+"+" onChange="+"document.forms.formFieldPlan.action='FieldPlanUpdate.asp?RotationID="+aRotationID+"';document.forms.update.action='javascript:document.forms.formFieldPlan.submit();';this.style.color='#ff0000';"+">"));  

                            //CreateSelectList_ aFeedStuffID,nMainFeedStuff,arrMainFeedStuff
                            //  Panel1.Controls.Add(new LiteralControl("</select>"));
                            DropDownList MainFeedStuff = new DropDownList();
                            MainFeedStuff.ID = "MainFeedStuff" + RotationName.Text + f.ToString();
                            MainFeedStuff.SelectedIndexChanged += new EventHandler(SaveRotationEventDropDown);
                            MainFeedStuff.AutoPostBack = true;
                            indexNumber = 1;
                            for (int i = 0; i < arrMainFeedStuff.Rows.Count; i++)
                            {

                                MainFeedStuff.Items.Add(arrMainFeedStuff.Rows[i][0] + "  " + arrMainFeedStuff.Rows[i][1]);
                                MainFeedStuff.Items[i].Attributes.Add("dbValue", arrMainFeedStuff.Rows[i][0].ToString());

                                MainFeedStuff.Attributes.Add("aRotationID", aRotationID.ToString());
                                MainFeedStuff.Attributes.Add("arrFieldPlan", arrFieldPlan.Rows.Count.ToString());

                                if (aFeedStuffID.ToString() == arrMainFeedStuff.Rows[i][0].ToString())
                                {
                                    indexNumber = i;
                                }
                            }
                            MainFeedStuff.SelectedIndex = indexNumber;
                            string studddff = MainFeedStuff.SelectedItem.Attributes["dbValue"];
                            Panel1.Controls.Add(MainFeedStuff);
                            while (arrFieldPlanMainFeedStuff.Count() < arrFieldPlan.Rows.Count)
                            {
                                arrFieldPlanMainFeedStuff.Add(SoilType);
                            }
                            arrFieldPlanMainFeedStuff[f] = MainFeedStuff;

                            if (someSelected == false)
                            {
                                needUpdateID = aRotationID;
                            }
                        }
                        else
                        {
                            DropDownList MainFeedStuff = new DropDownList();
                            MainFeedStuff.ID = "MainFeedStuff" + RotationName.Text + f.ToString();

                            MainFeedStuff.Items.Add("0 None");
                            MainFeedStuff.Items[0].Attributes.Add("dbValue", "0");

                            while (arrFieldPlanMainFeedStuff.Count() < arrFieldPlan.Rows.Count)
                            {
                                arrFieldPlanMainFeedStuff.Add(SoilType);
                            }
                            arrFieldPlanMainFeedStuff[f] = MainFeedStuff;
                        }
                        Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + ">"));
                        if (aUseStraw == true)
                        {
                            // Panel1.Controls.Add(new LiteralControl("<select style="+"width: 270px;"+" name="+"selectStrawFeedStuff_"+aRotationID+"_"+f+"+"+" onChange="+"document.forms.formFieldPlan.action='FieldPlanUpdate.asp?RotationID="+aRotationID+"';document.forms.update.action='javascript:document.forms.formFieldPlan.submit();';this.style.color='#ff0000';"+">"));
                            //  CreateSelectList_ aStrawFeedStuffID,nStrawFeedStuff,arrStrawFeedStuff
                            // Panel1.Controls.Add(new LiteralControl("</select>")); ;
                            DropDownList StrawFeedStuff = new DropDownList();
                            StrawFeedStuff.ID = "StrawFeedStuff" + RotationName.Text + f.ToString();
                            StrawFeedStuff.SelectedIndexChanged += new EventHandler(SaveRotationEventDropDown);
                            StrawFeedStuff.AutoPostBack = true;
                            indexNumber = 1;
                            for (int i = 0; i < arrStrawFeedStuff.Rows.Count; i++)
                            {

                                StrawFeedStuff.Items.Add(arrStrawFeedStuff.Rows[i][0] + "  " + arrStrawFeedStuff.Rows[i][1]);
                                StrawFeedStuff.Items[i].Attributes.Add("dbValue", arrStrawFeedStuff.Rows[i][0].ToString());

                                StrawFeedStuff.Attributes.Add("aRotationID", aRotationID.ToString());
                                StrawFeedStuff.Attributes.Add("arrFieldPlan", arrFieldPlan.Rows.Count.ToString());

                                if (aStrawFeedStuffID.ToString() == arrStrawFeedStuff.Rows[i][0].ToString())
                                {
                                    indexNumber = i;
                                }
                            }
                            StrawFeedStuff.SelectedIndex = indexNumber;
                            Panel1.Controls.Add(StrawFeedStuff);
                            while (arrFieldPlanStrawFeedStuf.Count() < arrFieldPlan.Rows.Count)
                            {
                                arrFieldPlanStrawFeedStuf.Add(SoilType);
                            }
                            arrFieldPlanStrawFeedStuf[f] = StrawFeedStuff;
                            string stuff = StrawFeedStuff.SelectedItem.Attributes["dbValue"];
                        }
                        else
                        {
                            DropDownList StrawFeedStuff = new DropDownList();
                            StrawFeedStuff.ID = "StrawFeedStuff" + RotationName.Text + f.ToString();
                            StrawFeedStuff.Items.Add("0 None");
                            StrawFeedStuff.Items[0].Attributes.Add("dbValue", "0");
                            while (arrFieldPlanStrawFeedStuf.Count() < arrFieldPlan.Rows.Count)
                            {
                                arrFieldPlanStrawFeedStuf.Add(SoilType);
                            }
                            arrFieldPlanStrawFeedStuf[f] = StrawFeedStuff;
                        }
                        Panel1.Controls.Add(new LiteralControl("</td>" + "\r")); ;

                        //   Panel1.Controls.Add(new LiteralControl("      <input type="+"hidden"+" name="+"inputArea_"+aRotationID+"_"+f+"+"+" value="+"+"+arrFieldPlan.Rows[f]["Area"]+"+"+" />" +"\r"));
                        Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + ">"));



                        TextBox StartTime = new TextBox();
                        StartTime.Width = 80;
                        StartTime.ID = "StartTimeBox" + aRotationID.ToString() + "_" + f.ToString();
          

                        StartTime.Text = aStartDate;
                        if(f==0)
                            StartTime.ReadOnly = false;
                        else
                            StartTime.ReadOnly = true;
                        StartTime.Attributes.Add("aRotationID", aRotationID.ToString());
                        StartTime.Attributes.Add("plant", f.ToString());

                        Panel1.Controls.Add(StartTime);
                        StartTimeList.Add(StartTime);

                        TextBox EndTime = new TextBox();

                        EndTime.Text = aEndDate;
                        EndTime.Width = 80;
                        EndTime.ID = "endtime" + aRotationID.ToString() + "_" + f.ToString();
                        EndTime.Attributes.Add("aRotationID", aRotationID.ToString());
                        EndTime.Attributes.Add("plant", f.ToString());
                        EndTime.TextChanged += new EventHandler(SelectionEnd);
                        EndTime.AutoPostBack = true;
                        EndTimeList.Add(EndTime);
                        Panel1.Controls.Add(EndTime);


               

                        if (f > 0)
                            strStartDateCheck = "if (document.getElementById('endDate_" + aRotationID + "_" + (f - 1) + "').value !='') {if (ValidateDate(this,'Start date',false)) {var prevarr = (document.getElementById('endDate_" + aRotationID + "_" + (f - 1) + "+";//').value).split('-');var thisarr=(this.value).split('-'); if (thisarr.length < 2) {thisarr=(this.value).split('/')};if (parseInt(Date.UTC(thisarr[2],thisarr[1],thisarr[0]))-parseInt(Date.UTC(prevarr[2],prevarr[1],prevarr[0]))>=0) {this.style.color='#ff0000'} else {alert('Date must be on or after '+prevarr[0]+'-'+prevarr[1]+'-'+prevarr[2]);this.value='';}} else {this.value='';}} else {this.value='';alert('Enter start- and stop-dates from the top')};"
                        else
                            strStartDateCheck = "if (ValidateDate(this,'Start date',false))  {this.style.color='#ff0000';document.getElementBy} else {this.value='';};";


                        strEndDateCheck = "if (document.getElementById('startDate_" + aRotationID + "_" + f + "').value !='') {if (ValidateDate(this,'End date',false)) {var prevarr = (document.getElementById('startDate_" + aRotationID + "_" + f + "+";//').value).split('-');var thisarr=(this.value).split('-'); if (thisarr.length < 2) {thisarr=(this.value).split('/')};if (parseInt(Date.UTC(thisarr[2],thisarr[1],thisarr[0]))-parseInt(Date.UTC(prevarr[2],prevarr[1],prevarr[0]))>=0) {this.style.color='#ff0000'} else {alert('Date must be on or after '+prevarr[0]+'-'+prevarr[1]+'-'+prevarr[2]);this.value='';}} else {this.value='';}} else {this.value='';alert('Enter start- and stop-dates from the top')};"

                        // Panel1.Controls.Add(new LiteralControl("<input type="+"text"+" size="+"9"+" name="+"startDate_"+aRotationID+"_"+f+"+"+" id="+"startDate_"+aRotationID+"_"+f+"+"+" value="+"+"+aStartDate+"+"+" onchange="+"document.forms.formFieldPlan.action='FieldPlanUpdate.asp?RotationID="+aRotationID+"';"+strStartDateCheck+"+"+">&nbsp;"));
                        // Panel1.Controls.Add(new LiteralControl("<input type="+"text"+" size="+"9"+" name="+"endDate_"+aRotationID+"_"+f+"+"+" id="+"endDate_"+aRotationID+"_"+f+"+"+" value="+"+"+aEndDate+"+"+" onchange="+"document.forms.formFieldPlan.action='FieldPlanUpdate.asp?RotationID="+aRotationID+"';"+strEndDateCheck+"+"+">"));
                        Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "selectAfterCrop_" + aRotationID + "_" + f + "+" + " value=" + "0" + ">"));
                        Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " id=" + "changes_" + aRotationID + "+" + " value=" + "+" + ">"));
                        Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                        //  column3 = AfterCrop;

                        Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + ">"));
                        if (aCropID != 31)
                        {
                            DropDownList aIrrigationDropDown = new DropDownList();
                            aIrrigationDropDown.ID = "aIrrigationDropDown" + RotationName.Text + " " + f.ToString();
                            string str = aIrrigationDropDown.ID;
                            aIrrigationDropDown.SelectedIndexChanged += new EventHandler(SaveRotationEventDropDown);
                            aIrrigationDropDown.AutoPostBack = true;
                            indexNumber = 1;
                            for (int i = 0; i < 2; i++)
                            {
                                aIrrigationDropDown.Items.Add(arrIrrigationType[0][i] + "  " + database.GetLanguageString(arrIrrigationType[1][i], language));
                                aIrrigationDropDown.Attributes.Add("aRotationID", aRotationID.ToString());
                                aIrrigationDropDown.Attributes.Add("arrFieldPlan", f.ToString());
                                if (aIrrigationType.ToString() == arrIrrigationType[0][i])
                                {
                                    indexNumber = i;
                                }
                            }
                            aIrrigationDropDown.SelectedIndex = indexNumber;
                            IrregationList.Add(aIrrigationDropDown);
                            Panel1.Controls.Add(aIrrigationDropDown);
                            //      Panel1.Controls.Add(new LiteralControl("<select name=" + "Irrigation_" + aRotationID + "_" + f + "+" + " onChange=" + "document.forms.formFieldPlan.action='FieldPlanUpdate.asp?RotationID=" + aRotationID + "';document.forms.update.action='javascript:document.forms.formFieldPlan.submit();';this.style.color='#ff0000';" + ">" + "\r"));
                            //   CreateSelectList aIrrigationType,nIrrigationType,arrIrrigationType
                            Panel1.Controls.Add(new LiteralControl("</select>" + "\r"));
                        }
                        else
                        {
                            DropDownList aIrrigationDropDown = new DropDownList();
                            aIrrigationDropDown.ID = "aIrrigationDropDown" + RotationName.Text + " " + f.ToString();
                            aIrrigationDropDown.Items.Add("None");
                            IrregationList.Add(aIrrigationDropDown);

                        }
                        Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));

                        Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " align=" + "left" + ">"));


                        Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                        Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));

                    }
                    List<DropDownList> tmp = new List<DropDownList>();
                    while (allCrops.Count() - 1 < aRotationID)
                    {

                        allCrops.Add(tmp);
                    }
                    allCrops[aRotationID] = arrFieldPlanCrops;

                    while (allMainFeedStuff.Count() - 1 < aRotationID)
                    {

                        allMainFeedStuff.Add(tmp);
                    }
                    allMainFeedStuff[aRotationID] = arrFieldPlanMainFeedStuff;

                    while (allStrawFeedStuff.Count() - 1 < aRotationID)
                    {

                        allStrawFeedStuff.Add(tmp);
                    }
                    allStrawFeedStuff[aRotationID] = arrFieldPlanStrawFeedStuf;


                    while (allIrregation.Count() - 1 < aRotationID)
                    {

                        allIrregation.Add(tmp);
                    }
                    allIrregation[aRotationID] = IrregationList;


                    List<TextBox> tmps = new List<TextBox>();
                    while (allStartTime.Count() - 1 < aRotationID)
                    {

                        allStartTime.Add(tmps);
                    }
                    allStartTime[aRotationID] = StartTimeList;
                    tmps = new List<TextBox>();
                    while (allEndTime.Count() - 1 < aRotationID)
                    {

                        allEndTime.Add(tmps);
                    }
                    allEndTime[aRotationID] = EndTimeList;
                   
                    Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("      <td class=" + "data" + " colspan=" + "3" + ">"));
                    //     Panel1.Controls.Add(new LiteralControl("<button style="+"width: 125px; height: 20px; font-size: 10px"+" onClick="+"javascript:if (checkStartEndDate("+aRotationID+","+f+")) {return false} else {veil();document.forms.formFieldPlan.action='FieldPlanUpdate.asp?RotationID="+aRotationID+"';document.forms.formFieldPlan.submit();};"+">"+"strSAVEROT"+"</button>"));
                    HtmlButton SaveRotation = new HtmlButton();

                    // Set the properties of the new HtmlButton control.
                    SaveRotation.ID = "SaveRotation" + aRotationID.ToString() + " " + arrFieldPlan.ToString(); ;
                    SaveRotation.InnerHtml = "Save Sequence";
                    SaveRotation.Attributes.Add("aRotationID", aRotationID.ToString());
                    SaveRotation.Attributes.Add("arrFieldPlan", arrFieldPlan.Rows.Count.ToString());
                    if (f == 0)
                    {
                        SaveRotation.Attributes.Add("Area", "");
                        SaveRotation.Attributes.Add("aFieldPlanID", "");
                    }
                    else
                    {
                        SaveRotation.Attributes.Add("Area", arrFieldPlan.Rows[f - 1]["Area"].ToString());
                        SaveRotation.Attributes.Add("aFieldPlanID", arrFieldPlan.Rows[f - 1]["FieldPlanID"].ToString());
                    }
                    // Create an EventHandler delegate for the method you want to handle the event
                    // and then add it to the list of methods called when the event is raised.
                    SaveRotation.ServerClick += new System.EventHandler(SaveRotationEvent);

                    // Add the new HtmlButton control to the Controls collection of the
                    // PlaceHolder control. 
                    Panel1.Controls.Add(SaveRotation);


                    Panel1.Controls.Add(new LiteralControl("&nbsp;"));
                    //  Panel1.Controls.Add(new LiteralControl("<button style="+"width: 125px; height: 20px; font-size: 10px"+" onClick="+"javascript:if (checkStartEndDate("+aRotationID+","+f+")) {return false} else {veil();document.forms.formFieldPlan.action='FieldPlanNew.asp?RotationID="+aRotationID+"';document.forms.formFieldPlan.submit();}"+">"+"strNEWCROP"+"</button>"));
                    HtmlButton NewCrop = new HtmlButton();

                    // Set the properties of the new HtmlButton control.

                    NewCrop.InnerHtml = "New Crop";
                    NewCrop.ID = "New Crop" + aRotationID.ToString();

                    NewCrop.Attributes.Add("aRotationID", aRotationID.ToString());

                    // Create an EventHandler delegate for the method you want to handle the event
                    // and then add it to the list of methods called when the event is raised.
                    NewCrop.ServerClick += new System.EventHandler(NewCropEvent);

                    // Add the new HtmlButton control to the Controls collection of the
                    // PlaceHolder control. 
                    Panel1.Controls.Add(NewCrop);

                    Panel1.Controls.Add(new LiteralControl("&nbsp;"));

                    // Create a new HtmlButton control.
                    Button Delete;
               //     object tmpButten = HttpContext.Current.Session["DeleteSequence" + aRotationID.ToString()];
       
                 
                        Delete = new Button();

                        // Set the properties of the new HtmlButton control.
                        Delete.ID = aRotationID.ToString();
                        Delete.Text = "Delete Sequence";

                        // Create an EventHandler delegate for the method you want to handle the event
                        // and then add it to the list of methods called when the event is raised.
                        //Delete.Attributes.Add("onclick", "if(confirm('Are you sure you want to delete Crop Sequence?')) {alert('Sure?')};");

                     //   Delete.Attributes.Add("deleteInfo","false");
                       // Delete.Attributes.Add("onclick", "if(confirm('Are you sure you want to delete Crop Sequence?')) {this.deleteInfo='true';alert(this.deleteInfo)}else{this.deleteInfo='true';};Submit();");
                     

                        
                        
                        
                        // Add the new HtmlButton control to the Controls collection of the
                        // PlaceHolder control. 
                        string delInfoClient = "false";
                        try
                        {
                            delInfoClient = HttpContext.Current.Request["controlCropSequence$delInfo" + aRotationID.ToString() + ""].ToString();
                            
                        }
                        catch(Exception e)
                        {
                            string fejl = e.Message;
                        }


                        Delete.Attributes.Add("deleteInfo", delInfoClient);
                    string deleteInfosss = Delete.Attributes["deleteInfo"].ToString();
                    HiddenField DelInfo = new HiddenField();
                    DelInfo.ID = "delInfo" + aRotationID.ToString();
                    DelInfo.Value = "-1";
                    Panel1.Controls.Add(DelInfo);
                    Delete.Attributes.Add("onclick", "if(confirm('Are you sure you want to delete Crop Sequence?')) {document.getElementById('controlCropSequence_delInfo" + aRotationID.ToString() + "').value='true';alert(document.getElementById('controlCropSequence_delInfo" + aRotationID.ToString() + "').value)}else{document.getElementById('controlCropSequence_delInfo" + aRotationID.ToString() + "').value='false';};Submit();");


                  //  Delete.Attributes.Add("onclick", "if(confirm('Are you sure you want to delete Crop Sequence?')) {'<%Session[\"temp\"] = \"sds\"; %> '; alert('<%=Session[\"temp\"] %>)'}else{dfgdfg(\"f\")}");
                    HttpContext.Current.Session["DeleteSequence" + aRotationID.ToString()] = Delete;
                    Delete.Click += new System.EventHandler(this.Delete);
                    Panel1.Controls.Add(Delete);
         
                    //All related data will be deleted permanently.')) {alert('Sure?')};</script>"));

                

                    Panel1.Controls.Add(new LiteralControl("</td><td></td><td></td><td>" + "\r"));
                    if (f > 1)
                    {
                        HtmlButton DeleteCrop = new HtmlButton();

                        // Set the properties of the new HtmlButton control.
                        DeleteCrop.InnerHtml = "Delete Last Crop";
                        DeleteCrop.ID = "DeleteCrops " + RotationName.Text + f; ;
                        // Create an EventHandler delegate for the method you want to handle the event
                        // and then add it to the list of methods called when the event is raised.
                        DeleteCrop.ServerClick += new System.EventHandler(DeleteCropEvent);

                        DeleteCrop.Attributes.Add("aRotationID", aRotationID.ToString());
                        DeleteCrop.Attributes.Add("FieldPlanID", arrFieldPlan.Rows[f - 1]["FieldPlanID"].ToString());

                        // Add the new HtmlButton control to the Controls collection of the
                        // PlaceHolder control. 
                        Panel1.Controls.Add(DeleteCrop);
                        //   Panel1.Controls.Add(new LiteralControl("&nbsp;&nbsp;<button style=" + "width: 150px; height: 20px; font-size: 10px" + " onClick=" + "javascript:if(confirm('" + "strBekræftafgrøde" + "')) {veil();document.forms.formFieldPlan.action='FieldPlanDelete.asp?RotationID=" + aRotationID + "&FieldPlanID=" + arrFieldPlan.Rows[f-1]["FieldPlanID"] + "';document.forms.formFieldPlan.submit();}" + ">" + ("strDELETECROP") + "</button>"));
                    }
                    Panel1.Controls.Add(new LiteralControl("    </td></tr>" + "\r"));

                    Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("      <td colspan=" + "6" + "><hr/></td>" + "\r"));
                    Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));
                    aTotalArea = 0;

                }

                Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
                Panel1.Controls.Add(new LiteralControl("      <td colspan=" + "8" + ">"));
                // Create a new HtmlButton control.
                HtmlButton NewButtonControl1 = new HtmlButton();

                // Set the properties of the new HtmlButton control.
                NewButtonControl1.ID = "New Sequence";
                NewButtonControl1.InnerHtml = "New Sequence";

                // Create an EventHandler delegate for the method you want to handle the event
                // and then add it to the list of methods called when the event is raised.
                NewButtonControl1.ServerClick += new System.EventHandler(New);

                // Add the new HtmlButton control to the Controls collection of the
                // PlaceHolder control. 
                Panel1.Controls.Add(NewButtonControl1);

                Panel1.Controls.Add(new LiteralControl("</td>" + "\r"));
                Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));
                database.CloseFarmNDB();

                Panel1.Controls.Add(new LiteralControl("  </table>" + "\r"));
                Panel1.Controls.Add(new LiteralControl("</form>" + "\r"));
                CreateUpdateFormFieldPlan();



            }
        }
        void CreateUpdateFormFieldPlan()
        {
            Panel1.Controls.Add(new LiteralControl("<form name=" + "update" + ">" + "\r"));
            Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "saveUpdates" + " value=" + "false" + ">" + "\r"));
            Panel1.Controls.Add(new LiteralControl("</form>" + "\r"));
            if ((someSelected = false) && (needUpdateID != 0))
            {
                //Document.forms.formFieldPlan.action="FieldPlanUpdate.asp?RotationID=<%=needUpdateID%>";
                //document.forms.formFieldPlan.submit();
            }
        }

        void DefineValidateScript()
        {
            //%><script language="javascript" type="text/javascript">
        }
        void checkStartEndDate(int r, int f)
        {
            var ret = false;
            for (var i = 0; i < f; i++)
            {
                /*
              if (document.getElementById("startDate_"+r+'_'+i).value=="+"||document.getElementById("endDate_"+r+"_"+i).value=="+"||document.getElementById("inputArea_"+r).value=="0")
              {
              ret = true;

              }
              else
              {
              ret = false;

              }*/
            }
            /*
        if (ret == true)
            if (webBrowser1.Document.getElementById("inputArea_" + r).value != '0')
          {
     
              Page.ClientScript.RegisterStartupScript(this.GetType(), "ErrorAlert", "alert('fill out all start- and end-dates');", true);
                    }
         else
          {
       
                Page.ClientScript.RegisterStartupScript(this.GetType(), "ErrorAlert", "alert('specify area larger than zero');", true);
            }*/

        }

        void CreateSelectList_(string aType, int nItem, string[][] arrItem)
        {

            someSelected = false;


            for (int i = 0; i < (nItem - 1); i++)
            {
                if (arrItem[i][0] == aType)
                {
                    Panel1.Controls.Add(new LiteralControl("<option value=" + "+" + arrItem[i][0] + "+" + " selected>" + arrItem[i][1] + "</option>"));
                    someSelected = true;
                }
                else
                    Panel1.Controls.Add(new LiteralControl("<option value=" + "+" + arrItem[i][0] + "+" + ">" + arrItem[i][1] + "</option>"));
            }

        }
        void Delete(Object sender, EventArgs e)
        {

           // Panel1.Controls.Add(new LiteralControl("<script>if(confirm('Are you sure you want to delete Crop Sequence?"));

            //Panel1.Controls.Add(new LiteralControl("All related data will be deleted permanently.')) {alert('Sure?')};</script>"));



                Button button = (Button)sender;
                string tmp4 = button.Text;
                string rotationID = button.ID.ToString();
                string info = button.Attributes["deleteInfo"].ToString();
                if (info == "true")
                {
                    RotationDelete tmp = new RotationDelete();
                    tmp.ProcessRequest(HttpContext.Current, rotationID);

                    Panel1.Controls.Clear();
                    CreateTabField();
                }
                HttpContext.Current.Session["ShowResult"] = false;
        }
        void save(int RotationID)
        {

            string rotation = allRotationName[RotationID].Text;
            int selected = allSoilType[RotationID].SelectedIndex;
            string soiltype = allSoilType[RotationID].Items[selected].Attributes["dbValue"];
            List<TextBox> Listbox = allStartTime[RotationID];
            List<string> aStartDate = new List<string>();
            for (int i = 0; i < Listbox.Count; i++)
            {
                aStartDate.Add(Listbox[i].Text);
            }
            List<string> aEndDate = new List<string>();
            Listbox = allEndTime[RotationID];

            for (int i = 0; i < Listbox.Count; i++)
            {
                aEndDate.Add(Listbox[i].Text);
            }
            List<string> aCrop = new List<string>();
            List<DropDownList> crops = allCrops[RotationID];
            for (int i = 0; i < allCrops[RotationID].Count(); i++)
            {
                int indexs = crops[i].SelectedIndex;
                string name = crops[i].SelectedItem.Attributes["dbValue"];
                aCrop.Add(name);

            }
            FieldPlanUpdate tmp = new FieldPlanUpdate();
            List<string> aFeedstuffList = new List<string>();
            List<DropDownList> MainFeedstuff = allMainFeedStuff[RotationID];
            for (int i = 0; i < allMainFeedStuff[RotationID].Count(); i++)
            {

                string name = MainFeedstuff[i].SelectedItem.Attributes["dbValue"];
                aFeedstuffList.Add(name);

            }


            List<string> aStrawFeedStuffList = new List<string>();

            List<DropDownList> StrawFeedstuff = allStrawFeedStuff[RotationID];
            for (int i = 0; i < allStrawFeedStuff[RotationID].Count(); i++)
            {
                string name = StrawFeedstuff[i].SelectedItem.Attributes["dbValue"];
                aStrawFeedStuffList.Add(name);

            }
            List<DropDownList> Irregation = allIrregation[RotationID];
            List<string> aIrregationList = new List<string>();
            for (int i = 0; i < Irregation.Count(); i++)
            {
                string name = Irregation[i].SelectedIndex.ToString();
                aIrregationList.Add(name);

            }

            string area = allArea[RotationID].Text;
            tmp.ProcessRequest(HttpContext.Current, RotationID.ToString(), area, rotation, soiltype, aStartDate, aEndDate, aCrop, aFeedstuffList, aStrawFeedStuffList, aIrregationList);
            ViewState["UserInit"] = "3";
            HttpContext.Current.Session["UserInit"] = "3";
            HttpContext.Current.Session["ShowResult"] = false;
            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);
        }
        void SaveRotationEventDropDown(Object sender, EventArgs e)
        {

            DropDownList button = (DropDownList)sender;
            int RotationID = int.Parse(button.Attributes["aRotationID"]);
            save(RotationID);
        }
        void SaveRotationEvent(Object sender, EventArgs e)
        {
            HtmlButton button = (HtmlButton)sender;
            int RotationID = int.Parse(button.Attributes["aRotationID"]);

            string time = allStartTime[RotationID][0].Text;
            int month = int.Parse(time.Split('/')[1]);
            int day = int.Parse(time.Split('/')[0]);
            int year = int.Parse(time.Split('/')[2]);
            DateTime startTime = new DateTime(year, month, day, 0, 0, 0);
            time = allEndTime[RotationID][allEndTime[RotationID].Count - 1].Text;
            month = int.Parse(time.Split('/')[1]);
            day = int.Parse(time.Split('/')[0]);
            year = int.Parse(time.Split('/')[2]);
            DateTime endTime = new DateTime(year, month, day, 0, 0, 0);
            TimeSpan dif = endTime - startTime;
            startTime.ToLongDateString();
            int days = dif.Days;
            days -= (endTime.Year - startTime.Year-1) * 365;
            if (DateTime.IsLeapYear(startTime.Year) && (startTime.Month > 2))
            {
                days++;
            }
            if (DateTime.IsLeapYear(endTime.Year) && (endTime.Month <= 2))
            {
                days++;
            }

            while (startTime <= endTime.AddDays(1))
            {
                if (DateTime.IsLeapYear(startTime.Year))
                {
                    days -= 1;
                }
                startTime = new DateTime(startTime.Year + 1, startTime.Month, startTime.Day, 0, 0, 0);
            }

            if (days % 364 != 0)
            {
                Panel1.Controls.Add(new LiteralControl("<script>var myWindow = window.open(\"\", \"MsgWindow\", \"width=200, height=100\");"));
                Panel1.Controls.Add(new LiteralControl("myWindow.document.write(\"<p>CropData is not a year</p>\");</script>"));

            }
            else
            {
                save(RotationID);
            }
        }
        void SaveRotationText(Object sender, EventArgs e)
        {
            TextBox button = (TextBox)sender;
            int RotationID = int.Parse(button.Attributes["aRotationID"]);
            save(RotationID);
        }
        void New(Object sender, EventArgs e)
        {

            /*   HtmlButton button = (HtmlButton)sender;
               string tmp4 = button.ID.ToString();
               tmp4 = tmp4.Replace("RotationID", "");
               string soiltyp = HttpContext.Current.Request.Form["SoilType_" + tmp4 + ""];
               Console.WriteLine(soiltyp);
               tmp4 = (int.Parse(tmp4)-1).ToString();*/

            RotationNew tmp = new RotationNew();
            tmp.ProcessRequest(HttpContext.Current);
            ViewState["UserInit"] = "3";
            HttpContext.Current.Session["UserInit"] = "3";
            HttpContext.Current.Session["ShowResult"] = false;
            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);
        }


        void SelectionEnd(Object sender, EventArgs e)
        {
            TextBox button = (TextBox)sender;
            string RotationID = button.Attributes["aRotationID"];
            string plant = button.Attributes["plant"];
            string test = button.ID;
        
            //string test2 = allEndCalenders[int.Parse(RotationID)][int.Parse(plant)].SelectedDate.AddDays(1).ToString().Split(' ')[0];
            try
            {
                string time = allEndTime[int.Parse(RotationID)][int.Parse(plant)].Text;
                int month = int.Parse(time.Split('/')[1]);
                int day = int.Parse(time.Split('/')[0]);
                int year = int.Parse(time.Split('/')[2]);
                DateTime startTime = new DateTime(year, month, day, 0, 0, 0);

                if (allStartTime[int.Parse(RotationID)].Count > (int.Parse(plant) + 1))
                {
                    startTime = startTime.AddDays(1);
                    allStartTime[int.Parse(RotationID)][int.Parse(plant)+1].Text = "";
                    allStartTime[int.Parse(RotationID)][int.Parse(plant)+1].Text += startTime.Day.ToString() + "/" + startTime.Month.ToString() + "/" + startTime.Year.ToString();
                   // allStartTime[int.Parse(RotationID)][int.Parse(plant + 1)].Text = startTime.AddDays(1).ToString().Split(' ')[0];
                }
            }
            catch(Exception esd)
            {
                string test2 = esd.Message;
                Panel1.Controls.Add(new LiteralControl("<script>var myWindow = window.open(\"\", \"MsgWindow\", \"width=200, height=100\");"));
                Panel1.Controls.Add(new LiteralControl("myWindow.document.write(\"<p>You did enter a invalid date. </p>\");</script>"));
            }
 

        }
        protected void DeleteCropEvent(object sender, EventArgs e)
        {
            HtmlButton button = (HtmlButton)sender;
            string inerHTML = button.InnerHtml;
            string aRotationID = button.Attributes["aRotationID"];
            string aFieldPlanID = button.Attributes["FieldPlanID"];
            string tmps = button.InnerHtml;
            string aTotalArea = allArea[int.Parse(aRotationID)].Text;
            string nFieldPlan = allRotationName[int.Parse(aRotationID)].Text;
            FieldPlanDelete tmp = new FieldPlanDelete();
            tmp.ProcessRequest(HttpContext.Current, aRotationID, aFieldPlanID, aTotalArea, nFieldPlan);
            ViewState["UserInit"] = "3";
            HttpContext.Current.Session["UserInit"] = "3";
            HttpContext.Current.Session["ShowResult"] = false;
            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);
        }
        private void RemoveClickEvent(HtmlButton b)
        {

        }

        protected void deSelectDate(object sender, DayRenderEventArgs e)
        {
            Calendar cal = (Calendar)sender;

            string aRotationID = cal.Attributes["aRotationID"];
            string plant = cal.Attributes["plant"];
            string time = allStartTime[int.Parse(aRotationID)][int.Parse(plant)].Text;
            int month = int.Parse(time.Split('/')[1]);
            int day = int.Parse(time.Split('/')[0]);
            int year = int.Parse(time.Split('/')[2]);
            DateTime startTime = new DateTime(year, month, day, 0, 0, 0);

            if (e.Day.Date.Day == 29 && e.Day.Date.Month == 2)
            {
                e.Day.IsSelectable = false;
                e.Cell.ForeColor = System.Drawing.Color.Black;
                e.Cell.BackColor = System.Drawing.Color.White;
                e.Cell.Font.Bold = true;
            }
            if (e.Day.Date <= startTime)
            {
                e.Day.IsSelectable = false;
                e.Cell.ForeColor = System.Drawing.Color.Black;
                e.Cell.BackColor = System.Drawing.Color.White;
                e.Cell.Font.Bold = true;
            }


        }
        protected void NewCropEvent(object sender, EventArgs e)
        {


            HtmlButton button = (HtmlButton)sender;
            RemoveClickEvent(button);
            string name = button.InnerHtml;
            int aRotationID = int.Parse(button.Attributes["aRotationID"]);
            List<dbInterface.data> paramter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = HttpContext.Current.Session["farmNumber"].ToString();
            paramter.Add(data);
            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            paramter.Add(data);
            data = new dbInterface.data();
            data.name = "@aRotationID";
            data.value = aRotationID.ToString();
            paramter.Add(data);
            dbInterface database = new dbInterface();
            DataTable arrFieldPlan = database.GetValuesFromDB("GetFieldPlanList", paramter);

            List<DropDownList> crops = allCrops[aRotationID];
            List<string> aCropList = new List<string>();
            for (int i = 0; i < crops.Count; i++)
            {
                aCropList.Add(crops[i].SelectedItem.Attributes["dbValue"].ToString());
            }

            List<string> aFieldPlanIDList = new List<string>();

            for (int i = 0; i < arrFieldPlan.Rows.Count; i++)
            {
                aFieldPlanIDList.Add(arrFieldPlan.Rows[i]["FieldPlanID"].ToString());
            }
            List<string> StartTime = new List<string>();

            for (int i = 0; i < allStartTime[aRotationID].Count; i++)
            {
                StartTime.Add(allStartTime[aRotationID][i].Text);
            }
            List<string> EndTime = new List<string>();

            for (int i = 0; i < allEndTime[aRotationID].Count; i++)
            {
                EndTime.Add(allEndTime[aRotationID][i].Text);
            }
            List<string> feedStuff = new List<string>();

            for (int i = 0; i < allMainFeedStuff[aRotationID].Count; i++)
            {
                string tmpds = allMainFeedStuff[aRotationID][i].SelectedItem.Attributes["dbValue"];
                feedStuff.Add(allMainFeedStuff[aRotationID][i].SelectedItem.Attributes["dbValue"]);
            }
            List<string> StrawFeedStuff = new List<string>();

            for (int i = 0; i < allStrawFeedStuff[aRotationID].Count; i++)
            {
                StrawFeedStuff.Add(allStrawFeedStuff[aRotationID][i].SelectedItem.Attributes["dbValue"]);
            }
            FieldPlanNew tmp = new FieldPlanNew();

            tmp.ProcessRequest(HttpContext.Current.Session["yieldLevel"].ToString(), HttpContext.Current.Session["irrigation"].ToString(), HttpContext.Current.Session["scenarioNumber"].ToString(), HttpContext.Current.Session["yieldLevel"].ToString(), HttpContext.Current.Session["irrigation"].ToString(), HttpContext.Current.Session["soilType"].ToString(), HttpContext.Current.Session["soilType"].ToString(), HttpContext.Current.Session["farmNumber"].ToString(), HttpContext.Current.Request["changes"], aFieldPlanIDList, aCropList,  allArea[aRotationID].Text, aRotationID.ToString(), allSoilType[aRotationID].SelectedItem.Attributes["dbValue"], "0", "0", allRotationName[aRotationID].Text, StartTime.Count, StartTime, EndTime, feedStuff, StrawFeedStuff, Panel1);


            ViewState["UserInit"] = "3";
            HttpContext.Current.Session["ShowResult"] = false;
            HttpContext.Current.Session["UserInit"] = "3";

            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);
        }
        private void landlessEvent(object sender, System.EventArgs e)
        {
            LandlessUpdate update = new LandlessUpdate();
            HttpContext.Current.Session["ShowResult"] = false;
            string tesg = landless.Checked.ToString();
            update.ProcessRequest(HttpContext.Current, landless.Checked.ToString());
            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);
        }

        [System.Web.Services.WebMethod]
        public static void ChangeSession(string value)
        {
            HttpContext.Current.Session["yourSessionKey"] = value;
        }
    }
}