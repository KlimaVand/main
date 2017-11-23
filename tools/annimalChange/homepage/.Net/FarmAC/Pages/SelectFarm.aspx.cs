using FarmAC.Controls;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace FarmAC.Pages
{
    public partial class SelectFarm : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            mainPart();
        }
        DataTable arrFarm1;
        DataTable arrFarm;
        DataTable arrStandardFarm;
        void mainPart()
        {
            const string TITLE = "Farm AC";

            int aFarm;
            DataTable f;


            DataTable arrmaxFarmNumber;


            int newFarmNumber, refFarmNumber;
            int[] arr;



            if (HttpContext.Current.Session["hasLogin"] == "yes")
            {
                HttpContext.Current.Session["loginTry"] = "yes";
                HttpContext.Current.Session["tabNumber"] = 1;
            }
            else
            {
                Response.Redirect("default.asp");
                Response.End();
            }


            if (Request["selectFarm"] != null)
            {
                string tmpfdg = HttpContext.Current.Session["farmNumber"].ToString(); ;
                HttpContext.Current.Session["farmNumber"] = Request["selectFarm"];
                tmpfdg = HttpContext.Current.Session["farmNumber"].ToString(); ;
            }
            if (Request["DeleteFarm"] != null && Request["selectFarm"] != null)
            {

                List<dbInterface.data> paramter = new List<dbInterface.data>();
                dbInterface.data data = new dbInterface.data();
                data.name = "@aFarmNumber";
                data.value = (string)Request["selectFarm"];
                paramter.Add(data);
                dbInterface dataBase = new dbInterface();
                dataBase.InitFarmNDB();
                DataTable delFarm = dataBase.GetValuesFromDB("DeleteFarm", paramter);
                paramter = new List<dbInterface.data>();
                data.name = "@aUserID";
                data.value = (string)HttpContext.Current.Session["UserID"];
                paramter.Add(data);
                arrFarm1 = dataBase.GetValuesFromDB("GetFarmList", paramter);

                HttpContext.Current.Session["farmNumber"] = arrFarm1.Rows[0][0];
                HttpContext.Current.Session["ShowResult"] = "False";



                dataBase.CloseFarmNDB();

            }
            string tmp4 = Request["NewFarm"];
            if (Request["NewFarm"] != null)
            {
                dbInterface dataBase = new dbInterface();

                dataBase.InitFarmNDB();
                List<dbInterface.data> paramter = new List<dbInterface.data>();
                dbInterface.data data = new dbInterface.data();
                //data.name = "@aFarmNumber";
                //data.value = Request["selectFarm"];
                //paramter.Add(data);
                //dataBase.GetValuesFromDB("DeleteFarm", paramter);
                paramter = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.name = "@UserID";
                data.value = HttpContext.Current.Session["UserID"].ToString();
                paramter.Add(data);
                arrmaxFarmNumber = dataBase.GetValuesFromDB("GetMaxFarmNumberPrUser", paramter);

                int maxFarmNumber = int.Parse(arrmaxFarmNumber.Rows[0]["MaxFarmNumber"].ToString());
                newFarmNumber = maxFarmNumber + 1;
                paramter = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.value = (string)HttpContext.Current.Session["UserID"];
                data.name = "@UserID";
                paramter.Add(data);
                data = new dbInterface.data();
                data.value = newFarmNumber.ToString();
                data.name = "@FarmNumber";
                paramter.Add(data);
                f = dataBase.GetValuesFromDB("InsertNewFarm", paramter);
                HttpContext.Current.Session["farmNumber"] = newFarmNumber;

                dataBase.CloseFarmNDB();
            }
            if (Request["CopyFarm"] != null)
            {
                dbInterface dataBase = new dbInterface();
                dataBase.InitFarmNDB();

                List<dbInterface.data> paramter = new List<dbInterface.data>();
                dbInterface.data data = new dbInterface.data();
                data.value = (string)HttpContext.Current.Session["UserID"];
                data.name = "@UserID";
                paramter.Add(data);
                arrmaxFarmNumber = dataBase.GetValuesFromDB("GetMaxFarmNumberPrUser", paramter);

                int maxFarmNumber = int.Parse(arrmaxFarmNumber.Rows[0]["MaxFarmNumber"].ToString());
                newFarmNumber = maxFarmNumber + 1;

                paramter = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.value = (string)Request["selectFarm"];
                data.name = "@FromFarmNumber ";
                paramter.Add(data);
                data = new dbInterface.data();
                data.value = newFarmNumber.ToString(); ;
                data.name = "@ToFarmNumber";
                paramter.Add(data);
                data = new dbInterface.data();
                data.value = (string)HttpContext.Current.Session["UserID"];
                data.name = "@UserID";
                paramter.Add(data);
                f = dataBase.GetValuesFromDB("copy_Farm", paramter);
                HttpContext.Current.Session["farmNumber"] = newFarmNumber;


            }
            if (Request["CopyCattleFarm"] != null)
            {
                dbInterface dataBase = new dbInterface();
                dataBase.InitFarmNDB();
                List<dbInterface.data> paramter = new List<dbInterface.data>();
                dbInterface.data data = new dbInterface.data();
                data.value = (string)HttpContext.Current.Session["UserID"];
                data.name = "@UserID";
                paramter.Add(data);

                arrmaxFarmNumber = dataBase.GetValuesFromDB("GetMaxFarmNumberPrUser", paramter);

                int maxFarmNumber = (int)arrmaxFarmNumber.Rows[0]["MaxFarmNumber"];
                newFarmNumber = maxFarmNumber + 1;
                paramter = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.value = "178902";
                data.name = "@FromFarmNumber";
                paramter.Add(data);
                data = new dbInterface.data();
                data.value = newFarmNumber.ToString(); ;
                data.name = "@ToFarmNumber";
                paramter.Add(data);
                data = new dbInterface.data();
                data.value = (string)HttpContext.Current.Session["UserID"];
                data.name = "@UserID";
                paramter.Add(data);

                f = dataBase.GetValuesFromDB("copy_Farm", paramter);
                HttpContext.Current.Session["farmNumber"] = newFarmNumber;

                dataBase.CloseFarmNDB();
            }
            //if (Request["CopyPigFarm"] != null)
            //{
            //    dbInterface dataBase = new dbInterface();
            //    dataBase.InitFarmNDB();

            //    List<dbInterface.data> paramter = new List<dbInterface.data>();
            //    dbInterface.data data = new dbInterface.data();
            //    data.value = (string)HttpContext.Current.Session["UserID"];
            //    data.name = "@UserID";
            //    paramter.Add(data);
            //    arrmaxFarmNumber = dataBase.GetValuesFromDB("GetMaxFarmNumberPrUser", paramter);

            //    int maxFarmNumber = (int)arrmaxFarmNumber.Rows[0]["MaxFarmNumber"];
            //    newFarmNumber = maxFarmNumber + 1;

            //    paramter = new List<dbInterface.data>();
            //    data = new dbInterface.data();
            //    data.value = "178904";
            //    data.name = "@FromFarmNumber";
            //    paramter.Add(data);
            //    paramter = new List<dbInterface.data>();
            //    data = new dbInterface.data();
            //    data.value = newFarmNumber.ToString();
            //    data.name = "@ToFarmNumber";
            //    paramter.Add(data);
            //    paramter = new List<dbInterface.data>();
            //    data = new dbInterface.data();
            //    data.value = (string)HttpContext.Current.Session["UserID"];
            //    data.name = "@UserID";
            //    paramter.Add(data);
            //    f = dataBase.GetValuesFromDB("copy_Farm", paramter);
            //    HttpContext.Current.Session["farmNumber"] = newFarmNumber;
            //    dictMaxFarmNumber = null;
            //    dataBase.CloseFarmNDB();
            //}
            if (Request["CopyCropProductionFarm"] != null)
            {
                dbInterface dataBase = new dbInterface();
                dataBase.InitFarmNDB();

                List<dbInterface.data> paramter = new List<dbInterface.data>();
                dbInterface.data data = new dbInterface.data();
                data.value = (string)HttpContext.Current.Session["UserID"];
                data.name = "UserID";
                paramter.Add(data);
                arrmaxFarmNumber = dataBase.GetValuesFromDB("GetMaxFarmNumberPrUser ", paramter);

                int maxFarmNumber = (int)arrmaxFarmNumber.Rows[0]["MaxFarmNumber"];
                newFarmNumber = maxFarmNumber + 1;

                paramter = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.value = "178903";
                data.name = "@FromFarmNumber";
                paramter.Add(data);
                paramter = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.value = newFarmNumber.ToString();
                data.name = "@ToFarmNumber";
                paramter.Add(data);
                paramter = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.value = (string)HttpContext.Current.Session["UserID"];
                data.name = "@UserID";
                paramter.Add(data);
                f = dataBase.GetValuesFromDB("copy_Farm", paramter);
                HttpContext.Current.Session["farmNumber"] = newFarmNumber;

                dataBase.CloseFarmNDB();
            }
            string tmp6 = Request["CopyRef_Cow"];
            if (Request["CopyRef_Cow"] != null || Request["CopyRef_Pig"] != null || Request["CopyRef_Plant"] != null)
            {
                if (Request["CopyRef_Cow"].ToString() != "0")
                {
                    refFarmNumber = int.Parse(Request["CopyRef_Cow"]);

                    dbInterface dataBase = new dbInterface();
                    dataBase.InitFarmNDB();

                    List<dbInterface.data> paramter = new List<dbInterface.data>();
                    dbInterface.data data = new dbInterface.data();
                    data.value = (string)HttpContext.Current.Session["UserID"];
                    data.name = "UserID";
                    paramter.Add(data);
                    arrmaxFarmNumber = dataBase.GetValuesFromDB("GetMaxFarmNumberPrUser", paramter);

                    int maxFarmNumber = int.Parse(arrmaxFarmNumber.Rows[0]["MaxFarmNumber"].ToString());
                    newFarmNumber = maxFarmNumber + 1;


                    paramter = new List<dbInterface.data>();
                    data = new dbInterface.data();
                    data.value = refFarmNumber.ToString();
                    data.name = "@FromFarmNumber";
                    paramter.Add(data);
                    data = new dbInterface.data();
                    data.value = newFarmNumber.ToString();
                    data.name = "@ToFarmNumber";
                    paramter.Add(data);
                    data = new dbInterface.data();
                    data.value = HttpContext.Current.Session["UserID"].ToString();
                    data.name = "@UserID";
                    paramter.Add(data);
                    f = dataBase.GetValuesFromDB("copy_Farm", paramter);
                    HttpContext.Current.Session["farmNumber"] = newFarmNumber;

                    dataBase.CloseFarmNDB();
                }
            }
            dbInterface dataBases = new dbInterface();
            dataBases.InitFarmNDB();


            List<dbInterface.data> paramter1 = new List<dbInterface.data>();
            dbInterface.data data1 = new dbInterface.data();
            data1.value = HttpContext.Current.Session["farmNumber"].ToString();
            data1.name = "@aFarmNumber";
            paramter1.Add(data1);
            arrFarm = dataBases.GetValuesFromDB("GetFarmData", paramter1);

            dataBases.CloseFarmNDB();

            try
            {
                HttpContext.Current.Session["farmOwner"] = arrFarm.Rows[0]["FarmOwnerName"];

                HttpContext.Current.Session["scenarioNumber"] = arrFarm.Rows[0]["SelectedScenario"];

                HttpContext.Current.Session["scenarioName"] = arrFarm.Rows[0]["ScenarioName"];

                HttpContext.Current.Session["zipCode"] = arrFarm.Rows[0]["FarmOwnerZipCode"];

                HttpContext.Current.Session["soilType"] = arrFarm.Rows[0]["SoilType"];


                HttpContext.Current.Session["irrigation"] = arrFarm.Rows[0]["Irrigation"];
                if (HttpContext.Current.Session["irrigation"] != null)
                    HttpContext.Current.Session["irrigation"] = 1;
                else
                    HttpContext.Current.Session["irrigation"] = 0;
                string farmType = arrFarm.Rows[0][8].ToString();
                HttpContext.Current.Session["farmType"] = arrFarm.Rows[0][8];
                HttpContext.Current.Session["yieldLevel"] = arrFarm.Rows[0][10];
            }
            catch
            {
                HttpContext.Current.Session["farmOwner"] = -1;

                HttpContext.Current.Session["scenarioNumber"] = -1;

                HttpContext.Current.Session["scenarioName"] = -1;

                HttpContext.Current.Session["zipCode"] = -1;

                HttpContext.Current.Session["soilType"] = -1;


                HttpContext.Current.Session["irrigation"] = -1;
                if (HttpContext.Current.Session["irrigation"] != null)
                    HttpContext.Current.Session["irrigation"] = 1;
                else
                    HttpContext.Current.Session["irrigation"] = 0;

                HttpContext.Current.Session["farmType"] = -1;
                HttpContext.Current.Session["yieldLevel"] = -1;
            }

            object test = Request["DeleteFarm"];

            if (Request["DeleteFarm"] == null && (Request["OK"] != null || Request["NewFarm"] != null || Request["CopyFarm"] != null || Request["CopyCattleFarm"] != null || Request["CopyPigFarm"] != null || Request["CopyCropProductionFarm"] != null || Request["CopyRef_Cow"] != null || Request["CopyRef_Pig"] != null || Request["CopyRef_Plant"] != null))
            {

                Response.Redirect("FarmAC.aspx");
                Response.End();
            }

            else
            {

                aFarm = int.Parse(HttpContext.Current.Session["farmNumber"].ToString());


                //  CreateStyleLink "styles/stylesheet.css"

                dbInterface database = new dbInterface();
                database.InitFarmNDB();

                List<dbInterface.data> paramter = new List<dbInterface.data>();
                dbInterface.data data = new dbInterface.data();
                data.value = (string)HttpContext.Current.Session["UserID"];
                data.name = "@aUserID";
                paramter.Add(data);

                arrFarm = database.GetValuesFromDB("GetFarmList", paramter);
                paramter = new List<dbInterface.data>();
                arrStandardFarm = database.GetValuesFromDB("GetStandardFarms", paramter);

                // CreateLanguageDictionary
                database.CloseFarmNDB();
                for (int i = 0; i < arrFarm.Rows.Count; i++)
                {
                    arrFarm.Rows[i][0] = arrFarm.Rows[i][0];
                }

                CreatePage();

            }
        }

        void CreatePage()
        {
            while (selectFarm.Items.Count > 0)
                selectFarm.Items.RemoveAt(0);
            for (int i = 0; i < arrFarm.Rows.Count; i++)
            {
                ListItem name = new ListItem();
                name.Text = "("+arrFarm.Rows[i]["FarmNumber"].ToString()+") "+arrFarm.Rows[i]["FarmOwnerName"].ToString();
                name.Value = arrFarm.Rows[i]["FarmNumber"].ToString();
                selectFarm.Items.Add(name);

            }

            selectFarm.DataBind();

            ListItem name_0 = new ListItem();
            name_0.Value = "0";
            name_0.Text = "- - Choose - -";
            CopyRef_Cow.Items.Add(name_0);

            for (int i = 0; i < arrStandardFarm.Rows.Count; i++)
            {
                ListItem name = new ListItem();
                name.Text = arrStandardFarm.Rows[i]["FarmOwnerName"].ToString();
                name.Value = arrStandardFarm.Rows[i]["FarmNumber"].ToString();
                CopyRef_Cow.Items.Add(name);

            }
            CopyRef_Cow.AutoPostBack = true;
            CopyRef_Cow.SelectedIndexChanged += new EventHandler(CopyRef_Cow_SelectedIndexChanged);



        }

        protected void DeleteFarm_Click(object sender, EventArgs e)
        {

        }

        protected void NewFarm_Click(object sender, EventArgs e)
        {

        }

        protected void OK_Click(object sender, EventArgs e)
        {

        }

        protected void CopyRef_Cow_SelectedIndexChanged(object sender, EventArgs e)
        {
            Panel1.Controls.Add(new LiteralControl("?CopyRef_Cow=true"));
            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);
        }
    }
}